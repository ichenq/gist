// Copyright 2015-2018 Beyondtech, Inc. All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form 
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially 
// exploit the content. Nor may you transmit it or store it in any other website or 
// other form of electronic retrieval system.

#include "HttpServer.h"
#include <Windows.h>
#include <http.h>
#include <assert.h>
#include <stdexcept>
#include <unordered_map>
#include "Utility/Conv.h"
#include "Utility/WinUtil.h"
#include "HttpStatus.h"
#include "HeaderID.h"


#pragma comment(lib, "httpapi.lib")


HTTP_HEADER_ID GetHttpHeaderID(const std::string& s)
{
    static std::unordered_map<std::string, HTTP_HEADER_ID> dict;
    if (dict.empty())
    {
#define XX(name, id) dict[name] = id;
        GEN_HTTP_HEADER_ID_MAP(XX)
#undef XX
    }
    return dict[s];
}

std::string GetHttpHeaderName(HTTP_HEADER_ID hid)
{
    switch (hid)
    {
#define XX(name, id) case id: return name;
        GEN_HTTP_HEADER_ID_MAP(XX)
#undef XX
    };
    return "";
}

namespace net {

    
// HTTP request object
class HttpRequest : public IHttpRequest
{
public:
    explicit HttpRequest(HANDLE handle, HTTP_REQUEST* req)
        : hRequestQueue(handle), pRequest(req)
    {}

    const std::string& GetURL()
    {
        assert(pRequest != NULL);
        if (url.empty())
        {
            std::wstring urlWide(pRequest->CookedUrl.pFullUrl);
            url = WideToMultibyte(urlWide);
        }
        return url;
    }

    std::string GetHeader(const std::string& key)
    {
        assert(pRequest != NULL);
        HTTP_HEADER_ID hid = GetHttpHeaderID(key);
        HTTP_KNOWN_HEADER header = pRequest->Headers.KnownHeaders[hid];
        return header.pRawValue;
    }

    const std::string& GetBody()
    {
        if (body.empty())
        {
            ReadRequestBody();
        }
        return body;
    }

    int ReadRequestBody()
    {
        std::vector<char> buffer;
        buffer.resize(4096); // 4k buffer
        bool done = false;
        while (!done)
        {
            ULONG bytesRead = 0;
            ULONG r = HttpReceiveRequestEntityBody(hRequestQueue, pRequest->RequestId, 0, buffer.data(), 
                buffer.size() - 1, &bytesRead, NULL);
            if (r == NO_ERROR)
            {
                if (bytesRead > 0)
                {
                    buffer[bytesRead] = '\0';
                    body.append(buffer.data(), bytesRead);
                }
            }
            else if (r == ERROR_HANDLE_EOF)
            {
                if (bytesRead > 0)
                {
                    buffer[bytesRead] = '\0';
                    body.append(buffer.data(), bytesRead);
                }
                done = true;
            }
            else
            {
                DCHECK(r) << GetLastErrorMessage(r);
                return r;
            }
        }
        return 0;
    }

private:
    HANDLE          hRequestQueue;
    HTTP_REQUEST*   pRequest = NULL;
    std::string     url;
    std::string     body;
};

// response writer
class HttpResponseWriter : public IHttpResponseWriter
{
public:
    explicit HttpResponseWriter(HANDLE handle, HTTP_REQUEST_ID reqId)
        : hRequestQueue(handle), requestId(reqId)
    {
        memset(&resp, 0, sizeof(resp));
    }

    int WriteHeader(const std::string& key, const std::string& value)
    {
        HTTP_HEADER_ID hid = GetHttpHeaderID(key);
        resp.Headers.KnownHeaders[hid].pRawValue = key.c_str();
        resp.Headers.KnownHeaders[hid].RawValueLength = (USHORT)key.length();
        return 0;
    }

    int Write(const std::string& data)
    {
        if (!data.empty())
        {
            chunk.DataChunkType = HttpDataChunkFromMemory;
            chunk.FromMemory.pBuffer = (PVOID)data.c_str();
            chunk.FromMemory.BufferLength = (ULONG)data.length();
            resp.EntityChunkCount = 1;
            resp.pEntityChunks = &chunk;
        }
        return 0;
    }

    int WriteStatus(int status)
    {
        resp.StatusCode = (USHORT)status;
        DWORD bytesSent = 0;
        ULONG r = HttpSendHttpResponse(hRequestQueue, requestId, 0, &resp, NULL, &bytesSent, NULL, 0, NULL, NULL);
        DCHECK(r == NO_ERROR) << GetLastErrorMessage(r);
        return (int)r;
    }

public:
    HANDLE              hRequestQueue;
    HTTP_REQUEST_ID     requestId;
    HTTP_RESPONSE_V2    resp;
    HTTP_DATA_CHUNK     chunk;  // data chunk
};

class HttpServer::HttpServerImpl
{
public:
    std::string     host;
    uint16_t        port;
    std::unordered_map<std::wstring, HttpHandleFunc> handlers;

