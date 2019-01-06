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
#include "flow/drawing_container.h"

class FlowFactory;

class CFlowDocument : public CDocument, public ICommandObserver {
protected:
    CFlowDocument();

    DECLARE_DYNCREATE(CFlowDocument)
    DECLARE_MESSAGE_MAP()
public:
    virtual ~CFlowDocument();

    virtual BOOL OnNewDocument();
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

    virtual void DeleteContents();
    virtual IDrawing* GetDrawing();

    void CommandInputed(CommandEventArgs& evt);

    afx_msg void OnLayerBottom();
    afx_msg void OnLayerTop();
    afx_msg void OnLayerUp();
    afx_msg void OnLayerDown(); 

    afx_msg void OnAlignLeft();
    afx_msg void OnAlignRight();
    afx_msg void OnAlignUp();
    afx_msg void OnAlignDown();
    afx_msg void OnAutoLayout();

    afx_msg void OnUpdateBuildStop(CCmdUI* pCmdUI);
    afx_msg void OnUpdateBuildRun(CCmdUI* pCmdUI);
    afx_msg void OnBuildRun();
    afx_msg void OnBuildDebug();
    afx_msg void OnBuildStop();
    
private:
    IView* GetView();
    const std::vector<IFigure*>& GetSelectedFigures();
    size_t GetSelectionCount();

    scoped_ptr<DrawingContainer> drawing_;
    DISALLOW_EVIL_CONSTRUCTORS(CFlowDocument);
};
