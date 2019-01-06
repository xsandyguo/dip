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

class AbstractTool : public ITool{
public:
    AbstractTool();
    virtual ~AbstractTool();

    void AddToolObserver(IToolObserver* observer);
    void RemoveToolObserver(IToolObserver* observer);
    virtual void Active(IView* editor);
    virtual void Deactive(IView* editor);
    virtual void KeyUp(KeyEventArgs& evt);
    virtual void KeyDown(KeyEventArgs& evt);
    virtual void MouseMove(MouseEventArgs& evt) ;
    virtual void MouseDown(MouseEventArgs& evt);
    virtual void MouseUp(MouseEventArgs& evt);
    virtual void MouseDblClk(MouseEventArgs& evt);
    virtual void Draw(Graphics& g){}

protected:      
    virtual void OnAreaInvalidated(Rect invalidatedArea);	    
    virtual IView* get_view();    
    virtual IDrawing* get_drawing();
    virtual void OnToolDone();
    virtual void OnToolStart();

    /** 获得指定点的光标状态 
    * @param point 指定的点
    */
    virtual void UpdateCursor(const Point& point);

    bool IsDragging();
    const Point& get_anchor() const;
    Point Locate(const Point& p);

private:
    vector<IObserver*> observers_;
    IView* view_;
    bool is_dragging_;
    Point anchor_; 

    DISALLOW_EVIL_CONSTRUCTORS(AbstractTool);
};