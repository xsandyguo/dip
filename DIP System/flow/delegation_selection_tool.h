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

#include <commctrl.h>

#include "base/scoped_any.h"
#include "framework/framework.h"
#include "flow/selection_tool.h"

/** @brief 委托选择工具 
* 对选择工具的功能进行再次扩展 
*/
class DelegationSelectionTool : public SelectionTool{
public:
    DelegationSelectionTool(ITool* connection_tool);
    virtual ~DelegationSelectionTool();

    virtual void Active(IView* view);
    virtual void Deactive(IView* view);

    virtual void MouseMove(MouseEventArgs& evt);
    virtual void MouseUp(MouseEventArgs& evt); 

private:
    HWND CreateTrackingToolTip(HWND parent);
    void ShowToolTip(IFigure* figure, const Point& point);
    void HideToolTip();

    TOOLINFO toolinfo_;
    scoped_window tooltip_;
    bool is_showing_tip_;

    DISALLOW_EVIL_CONSTRUCTORS(DelegationSelectionTool);
};

