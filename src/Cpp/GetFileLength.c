// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <stdint.h>
#include <stdio.h>

int64_t GetFileLength(const char* filename)
{
    assert(filename);
    FILE* fp = fopen(filename, "r");
    if (fp)
    {
        int64_t length = 0;
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        return length;
    }
    return 0;
}