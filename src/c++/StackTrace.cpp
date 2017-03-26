// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <algorithm>    // std::min

#ifdef _WIN32

#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp")

#define TRACE_MAX_STACK_FRAMES 1024
#define TRACE_MAX_FUNCTION_NAME_LENGTH 1024

std::string  getStackTrace(int maxDepth)
{
    std::string result;
    void* stack[TRACE_MAX_STACK_FRAMES];
    HANDLE hProcess = GetCurrentProcess();
    SymInitialize(hProcess, NULL, TRUE);
    int numberOfFrames = CaptureStackBackTrace(0, TRACE_MAX_STACK_FRAMES, stack, NULL);
    maxDepth = (maxDepth > 0 ? std::min(maxDepth, numberOfFrames) : numberOfFrames);
    SYMBOL_INFO* symbol = (SYMBOL_INFO *)malloc(sizeof(SYMBOL_INFO) +
        (TRACE_MAX_FUNCTION_NAME_LENGTH - 1) * sizeof(TCHAR));
    symbol->MaxNameLen = TRACE_MAX_FUNCTION_NAME_LENGTH;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    DWORD displacement;
    IMAGEHLP_LINE64* line = (IMAGEHLP_LINE64*)malloc(sizeof(IMAGEHLP_LINE64));
    line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    for (int i = 0; i < numberOfFrames; i++)
    {
        char text[256];
        DWORD64 address = (DWORD64)(stack[i]);
        SymFromAddr(hProcess, address, NULL, symbol);
        if (SymGetLineFromAddr64(hProcess, address, &displacement, line))
        {
            sprintf_s(text, 256, "%s[%d]: %s() at 0x%p.\n", line->FileName, line->LineNumber, 
                symbol->Name, symbol->Address);
        }
        else
        {
            sprintf_s(text, 256, "unknown file: %s() at 0x%p.\n", symbol->Name, symbol->Address);
        }
        result += text;
    }
    free(line);
    CloseHandle(hProcess);
    return result;
}

#else
#include <execinfo.h>
#include <stdio.h>

// compile with -rdynamic
std::string getStackTrace(int maxDepth)
{
    std::string result;
    static const int kStackLength = 64;
    void* stack[kStackLength];
    int size = backtrace(stack, kStackLength);
    maxDepth = (maxDepth > 0 ? std::min(maxDepth, size) : size);
    char** symbols = backtrace_symbols(stack, maxDepth);
    if (symbols)
    {
        for (int i = 0; i < maxDepth; i++)
        {
            char text[256];
            snprintf(text, 256, "%s\n", symbols[i]);
            result += text;
        }
        free(symbols);
    }
    return result;
}

#endif