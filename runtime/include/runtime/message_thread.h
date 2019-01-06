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

#ifndef DIP_THREAD_WIN32_H_
#define DIP_THREAD_WIN32_H_

#include <windows.h>
#include "base/basictypes.h"
#include "base/scoped_any.h"

#include "runtime/common.h"
#include "runtime/thread.h"

namespace dip{

class MessageThread : public Thread{
public:
    DECLEAR_CLASSINFO(MessageThread, Thread)

    virtual bool CanConnect(IConnectable *prev, int outIdx, int inIdx);
    virtual void Connect(IConnectable *prev, int outIdx, int inIdx)   ;
    virtual void Disconnect(IConnectable *prev, int outIdx, int inIdx);
    virtual int GetValidInNum();

    virtual void AddRender(IRender* render)   ;
    virtual void RemoveRender(IRender* render);
    
protected:
    MessageThread(void);
    virtual ~MessageThread(void);

    void DoProcess();
    bool DoStop();
    bool DoRun();

    typedef std::vector<IRender*>::iterator RenderIter;
    typedef std::vector<IRender*> RenderCollection;

    RenderCollection& GetRenders();

private:
    RenderCollection renders_;
    DWORD thread_id_;
    LLock lock_;

    DISALLOW_EVIL_CONSTRUCTORS(MessageThread);
};

} // namespace dip

#endif