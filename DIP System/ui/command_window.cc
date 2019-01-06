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

#include "ui/command_window.h"

#include <algorithm>
#include "resource.h"

#include "base/constants.h"
#include "base/debug.h"
#include "ui/command_window.h"
#include "runtime/reflector.h"

const TCHAR* const kEnumMethod = _T("?");

BEGIN_MESSAGE_MAP(CCommandEdit, CEdit)
    ON_WM_CREATE()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
END_MESSAGE_MAP()

CCommandEdit::CCommandEdit(){

}

CCommandEdit::~CCommandEdit(){

}

int CCommandEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (CEdit::OnCreate(lpCreateStruct) == -1)
        return -1;
    font_.CreatePointFont(100, kCommandFontName);
    
    SetFont(&font_);

    SetWindowText(kCmdPrefix);
    return 0;
}

void CCommandEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
    int start_pos;
    int end_pos;
    int last_line_begin;

    // All edit operation must in last line   
    GetSel(start_pos, end_pos);
    int last_cmd_begin = LineIndex(GetLineCount() - 1) + _tcslen(kCmdPrefix);

    if(VK_BACK == nChar){
        last_cmd_begin ++;
    }

    if(last_cmd_begin > start_pos){
        return;
    }    

    if(VK_RETURN == nChar){     
        ParseInput();

        CString content;
        GetWindowText(content);
        content.Append(kLineBreak);
        content.Append(kCmdPrefix);

        SetWindowText(content);
        last_line_begin = LineIndex(GetLineCount() - 1);
        start_pos = end_pos = last_line_begin + LineLength(last_line_begin);
        SetSel(start_pos, end_pos);
    }else{
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }

    Invalidate(TRUE);
}

void CCommandEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
    // Disable Delete key
    if(nChar != VK_DELETE){
        __super::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}

void CCommandEdit::ParseInput(){
    int last_line = GetLineCount() - 1;
    int len = LineLength(LineIndex(last_line));

    CString text;
    GetLine(last_line, text.GetBuffer(len), len);
    text.ReleaseBuffer(len);
    text = text.Right(text.GetLength() - _tcslen(kCmdPrefix)).Trim();

    if(text.GetLength()){
        OnCommandInput(text);
    }
}

void CCommandEdit::OnCommandInput(const CString& command){
    HWND parent = GetParent()? GetParent()->GetSafeHwnd() : NULL;
    if(parent){
        ::SendMessage(parent, WM_COMMAND_INPUT, (WPARAM)&command, NULL);
    }
}

BEGIN_MESSAGE_MAP(CCommandWindow, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_COMMAND_INPUT, &CCommandWindow::OnCommandInput)
END_MESSAGE_MAP()

CCommandWindow::CCommandWindow():selected_figure_(0){

}

CCommandWindow::~CCommandWindow(){

}

LRESULT CCommandWindow::OnCommandInput(WPARAM wParam, LPARAM lParam){
    const CString& command = *(reinterpret_cast<CString*>(wParam));
    vector<wstring> tokens;
    int i = 0;
    CString token;

    while(AfxExtractSubString(token, command, i++, _T(' '))){  
        token = token.Trim();
        if(token.GetLength()){
            tokens.push_back((const wchar_t*)(token));
        }
    }

   
    if(tokens.size() == 1){
        Object* obj = GetSelectedObject();
        if(!obj){
            LOG_ERROR(_T("[CCommandWindow::OnCommandInput][Please selected a target first.]"));
            return 0;
        }

        token = tokens.at(0).c_str();
        if(token == kEnumMethod){
            std::vector<Method*> methods;
            obj->GetClass()->EnumMethods(methods);

            CString signature(_T("no method found."));
            std::for_each(methods.begin(), methods.end(), [obj, &methods, &signature](Method* m){
                signature.Format(_T("%s::%s("), obj->get_name().c_str(), m->GetName().c_str());
                for(int i = 0; i < m->GetArgCount(); ++i){
                    if(i == 0){
                        signature.AppendFormat(_T("%s"), dip::GetTypeString(m->GetArgumentType(i)).c_str());
                    }else{
                        signature.AppendFormat(_T(",%s"), dip::GetTypeString(m->GetArgumentType(i)).c_str());
                    }
                }
                signature.Append(_T(")"));
            });       

            LOG_CMD(signature);
        }else{
            tokens.insert(tokens.begin(), obj->get_name());
        }
    }

    if(tokens.size() >= 2){
        ImmediateCommand cmd;
        cmd.object_name = tokens.at(0);
        cmd.method_name = tokens.at(1);
        tokens.erase(tokens.begin(), tokens.begin() + 2);
        cmd.arguments = tokens;

        CommandEventArgs event(*this, 
                                cmd.object_name,
                                cmd.method_name,
                                tokens);        
        ObserverIterCall(observers_, &ICommandObserver::CommandInputed, event);       
    }

    return 0;
}

void CCommandWindow::AddObserver(ICommandObserver* observer){
    ::AddObserver(&observers_, observer);
}

void CCommandWindow::RemoveObserver(ICommandObserver* observer){
    ::RemoveObserver(&observers_, observer);
}

int CCommandWindow::OnCreate(LPCREATESTRUCT lpCreateStruct){
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // 创建视图:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES |
        WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOHSCROLL
        | ES_AUTOVSCROLL | TVS_LINESATROOT | TVS_HASBUTTONS;

    if (!command_edit_.Create(dwViewStyle, rectDummy, this, IDC_COMMAND_EDIT)){
        TRACE0("未能创建文件视图\n");
        return -1;      // 未能创建
    }     
    return 0;
}

void CCommandWindow::OnSize(UINT nType, int cx, int cy){
    CDockablePane::OnSize(nType, cx, cy);
    command_edit_.SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE 
        | SWP_NOACTIVATE | SWP_NOZORDER);
}

HBRUSH CCommandWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    HBRUSH hbr;
    if(nCtlColor == CTLCOLOR_EDIT) {
        pDC-> SetBkMode(TRANSPARENT); 
        hbr = CreateSolidBrush(RGB(13, 14, 43)); 
        pDC->SetTextColor(RGB(191, 191, 0)); 
        return hbr;
    }else{
       return CDockablePane::OnCtlColor(pDC, pWnd, nCtlColor);
    }
}

void CCommandWindow::SetSelectedFigure(ProcessFigure* figure){
    if(selected_figure_){
        selected_figure_->RemoveFigureObserver(this);
    }

    selected_figure_ = figure;

    if(selected_figure_){
        selected_figure_->AddFigureObserver(this);
    }
}

Object* CCommandWindow::GetSelectedObject(){
    if(selected_figure_){
        return &(selected_figure_->GetFilter().GetObject());
    }

    return NULL;
}

void CCommandWindow::SelectionChanged(FigureSelectionEventArgs& evt){
    if(evt.GetNewSelection().size() == 1){
        SetSelectedFigure(dynamic_cast<ProcessFigure*>(evt.GetNewSelection().at(0)));
    }else{
        SetSelectedFigure(NULL);
    }
}

void CCommandWindow::RequestRemove(FigureEventArgs& evt){
    ASSERT1(evt.GetFigure() == selected_figure_);

    selected_figure_ = NULL;
}