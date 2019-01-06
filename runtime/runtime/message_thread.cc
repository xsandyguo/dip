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

#include "runtime/message_thread.h"

#include <algorithm>
#include "base/debug.h"
#include "base/scoped_ptr.h"

#include "runtime/image_window.h"
#include "runtime/thread_pool.h"
#include "runtime/thread_pool_callback.h"

namespace dip{

BEGIN_REGISTER_CLASSINFO( MessageThread, Thread, \
    "{AA29A9AA-2431-4DA3-9E0C-F9FE4060CA33}")
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : Used to power windows and other needs message process.")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO( MessageThread)

MessageThread::MessageThread(void): Thread(5),
                            thread_id_(NULL){    
}

MessageThread::~MessageThread(void){
}

MessageThread::RenderCollection& MessageThread::GetRenders(){
    return renders_;
}

void MessageThread::AddRender(IRender *win){
    renders_.push_back(win);
}

void MessageThread::RemoveRender(IRender *win){
    RenderIter pos = std::find(renders_.begin(), renders_.end(), win);

    if(pos != renders_.end()){
        renders_.erase(pos);
    }
}

bool MessageThread::DoRun(){           
    typedef ThreadPoolCallBack0<MessageThread> Callback;
    scoped_ptr<Callback> callback(
        new ThreadPoolCallBack0<MessageThread>(this, &MessageThread::DoProcess));
    HRESULT hr = ThreadPool::Instance().QueueUserWorkItem(callback.get(),
                     WT_EXECUTELONGFUNCTION);

    bool ret;
    if( ret = SUCCEEDED(hr)){
        callback.release();
    }

    return ret;
}

void MessageThread::DoProcess(){
    thread_id_ = GetCurrentThreadId();
    bool status = true;
    RenderCollection renders;
    __mutexBlock(lock_){
        renders = GetRenders();
    }

    for(RenderIter itor = renders.begin(); itor != renders.end();
        ++itor){
        IRender* win = *itor;

        try{
            if(!win->Initialize() || !win->Execute()){
                status = false;
            }
        }catch(...){
            status = false;
        }

        if(!status){ 
            LOG_ERROR(_T("[MessageThread::DoProcess show window %s failed]"), 
                      win->GetObject().get_name().c_str());
            break; 
        }
    }

    if(status){
        MSG msg;
        while(GetMessage(&msg, NULL, 0, 0)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    thread_id_ = 0;

    AsyncStopCompleted();
}

bool MessageThread::DoStop(){
    RenderCollection renders;
    __mutexBlock(lock_){
        renders = GetRenders();
    }

    for(RenderIter itor = renders.begin(); itor != renders.end();
        ++itor){
        IRender* win = *itor;
        if(!win->Dispose()){
            LOG_ERROR(_T("[MessageThread::Stop][Dispose window failed.][%s]"),
                      win->GetObjectW().get_name().c_str());
        }
    }
    ASSERT1(thread_id_);

    PostThreadMessage(thread_id_, WM_QUIT, NULL, NULL);    

    return true;
}

bool MessageThread::CanConnect(IConnectable *prev, int outIdx, int inIdx){
    return prev->GetType() == DOT_WINDOW;
}

void MessageThread::Connect(IConnectable *prev, int outIdx, int inIdx){
    ASSERT1(CanConnect(prev, outIdx, inIdx));

    AddRender(dynamic_cast<IRender*>(prev));
}

void MessageThread::Disconnect(IConnectable *prev, int outIdx, int inIdx){
    RemoveRender(dynamic_cast<IRender*>(prev));
}

int MessageThread::GetValidInNum(){
    return static_cast<int>(renders_.size());
}

} // namespace dip