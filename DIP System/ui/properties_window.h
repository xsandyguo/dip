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

#include <afxcontrolbars.h>
#include "base/basictypes.h"
#include "runtime/object.h"
#include "framework/framework.h"
#include "flow/process_figure.h"

using namespace dip;
class CPropertiesToolBar : public CMFCToolBar {
public:
    virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler){
        CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList() const { return FALSE; }
private:
};

class CPropertiesWindow : public CDockablePane,
    public IFigureSelectionObserver, 
    public IContainerObserver {
public:
    CPropertiesWindow();
    virtual ~CPropertiesWindow();

    void AdjustLayout();
    void SetVSDotNetLook(BOOL bSet){
        property_grid_.SetVSDotNetLook(bSet);
        property_grid_.SetGroupNameFullWidth(bSet);
    }

    virtual void FigureRemoved(DrawingEventArgs& evt);
    virtual void FigureAdded(DrawingEventArgs& evt);
    virtual void SelectionChanged(FigureSelectionEventArgs& evt);

protected:
    void InitPropList();
    void SetPropListFont();
    void CreateProperty();

    void BindObjectProperties();
    Object& GetBindObject(){ return binding_figure_->GetFilter().GetObject() ;}
    bool ConvertTo(_variant_t* var, const VariantData& data);
    bool ConvertTo(VariantData* var, const _variant_t& data);

    afx_msg LRESULT OnPropertyChanged(WPARAM,LPARAM);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnExpandAllProperties();
    afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
    afx_msg void OnSortProperties();
    afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
    afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

    DECLARE_MESSAGE_MAP()

protected:
    ProcessFigure* binding_figure_;
    CFont prop_list_;
    CPropertiesToolBar toolbar_;
    CMFCPropertyGridCtrl property_grid_;

private:
    DISALLOW_EVIL_CONSTRUCTORS(CPropertiesWindow);
};

