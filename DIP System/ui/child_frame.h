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

class CChildFrame : public CMDIChildWndEx {
    DECLARE_DYNCREATE(CChildFrame)
public:
    CChildFrame();
    virtual ~CChildFrame();
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
    virtual void ActivateFrame(int nCmdShow);

    afx_msg void OnFilePrint();
    afx_msg void OnFilePrintPreview();
    afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()

private:
    CSplitterWndEx wnd_splitter_;
};
