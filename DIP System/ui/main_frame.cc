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

#include "ui/main_frame.h"

#include <base/debug.h>
#include "app/app.h"
#include "ui/widget_manager.h"
#include "flow/ui/flow_view.h"

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
    ON_WM_CREATE()
    ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
    ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, \
        &CMainFrame::OnApplicationLook)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, \
    ID_VIEW_APPLOOK_WINDOWS_7,                &CMainFrame::OnUpdateApplicationLook)
    ON_UPDATE_COMMAND_UI(ID_VIEW_CAPTION_BAR, &CMainFrame::OnUpdateViewCaptionBar)
    ON_COMMAND(ID_VIEW_CAPTION_BAR          , &CMainFrame::OnViewCaptionBar)
    ON_COMMAND(ID_TOOLS_OPTIONS             , &CMainFrame::OnOptions)

    ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

CMainFrame::CMainFrame(){
    kTheApp.SetAppLook(kTheApp.GetInt(_T("ApplicationLook"),
                       ID_VIEW_APPLOOK_WINDOWS_7));

    logo_font_.CreateFont(20, 0, 0, 0, FW_SEMIBOLD, 1, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        FIXED_PITCH | FF_ROMAN, _T("Times New Roman"));

    logo_str_ = _T("DIPS CUST");
}

CMainFrame::~CMainFrame(){
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct){
    if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    OnApplicationLook(kTheApp.GetAppLook());

    CMDITabInfo tab_params;
    tab_params.m_style                 = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
    tab_params.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
    tab_params.m_bTabIcons             = TRUE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
    tab_params.m_bAutoColor            = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
    tab_params.m_bDocumentMenu         = TRUE; // 在选项卡区域的右边缘启用文档菜单
    EnableMDITabbedGroups(TRUE, tab_params);

    ribbonbar_.Create(this);
    ribbonbar_.LoadFromResource(IDR_RIBBON);    
    if(!CreateStatusBar()){
        TRACE0("未能创建状态栏\n");
        return -1;
    }

    CDockingManager::SetDockingMode(DT_SMART);
    EnableAutoHidePanes(CBRS_ALIGN_ANY);    

    if (!CreateCaptionBar()){
        TRACE0("未能创建标题栏\n");
        return -1;     
    }
    // 加载菜单项图像(不在任何标准工具栏上):
    CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, IDB_MENU_IMAGES_24);

    if (!CreateDockingWindows()){
        TRACE0("未能创建停靠窗口\n");
        return -1;
    }

    //histogram bar
    m_HistoBar.Create(this, IDD_HISTO, CBRS_TOP, ID_VIEW_HISTOGRAM);
    m_HistoBar.ShowWindow(0);
    m_HistoBar.EnableDocking(0);

    widget_file_.EnableDocking(CBRS_ALIGN_ANY);
    widget_thumbnail_.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&widget_file_);
    CDockablePane* pTabbedBar = NULL;
    widget_thumbnail_.AttachToTabWnd(&widget_file_, DM_SHOW, TRUE, &pTabbedBar);
    
    widget_command_.EnableDocking(CBRS_ALIGN_ANY);
    widget_output_.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&widget_output_);
    DockPane(&widget_command_);

    widget_properties_.EnableDocking(CBRS_ALIGN_ANY);
    widget_toolbox_.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&widget_properties_);
    pTabbedBar = NULL;
    widget_toolbox_.AttachToTabWnd(&widget_properties_, DM_SHOW, TRUE, &pTabbedBar);    

    // 启用增强的窗口管理对话框
    EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

    // 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
    // 将改进任务栏的可用性，因为显示的文档名带有缩略图。
    ModifyStyle(0, FWS_PREFIXTITLE);

    WidgetManager::Register(&widget_command_);
    WidgetManager::Register(&widget_file_);
    WidgetManager::Register(&widget_output_);
    WidgetManager::Register(&widget_thumbnail_);
    WidgetManager::Register(&widget_toolbox_);
    WidgetManager::Register(&widget_properties_);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs){
    if( !CMDIFrameWndEx::PreCreateWindow(cs) )
        return FALSE;
    return TRUE;
}

BOOL CMainFrame::CreateStatusBar(){
    if (!statusbar_.Create(this)){
        return FALSE;     
    }
    
    CString strTemp;
    strTemp.LoadString(ID_STATUSBAR_POS);
    statusbar_.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_POS, strTemp, TRUE), strTemp);
    
    strTemp.LoadString(ID_STATUSBAR_ZOOM_LIST);
    CMFCRibbonButton *m_zoolist = new CMFCRibbonButton(ID_STATUSBAR_ZOOM_LIST, strTemp);

    m_zoolist->AddSubItem(new CMFCRibbonButton(ID_STATUSBAR_ZOOM_LIST, strTemp));
    statusbar_.AddExtendedElement(m_zoolist, strTemp);

    strTemp.LoadString(ID_STATUSBAR_ZOOM_SLIDER);
    statusbar_.AddExtendedElement(new CMFCRibbonSlider(ID_STATUSBAR_ZOOM_SLIDER), strTemp);
    
    return TRUE;
}