    HANDLE              hRequestQueue;
    HTTP_REQUEST_ID     requestId;
    HTTP_REQUEST*       pRequest = NULL;
    ULONG               reqBufferLen = 0;

public:
    HttpServerImpl()
    {
        // Allocate a 4 KB buffer.This size should work for most requests
        AllocateBuffer(4096);
    }

    ~HttpServerImpl()
    {
        FreeRequest();
        CloseHandle(hRequestQueue);
    }

    void AllocateBuffer(int size)
    {
        FreeRequest();
        reqBufferLen = sizeof(HTTP_REQUEST) + size;
        pRequest = (HTTP_REQUEST*)new char[reqBufferLen];
        memset(pRequest, 0, reqBufferLen);
        HTTP_SET_NULL_ID(&requestId);
    }

    void FreeRequest()
    {
        if (pRequest != NULL)
        {
            delete pRequest;
            pRequest = NULL;
        }
    }

    int Run()
    {
        DWORD bytesRead = 0;
        ULONG r = HttpReceiveHttpRequest(hRequestQueue, requestId, 0, pRequest, reqBufferLen, &bytesRead, NULL);
        if (r == NO_ERROR)
        {
            DispatchReqest();
            return 0;
        }
        else
        {
            // The input buffer was too small to hold the request headers.
            if (r == ERROR_MORE_DATA)
            {
                AllocateBuffer(reqBufferLen * 2);
                // try again
            }
            else if (r == ERROR_CONNECTION_INVALID && !HTTP_IS_NULL_ID(&requestId))
            {
                // The TCP connection was corrupted by the peer when attempting to handle a request with more buffer. 
                // Continue to the next request.
                HTTP_SET_NULL_ID(&requestId);
            }
            else
            {
                return 1;
            }
            return 0;
        }
    }

    int DispatchReqest()
    {
        // Reset the Request ID to handle the next request.
        SCOPE_EXIT{ HTTP_SET_NULL_ID(&requestId); };

        switch (pRequest->Verb)
        {
        case HttpVerbPOST:
        case HttpVerbGET:
            {
                std::wstring url(pRequest->CookedUrl.pAbsPath);
                auto iter = handlers.find(url);
                if (iter != handlers.end() && iter->second)
                {
                    HttpRequest req(hRequestQueue, pRequest);
                    HttpResponseWriter writer(hRequestQueue, pRequest->RequestId);
                    try
                    {
                        (iter->second)(&req, &writer);
                    }
                    catch (std::exception& ex)
                    {
                        LOG(ERROR) << "exception " << typeid(ex).name() << ": " << ex.what();
                    }
                }
            }
            break;

        default:
            SendHttpResponse(HTTP_SERVICE_UNAVAILABLE, "Not implemented", "");
        }

        return 0;
    }

    void SendHttpResponse(int status, const std::string& reason, const std::string& body)
    {
        HttpResponseWriter writer(hRequestQueue, pRequest->RequestId);
        writer.resp.pReason = reason.c_str();
        writer.resp.ReasonLength = (USHORT)reason.length();
        writer.WriteHeader("Content-Type", "application/octet-stream");
        writer.Write(body);
        writer.WriteStatus(status);
    }

};

//////////////////////////////////////////////////////////////////////////

HttpServer::HttpServer()
{
    ULONG r = HttpInitialize(HTTPAPI_VERSION_1, HTTP_INITIALIZE_SERVER, NULL);
    CHECK(r == NO_ERROR) << GetLastErrorMessage(r);
}

HttpServer::~HttpServer()
{
    ULONG r = HttpTerminate(HTTP_INITIALIZE_SERVER, NULL);
    DCHECK(r == NO_ERROR) << GetLastErrorMessage(r);
}

int HttpServer::Init(const std::string& host, const std::string& port)
{
    impl_.reset(new HttpServerImpl);
    impl_->host = host;
    impl_->port = to<uint16_t>(port);
    ULONG r = HttpCreateHttpHandle(&impl_->hRequestQueue, 0);
    DCHECK(r == NO_ERROR) << GetLastErrorMessage(r);
    return (int)r;
}

int HttpServer::Run()
{ 
    int r = 0;
    while (r == 0)
    {
        r = impl_->Run();
    }
    return r;
}

void HttpServer::HandleFunc(std::string&& pattern, HttpHandleFunc fn, bool /*httpPost*/)
{
    std::string url = stringPrintf("http://%s:%d%s", impl_->host.c_str(), impl_->port, pattern.c_str());
    std::wstring urlWide = MultibyteToWide(url);
    ULONG r = HttpAddUrl(impl_->hRequestQueue, urlWide.c_str(), NULL);
    CHECK(r == NO_ERROR) << GetLastErrorMessage(r);
    impl_->handlers[MultibyteToWide(pattern)] = fn;
    LOG(INFO) << url;
}


} // namespace net