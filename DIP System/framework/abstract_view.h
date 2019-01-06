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

#include <afxwin.h>
#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "framework/framework.h"

class AbstractView  : public CScrollView, public IView, public IEditableComponent { 
public:
    AbstractView();  
    virtual ~AbstractView(); 

    virtual void AddSelectionObserver(IFigureSelectionObserver* observer);
    virtual void AddMouseObserver(IMouseObserver* observer);
    virtual void AddKeyObserver(IKeyObserver* observer);
    virtual void RemoveSelectionObserver(IFigureSelectionObserver* observer);
    virtual void RemoveMouseObserver(IMouseObserver* observer);
    virtual void RemoveKeyObserver(IKeyObserver* observer);

    virtual IDrawingEditor* GetEditor();
    virtual void AddNotify(IDrawingEditor* editor);
    virtual void RemoveNotify(IDrawingEditor* editor);

    virtual IHandle* FindHandle(const Point& p);
    virtual vector<IFigure*>& GetSelectedFigures(void);
    virtual void ClearSelection(void);
    virtual int GetSelectionCount(void); 

    virtual ITool* GetTool(); 
    virtual void AddToSelection(IFigure* figure);
    virtual void RemoveFromSelection(IFigure* figure);
    virtual IEditableComponent* GetEditableComponent(){return this;}

    virtual void OnDraw(CDC* pDC);

    // À©Õ¹¹¦ÄÜ
    virtual void DoDelete() ; 
    virtual void DoCut();
    virtual void DoCopy();
    virtual void DoPaste();
    virtual void DoSelectAll(); 

    virtual void Invalidate(const Rect& rc);
    virtual void Invalidate();

protected: 
    virtual CPoint VirtualToClient(const Point& p) ;
    virtual Point ClientToVirtual(const CPoint& p) ;
    virtual Rect ClientToVirtual(const CRect& p)   ;
    virtual CRect VirtualToClient(const Rect& rect);

    virtual CWnd& GetWindow(){return *this;}
    virtual void DrawCanvas(Graphics& g);
    virtual void DrawHandle(Graphics& g);
    virtual void DrawTool(Graphics& g);
    virtual void DrawFigure(Graphics& g);

    virtual void OnSelectionChanged(vector<IFigure*>* old, vector<IFigure*>* newOne);
    virtual void OnKeyDown(KeyEventArgs& evt);
    virtual void OnKeyUp(KeyEventArgs& evt);
    virtual	void OnMouseMove(MouseEventArgs& evt);
    virtual	void OnMouseDblClk(MouseEventArgs& evt);
    virtual void OnMouseDown(MouseEventArgs& evt);
    virtual void OnMouseUp(MouseEventArgs& evt);  	

    afx_msg void OnMouseMove(UINT nFlags    , CPoint point); 
    afx_msg void OnLButtonUp(UINT nFlags    , CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags  , CPoint p); 
    afx_msg void OnRButtonUp(UINT nFlags    , CPoint point);
    afx_msg void OnRButtonDown(UINT nFlags  , CPoint point); 
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

    DECLARE_MESSAGE_MAP()
private: 
    class InnerEventHandler :  
        public IHandleObserver{
    public:
        InnerEventHandler(AbstractView* view)
        :view_(view){}
        ~InnerEventHandler(){}
        void AreaInvalidated(HandleEventArgs& evt);
    private:
        AbstractView* view_;
        DISALLOW_EVIL_CONSTRUCTORS(InnerEventHandler);
    };

    scoped_ptr<InnerEventHandler> event_handler_;
    ObserverCollection observers_;
    IDrawingEditor* editor_;

    DISALLOW_EVIL_CONSTRUCTORS(AbstractView);

protected:
    vector<IFigure*> selected_figures_;
    vector<IHandle*> selected_handles_;
};