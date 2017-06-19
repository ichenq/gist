// Copyright (C) 2013-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.
//
// Run-length encoding and decoding
// Created at Oct 23, 2013


#include <assert.h>
#include <vector>


using std::vector;

typedef unsigned char BYTE;
typedef std::vector<BYTE>   Buffer;


#define MAX_RUN_LEN     8
#define MIN_RLE_BYTE    (0xFF - MAX_RUN_LEN)



// 
inline void XOR(BYTE* input, int length)
{
    assert(input && length > 0);
    int i = 0;
    for (; i < length-4; i+=4)
    {
        *((unsigned*)&input[i]) ^= 0x329FA929;
    }
    for (; i < length; ++i)
    {
        input[i] ^= 0x8F;
    }
}

inline void EncodeChar(BYTE ch, int length, Buffer& buffer)
{
    assert(length > 0 && length <= MAX_RUN_LEN);
    if (length == 1)
    {
        if (ch > MIN_RLE_BYTE)
        {
            buffer.push_back(MIN_RLE_BYTE+1);
        }
    }
    else
    {
        buffer.push_back(MIN_RLE_BYTE + length);
    }
    buffer.push_back(ch);
}

void EncodeString(const BYTE* input, int length, Buffer& buffer)
{
    assert(input && length > 0);
    BYTE pre_ch = input[0];
    int run_len = 1;
    for (int i = 1; i <= length; ++i)
    {
        BYTE ch = input[i];
        if (ch == pre_ch && run_len < MAX_RUN_LEN)
        {
            ++run_len;
        }
        else
        {
            EncodeChar(ch, run_len, buffer);
            run_len = 1;
        }
        pre_ch = ch;
    }

    if (!buffer.empty())
    {
        XOR(&buffer[0], buffer.size());
    }
}


inline void DecodeChar(BYTE ch, int length, Buffer& out)
{
    for (int i = 0; i < length; ++i)
    {
        out.push_back(ch);
    }
}

void DecodeString(BYTE* input, int length, Buffer& out)
{
    XOR(input, length);
    for (int i = 0; i < length; ++i)
    {
        BYTE ch = input[i];
        if (ch > MIN_RLE_BYTE)
        {
            assert(i+1 < length);
            DecodeChar(input[i+1], ch - MIN_RLE_BYTE, out);
            ++i;
        }
        else
        {
            DecodeChar(ch, 1, out);
        }
    }
}