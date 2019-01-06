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

#include "runtime/thread.h"
#include <algorithm>

#include "base/debug.h"
#include "runtime/vm.h"

namespace dip{

BEGIN_REGISTER_ABSTRACT_CLASSINFO( Thread, Presentation, \
    "{A60ED890-D205-4893-966A-A1A0FDC549FF}")
END_REGISTER_CLASSINFO( Thread)

Thread::Thread(int in_num):Presentation(in_num, 0),
    is_running_(false),
    is_stopping_(false){
    SetRequriedInNum(1);
    
    reset(syc_stop_event_, ::CreateEvent(NULL, false, false, NULL));
    ASSERT1(valid(syc_stop_event_));
}

void Thread::Register(){
    VM::Instance().Add(this);
}

void Thread::UnRegister(){
    VM::Instance().Remove(this);
}

void Thread::Stop(){
    if(!is_running_){
        return;
    }

    if(is_stopping_){
        return;
    }else{
        is_stopping_ = true;
    }

    DoStop();

    DWORD ret = ::WaitForSingleObject(get(syc_stop_event_), 10 * 1000);
    ASSERT1(ret == WAIT_OBJECT_0);

    is_stopping_ = false;
    is_running_ = false;
}

bool Thread::Run(){
    if(is_running_ || !GetValidInNum()){
        return false;
    }else{
        is_running_ = true;
    }

    return DoRun();    
}

void Thread::AsyncStopCompleted(){
    ::SetEvent(get(syc_stop_event_));
}

} // namespace dip