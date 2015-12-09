// Copyright (C) 2014-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.


// g++ -std=c++11 -lrt -luuid

 
#include "bench.h"
#include <iostream>
#include <set>
#include <unordered_set>
#include <string>

#ifdef _WIN32
#define snprintf    sprintf_s
#endif
 
using namespace std;
 
string randomString()
{
    UUID_T uuid = createUUID();
    char s[40];
    const char* fmt = "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X";
    int c = snprintf(s, 40, fmt,
        uuid[0], uuid[1], uuid[2], uuid[3],
        uuid[4], uuid[5],
        uuid[6], uuid[7],
        uuid[8], uuid[9],
        uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
    return string(s, c);
}
 
static const int max_num = 1000000;
 
template <typename T>
void testInsert(T& set)
{
    TimeCounter counter;
    for (int i = 0; i < max_num; i++)
    {
        set.insert(randomString());
    }
}
 
template <typename T>
void testFind(T& set)
{
    TimeCounter counter;
    int64_t sum = 0;
    auto s = randomString();
    for (int i = 0; i < max_num; i++)
    {
        sum += set.count(s);
    }
    doNotOptimizeAway(sum);
}
 
int main(int argc, char* argv[])
{
    set<string> a;
    unordered_set<string> b;
 
    printf("testing insert:\n");
    printf("\t set: ");
    testInsert(a);
    printf("\n\t unordered_set: ");
    testInsert(b);
 
    printf("\n testing find:\n");
    printf("\t set: ");
    testFind(a);
    printf("\n\t unordered_set: ");
    testFind(b);
 
	return 0;
}