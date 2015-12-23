// Copyright (C) 2011-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include <Windows.h>

class CompletionPort 
{
public:   
    explicit CompletionPort(size_t maxConcurrency = 0);
    ~CompletionPort();

	bool AssociateDevice(HANDLE hDevice, ULONG_PTR completionKey);

    bool PostStatus(ULONG_PTR completionKey,
                    OVERLAPPED* pOverlapped = NULL,
                    DWORD dwNumBytes = 0);
		            
    DWORD GetStatus(ULONG_PTR* pCompletionKey,
                    OVERLAPPED** ppOverlapped,
		            PDWORD pdwNumBytes);

    DWORD GetStatus(ULONG_PTR* pCompletionKey,
                    OVERLAPPED** ppOverlapped,
		            PDWORD pdwNumBytes,
		            DWORD dwMilliseconds );

private:      
	HANDLE      hIocp_;
};
