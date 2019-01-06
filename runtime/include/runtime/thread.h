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

#ifndef DIP_RT_THREAD_H_
#define DIP_RT_THREAD_H_

#include <vector>

#include "base/scoped_any.h"
#include "runtime/common.h"
#include "runtime/framework.h"
#include "runtime/presentation.h"

namespace dip{

class DIP_EXPORT Thread : public Presentation, public IThread{
public:
    DECLEAR_ABSTRACT_CLASSINFO(Thread, Presentation)
        
    void Register();
    void UnRegister();

protected:
    Thread(int in_num);
    virtual ~Thread(){}
     
    virtual void Stop();
    virtual bool Run();

    virtual void AsyncStopCompleted();
    virtual bool DoStop() = 0;
    virtual bool DoRun() = 0;

    bool IsRunning(){ return is_running_; }

private:
    volatile bool is_running_;
    volatile bool is_stopping_;

    scoped_handle syc_stop_event_;

    DISALLOW_EVIL_CONSTRUCTORS(Thread);
};

} // namespace dip
#endif
