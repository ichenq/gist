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


#pragma once

#include <string>
#include <memory>
#include <functional>


namespace net {

struct IHttpRequest
{
    virtual const std::string& GetURL() = 0;
    virtual std::string GetHeader(const std::string&) = 0;
    virtual const std::string& GetBody() = 0;
};

struct IHttpResponseWriter
{
    virtual int WriteHeader(const std::string&, const std::string&) = 0;
    virtual int Write(const std::string& data) = 0;
    virtual int WriteStatus(int) = 0;
};

typedef std::function<void(IHttpRequest*, IHttpResponseWriter*)> HttpHandleFunc;

// HTTP server
class HttpServer 
{
public:
    class HttpServerImpl;

public:
    HttpServer();
    ~HttpServer();

    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

public:
    int Init(const std::string& host, const std::string& port);
    int Run();
    void HandleFunc(std::string&& pattern, HttpHandleFunc fn, bool httpPost = false);

private:
    // Pointer To Implementation (pImpl) idiom
    // See https://en.wikibooks.org/wiki/C%2B%2B_Programming/Idioms#Pointer_To_Implementation_.28pImpl.29
    std::shared_ptr<HttpServerImpl> impl_;
};

} // namespace net
