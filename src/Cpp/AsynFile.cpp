// Copyright (C) 2011-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include "AsynFile.h"
#include <assert.h>
#include <string.h>
#include <process.h>
#include <algorithm>
#include "Logging.h"

using std::string;
using std::vector;
using namespace std::placeholders;

CompletionPort    AsynFile::completion_port_;


//////////////////////////////////////////////////////////////////////////
AsynFile::AsynFile(IOCompletionPort& completion_port)
{
}

AsynFile::~AsynFile()
{
}


bool AsynFile::Write(const std::string& filename, const void* data, size_t size, CompleteCallback callback)
{
    assert(data && size);
    HANDLE hFile = ::CreateFileA(filename.c_str(), GENERIC_WRITE, 0, NULL,
        OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        LOG(ERROR) << LAST_ERROR_MSG;
        return false;
    }

    AsynFileOper* pOperation = AllocOper(size);
    pOperation->file_handle = hFile;
    pOperation->filename = filename;
    pOperation->onComplete = callback;
    pOperation->oper_type = ASYN_READ;
    memcpy(pOperation->data, data, size);
    
    if (!completion_port_.AssociateDevice(hFile, (ULONG_PTR)pOperation))
    {
        LOG(ERROR) << LAST_ERROR_MSG;
        FreeOper(pOperation, true);
        return false;
    }

    DWORD bytes_transferred = 0;
    if (!::WriteFile(hFile, pOperation->data, size, &bytes_transferred,
        &pOperation->overlap))
    {
        if (::GetLastError() != ERROR_IO_PENDING)
        {
            LOG(ERROR) << LAST_ERROR_MSG;
            FreeOper(pOperation, true);
            return false;
        }
    }
    return true;
}


bool AsynFile::Read(const std::string& filename, CompleteCallback callback)
{
    HANDLE hFile = ::CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ,
        NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        LOG(ERROR) << LAST_ERROR_MSG;
        return false;
    }
    size_t buffer_size = ::GetFileSize(hFile, NULL); // limit to 4Gb
    AsynFileOper* pOperation = AllocOper(buffer_size);
    pOperation->file_handle = hFile;
    pOperation->filename = filename;
    pOperation->onComplete = callback;
    pOperation->oper_type = ASYN_READ;

    if (!completion_port_.AssociateDevice(hFile, (ULONG_PTR)pOperation))
    {
        LOG(ERROR) << LAST_ERROR_MSG;
        FreeOper(pOperation, true);
        return false;
    }

    DWORD bytes_transferred = 0;
    if (!::ReadFile(hFile, pOperation->data, buffer_size, &bytes_transferred,
        &pOperation->overlap))
    {
        if (::GetLastError() != ERROR_IO_PENDING)
        {
            LOG(ERROR) << LAST_ERROR_MSG;
            FreeOper(pOperation, true);
            return false;
        }
    }
    return true;
}

bool AsynFile::ReadDir(const string& dir, const string& ext, CompleteCallback callback)
{
    if (callback == NULL || dir.empty() || ext.empty() || *ext.rbegin() == '.')
    {
        return false;
    }

    FindFileReq* pReq = new FindFileReq;
    pReq->dir = dir;
    pReq->ext = ext;
    pReq->onComplete = callback;

    string& mydir = pReq->dir;
    std::for_each(mydir.begin(), mydir.end(), tolower);
    std::replace(mydir.begin(), mydir.end(), '/', '\\');
    if (*mydir.rbegin() != '\\')
    {
        mydir.push_back('\\');
    }

    AsynFileOper* pOperation = AllocOper(0);
    pOperation->data = (char*)pReq;
    pOperation->filename = mydir;
    pOperation->onComplete = std::bind(&AsynFile::AsynFindDir, this, _1, _2, _3);
    bool post_ok = completion_port_.PostStatus((ULONG_PTR)pOperation, &pOperation->overlap);
    if (!post_ok)
    {
        delete pReq;
    }
    return post_ok;
}


bool AsynFile::ReadDirWithExt(const FindFileReq& req)
{
    string mydir = req.dir + "*.*";
    WIN32_FIND_DATAA  file_data = {};
    HANDLE hFind = ::FindFirstFileA(mydir.c_str(), &file_data);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        LOG(ERROR) << LAST_ERROR_MSG;
        return false;
    }
    do
    {
        string filename = file_data.cFileName;
        if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (filename != "." && filename != "..")
            {
                filename = req.dir + filename;
                filename.push_back('\\');
                FindFileReq subreq = {filename, req.ext, req.onComplete};
                ReadDirWithExt(subreq);
            }
        }
        else
        {
            if (file_data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
            {
                if (IsFileExtMatch(filename, req.ext))
                {
                    Read(req.dir + filename, req.onComplete);
                }
            }
            
        }
    }while (::FindNextFileA(hFind, &file_data));

    ::FindClose(hFind);
    return true;
}

bool AsynFile::IsFileExtMatch(const string& filename, const string& ext)
{
    assert(!filename.empty() && !ext.empty() && ext[0] == '.');
    if (ext == ".*")
    {
        return true;
    }
    int pos = filename.find_last_of('.');
    if (pos == string::npos)
    {
        return false;
    }
    return std::equal(filename.begin() + pos, filename.end(), ext.begin());
}
                           

AsynFile::AsynFileOper* AsynFile::AllocOper(size_t buffer_size)
{    
    AsynFileOper* pOperation = new AsynFileOper;
    memset(pOperation, 0, sizeof(*pOperation));
    if (buffer_size > 0)
    {
        pOperation->data = new char[buffer_size];        
    }
    return pOperation;
}

void AsynFile::FreeOper(AsynFileOper* ptr, bool is_free_buffer)
{
    if (ptr)
    {
        if (is_free_buffer)
        {
            delete ptr->data;
        }
        ::CloseHandle(ptr->file_handle);
        delete ptr;
    }
}

void AsynFile::AsynFindDir(const std::string& dir, char* data, size_t size)
{
    assert(data);
    FindFileReq* pReq = (FindFileReq*)data;
    ReadDirWithExt(*pReq);

    AsynFileOper* pOperation = AllocOper(0);
    pOperation->filename = dir;
    pOperation->onComplete = pReq->onComplete;
    completion_port_.PostStatus((ULONG_PTR)pOperation, &pOperation->overlap);
    delete pReq;
}

bool AsynFile::Run()
{
    const int TIME_OUT_MILSEC = 500;
    DWORD bytes_transferred = 0;
    OVERLAPPED* pOverlapped = NULL;
    AsynFileOper* pOperation = NULL;
    DWORD error = completion_port_.GetStatus((ULONG_PTR*)&pOperation,
        &pOverlapped, &bytes_transferred);
    if (error)
    {
        if (pOverlapped != NULL)
        {
            FreeOper(pOperation, true);
        }
        if (error == ERROR_INVALID_HANDLE)
        {
            return false;
        }
    }
    else
    {
        CompleteCallback callback = pOperation->onComplete;
        if (callback)
        {
            callback(pOperation->filename, pOperation->data, bytes_transferred);
        }
        // data buffer must be freed by completion callback routine
        FreeOper(pOperation, false); 
    }
    return true;
}
