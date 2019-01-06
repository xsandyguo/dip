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
#include "base/scoped_ptr.h"

#include "ui/command_window.h"
#include "ui/file_view.h"
#include "ui/output_window.h"
#include "ui/properties_window.h"
#include "ui/thumbnail_view.h"
#include "ui/toolbox_window.h"

#include "image/ui/DlgFloodFill.h"
#include "image/ui/DlgHisto.h"

class CFlowView;
class CreationTool;

class CMainFrame :  public CMDIFrameWndEx{
    DECLARE_DYNAMIC(CMainFrame)
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    DlgHisto m_HistoBar;
    DlgFloodFill* m_pDlgFlood;

protected:
    afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnWindowManager();
    afx_msg void OnApplicationLook(UINT id);
    afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
    afx_msg void OnViewCaptionBar();
    afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
    afx_msg void OnOptions();
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    BOOL OnEraseMDIClientBackground( CDC *pDC );

    DECLARE_MESSAGE_MAP()

private:
    BOOL CreateStatusBar();
    BOOL CreateCaptionBar();
    BOOL CreateDockingWindows();
    void SetDockingWindowIcons();

private:
    CMFCRibbonApplicationButton main_button_;
    CMFCToolBarImages           panel_images_;
    CMFCRibbonBar               ribbonbar_;
    CMFCRibbonStatusBar         statusbar_;
    CMFCCaptionBar              captionbar_;

    CFileView                   widget_file_;
    CThumbnailView              widget_thumbnail_;
    COutputWindow               widget_output_;
    CPropertiesWindow           widget_properties_;
    CToolBoxWindow              widget_toolbox_;
    CCommandWindow              widget_command_;

    CFont logo_font_;
    CString logo_str_;
    CRect logo_bound_;

    DISALLOW_EVIL_CONSTRUCTORS(CMainFrame);
};

