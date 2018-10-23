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

//
// Windows platform only HTTP utility funcions
//

#if defined(_WIN32)

#include <wininet.h>
#include <string>

void MustInitializeWinInet();
void UnInitializeWinInet();

int WinHttpGet(const std::string& strUrl, std::string* outData, std::string* outErr);
int WinHttpPost(const std::string& strUrl, const std::string& postData, std::string* outData, std::string* outErr);


#endif
