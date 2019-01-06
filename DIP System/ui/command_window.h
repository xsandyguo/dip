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

#ifndef DIP_UI_COMMAND_WINDOW_H_
#define DIP_UI_COMMAND_WINDOW_H_

#include <afxwin.h>  
#include <afxdockablepane.h>
#include "base/basictypes.h"
#include "base/observer.h"
#include "framework/framework.h"
#include "flow/process_figure.h"

class CCommandEdit : public CEdit{
public:
    CCommandEdit();
    virtual ~CCommandEdit();
public:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
protected:
    void OnCommandInput(const CString& command);

    DECLARE_MESSAGE_MAP()
private:
    void ParseInput();
    CFont font_;
    DISALLOW_EVIL_CONSTRUCTORS(CCommandEdit);
};

class CCommandWindow : public CDockablePane, public IFigureSelectionObserver, 
public FigureAdapter{
public:
    CCommandWindow();
    virtual ~CCommandWindow();
    void AddObserver(ICommandObserver* observer);
    void RemoveObserver(ICommandObserver* observer);

    void SelectionChanged(FigureSelectionEventArgs& evt);
    void RequestRemove(FigureEventArgs& evt);
protected:
    void SetSelectedFigure(ProcessFigure* figure);
    Object* GetSelectedObject();

    afx_msg LRESULT OnCommandInput(WPARAM wParam, LPARAM lParam);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

    DECLARE_MESSAGE_MAP()    
private:
    struct ImmediateCommand{
        wstring object_name;
        wstring method_name;
        std::vector<wstring> arguments;
    };

    CCommandEdit command_edit_;
    ProcessFigure* selected_figure_;
    typedef std::vector<IObserver*>::iterator ObserverIter;
    ObserverCollection observers_;
    DISALLOW_EVIL_CONSTRUCTORS(CCommandWindow);
};


#endif