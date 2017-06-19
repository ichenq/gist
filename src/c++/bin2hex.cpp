// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

// Binary to hex present
std::string bin2hex(const void* data, size_t len)
{
    std::string result;
    result.reserve(len * 2);
    static const char hex[] = "0123456789abcdef";
    for (size_t i = 0; i < len; i++)
    {
        char ch = ((const char*)data)[i];
        result += hex[ch >> 4];
        result += hex[ch & 0x0f];
    }
    return std::move(result);
}