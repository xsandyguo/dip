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
#include <afxcontrolbars.h>

#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "framework/framework.h"
#include "framework/abstract_view.h"
#include "framework/grid.h"

class CreationTool;
class CFlowDocument;

class CFlowView : public AbstractView,
                  public ToolboxAdapter,
                  public ToolAdapter{
public:
    virtual ~CFlowView();
    CFlowDocument* GetDocument() const;
    virtual IDrawing* GetDrawing();
    void ExtendViewportTo(const Point& gain);

protected:
    CFlowView();
    DECLARE_DYNCREATE(CFlowView)
    
    virtual void OnInitialUpdate();
    virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
    virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
    virtual void RelayoutView();
    virtual void OnMouseMove(MouseEventArgs& evt);
    Size GetPreferSize();

    CreationTool* GetCreationTool();
    ITool* GetSelectionTool();
    ITool* GetConnectionTool();

    ITool* GetTool();
    void   SetTool(ITool* tool);
    virtual void DrawCanvas(Graphics& g);

    void ItemClicked(ToolboxEventArgs& evt);
    void AreaInvalidated(ToolEventArgs& evt);
    void ToolDone(ToolEventArgs& evt);
    void ToolDone();

    ILocator& GetLocator();

    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnSize(UINT nType, int cx, int cy); 
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnFilePrintPreview();
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

    afx_msg void OnViewGrid();
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
    afx_msg void OnUpdateViewGrid(CCmdUI* pCmdUI);
    afx_msg void OnUpdateStatusPos(CCmdUI *pCmdUI);
    afx_msg void OnUpdateStatusbarZoomSlider(CCmdUI *pCmdUI);
    afx_msg void OnUpdateStatusbarZoomList(CCmdUI *pCmdUI);    

    afx_msg void OnToolPointer();
    afx_msg void OnToolLine();

    DECLARE_MESSAGE_MAP()

private:
    class InnerEventHandler : 
        public IFigureObserver, 
        public IHandleObserver, 
        public IContainerObserver{
    public:
        InnerEventHandler(CFlowView* view);
        ~InnerEventHandler(); 
        void AreaInvalidated(FigureEventArgs& evt);
        void AreaInvalidated(HandleEventArgs& evt);
        void FigureChanged(FigureEventArgs& evt);
        void AttributeChanged(FigureEventArgs& evt); 
        void FigureAdded(DrawingEventArgs& evt);
        void FigureRemoved(DrawingEventArgs& evt);
        void RequestRemove(FigureEventArgs& evt);

    private:
        CFlowView* view_;

        DISALLOW_EVIL_CONSTRUCTORS(InnerEventHandler);
    };

    scoped_ptr<InnerEventHandler> event_handler_;
    Point cursor_position_;
    SIZE viewport_size_;
    Grid grid_;

    scoped_ptr<ITool>           selection_tool_;
    scoped_ptr<ITool>           connection_tool_;
    scoped_ptr<CreationTool>    creation_tool_;
    ITool*                      current_tool_;

    DISALLOW_EVIL_CONSTRUCTORS(CFlowView);
};
