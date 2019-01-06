/*
 * Copyright (c) 1997, 2019. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact xsandyguo@qq.com if you need additional information or have any
 * questions.
 *
 */

#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <windows.h>
#include "base/basictypes.h"
#include "base/scoped_any.h"

#include "runtime/common.h"
#include "runtime/synchronized.h"

namespace dip{
class DIP_EXPORT UserWorkItem {
public:
    UserWorkItem() : shutdown_event_(NULL) {}
    virtual ~UserWorkItem() {}

    // Template method interface
    void Process() { DoProcess(); }

    HANDLE shutdown_event() const { return shutdown_event_; }
    void set_shutdown_event(HANDLE shutdown_event) {
        shutdown_event_ = shutdown_event;
    }

private:
    // Executes the work item.
    virtual void DoProcess() = 0;

    // It is the job of implementers to watch for the signaling of this event
    // and shutdown correctly. This event is set when the thread pool is closing.
    // Do not close this event as is owned by the thread pool.
    HANDLE shutdown_event_;
    DISALLOW_EVIL_CONSTRUCTORS(UserWorkItem);
};

class DIP_EXPORT ThreadPool {
public:
    static ThreadPool& Instance();

    // The destructor might block for 'shutdown_delay'.
    ~ThreadPool();

    HRESULT Initialize(int shutdown_delay);

    // Returns true if any work items are still in progress.
    bool HasWorkItems() const { return (0 != work_item_count_); }

    // Adds a work item to the queue. If the add fails the ownership of the
    // work items remains with the caller.
    HRESULT QueueUserWorkItem(UserWorkItem* work_item, uint32 flags);

private:
    ThreadPool();

    // Calls UserWorkItem::Process() in the context of the worker thread.
    void ProcessWorkItem(UserWorkItem* work_item);

    // This is the thread callback required by the underlying windows API.
    static DWORD WINAPI ThreadProc(void* context);

    // Approximate number of work items in the pool.
    volatile LONG work_item_count_;

    // This event signals when the thread pool destructor is in progress.
    scoped_event shutdown_event_;

    // How many milliseconds to wait for the work items to finish when
    // the thread pool is shutting down. The shutdown delay resolution is ~10ms.
    int shutdown_delay_;

    static LLock lock_;
    static ThreadPool* instance_;

    DISALLOW_EVIL_CONSTRUCTORS(ThreadPool);
};

} // namespace dip

#endif

