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

#include "runtime/synchronized.h"

#include "base/debug.h"
namespace dip{

SimpleMutexLock::SimpleMutexLock(const wchar_t* name)
:lock_(NULL){
    lock_ = ::CreateMutex(NULL, false, name);
}

SimpleMutexLock::~SimpleMutexLock(){
    if(lock_){
        Unlock();
    }
}

MutexLock::MutexLock(){

}

void SimpleMutexLock::Lock()  const{
    DWORD ret = ::WaitForSingleObject(lock_, INFINITE);
    /* if (ret == WAIT_OBJECT_0) {
    return true;
    } else if (ret == WAIT_ABANDONED) {        
    return true;
    }
    return false;*/
}

void SimpleMutexLock::Unlock() const{
    ::ReleaseMutex(lock_);
    lock_ = NULL;
}

LLock::LLock(){
    InitializeCriticalSection(&critical_sec_);
}

LLock::~LLock(){
    DeleteCriticalSection(&critical_sec_);
}

void LLock::Lock() const{
    EnterCriticalSection(&critical_sec_);
}

void LLock::Unlock() const{
    LeaveCriticalSection(&critical_sec_);
}


// c-tor will take mutex.
AutoSync::AutoSync(const Lockable *pLock)
    : lock_(pLock),
    first_time_(true) {
        ASSERT1(lock_);
        lock_->Lock();
}

// c-tor will take mutex.
AutoSync::AutoSync(const Lockable &rLock)
    : lock_(&rLock),
    first_time_(true) {
        ASSERT1(lock_);
        lock_->Lock();
}

// d-tor will release mutex.
AutoSync::~AutoSync() {
    ASSERT1(lock_);
    lock_->Unlock();
}

// Allows to write the for loop of __mutexBlock macro
bool AutoSync::FirstTime() {
    if (first_time_) {
        first_time_ = false;
        return true;
    }
    return false;
}
} // namespace dip