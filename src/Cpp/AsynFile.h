// Copyright (C) 2011-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#pragma once


#include "CompletionPort.h"
#include <vector>
#include <string>
#include <functional>

class AsynFile
{
public:
    // @notice          
    //      1, Callback routine is responsible for data buffer's deallocating.
    //      2, Callback routine DONOT handle any synchronous issue.
    //      3, `data` and `size` with value 0 indicates the total work had been done(for `ReadDir`).
    typedef std::function< void(const std::string& file, char* data, size_t size)> CompleteCallback;

    enum OperationType
    {
        ASYN_READ = 1,
        ASYN_WRITE = 2,
    };

    struct FindFileReq
    {
        std::string         dir;            // file directory
        std::string         ext;            // file extension
        CompleteCallback    onComplete;     // completion callback routine
    };

    struct AsynFileOper
    {
        OVERLAPPED          overlap;        // overlap structure
        HANDLE              file_handle;    // file handle
        CompleteCallback    onComplete;     // completion callback routine
        std::string         filename;       // file name
        char*               data;           // data buffer
        OperationType       oper_type;      // operation type
    };

public:
    AsynFile(IOCompletionPort& completion_port);
    ~AsynFile();

    bool Run();

    // Write data to file asynchronously   
    bool Write(const std::string& filename, 
                      const void* data, 
                      size_t size, 
                      CompleteCallback callback);


    // Read data from file asynchronously
    bool Read(const std::string& filename, 
                     CompleteCallback callback);

    // Read files with extion from directory recursively
    bool ReadDir(const std::string& dir, 
                        const std::string& ext, 
                        CompleteCallback callback);

    // 
    bool IsFileExtMatch(const std::string& filename, 
                               const std::string& ext);

private:
    // Find files with specific extention callback
    void AsynFindDir(const std::string& dir, char* data, size_t size);

    // Read directory
    bool ReadDirWithExt(const FindFileReq& req);

    AsynFileOper*    AllocOper(size_t buffer_size);
    void             FreeOper(AsynFileOper* ptr, bool is_free_buffer);

private:
    static CompletionPort    completion_port_;
};
