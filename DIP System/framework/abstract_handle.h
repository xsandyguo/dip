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

/** @brief handle基本实现 
* handle抽象类，提供了基本实现
*/
class AbstractHandle : public IHandle,
    public IFigureObserver{
public:
    explicit AbstractHandle(IFigure* owner);

    virtual ~AbstractHandle();
    virtual void AddHandleObserver(IHandleObserver* observer);
    virtual void RemoveHandleObserver(IHandleObserver* observer);
    virtual void Draw(Graphics& g);
    virtual IFigure* GetOwner();

    virtual Rect GetBounds();
    virtual bool Contains(const Point& p);	
    virtual void Invalidate();
    virtual void SetView(IView* view); 	
    virtual HCURSOR GetCursor();
    virtual void AttributeChanged(FigureEventArgs& evt);   
    virtual void AreaInvalidated(FigureEventArgs& evt);
    virtual void FigureChanged(FigureEventArgs& evt);
    virtual void RequestRemove(FigureEventArgs& evt);

    virtual void TrackStart(const Point& anchor){}
    virtual void TrackStep(const Point& anchor, const Point& lead, UINT nFlag){}
    virtual void TrackEnd(const Point& anchor, const Point& lead, UINT nFlag){}

protected:
    virtual Point Locate() = 0;
    virtual void UpdateBounds();
    virtual IDrawing* GetContainer();
    virtual IView* GetView();	
    virtual Rect BasicGetBounds();	
    virtual void OnAreaInvalidated();
    virtual void OnAreaInvalidated(Rect rect);
private:
    IView* view_;
    vector<IObserver*> observers_;
    IFigure* owner_;
    Rect bounds_;
    DISALLOW_EVIL_CONSTRUCTORS(AbstractHandle);
};