BOOL CMainFrame::CreateDockingWindows(){
    BOOL name_valid;

    CString strClassView;
    name_valid = strClassView.LoadString(IDS_THUMBNAIL_VIEW);
    ASSERT1(name_valid);
    if (!widget_thumbnail_.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE,
        ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN 
        | CBRS_LEFT | CBRS_FLOAT_MULTI)){
        TRACE0("未能创建“类视图”窗口\n");
        return FALSE;
    }

    CString strFileView;
    name_valid = strFileView.LoadString(IDS_FILE_VIEW);
    ASSERT1(name_valid);
    if (!widget_file_.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE,
        ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN 
        | CBRS_LEFT| CBRS_FLOAT_MULTI)){
        TRACE0("未能创建“文件视图”窗口\n");
        return FALSE;
    }

    CString strOutputWnd;
    name_valid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
    ASSERT1(name_valid);
    if (!widget_output_.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE,
        ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN 
        | CBRS_BOTTOM | CBRS_FLOAT_MULTI)){
        TRACE0("未能创建输出窗口\n");
        return FALSE;
    }

    CString strCommandWnd;
    name_valid = strCommandWnd.LoadString(IDS_COMMAND_WND);
    ASSERT1(name_valid);
    if (!widget_command_.Create(strCommandWnd, this, CRect(0, 0, 100, 100), TRUE,
        ID_VIEW_COMMANDWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN 
        | CBRS_BOTTOM | CBRS_FLOAT_MULTI)){
        TRACE0("未能创建命令窗口\n");
        return FALSE;
    }

    CString strPropertiesWnd;
    name_valid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
    ASSERT1(name_valid);
    if (!widget_properties_.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, 
        ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN 
        | CBRS_RIGHT | CBRS_FLOAT_MULTI)){
        TRACE0("未能创建“属性”窗口\n");
        return FALSE;
    }

    CString strToolBoxWnd;
    name_valid = strToolBoxWnd.LoadString(IDS_TOOLBOX_WND);
    ASSERT1(name_valid);
    if (!widget_toolbox_.Create(strToolBoxWnd, this, CRect(0, 0, 200, 200), TRUE, 
        ID_VIEW_TOOLBOX, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN 
        | CBRS_RIGHT | CBRS_FLOAT_MULTI)){
        TRACE0("未能创建“工具箱”窗口\n");
        return FALSE;
    }

    SetDockingWindowIcons();
    return TRUE;
}

void CMainFrame::SetDockingWindowIcons(){
    HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), 
        MAKEINTRESOURCE(IDI_FILE_VIEW_HC), IMAGE_ICON, 
        ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    widget_file_.SetIcon(hFileViewIcon, FALSE);

    HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), 
        MAKEINTRESOURCE(IDI_CLASS_VIEW_HC), IMAGE_ICON, 
        ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    widget_thumbnail_.SetIcon(hClassViewIcon, FALSE);

    HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), 
        MAKEINTRESOURCE(IDI_OUTPUT_WND_HC), IMAGE_ICON, 
        ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    widget_output_.SetIcon(hOutputBarIcon, FALSE);

    HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), 
        MAKEINTRESOURCE(IDI_PROPERTIES_WND_HC), IMAGE_ICON, 
        ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    widget_properties_.SetIcon(hPropertiesBarIcon, FALSE);

    HICON hToolBoxBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), 
        MAKEINTRESOURCE(IDI_TOOLBOX_WND_HC), IMAGE_ICON, 
        ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    widget_toolbox_.SetIcon(hToolBoxBarIcon, FALSE);
    UpdateMDITabbedBarsIcons();
}

BOOL CMainFrame::CreateCaptionBar(){
    if (!captionbar_.Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, this, 
        ID_VIEW_CAPTION_BAR, -1, TRUE)){
        TRACE0("未能创建标题栏\n");
        return FALSE;
    }

    BOOL bNameValid;

    CString strTemp, strTemp2;
    bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON);
    ASSERT1(bNameValid);
    captionbar_.SetButton(strTemp, ID_TOOLS_OPTIONS, CMFCCaptionBar::ALIGN_LEFT, FALSE);
    bNameValid = strTemp.LoadString(IDS_CAPTION_BUTTON_TIP);
    ASSERT1(bNameValid);
    captionbar_.SetButtonToolTip(strTemp);

    bNameValid = strTemp.LoadString(IDS_CAPTION_TEXT);
    ASSERT1(bNameValid);
    captionbar_.SetText(strTemp, CMFCCaptionBar::ALIGN_LEFT);

    captionbar_.SetBitmap(IDB_INFO, RGB(255, 255, 255), FALSE, CMFCCaptionBar::ALIGN_LEFT);
    bNameValid = strTemp.LoadString(IDS_CAPTION_IMAGE_TIP);
    ASSERT1(bNameValid);
    bNameValid = strTemp2.LoadString(IDS_CAPTION_IMAGE_TEXT);
    ASSERT1(bNameValid);
    captionbar_.SetImageToolTip(strTemp, strTemp2);

    return TRUE;
}

