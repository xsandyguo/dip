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
#include "runtime/framework.h"
#include "runtime/synchronized.h"
#include "runtime/thread.h"
#include "runtime/thread_pool.h"

namespace dip{
class PlainThread : public Thread{
public:
    DECLEAR_CLASSINFO(PlainThread, Thread)

    virtual bool CanConnect(IConnectable *prev, int outIdx, int inIdx);
    virtual void Connect(IConnectable *prev, int outIdx, int inIdx)   ;
    virtual void Disconnect(IConnectable *prev, int outIdx, int inIdx);
    SetMacro(interval, unsigned int)
    GetMacro(interval, unsigned int)

    bool IsRunning(){ return !!is_running_; }

    virtual int GetValidInNum();
protected:
    PlainThread(void);
    virtual ~PlainThread(void);

    void DoProcess();
    void DoStopMapper();
    bool DoRunMapper();

    bool DoStop();
    bool DoRun();
private:

    typedef std::vector<IMapper*> MapperCollection;
    typedef MapperCollection::iterator MapperIter;

    MapperCollection mappers_;

    scoped_event busy_event_;
    volatile LONG is_running_;
    volatile bool is_signed_stopped_;
    LLock lock_;
    DWORD thread_id_;

    unsigned int interval_;

    DISALLOW_EVIL_CONSTRUCTORS(PlainThread);
};

} // namespace dip

#endif