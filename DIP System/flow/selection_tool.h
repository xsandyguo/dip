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
#include "base/scoped_ptr.h"
#include "framework/framework.h"
#include "framework/abstract_tool.h"

#include "flow/drag_tracker.h"
#include "flow/selection_area_tracker.h"
#include "flow/handle_tracker.h"
#include "flow/connection_tool.h"

/** @brief 选择工具 
* 采用策略模式，选择工具根据当前光标的按下的点的状态，在三个跟踪器(HandleTracker SelectionAreaTracker 
* DragTracker)之间切换 
* 始终把当前的键盘鼠标事件传递给当前的跟踪器 
*/
class SelectionTool:
    public AbstractTool{
public:
    SelectionTool(ITool* connectoin_tool);
    virtual ~SelectionTool();

    virtual void KeyUp(KeyEventArgs& evt);
    virtual void KeyDown(KeyEventArgs& evt);
    virtual void MouseMove(MouseEventArgs& evt) ;    
    virtual void MouseDown(MouseEventArgs& evt);
    virtual void MouseUp(MouseEventArgs& evt); 
    virtual void Draw(Graphics& g);

protected:   
    void OnMoveByKey(KeyEventArgs& evt);

    virtual bool CanConnectAt(const Point& p);
    virtual ITool* GetHandleTracker(IHandle* handle);
    virtual ITool* GetSelectionAreaTracker();
    virtual ITool* GetDragTracker(IFigure* figure);
    virtual ITool* GetConnectionTool();
    virtual void set_tracker(ITool* newTracker);
    virtual ITool* get_tracker();
    virtual ITool* FindTracker(MouseEventArgs& evt);
private:  	 
    
    class InnerEventHandler : public ToolAdapter{
    public:
        explicit InnerEventHandler(SelectionTool* tool);
        virtual ~InnerEventHandler();
        virtual void AreaInvalidated(ToolEventArgs& evt);        
        virtual void ToolDone(ToolEventArgs& evt);
    private:
        SelectionTool* selection_tool_;
    };

    scoped_ptr<SelectionAreaTracker> selection_area_tracker_;
    scoped_ptr<DragTracker> drag_tracker_;
    scoped_ptr<HandleTracker> handle_tracker_; 
    scoped_ptr<InnerEventHandler> tracker_handler_;
    scoped_ptr<ITool> connection_tool_;
    ITool* tracker_;

    DISALLOW_EVIL_CONSTRUCTORS(SelectionTool);
};