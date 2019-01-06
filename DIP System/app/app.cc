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

#include "app/app.h"

#include <algorithm>

#include "app/app_model.h"
#include "base/constants.h"
#include "ui/main_frame.h"
#include "ui/about_dialog.h"

BEGIN_MESSAGE_MAP(DipsApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT       , &DipsApp::OnAppAbout)
    ON_COMMAND(ID_FILE_NEW        , &DipsApp::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN       , &DipsApp::OnFileOpen)
    ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

DipsApp::DipsApp(){
    // to support restart manager
    m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
    SetAppID(_T("CUST.DIPSystem.AppID.NoVersion"));    
}

DipsApp kTheApp;

BOOL DipsApp::InitInstance(){
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);
    AfxInitRichEdit2();

    CWinAppEx::InitInstance();

    CoInitialize(NULL);
    if (!AfxOleInit()){
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();
    EnableTaskbarInteraction();
    
    SetRegistryKey(_T("CUST"));
    LoadStdProfileSettings(5);  // 加载标准 INI 文件选项(包括 MRU)

    InitContextMenuManager();
    InitKeyboardManager();
    InitTooltipManager();
    InitShellManager();

    CMFCToolTipInfo ttParams;
    ttParams.m_bVislManagerTheme = TRUE;
    kTheApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
        RUNTIME_CLASS(CMFCToolTipCtrl), 
        &ttParams);

    std::for_each(g_app_models.begin(), g_app_models.end(), [&](IAppModel* model){
        if(model->Initialize()){
            AddDocTemplate(&model->GetDocTemplate());
        }
    });

    CMainFrame* pMainFrame = new CMainFrame;
     if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME)){
        delete pMainFrame;
        return FALSE;
    }
    m_pMainWnd = main_frame_ = pMainFrame;
    m_pMainWnd->DragAcceptFiles();
    EnableShellOpen();
    RegisterShellFileTypes(TRUE);

    CCommandLineInfo cmdInfo;
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
    ParseCommandLine(cmdInfo);
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;
    
    m_pMainWnd->ShowWindow(SW_SHOW);
    m_pMainWnd->UpdateWindow();
    m_pMainWnd->DragAcceptFiles();

    return TRUE;
}

void DipsApp::OnFileOpen(){
    CString title =_T("Open image file");
    
    CString filters;
    std::for_each(g_app_models.begin(), g_app_models.end(), [&filters](IAppModel* model){
            filters.AppendFormat(_T("%s|"), model->GetFilterStr());
    });

    CFileDialog dialog(TRUE, NULL, NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filters);
    if(dialog.DoModal() != IDOK){
        return;
    }

    CString file_ext(dialog.GetFileExt());
    for(std::vector<IAppModel*>::iterator iter = g_app_models.begin();
        iter != g_app_models.end();
        ++iter){
        if((*iter)->IsSupport(file_ext)){
            (*iter)->OpenFile(dialog.m_ofn.lpstrFile);
        }
    }    
}

void DipsApp::OnFileNew(){
    std::for_each(g_app_models.begin(), g_app_models.end(), [](IAppModel* model){
        model->OpenFile(NULL);
    });
}

UINT DipsApp::GetAppLook(){
    return app_look_;
}

void DipsApp::SetAppLook(UINT id){
    app_look_ = id;
}

int DipsApp::ExitInstance(){
    AfxOleTerm(FALSE);
    
    return CWinAppEx::ExitInstance();
}


void DipsApp::OnAppAbout(){
    CAboutDialog aboutDlg;
    aboutDlg.DoModal();
}

void DipsApp::PreLoadState(){
    BOOL bNameValid;
    CString strName;
    bNameValid = strName.LoadString(IDS_EDIT_MENU);
    ASSERT1(bNameValid);
    GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
    bNameValid = strName.LoadString(IDS_EXPLORER);
    ASSERT1(bNameValid);
    GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void DipsApp::LoadCustomState(){
}

void DipsApp::SaveCustomState(){
}
