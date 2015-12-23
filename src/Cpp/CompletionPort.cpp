// Copyright (C) 2011-2015 ichenq@gmail.com. All rights reserved.
// Distributed under the terms and conditions of the Apache License.
// See accompanying files LICENSE.

#include "CompletionPort.h"
#include "Logging.h"


CompletionPort::CompletionPort(size_t maxConcurrency /* = 0 */)
{
    hIocp_ = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, maxConcurrency);
}

CompletionPort::~CompletionPort()
{
    ::CloseHandle(hIocp_);
    hIocp_ = 0;
}

bool CompletionPort::AssociateDevice(HANDLE hDevice, ULONG_PTR completionKey)
{
    if (hIocp_ != ::CreateIoCompletionPort(hDevice,
                                           hIocp_,
                                           completionKey,
                                           0) )
    {
        LOG(DEBUG) << LAST_ERROR_MSG;
        return false;
    }
    return true;
}

bool CompletionPort::PostStatus(ULONG_PTR completionKey,
                                  OVERLAPPED *pOverlapped /* = NULL */,
			                      DWORD dwNumBytes /* = 0 */) 
{
    if (!::PostQueuedCompletionStatus(hIocp_, 
                                      dwNumBytes, 
                                      completionKey, 
                                      pOverlapped))
    {
        LOG(DEBUG) << LAST_ERROR_MSG;
        return false;
    }
    return true;
}

DWORD CompletionPort::GetStatus(ULONG_PTR *pCompletionKey,
                                  OVERLAPPED **ppOverlapped,
				                  PDWORD pdwNumBytes,
                                  DWORD dwMilliseconds)
{
    if (0 == ::GetQueuedCompletionStatus(hIocp_,
						                 pdwNumBytes, 
						                 pCompletionKey, 
						                 ppOverlapped, 
                                         dwMilliseconds))
	{
        DWORD last_error = ::GetLastError();
        if (last_error != WAIT_TIMEOUT)
        {
            return last_error;
        }
	}

    return S_OK;
}

DWORD CompletionPort::GetStatus(ULONG_PTR *pCompletionKey,
                                  OVERLAPPED **ppOverlapped,
				                  PDWORD pdwNumBytes)
{
    return GetStatus(pCompletionKey, ppOverlapped, pdwNumBytes, INFINITE);
}
