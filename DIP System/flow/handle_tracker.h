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

#pragma once
#include "base/basictypes.h"

#include "framework/framework.h"
#include "framework/abstract_tool.h"

/** @brief Handle跟踪器 
* 将自已的接收的鼠标事件经处理，传给Handle处理 
*/
class HandleTracker : public AbstractTool {
public:
    HandleTracker();
    virtual ~HandleTracker();

    /** 设置被跟踪的Handle 
    * @param handle 被跟踪的Handle
    */
    virtual void SetHandle(IHandle* handle);
    virtual void MouseDown(MouseEventArgs& evt);
    virtual void MouseUp(MouseEventArgs& evt);
    virtual void MouseMove(MouseEventArgs& evt);
private:
    
    IHandle* handle_;
    DISALLOW_EVIL_CONSTRUCTORS(HandleTracker);
};
