// Copyright 2018-preset Beyondtech Inc. All Rights Reserved.
//
// http://www.3bodygame.com
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

#include "stdafx.h"
#include "Win32Http.h"

#if defined(_WIN32)

#include "Utility/WinUtil.h"
#include "Utility/Logging.h"
#include "Utility/Strings.h"
#include "Utility/ScopeGuard.h"
#include "Uri.h"

#pragma comment(lib, "Wininet.lib")

#define BUFFER_SIZE 4096

#define CHECK_RETURN(cond, ctx) if ((cond)) { \
                                    DWORD dwErr = GetLastError(); \
                                    (ctx)->strError = GBKToUtf8(GetLastErrorMessage(dwErr)); \
                                    return dwErr; \
                                }

namespace {

static HINTERNET hInternet = NULL;

}

struct WinHttpRequestContext
{
    std::string     strUrl;
    std::string     strHost;
    INTERNET_PORT   nPort = 0;
    std::string     strUriPath;
    std::string     strPostData;

    DWORD                       dwStatusCode = 0;
    std::vector<std::string>    headerFiels;
    std::string                 strContent;
    std::string                 strError;

    void ParseUri(const std::string& url)
    {
        net::Uri uri(url);
        INTERNET_PORT port = 80;
        if (uri.port() > 0)
        {
            port = uri.port();
        }

        std::string uriPath = uri.path();
        if (!uri.query().empty())
        {
            uriPath += "?";
            uriPath += uri.query();
        }

        nPort = port;
        strHost = uri.host();
        strUriPath = uriPath;
    }
};


//////////////////////////////////////////////////////////////////////////

void MustInitializeWinInet()
{
    DWORD dwErr = InternetAttemptConnect(0);
    CHECK(dwErr == ERROR_SUCCESS) << GBKToUtf8(GetLastErrorMessage(dwErr));

    hInternet = InternetOpenA(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
    CHECK(hInternet != NULL) << GBKToUtf8(GetLastErrorMessage(GetLastError()));
}

void UnInitializeWinInet()
{
    if (hInternet != NULL)
    {
        InternetCloseHandle(hInternet);
        hInternet = NULL;
    }
}

static int WinHttpDoRequest(const std::string& method, WinHttpRequestContext* ctx)
{
    HINTERNET hConnection = InternetConnectA(hInternet, ctx->strHost.data(), ctx->nPort, 
        NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    CHECK_RETURN(hConnection == NULL, ctx);
    
    SCOPE_EXIT{ InternetCloseHandle(hConnection); };
    

    HINTERNET hFile = HttpOpenRequestA(hConnection, method.data(), ctx->strUriPath.data(), HTTP_VERSIONA,
        NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT, 0);
    CHECK_RETURN(hFile == NULL, ctx);

    SCOPE_EXIT{ InternetCloseHandle(hFile); };

    BOOL bOK = HttpSendRequestA(hFile, NULL, 0, (LPVOID)ctx->strPostData.data(), ctx->strPostData.length());
    CHECK_RETURN(!bOK, ctx);

    char buffer[BUFFER_SIZE] = {};
    DWORD dwLen = BUFFER_SIZE;
    
    bOK = HttpQueryInfoA(hFile, HTTP_QUERY_STATUS_CODE, buffer, &dwLen, NULL);
    CHECK_RETURN(!bOK, ctx);

    ctx->dwStatusCode = to<uint32_t>(buffer);

    dwLen = BUFFER_SIZE;
    bOK = HttpQueryInfoA(hFile, HTTP_QUERY_RAW_HEADERS_CRLF, buffer, &dwLen, NULL);
    CHECK_RETURN(!bOK, ctx);

    StringPiece content(buffer, dwLen);
    auto headerFields = Split(content, "\r\n");
    for (auto field : headerFields)
    {
        field = trimWhitespace(field);
        if (!field.empty())
        {
            const std::string& text = field.str();
            ctx->headerFiels.push_back(text);
            LOG(INFO) << text;
        }
    }

    while (true)
    {
        dwLen = 0;
        bOK = InternetReadFile(hFile, buffer, BUFFER_SIZE, &dwLen);
        CHECK_RETURN(!bOK, ctx);
        if (dwLen > 0)
        {
            ctx->strContent.append(buffer, dwLen);
            continue;
        }
        break;
    }

    return 0;
}

int WinHttpGet(const std::string& strUrl, std::string* outData, std::string* outErr)
{
    WinHttpRequestContext ctx;
    ctx.ParseUri(strUrl);
    WinHttpDoRequest("GET", &ctx);
    outData->swap(ctx.strContent);
    outErr->swap(ctx.strError);
    return 0;
}

int WinHttpPost(const std::string& strUrl, const std::string& postData, std::string* outData, std::string* outErr)
{
    WinHttpRequestContext ctx;
    ctx.strPostData = postData;
    ctx.ParseUri(strUrl);
    WinHttpDoRequest("POST", &ctx);
    outData->swap(ctx.strContent);
    outErr->swap(ctx.strError);
    return 0;
}

#endif // _WIN32
