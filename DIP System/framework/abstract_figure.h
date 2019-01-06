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

#pragma  once

#include "base/basictypes.h"

#include "framework/framework.h"

class AbstractFigure : public virtual IFigure{
public: 
    AbstractFigure(void);
    virtual ~AbstractFigure(void); 
    virtual void AddFigureObserver(IFigureObserver* observer); 
    virtual void RemoveFigureObserver(IFigureObserver* observer); 
    virtual bool Contains(const Point& p);  

    virtual void Draw(Graphics& g) = 0; 
    virtual void Invalidate();  

    virtual void SetVisible(bool val);
    bool IsVisible(); 
    bool IsRemovable(); 
    bool IsSelected();
    void SetRemovale(bool val); 
    void SetSelected(bool val); 

    /** 设置是否可重置大小 */
    void SetCanResize(bool val);
    virtual Rect GetBounds(void) = 0;
    virtual Rect GetDrawingArea();
    virtual bool CanConnect();
    virtual IConnector* ConnectorAt(const Point& p);
    virtual void GetToolTipAt(std::wstring *content, Point* p);

    /** 设置物理边界 */
    virtual void SetBounds(const Rect& rect); 	 
    virtual void SetBounds(const Point& start, const Point& end); 
    virtual void SetPosition(const Point& pos);
    virtual bool CanMove(); 
    virtual void SetCanMove(bool val); 
    virtual bool CanResize(); 
    virtual void CreateHandles(vector<IHandle*>* handles); 
    virtual void WillChange(); 
    virtual void Changed();
    virtual void Release();

    virtual int GetLayer();

protected:  
    virtual void BasicSetBounds(const Point& orgin, const Point& corner) = 0;
    virtual void OnAreaInvalidated();
    virtual void OnAreaInvalidated(Rect rc);
    virtual void OnAreaInvalidated(FigureEventArgs& evt);

    virtual void OnFigureChanged();
    virtual void OnFigureChanged(Rect rc);
    virtual void OnFigureChanged(FigureEventArgs& evt);
    virtual void OnAttributeChanged(FigureEventArgs& evt);
    virtual void OnRequestRemove();
    virtual void OnRequestRemove(FigureEventArgs& evt);

private: 
    bool is_visible_;
    bool can_resizable_;
    bool is_movable_;
    bool is_removable_; 
    bool is_selected_;
    ObserverCollection observers_;

    DISALLOW_EVIL_CONSTRUCTORS(AbstractFigure);
};  