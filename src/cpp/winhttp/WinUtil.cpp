// Copyright 2018-preset Beyondtech Inc. All Rights Reserved.
//
// Any redistribution or reproduction of part or all of the contents in any form
// is prohibited.
//
// You may not, except with our express written permission, distribute or commercially
// exploit the content. Nor may you transmit it or store it in any other website or
// other form of electronic retrieval system.

#include "stdafx.h"
#include "WinUtil.h"

#if defined(_WIN32)

std::string GetLastErrorMessage(DWORD dwError)
{
    // this should be thread local
    char szText[1024] = {};
    DWORD dwRet = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, szText, 1024, NULL);
    szText[dwRet] = '\0';
    return std::string(szText, dwRet);
}

std::wstring MultibyteToWide(const std::string& strMultibyte, unsigned int codePage /*= CP_ACP*/)
{
    std::wstring strWide;
    int count = MultiByteToWideChar(codePage, 0, strMultibyte.data(), (int)strMultibyte.length(), NULL, 0);
    if (count > 0)
    {
        strWide.resize(count);
        MultiByteToWideChar(codePage, 0, strMultibyte.data(), (int)strMultibyte.length(),
            const_cast<wchar_t*>(strWide.data()), (int)strWide.length());
    }
    return strWide;
}

std::string WideToMultibyte(const std::wstring& strWide, unsigned int codePage /*= CP_ACP*/)
{
    std::string strMultibyte;
    int count = WideCharToMultiByte(codePage, 0, strWide.data(), (int)strWide.length(), NULL, 0, NULL, NULL);
    if (count > 0)
    {
        strMultibyte.resize(count);
        WideCharToMultiByte(codePage, 0, strWide.data(), (int)strWide.length(),
            const_cast<char*>(strMultibyte.data()), (int)strMultibyte.length(), NULL, NULL);
    }
    return strMultibyte;
}

#endif // _WIN32
