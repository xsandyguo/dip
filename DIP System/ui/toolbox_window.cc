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

#include "ui/toolbox_window.h"
#include <tchar.h>
#include <algorithm>

#include "base/scoped_ptr.h"
#include "base/utility.h"
#include "base/debug.h"
#include "app/app.h"
#include "runtime/reflector.h"

using namespace dip;
using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CToolBoxWindow, CDockablePane)

CToolBoxWindow::CToolBoxWindow(){    
}

CToolBoxWindow::~CToolBoxWindow(){
}

BEGIN_MESSAGE_MAP(CToolBoxWindow, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CToolBoxWindow::OnCreate(LPCREATESTRUCT lpCreateStruct){
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    CRect rectDummy;
    rectDummy.SetRectEmpty();
    if (!toolbox_ctrl_.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 12)){
        TRACE0("Failed to create ToolBox\n");
        return -1;      // fail to create
    }
    InitToolList();

    callback_.args = this;
    callback_.invoke = ChangeCallback;
    toolbox_ctrl_.SetCallback(&callback_);

    toolbar_.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
    toolbar_.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 已锁定*/);    
    toolbar_.SetPaneStyle(toolbar_.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    toolbar_.SetPaneStyle(toolbar_.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC 
        | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    toolbar_.SetOwner(this);

    // 所有命令将通过此控件路由，而不是通过主框架路由:
    toolbar_.SetRouteCommandsViaFrame(FALSE);

    font_.reset(new Gdiplus::Font(_T("宋体"), 10, FontStyleRegular));
    format_.reset(new StringFormat());
    desc_bounds_.Height = 150;    
    desc_bounds_.X = desc_bounds_.Width = desc_bounds_.Y = -1;

    return 0;
}

void CToolBoxWindow::AddObserver(IToolboxObserver* observer){
    ::AddObserver(&observers_, observer);
}

void CToolBoxWindow::RemoveObserver(IToolboxObserver* observer){
    ::RemoveObserver(&observers_, observer);
}

void CToolBoxWindow::ChangeCallback(SelectionCallback* callback){
    CToolBoxWindow* toolbox = static_cast<CToolBoxWindow*>(callback->args);
    ASSERT1(toolbox);

    dip::Class* old_choice = callback->old_value ? 
        static_cast<dip::Class*>(callback->old_value->GetTaggedData()) : NULL;
    dip::Class* new_choice = callback->new_value ? 
        static_cast<dip::Class*>(callback->new_value->GetTaggedData()) : NULL;
    if(!(old_choice || new_choice)){
        return;
    }

    if(new_choice){
        toolbox->descbox_str_ = new_choice->GetDescription().c_str();
        if(toolbox->descbox_str_.IsEmpty()){
            toolbox->descbox_str_ = new_choice->GetClassName().c_str();
        }
        toolbox->InvalidateDesc();
    }

    wstring empty;    
    if(callback->has_changed()){
        ToolboxEventArgs argv(*toolbox, old_choice ? old_choice->GetID() : empty, 
            new_choice ? new_choice->GetID() : empty);
        ObserverIterCall(toolbox->observers_, &IToolboxObserver::SelectionChanged, argv);
    }

    ToolboxEventArgs argv(*toolbox, new_choice ? new_choice->GetID() : empty);
    ObserverIterCall(toolbox->observers_, &IToolboxObserver::ItemClicked, argv);
}

void CToolBoxWindow::InvalidateDesc(){
    if(desc_bounds_.Width < 0){
        Invalidate();
    }else{
        RECT rc;
        rc.left   = desc_bounds_.GetLeft();
        rc.top    = desc_bounds_.GetTop();
        rc.right  = desc_bounds_.GetRight();
        rc.bottom = desc_bounds_.GetBottom();

        InvalidateRect(&rc);
    }
}

void CToolBoxWindow::OnSize(UINT nType, int cx, int cy){
    CDockablePane::OnSize(nType, cx, cy);

    ASSERT1(GetSafeHwnd());

    CRect rectClient;
    GetClientRect(rectClient);
    int cyTlb = toolbar_.CalcFixedLayout(FALSE, TRUE).cy;

    toolbar_.SetWindowPos(NULL,
        rectClient.left, 
        rectClient.top, 
        rectClient.Width(), 
        cyTlb,
        SWP_NOACTIVATE | SWP_NOZORDER);

    toolbox_ctrl_.SetWindowPos(NULL, 
        rectClient.left + 1, 
        rectClient.top + cyTlb + 1,
        rectClient.Width() - 2,
        rectClient.Height() - cyTlb - 2 - desc_bounds_.Height,
        SWP_NOACTIVATE | SWP_NOZORDER);

    InvalidateDesc();
}

void CToolBoxWindow::CreateObjectGroup(const wstring& group_name, 
                                       const wstring &type_name){
    CToolElement* group = new CToolElement(CString(group_name.c_str()));

    vector<wstring> classNames;
    Reflector::EnumClassNames(classNames);

    CToolElement* tool = NULL;
    for(vector<wstring>::iterator itor = classNames.begin();
        itor != classNames.end();
        ++itor){
        Class* namedClass = Reflector::GetClass(*itor);
        std::wstring name = namedClass->GetClassName();

        if(Reflector::IsDerivedFrom(name, type_name) && !namedClass->IsAbstract()){
            group->AddSubTool(tool = new CToolElement(CString(name.c_str()), 
                0,
                0));
            tool->SetTaggedData(namedClass);
        }    
    }    

    toolbox_ctrl_.AddToolTab(group);
}

void CToolBoxWindow::InitToolList(){
    CreateObjectGroup(_T("Filter"), _T("Filter"));
    CreateObjectGroup(_T("Source"), _T("Source"));
    CreateObjectGroup(_T("Mapper"), _T("Mapper"));
    CreateObjectGroup(_T("Window"), _T("ImageWindow"));
    CreateObjectGroup(_T("Thread"), _T("Thread"));
}

void CToolBoxWindow::OnPaint(){
    CPaintDC dc(this); 
    
    scoped_ptr<Graphics> g(Graphics::FromHDC(dc.GetSafeHdc()));

    CRect client;
    GetClientRect(client);
    SolidBrush brush(Color(222, 225, 231));
    Point anchor(client.left, client.bottom - desc_bounds_.Height);

    RECT clip;
    dc.GetClipBox(&clip);    
    Rect clip_bound(clip.left, clip.top, clip.right - clip.left, clip.bottom - clip.top);
    g->FillRectangle(&brush, clip_bound);

    Pen pen(Color(160, 160, 160));
    g->DrawLine(&pen, anchor, Point(client.Width(), anchor.Y));

    anchor.Y += 4;
    g->DrawLine(&pen, anchor, Point(client.Width(), anchor.Y));

    if(descbox_str_.IsEmpty()){
        return;
    }
    anchor.Y += 2;

    FontFamily fontfamily;
    font_->GetFamily(&fontfamily);

    desc_bounds_ = RectF(PointF(anchor.X, anchor.Y), SizeF(client.Width(), desc_bounds_.Height));
    SolidBrush brush2(Color(0, 0, 0));
    g->DrawString(descbox_str_, descbox_str_.GetLength(), font_.get(),
        desc_bounds_, format_.get(), &brush2);
}

BOOL  CToolBoxWindow::OnEraseBkgnd(CDC* pDC){
    return TRUE;
}