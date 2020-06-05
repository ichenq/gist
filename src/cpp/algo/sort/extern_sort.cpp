// Copyright (C) 2016 ichenq@outlook.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <memory>
#include <algorithm>

//
// 外部排序，多路归并
//

// limited memory usage
const int MEM_BUFFER_SIZE = 1024 * 1024 * 8; // 8M

int64_t GetFileSize(const char* filename)
{
    FILE* fp = fopen(filename, "rb");
    if (fp != nullptr) {
        fseek(fp, 0L, SEEK_END);
        return ftell(fp);
    }
    return 0;
}

int64_t readIntFromFile(const char* filename, int64_t offset, int list[], int count)
{
    std::ifstream fin(filename, std::ios::in);
    assert(fin.is_open());
    fin.seekg(offset);
    size_t i = 0;
    int64_t bytesRead = 0;
    std::string s;
    while (fin >> s) {
        bytesRead += s.size() + 1; // plus a delim
        int n = atoi(s.c_str());
        list[i++] = n;
        if (i >= (size_t)count) {
            break;
        }
    }
    return bytesRead;
}

void writeIntToFile(int list[], int count, const char* filename)
{
    std::ofstream out(filename, std::ios::out | std::ios::app);
    assert(out.is_open());
    for (int i = 0; i < count; i++) {
        const std::string& s = std::to_string(list[i]);
        out << s << "\n";
    }
    out.close();
}

int readLineToInt(std::shared_ptr<std::ifstream> ifs) {
    std::string s;
    (*ifs) >> s;
    return atoi(s.c_str());
}

// `filename是以行分隔的整数集合`
void ExternalSort(const char* filename)
{
    std::vector<int> list;
    list.resize(MEM_BUFFER_SIZE / sizeof(int));

    int64_t maxSize = GetFileSize(filename);
    int part = 0;
    int64_t offset = 0;
    while (offset < maxSize) {
        // 分解成n个有序的小文件
        offset += readIntFromFile(filename, offset, &list[0], list.size());
        std::sort(list.begin(), list.end());

        char partFilename[512];
        sprintf(partFilename, "%s.sorted-part.%d", filename, ++part);
        writeIntToFile(&list[0], list.size(), partFilename);
    }

    // 合并n个有序的小文件成1个有序的大文件
    char sortedFilename[512];
    sprintf(sortedFilename, "%s.sorted");

    std::vector<std::shared_ptr<std::ifstream>> iflist;
    for (int i = 0; i < part; i++) {
        char partFilename[512];
        sprintf(partFilename, "%s.sorted-part.%d", filename, ++part);
        std::ifstream fin(partFilename, std::ios::in);
    }
    std::vector<int> tmp;
    tmp.resize(part);
    for (int i = 0; i < part; i++) {
        tmp[i] = readLineToInt(iflist[i]);
    }
    int j = 0;
    bool canContinue = true;
    while (canContinue) {
        auto iter = std::min_element(tmp.begin(), tmp.end());
        int idx = std::distance(tmp.begin(), iter);
        list[j++] = tmp[idx];
        if (j >= list.size()) {
            writeIntToFile(&list[0], list.size(), sortedFilename);
        }
        tmp[idx] = readLineToInt(iflist[idx]);

        canContinue = false;
        for (int i = 0; i < part; i++) {
            if (iflist[i]->good()) {
                canContinue = true;
                break;
            }
        }
    }
}

void makeBigDataFile(int count, const char* filename)
{
    std::random_device rd;
    std::default_random_engine prg(rd());

    std::vector<int> list;
    list.resize(MEM_BUFFER_SIZE / sizeof(int));

    while (count > 0) {
        for (size_t i = 0; i < list.size(); i++) {
            list[i] = prg();
        }
        writeIntToFile(&list[0], list.size(), filename);
        count -= (int)list.size();
    }
}

void simple_test()
{
    const char* source = "bigdata.dat";
    //makeBigDataFile(10000000, source);
    ExternalSort(source);
}