void CMainFrame::OnWindowManager(){
    ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id){
    CWaitCursor wait;

    kTheApp.SetAppLook(id);

    switch (kTheApp.GetAppLook()){
    case ID_VIEW_APPLOOK_WIN_2000:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
        ribbonbar_.SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_OFF_XP:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
        ribbonbar_.SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_WIN_XP:
        CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
        ribbonbar_.SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_OFF_2003:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
        CDockingManager::SetDockingMode(DT_SMART);
        ribbonbar_.SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_VS_2005:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
        CDockingManager::SetDockingMode(DT_SMART);
        ribbonbar_.SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_VS_2008:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
        CDockingManager::SetDockingMode(DT_SMART);
        ribbonbar_.SetWindows7Look(FALSE);
        break;

    case ID_VIEW_APPLOOK_WINDOWS_7:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
        CDockingManager::SetDockingMode(DT_SMART);
        ribbonbar_.SetWindows7Look(TRUE);
        break;

    default:
        switch (kTheApp.GetAppLook()){
        case ID_VIEW_APPLOOK_OFF_2007_BLUE:
            CMFCVisualManagerOffice2007::SetStyle(
                CMFCVisualManagerOffice2007::Office2007_LunaBlue);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_BLACK:
            CMFCVisualManagerOffice2007::SetStyle(
                CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_SILVER:
            CMFCVisualManagerOffice2007::SetStyle(
                CMFCVisualManagerOffice2007::Office2007_Silver);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_AQUA:
            CMFCVisualManagerOffice2007::SetStyle(
                CMFCVisualManagerOffice2007::Office2007_Aqua);
            break;
        }

        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
        CDockingManager::SetDockingMode(DT_SMART);
        ribbonbar_.SetWindows7Look(FALSE);
    }

    RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW 
        | RDW_FRAME | RDW_ERASE);

    kTheApp.WriteInt(_T("ApplicationLook"), kTheApp.GetAppLook());
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI){
    pCmdUI->SetRadio(kTheApp.GetAppLook() == pCmdUI->m_nID);
}

void CMainFrame::OnViewCaptionBar(){
    captionbar_.ShowWindow(captionbar_.IsVisible() ? SW_HIDE : SW_SHOW);
    RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewCaptionBar(CCmdUI* pCmdUI){
    pCmdUI->SetCheck(captionbar_.IsVisible());
}

void CMainFrame::OnOptions(){
    CMFCRibbonCustomizeDialog *pOptionsDlg = 
        new CMFCRibbonCustomizeDialog(this, &ribbonbar_);
    ASSERT1(pOptionsDlg != NULL);

    pOptionsDlg->DoModal();
    delete pOptionsDlg;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection){
    CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);

    widget_output_.UpdateFonts();
}

BOOL CMainFrame::OnEraseMDIClientBackground(CDC *pDC ){
    CBrush brush(RGB(127, 127, 127));
    CRect clipbox;
    pDC->GetClipBox(&clipbox);
    pDC->FillRect(&clipbox, &brush);

    CRect databox;
    ::GetClientRect(m_hWndMDIClient, &databox);

    if(!databox.EqualRect(logo_bound_)){
        ::InvalidateRect(m_hWndMDIClient, &logo_bound_, TRUE);
    }
    logo_bound_ = databox;

    TEXTMETRIC tm;
    pDC->GetTextMetrics(&tm);
    CFont* oldFont = pDC->SelectObject(&logo_font_);
    CSize sz       = pDC->GetTextExtent(logo_str_, logo_str_.GetLength());

    // GetTextExtent calculates the size of the displayed logo
    // which depends on the device context....
    // Calculate the box size by subtracting the text width and height from the
    // window size.  Also subtract 20% of the average character size to keep the
    //// logo from printing into the borders...
    databox.left = databox.right  - sz.cx - tm.tmAveCharWidth / 2;
    databox.top  = databox.bottom - sz.cy - tm.tmHeight / 5;
    pDC->SetBkMode(TRANSPARENT);

    // shift logo box right, and print black...
    COLORREF oldColor = pDC->SetTextColor(RGB(0,0,0));
    pDC->DrawText(logo_str_, logo_str_.GetLength(), &databox, 
                  DT_VCENTER | DT_SINGLELINE | DT_CENTER);

    //// shift logo box left and print white
    databox.left -= 3 * tm.tmAveCharWidth / 5;
    pDC->SetTextColor(RGB(255,255,255));
    pDC->DrawText(logo_str_, logo_str_.GetLength(), &databox,
        DT_VCENTER | DT_SINGLELINE | DT_CENTER);

    //// Restore original location and print in the button face color
    databox.left += tm.tmAveCharWidth / 5;
    pDC->SetTextColor(GetSysColor(COLOR_BTNFACE));
    pDC->DrawText(logo_str_, logo_str_.GetLength(), &databox, 
                  DT_VCENTER | DT_SINGLELINE | DT_CENTER);

    // restore the original properties and release resources...
    pDC->SelectObject(oldFont);
    pDC->SetTextColor(oldColor);   

    return TRUE;
}