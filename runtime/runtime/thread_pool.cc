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

#include "runtime/thread_pool.h"

#include "base/debug.h"
#include "base/error.h"
#include "base/scoped_ptr.h"

namespace dip{
// Context keeps track the information necessary to execute a work item
// inside a thread pool thread.
class Context {
public:
    Context(ThreadPool* pool, UserWorkItem* work_item)
        : pool_(pool),
        work_item_(work_item) {
            ASSERT1(pool);
            ASSERT1(work_item);
    }

    ThreadPool*   pool() const { return pool_; }
    UserWorkItem* work_item() const { return work_item_; }

private:
    ThreadPool*   pool_;
    UserWorkItem* work_item_;

    DISALLOW_EVIL_CONSTRUCTORS(Context);
};

// Returns true if delta time since 'baseline' is greater or equal than
// 'milisecs'. Note: GetTickCount wraps around every ~48 days.
bool TimeHasElapsed(DWORD baseline, DWORD milisecs) {
    DWORD current = ::GetTickCount();
    DWORD wrap_bias = 0;
    if (current < baseline) {
        wrap_bias = static_cast<DWORD>(0xFFFFFFFF);
    }
    return (current - baseline + wrap_bias) >= milisecs ? true : false;
}



DWORD WINAPI ThreadPool::ThreadProc(void* param) {
    ASSERT1(param);
    Context* context = static_cast<Context*>(param);
    context->pool()->ProcessWorkItem(context->work_item());
    delete context;
    return 0;
}

ThreadPool::ThreadPool()
    : work_item_count_(0),
    shutdown_delay_(0) {
}

ThreadPool::~ThreadPool() {
    if (!shutdown_event_) {
        return;
    }

    DWORD baseline_tick_count = ::GetTickCount();
    if (::SetEvent(get(shutdown_event_))) {
        while (work_item_count_ != 0) {
            ::Sleep(1);
            if (TimeHasElapsed(baseline_tick_count, shutdown_delay_)) {
                break;
            }
        }
    }
}

HRESULT ThreadPool::Initialize(int shutdown_delay) {
    shutdown_delay_ = shutdown_delay;
    reset(shutdown_event_, ::CreateEvent(NULL, true, false, NULL));
    return shutdown_event_ ? S_OK : HRESULTFromLastError();
}

void ThreadPool::ProcessWorkItem(UserWorkItem* work_item) {
    ASSERT1(work_item);
    work_item->Process();
    delete work_item;
    ::InterlockedDecrement(&work_item_count_);
}

HRESULT ThreadPool::QueueUserWorkItem(UserWorkItem* work_item, uint32 flags) {
    ASSERT1(work_item);

    scoped_ptr<Context> context(new Context(this, work_item));
    work_item->set_shutdown_event(get(shutdown_event_));
    ::InterlockedIncrement(&work_item_count_);
    if (!::QueueUserWorkItem(&ThreadPool::ThreadProc, context.get(), flags)) {
        ::InterlockedDecrement(&work_item_count_);
        return HRESULTFromLastError();
    }

    // The thread pool has the ownership of the work item thereon.
    context.release();
    return S_OK;
}

ThreadPool* ThreadPool::instance_ = NULL;
LLock ThreadPool::lock_;

ThreadPool& ThreadPool::Instance(){
    if(!instance_){
        __mutexScope(lock_);
        if(!instance_){
            instance_ = new ThreadPool();
            HRESULT hr = instance_->Initialize(5000);
            ASSERT1(SUCCEEDED(hr));
        }
    }

    return *instance_;
}

} // namespace dip