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

#include <vector>

#include "base/basictypes.h"
#include "base/debug.h"
#include "runtime/synchronized.h"

class COutputList : public CListBox {
public:
    COutputList();
    virtual ~COutputList();

protected:
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnEditCopy();
    afx_msg void OnEditClear();
    afx_msg void OnViewOutput();
    afx_msg int OnCreate(LPCREATESTRUCT /*lpCreateStruct*/);

    DECLARE_MESSAGE_MAP()

private:
    DISALLOW_EVIL_CONSTRUCTORS(COutputList);
};

class CRichOutputCtrl : public CRichEditCtrl, public LoggingObserver{
public:
    CRichOutputCtrl();
    virtual ~CRichOutputCtrl();

    virtual void LogMessage(LOG_CATEGORY cat, const CString& msg);

protected:
    void AddLog(LOG_CATEGORY cat, const wchar_t* msg);

    afx_msg LRESULT OnLogAdded(WPARAM wParam, LPARAM lParam);
    afx_msg int OnCreate(LPCREATESTRUCT);

    DECLARE_MESSAGE_MAP();

private:
    dip::LLock lock_;
    typedef std::vector<std::pair<LOG_CATEGORY, CString>> LogCollection;
    typedef LogCollection::iterator LogIter;
    LogCollection log_cache_;

    DISALLOW_EVIL_CONSTRUCTORS(CRichOutputCtrl);
};

class COutputWindow : public CDockablePane {
public:
    COutputWindow();
    virtual ~COutputWindow();
    void UpdateFonts();

protected:
    void AdjustHorzScroll(CListBox& wndListBox);

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()

// Ьиад
protected:
    CMFCTabCtrl	tab_ctrl_;

    COutputList output_build_;
    CRichOutputCtrl output_debug_;
    static const int kMaxLine = 200;

private:

    DISALLOW_EVIL_CONSTRUCTORS(COutputWindow);
};

