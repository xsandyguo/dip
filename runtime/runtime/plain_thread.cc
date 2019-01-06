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

#include "runtime/plain_thread.h"

#include <algorithm>

#include "base/debug.h"
#include "runtime/image_window.h"
#include "runtime/thread_pool.h"
#include "runtime/thread_pool_callback.h"

namespace dip{

BEGIN_REGISTER_CLASSINFO( PlainThread, Thread, \
    "{918F2A0F-4E12-45E4-A0AA-94DA8FF29A20}")

    PROPERTY(unsigned int, "execution interval", get_interval, set_interval)
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : Could be connected with any process that independens of message.")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO( PlainThread)


PlainThread::PlainThread(void): Thread(4),
                            is_running_(false),
                            thread_id_(NULL),
                            interval_(0){
    
    reset(busy_event_, ::CreateEvent(NULL, true, false, NULL));
    ASSERT1(busy_event_);
}

PlainThread::~PlainThread(void){
}


bool PlainThread::CanConnect(IConnectable *prev, int outIdx, int inIdx){
    return prev->GetType() == DOT_MAPPER;
}

void PlainThread::Connect(IConnectable *prev, int outIdx, int inIdx)   {
    ASSERT1(CanConnect(prev, outIdx, inIdx));

    IMapper* p = dynamic_cast<IMapper*>(prev);
    mappers_.push_back(p);
}

void PlainThread::Disconnect(IConnectable *prev, int outIdx, int inIdx){
    IMapper* p = dynamic_cast<IMapper*>(prev);
    MapperIter pos = std::find(mappers_.begin(), mappers_.end(), p);
    if(pos != mappers_.end()){
        mappers_.erase(pos);
    }
}

bool PlainThread::DoRun(){          
    typedef ThreadPoolCallBack0<PlainThread> Callback;
    scoped_ptr<Callback> callback(
        new ThreadPoolCallBack0<PlainThread>(this, &PlainThread::DoProcess));
    HRESULT hr = ThreadPool::Instance().QueueUserWorkItem(callback.get(),
        WT_EXECUTELONGFUNCTION);

    bool ret;
    if( ret = SUCCEEDED(hr)){
        callback.release();
    }

    return ret;
}

int PlainThread::GetValidInNum(){
    return  static_cast<int>(mappers_.size());
}

void PlainThread::DoStopMapper(){
    for(MapperIter itor = mappers_.begin(); itor != mappers_.end();
        ++itor){
        IMapper* p = *itor;

        bool normal = false;
        try{
            normal = p->Dispose();
        }catch (...){
            normal = false;
        }

        if(!normal){
            LOG_ERROR(_T("[PlainThread::DoProcess execute process %s failed]"),
                p->GetObject().get_name().c_str());
        }                
    }
}

bool PlainThread::DoRunMapper(){
    __mutexScope(lock_);
    for(MapperIter itor = mappers_.begin(); itor != mappers_.end();
        ++itor){
        IMapper* p = *itor;

        bool normal = false;
        try{
            normal = p->Execute();
        }catch (...){
            normal = false;
        }

        if(!normal){
            LOG_ERROR(_T("[PlainThread::DoProcess execute process %s failed]"),
                p->GetObject().get_name().c_str());
            return false;
        }
    }        

    return true;
}

void PlainThread::DoProcess(){
    is_signed_stopped_ = false;
    VERIFY1(::ResetEvent(get(busy_event_)));

    thread_id_ = GetCurrentThreadId();

    for(;;){
        if(is_signed_stopped_){
            DoStopMapper();
            break;
        }
        
        if(!DoRunMapper()){
            break;
        }

        ::Sleep(interval_);
    }
    
    thread_id_ = 0;

    AsyncStopCompleted();
}

bool PlainThread::DoStop(){
    __mutexScope(lock_);
    is_signed_stopped_ = true;

    return true;
}

} // namespace dip