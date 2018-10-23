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

namespace net {
    
enum HTTPStatus
{
    HTTP_CONTINUE = 100,
    HTTP_SWITCHING_PROTOCOLS = 101,
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_ACCEPTED = 202,
    HTTP_NONAUTHORITATIVE = 203,
    HTTP_NO_CONTENT = 204,
    HTTP_RESET_CONTENT = 205,
    HTTP_PARTIAL_CONTENT = 206,
    HTTP_MULTIPLE_CHOICES = 300,
    HTTP_MOVED_PERMANENTLY = 301,
    HTTP_FOUND = 302,
    HTTP_SEE_OTHER = 303,
    HTTP_NOT_MODIFIED = 304,
    HTTP_USEPROXY = 305,
    // UNUSED: 306
    HTTP_TEMPORARY_REDIRECT = 307,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_PAYMENT_REQUIRED = 402,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_NOT_ACCEPTABLE = 406,
    HTTP_PROXY_AUTHENTICATION_REQUIRED = 407,
    HTTP_REQUEST_TIMEOUT = 408,
    HTTP_CONFLICT = 409,
    HTTP_GONE = 410,
    HTTP_LENGTH_REQUIRED = 411,
    HTTP_PRECONDITION_FAILED = 412,
    HTTP_REQUESTENTITYTOOLARGE = 413,
    HTTP_REQUESTURITOOLONG = 414,
    HTTP_UNSUPPORTEDMEDIATYPE = 415,
    HTTP_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
    HTTP_EXPECTATION_FAILED = 417,
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_NOT_IMPLEMENTED = 501,
    HTTP_BAD_GATEWAY = 502,
    HTTP_SERVICE_UNAVAILABLE = 503,
    HTTP_GATEWAY_TIMEOUT = 504,
    HTTP_VERSION_NOT_SUPPORTED = 505
};


inline const char* GetHTTPStatus(int status)
{
    HTTPStatus s = HTTPStatus(status);
    switch (s)
    {
    case HTTP_CONTINUE: return "Continue";
    case HTTP_SWITCHING_PROTOCOLS: return "Switching Protocols";
    case HTTP_OK: return "OK";
    case HTTP_CREATED: return "Created";
    case HTTP_ACCEPTED: return "Accepted";
    case HTTP_NONAUTHORITATIVE: return "Non-Authoritative Information";
    case HTTP_NO_CONTENT:  return "No Content";
    case HTTP_RESET_CONTENT: return "Reset Content";
    case HTTP_PARTIAL_CONTENT: return "Partial Content";
    case HTTP_MULTIPLE_CHOICES: return "Multiple Choices";
    case HTTP_MOVED_PERMANENTLY: return "Moved Permanently";
    case HTTP_FOUND: return "Found";
    case HTTP_SEE_OTHER: return "See Other";
    case HTTP_NOT_MODIFIED: return "Not Modified";
    case HTTP_USEPROXY: return "Use Proxy";
        // UNUSED: 306
    case HTTP_TEMPORARY_REDIRECT: return "Temporary Redirect";
    case HTTP_BAD_REQUEST: return "Bad Request";
    case HTTP_UNAUTHORIZED: return "Unauthorized";
    case HTTP_PAYMENT_REQUIRED: return "Payment Required";
    case HTTP_FORBIDDEN: return "Forbidden";
    case HTTP_NOT_FOUND: return "Not Found";
    case HTTP_METHOD_NOT_ALLOWED: return "Method Not Allowed";
    case HTTP_NOT_ACCEPTABLE: return "Not Acceptable";
    case HTTP_PROXY_AUTHENTICATION_REQUIRED: return "Proxy Authentication Required";
    case HTTP_REQUEST_TIMEOUT: return "Request Time-out";
    case HTTP_CONFLICT: return "Conflict";
    case HTTP_GONE: return "Gone";
    case HTTP_LENGTH_REQUIRED: return "Length Required";
    case HTTP_PRECONDITION_FAILED: return "Precondition Failed";
    case HTTP_REQUESTENTITYTOOLARGE: return "Request Entity Too Large";
    case HTTP_REQUESTURITOOLONG: return "Request-URI Too Large";
    case HTTP_UNSUPPORTEDMEDIATYPE: return "Unsupported Media Type";
    case HTTP_REQUESTED_RANGE_NOT_SATISFIABLE: return "Requested Range Not Satisfiable";
    case HTTP_EXPECTATION_FAILED: return "Expectation Failed";
    case HTTP_INTERNAL_SERVER_ERROR: return "Internal Server Error";
    case HTTP_NOT_IMPLEMENTED: return "Not Implemented";
    case HTTP_BAD_GATEWAY: return "Bad Gateway";
    case HTTP_SERVICE_UNAVAILABLE: return "Service Unavailable";
    case HTTP_GATEWAY_TIMEOUT: return "Gateway Time-out";
    case HTTP_VERSION_NOT_SUPPORTED: return "HTTP Version not supported";
    default:  return "???";
    }
}
    
} // namespace net
