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

#include "ui/toolbox_ctrl.h"

#include "app/app.h"
#include "base/debug.h"

#define AFX_ID_SCROLL_VERT 2

#define kVisualManager CMFCVisualManager::GetInstance()

IMPLEMENT_DYNAMIC(CToolElement, CObject)

CToolElement::CToolElement(const CString& strTabName)
    :name_(strTabName),
    category_(-1),
    tagged_data_(NULL),
    is_tooltab_(TRUE),
    is_expanded_(TRUE){
    Init();
}

CToolElement::CToolElement(const CString& strName, int category, UINT nIDIcon)
    :name_(strName), 
    category_(category),
    tagged_data_(NULL),
    is_tooltab_(FALSE),
    is_expanded_(FALSE){
    icon_handle_ = (HICON)::LoadImage(AfxGetInstanceHandle(),
        MAKEINTRESOURCE(nIDIcon), 
        IMAGE_ICON,16, 16, 0);

    Init();
}

CToolElement::~CToolElement(){
    while (!subtool_list_.IsEmpty()){
        delete subtool_list_.RemoveHead();
    }
}

void CToolElement::Init(){
    toolbox_list_      = NULL;
    is_enabled_        = TRUE;
    parent_            = NULL;
    is_name_truncated_ = FALSE;

    rect_.SetRectEmpty();
    is_visible_ = TRUE;
}

BOOL CToolElement::AddSubTool(CToolElement* pTool){
    ASSERT_VALID(this);
    ASSERT_VALID(pTool);

    if (!IsToolTab()){
        ASSERT1(FALSE);
        return FALSE;
    }

    if (pTool->toolbox_list_ != NULL){
        for (POSITION pos = pTool->toolbox_list_->tooltabs_list_.GetHeadPosition(); 
            pos != NULL;){
            CToolElement* pLstToolTab = pTool->toolbox_list_->tooltabs_list_.GetNext(pos);
            ASSERT_VALID(pLstToolTab);

            if (pLstToolTab == pTool || pLstToolTab->IsSubTool(pTool)){
                // Can't ad the same tool twice
                ASSERT1(FALSE);
                return FALSE;
            }
        }
    }

    pTool->parent_ = this;

    subtool_list_.AddTail(pTool);
    pTool->toolbox_list_ = toolbox_list_;

    return TRUE;
}

BOOL CToolElement::RemoveSubTool(CToolElement*& pTool, BOOL bDelete/* = TRUE*/){
    ASSERT_VALID(this);
    ASSERT_VALID(pTool);

    for (POSITION pos = subtool_list_.GetHeadPosition(); pos != NULL;){
        POSITION posSaved = pos;

        CToolElement* pSubTool = subtool_list_.GetNext(pos);
        ASSERT_VALID(pSubTool);

        if (pSubTool == pTool){
            subtool_list_.RemoveAt(posSaved);

            if (toolbox_list_ != NULL && toolbox_list_->selected_item_ == pTool){
                toolbox_list_->selected_item_ = NULL;
            }

            if (bDelete){
                delete pTool;
                pTool = NULL;
            }

            return TRUE;
        }
    }

    return FALSE;
}

void CToolElement::Expand(BOOL bExpand) {
    ASSERT_VALID(this);
    ASSERT1(IsToolTab());

    if (is_expanded_ == bExpand || subtool_list_.IsEmpty()){
        return;
    }

    is_expanded_ = bExpand;

    if (toolbox_list_ != NULL && toolbox_list_->GetSafeHwnd() != NULL){
        ASSERT_VALID(toolbox_list_);
        toolbox_list_->AdjustLayout();

        CRect rectRedraw = toolbox_list_->rect_list_;
        rectRedraw.top = rect_.top;

        toolbox_list_->RedrawWindow(rectRedraw);
    }
}

CToolElement* CToolElement::GetTool(int nClass) const {
    ASSERT_VALID(this);

    for (POSITION pos = subtool_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pTool = subtool_list_.GetNext(pos);
        ASSERT_VALID(pTool);

        if (pTool->GetCategory() == nClass){
            return pTool;
        }
    }

    return NULL;
}

BOOL CToolElement::IsSubTool(CToolElement* pSubTool) const {
    ASSERT_VALID(this);
    ASSERT_VALID(pSubTool);

    for (POSITION pos = subtool_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pTool = subtool_list_.GetNext(pos);
        ASSERT_VALID(pTool);

        if (pSubTool == pTool){
            return TRUE;
        }
    }

    return FALSE;
}

int CToolElement::GetExpandedSubTools(BOOL bIncludeHidden) const {
    ASSERT_VALID(this);

    if (!is_expanded_){
        return 0;
    }

    int nCount = 0;
    if(bIncludeHidden){
        nCount=subtool_list_.GetCount();
    }else {
        for (POSITION pos = subtool_list_.GetHeadPosition(); pos != NULL;){
            CToolElement* pTool = subtool_list_.GetNext(pos);
            ASSERT_VALID(pTool);

            if (pTool->IsVisible()){
                nCount ++;
            }
        }
    }
    return nCount;
}

CToolElement* CToolElement::HitTest(CPoint point){
    ASSERT_VALID(this);
    ASSERT_VALID(toolbox_list_);

    if (rect_.PtInRect(point)){
        return this;
    }

    if(!IsToolTab()||!IsExpanded())
        return NULL;

    for (POSITION pos = subtool_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pTool = subtool_list_.GetNext(pos);
        ASSERT_VALID(pTool);

        CToolElement* pHit = pTool->HitTest(point);
        if (pHit != NULL){
            return pHit;
        }
    }

    return NULL;
}

BOOL CToolElement::IsParentExpanded() const {
    ASSERT_VALID(this);
    if(parent_==NULL)
        return TRUE;

    if (!parent_->IsExpanded()){
        return FALSE;
    }

    return TRUE;
}

BOOL CToolElement::IsSelected() const {
    ASSERT_VALID(this);
    ASSERT_VALID(toolbox_list_);

    return toolbox_list_->selected_item_ == this;
}

BOOL CToolElement::IsHovered() const {
    ASSERT_VALID(this);
    ASSERT_VALID(toolbox_list_);

    return toolbox_list_->hover_item_==this;
}

void CToolElement::SetOwnerList(CToolBoxCtrl* pWndList){
    ASSERT_VALID(this);
    ASSERT_VALID(pWndList);

    toolbox_list_ = pWndList;

    for (POSITION pos = subtool_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pTool = subtool_list_.GetNext(pos);
        ASSERT_VALID(pTool);

        pTool->SetOwnerList(toolbox_list_);
    }
}

void CToolElement::Redraw(){
    ASSERT_VALID(this);

    if (toolbox_list_ != NULL){
        ASSERT_VALID(toolbox_list_);
        toolbox_list_->InvalidateRect(rect_);

        toolbox_list_->UpdateWindow();
    }
}

void CToolElement::OnDrawExpandBox(CDC* pDC, CRect rect){
    ASSERT_VALID(this);
    ASSERT_VALID(pDC);
    ASSERT_VALID(toolbox_list_);
    ASSERT1(IsToolTab());

    CPoint ptCenter = rect.CenterPoint();

    int nBoxSize = min(9, rect.Width());

    rect = CRect(ptCenter, CSize(1, 1));
    rect.InflateRect(nBoxSize / 2, nBoxSize / 2);

    COLORREF clrText = afxGlobalData.clrBarText;

    kVisualManager->OnDrawExpandingBox(pDC, rect, is_expanded_ 
        && !subtool_list_.IsEmpty(), clrText);
}

void CToolElement::OnDrawIcon(CDC* pDC, CRect rect){
    ASSERT_VALID(this);
    ASSERT_VALID(pDC);
    ASSERT_VALID(toolbox_list_);

    if (IsToolTab()||icon_handle_==NULL){
        return;
    }

    rect.OffsetRect(3, 2);
    DrawIconEx(pDC->GetSafeHdc(),rect.left,rect.top,icon_handle_,16,16,0,NULL,DI_NORMAL);
}

void CToolElement::OnDrawName(CDC* pDC, CRect rect){
    ASSERT_VALID(this);
    ASSERT_VALID(pDC);
    ASSERT_VALID(toolbox_list_);

    rect.DeflateRect(AFX_TEXT_MARGIN, 0);

    pDC->DrawText(name_, rect, DT_LEFT | DT_SINGLELINE | DT_VCENTER 
        | DT_NOPREFIX | DT_END_ELLIPSIS);

    is_name_truncated_ = pDC->GetTextExtent(name_).cx > rect.Width();
}

void CToolElement::Reposition(int& y){
    ASSERT_VALID(this);
    ASSERT_VALID(toolbox_list_);

    CRect rectOld = rect_;

    BOOL bShowProperty = is_visible_ &&IsParentExpanded();

    if (bShowProperty){
        int nHeight = IsToolTab() ? toolbox_list_->tab_height_:toolbox_list_->tool_height_;

        rect_ = CRect(toolbox_list_->rect_list_.left+3, y, 
            toolbox_list_->rect_list_.right-3, y + nHeight);
        y += nHeight;
    }else{
        rect_.SetRectEmpty();
    }

    for (POSITION pos = subtool_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pTool = subtool_list_.GetNext(pos);
        ASSERT_VALID(pTool);

        pTool->Reposition(y);
    }
}

///////////////////////////////////////////////////////////////
// CToolBoxCtrl

IMPLEMENT_DYNAMIC(CToolBoxCtrl, CWnd)

CToolBoxCtrl::CToolBoxCtrl(){
    regular_font_           = NULL;
    rect_list_.SetRectEmpty();
    tab_height_             = 16;
    tool_height_            = 20;
    vertical_scroll_offset_ = 0;
    vertical_scroll_total_  = 0;
    vertical_scroll_page_   = 0;
    selected_item_          = NULL;
    hover_item_             = NULL;
    callback_               = NULL;

    background_color_       = (COLORREF)-1;
    selected_color_         = RGB(225,230,232);
    hover_color_            = RGB(193,210,238);
    selected_border_color_  = RGB(49,106,197);
    tooltab_color_          = RGB(217,215,198);
}

CToolBoxCtrl::~CToolBoxCtrl(){
}


BEGIN_MESSAGE_MAP(CToolBoxCtrl, CWnd)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_CREATE()
    ON_WM_VSCROLL()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_ERASEBKGND()
    ON_WM_MOUSELEAVE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CToolBoxCtrl message handlers
BOOL CToolBoxCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID){
    return CWnd::Create(afxGlobalData.RegisterWindowClass(_T("Afx:ToolBox")),
                        _T(""),
                        dwStyle,
                        rect, 
                        pParentWnd, 
                        nID, 
                        NULL);
}

int CToolBoxCtrl::AddToolTab(CToolElement* pToolTab, BOOL bRedraw, BOOL bAdjustLayout){
    ASSERT_VALID(this);
    ASSERT_VALID(pToolTab);

    for (POSITION pos = tooltabs_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pListTool = tooltabs_list_.GetNext(pos);
        ASSERT_VALID(pListTool);

        if (pListTool == pToolTab){
            // Can't add the same ToolTab twice
            ASSERT1(FALSE);
            return -1;
        }
    }

    pToolTab->SetOwnerList(this);

    tooltabs_list_.AddTail(pToolTab);
    int nIndex = (int) tooltabs_list_.GetCount() - 1;

    if (bAdjustLayout){
        AdjustLayout();

        if (bRedraw && GetSafeHwnd() != NULL){
            pToolTab->Redraw();
        }
    }

    return nIndex;
}

BOOL CToolBoxCtrl::DeleteToolTab(CToolElement*& pTool, BOOL bRedraw/* = TRUE*/, BOOL bAdjustLayout/* = TRUE*/){
    ASSERT_VALID(this);
    ASSERT_VALID(pTool);

    BOOL bFound = FALSE;

    for (POSITION pos = tooltabs_list_.GetHeadPosition(); pos != NULL;){
        POSITION posSaved = pos;

        CToolElement* pToolTab = tooltabs_list_.GetNext(pos);
        ASSERT_VALID(pToolTab);

        if (pToolTab == pTool) // ToolTab
        {
            bFound = TRUE;

            tooltabs_list_.RemoveAt(posSaved);
            break;
        }

        if (pToolTab->RemoveSubTool(pTool, FALSE)){
            bFound = TRUE;
            break;
        }
    }

    if (!bFound){
        return FALSE;
    }

    if (selected_item_ == pTool){
        selected_item_ = NULL;
    }

    delete pTool;
    pTool = NULL;

    if (bAdjustLayout){
        AdjustLayout();
        return TRUE;
    }

    if (bRedraw && GetSafeHwnd() != NULL){
        RedrawWindow();
    }

    return TRUE;
}

void CToolBoxCtrl::RemoveAll(){
    ASSERT_VALID(this);

    while (!tooltabs_list_.IsEmpty()){
        delete tooltabs_list_.RemoveHead();
    }

    selected_item_ = NULL;
}

void CToolBoxCtrl::SetCurSel(CToolElement* pTool, BOOL bRedraw/* = TRUE*/){
    if(pTool==NULL)
        return;

    selected_item_=pTool;
    if(bRedraw)
    selected_item_->Redraw();
}

void CToolBoxCtrl::SetCurSel(int nClass, BOOL bRedraw/*=TRUE*/){
    ASSERT_VALID(this);

    CToolElement* pTool = NULL;
    for (POSITION pos = tooltabs_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pToolTab = tooltabs_list_.GetNext(pos);
        ASSERT_VALID(pToolTab);

        pTool = pToolTab->GetTool(nClass);
        if(pTool){
            CToolElement* pOldSel = selected_item_;
            selected_item_ = pTool;
            if(bRedraw){
                if(pOldSel)
                    pOldSel->Redraw();
                selected_item_->Redraw();
            }

            return;
        }
    }
}

void CToolBoxCtrl::ExpandAll(BOOL bExpand/* = TRUE*/){
    ASSERT_VALID(this);

    for (POSITION pos = tooltabs_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pToolTab = tooltabs_list_.GetNext(pos);
        ASSERT_VALID(pToolTab);

        pToolTab->Expand(bExpand);
    }
}

void CToolBoxCtrl::OnPaint(){
    CPaintDC dc(this); // device context for painting
    // TODO: 在此处添加消息处理程序代码
    // 不为绘图消息调用 CWnd::OnPaint()
    CMemDC memDC(dc, this);
    CDC* pDC = &memDC.GetDC();

    CRect rectClient;
    GetClientRect(rectClient);
    OnFillBackground(pDC, rectClient);

    HFONT hfontOld = SetCurrFont(pDC);
    pDC->SetBkMode(TRANSPARENT);
    OnDrawList(pDC);
    ::SelectObject(pDC->GetSafeHdc(), hfontOld);
}

void CToolBoxCtrl::OnFillBackground(CDC* pDC, CRect rectClient){
    ASSERT_VALID(pDC);

    if (background_brush_.GetSafeHandle() == NULL){
        pDC->FillRect(rectClient, &afxGlobalData.brBtnFace);
    }
    else
    {
        pDC->FillRect(rectClient, &background_brush_);
    }
}

void CToolBoxCtrl::OnDrawList(CDC* pDC){
    ASSERT_VALID(this);
    ASSERT_VALID(pDC);

    for (POSITION pos = tooltabs_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pToolTab = tooltabs_list_.GetNext(pos);
        ASSERT_VALID(pToolTab);

        if (!OnDrawTool(pDC, pToolTab)){
            break;
        }
    }
}

BOOL CToolBoxCtrl::OnDrawTool(CDC* pDC, CToolElement* pTool) const
{
    ASSERT_VALID(this);
    ASSERT_VALID(pDC);
    ASSERT_VALID(pTool);

    if (!pTool->rect_.IsRectEmpty()){
        if (pTool->rect_.top >= rect_list_.bottom){
            return FALSE;
        }

        CRect rectName = pTool->rect_;
        BOOL bFlag=FALSE;
        if(pTool->IsSelected()){
            bFlag=TRUE;
            CBrush bsh(selected_color_);
            CBrush* oldBsh=pDC->SelectObject(&bsh);
            CPen pen(PS_SOLID,1,selected_border_color_);
            CPen* oldPen=pDC->SelectObject(&pen);
            pDC->Rectangle(&rectName);
            pDC->SelectObject(oldBsh);
            pDC->SelectObject(oldPen);
        }
        if(!bFlag&&pTool->IsHovered()){
            bFlag=TRUE;
            CBrush bsh(hover_color_);
            CBrush* oldBsh=pDC->SelectObject(&bsh);
            CPen pen(PS_SOLID,1,selected_border_color_);
            CPen* oldPen=pDC->SelectObject(&pen);
            pDC->Rectangle(&rectName);
            pDC->SelectObject(oldBsh);
            pDC->SelectObject(oldPen);
        }
        

        int nHeight=tool_height_;
        if (pTool->IsToolTab()){
            CRect rectExpand = rectName;
            nHeight=tab_height_;
            rectExpand.right = nHeight;

            if(!bFlag){
                CRect rectFill=rectName;
                rectFill.DeflateRect(1,1);
                CBrush bsh(tooltab_color_);
                pDC->FillRect(rectName, &bsh);
            }

            CRgn rgnClipExpand;
            CRect rectExpandClip = rectExpand;
            rectExpandClip.bottom = min(rectExpandClip.bottom, rect_list_.bottom);

            rgnClipExpand.CreateRectRgnIndirect(&rectExpandClip);
            pDC->SelectClipRgn(&rgnClipExpand);

            pTool->OnDrawExpandBox(pDC, rectExpand);
        }

        COLORREF clrTextOld = (COLORREF)-1;
        if (!pTool->IsEnabled()){
            clrTextOld = pDC->SetTextColor(afxGlobalData.clrGrayedText);
        }

        CRect rectIcon = pTool->rect_;
        rectIcon.right = rectIcon.left + nHeight;

        CRgn rgnClipIcon;
        CRect rectIconClip = rectIcon;
        rectIconClip.bottom = min(rectIconClip.bottom, rect_list_.bottom);

        rgnClipIcon.CreateRectRgnIndirect(&rectIconClip);
        pDC->SelectClipRgn(&rgnClipIcon);

        pTool->OnDrawIcon(pDC, rectIcon);

        CRgn rgnClipName;
        rectName.left += rectIcon.right;
        CRect rectNameClip = rectName;
        rectNameClip.bottom = min(rectNameClip.bottom, rect_list_.bottom);

        rgnClipName.CreateRectRgnIndirect(&rectNameClip);
        pDC->SelectClipRgn(&rgnClipName);

        HFONT hOldFont = NULL;
        if (pTool->IsToolTab()){
            hOldFont = (HFONT) ::SelectObject(pDC->GetSafeHdc(), tooltab_font_.GetSafeHandle());
        }

        pTool->OnDrawName(pDC, rectName);

        if (hOldFont != NULL){
            ::SelectObject(pDC->GetSafeHdc(), hOldFont);
        }

        pDC->SelectClipRgn(NULL);

        if (clrTextOld != (COLORREF)-1){
            pDC->SetTextColor(clrTextOld);
        }
    }

    if (pTool->IsExpanded()){
        for (POSITION pos = pTool->subtool_list_.GetHeadPosition(); pos != NULL;){
            if (!OnDrawTool(pDC, pTool->subtool_list_.GetNext(pos))){
                return FALSE;
            }
        }
    }

    return TRUE;
}

void CToolBoxCtrl::SetCustomColors(COLORREF clrBackground, 
                                   COLORREF clrSelected, 
                                   COLORREF clrHover, 
                                   COLORREF clrSelectedBorder,
                                   COLORREF clrToolTab){
    background_color_      = clrBackground;
    selected_color_        = clrSelected;
    hover_color_           = clrHover;
    selected_border_color_ = clrSelectedBorder;
    tooltab_color_         = clrToolTab;

    background_brush_.DeleteObject();
    if (background_color_ != (COLORREF)-1)
        background_brush_.CreateSolidBrush(background_color_);
}

void CToolBoxCtrl::GetCustomColors(COLORREF& clrBackground,
                                   COLORREF& clrSelected,
                                   COLORREF& clrHover,
                                   COLORREF& clrSelectedBorder,
                                   COLORREF& clrToolTab){
    clrBackground     = background_color_;
    clrSelected       = selected_color_;
    clrHover          = hover_color_;
    clrSelectedBorder = selected_border_color_;
    clrToolTab        = tooltab_color_;
}

HFONT CToolBoxCtrl::SetCurrFont(CDC* pDC){
    ASSERT_VALID(pDC);

    return(HFONT) ::SelectObject(pDC->GetSafeHdc(), regular_font_ != NULL 
        ? regular_font_ : ::GetStockObject(DEFAULT_GUI_FONT));
}

CToolElement* CToolBoxCtrl::HitTest(CPoint pt) const
{
    ASSERT_VALID(this);

    for (POSITION pos = tooltabs_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pToolTab = tooltabs_list_.GetNext(pos);
        ASSERT_VALID(pToolTab);

        CToolElement* pHit = pToolTab->HitTest(pt);
        if (pHit != NULL){
            return pHit;
        }
    }

    return NULL;
}

void CToolBoxCtrl::AdjustLayout(){
    if (GetSafeHwnd() == NULL){
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);
    rect_list_ = rectClient;

    SetScrollSizes();

    ReposTools();

    RedrawWindow();
}

void CToolBoxCtrl::SetScrollSizes(){
    ASSERT_VALID(this);

    if (vertical_scrollbar_.GetSafeHwnd() == NULL){
        return;
    }

    if (tool_height_ == 0){
        vertical_scroll_page_ = 0;
        vertical_scroll_total_ = 0;
        vertical_scroll_offset_ = 0;
    }
    else
    {
        vertical_scroll_page_ = rect_list_.Height() / tool_height_ + 1;
        vertical_scroll_total_ = GetTotalItems(FALSE /* Visible only */);

        if (vertical_scroll_total_ < vertical_scroll_page_){
            vertical_scroll_page_ = 0;
            vertical_scroll_total_ = 0;
        }

        vertical_scroll_offset_ = min(vertical_scroll_offset_, vertical_scroll_total_);
    }

    SCROLLINFO si;

    ZeroMemory(&si, sizeof(SCROLLINFO));
    si.cbSize = sizeof(SCROLLINFO);

    si.fMask  = SIF_RANGE | SIF_POS | SIF_PAGE;
    si.nMin   = 0;
    si.nMax   = vertical_scroll_total_;
    si.nPage  = vertical_scroll_page_;
    si.nPos   = vertical_scroll_offset_;

    SetScrollInfo(SB_VERT, &si, TRUE);
    vertical_scrollbar_.EnableScrollBar(vertical_scroll_total_ > 0 
        ? ESB_ENABLE_BOTH : ESB_DISABLE_BOTH);
    vertical_scrollbar_.EnableWindow();
}

int CToolBoxCtrl::GetTotalItems(BOOL bIncludeHidden) const {
    ASSERT_VALID(this);

    int nCount = 0;

    for (POSITION pos = tooltabs_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pToolTab = tooltabs_list_.GetNext(pos);
        ASSERT_VALID(pToolTab);

        nCount += pToolTab->GetExpandedSubTools(bIncludeHidden) + 1;
    }

    return nCount;
}

void CToolBoxCtrl::ReposTools(){
    ASSERT_VALID(this);

    int y = rect_list_.top - tool_height_ * vertical_scroll_offset_;

    for (POSITION pos = tooltabs_list_.GetHeadPosition(); pos != NULL;){
        CToolElement* pToolTab = tooltabs_list_.GetNext(pos);
        ASSERT_VALID(pToolTab);

        pToolTab->Reposition(y);
    }
}

void CToolBoxCtrl::CreateToolTabFont(){
    if (tooltab_font_.GetSafeHandle() != NULL){
        tooltab_font_.DeleteObject();
    }

    CFont* pFont = CFont::FromHandle((HFONT) ::GetStockObject(DEFAULT_GUI_FONT));
    ASSERT_VALID(pFont);

    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));

    pFont->GetLogFont(&lf);

    lf.lfWeight = FW_BOLD;
    tooltab_font_.CreateFontIndirect(&lf);
}
void CToolBoxCtrl::OnSize(UINT nType, int cx, int cy){
    CWnd::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    AdjustLayout();
}

int CToolBoxCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct){
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  Add your specialized creation code here
    CRect rectDummy;
    rectDummy.SetRectEmpty();

    vertical_scrollbar_.Create(WS_CHILD | WS_VISIBLE | SBS_VERT, 
        rectDummy, this, AFX_ID_SCROLL_VERT);

    AdjustLayout();
    CreateToolTabFont();

    return 0;
}

void CToolBoxCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){
    if (selected_item_ != NULL && pScrollBar->GetSafeHwnd() != NULL){
        return;
    }

    int nPrevOffset = vertical_scroll_offset_;

    switch (nSBCode){
    case SB_LINEUP:
        vertical_scroll_offset_--;
        break;
    case SB_LINEDOWN:
        vertical_scroll_offset_++;
        break;
    case SB_TOP:
        vertical_scroll_offset_ = 0;
        break;
    case SB_BOTTOM:
        vertical_scroll_offset_ = vertical_scroll_total_;
        break;
    case SB_PAGEUP:
        vertical_scroll_offset_ -= vertical_scroll_page_;
        break;
    case SB_PAGEDOWN:
        vertical_scroll_offset_ += vertical_scroll_page_;
        break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:
        vertical_scroll_offset_ = nPos;
        break;
    default:
        return;
    }

    vertical_scroll_offset_ = min(max(0, vertical_scroll_offset_), 
        vertical_scroll_total_ - vertical_scroll_page_ + 1);

    if (vertical_scroll_offset_ == nPrevOffset){
        return;
    }

    SetScrollPos(SB_VERT, vertical_scroll_offset_);

    ReposTools();

    int dy = tool_height_ *(nPrevOffset - vertical_scroll_offset_);
    ScrollWindow(0, dy, rect_list_, rect_list_);

// 	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CToolBoxCtrl::OnMouseMove(UINT nFlags, CPoint point){
    // TODO: Add your message handler code here and/or call default
    CToolElement* pOldHover=hover_item_;
    hover_item_ = HitTest(point);
    if(pOldHover==hover_item_)
        return;
    if(pOldHover)
        pOldHover->Redraw();
    if(hover_item_)
        hover_item_->Redraw();

    // Post message when the mouse pointer leaves the window 
    TRACKMOUSEEVENT   tme;
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_LEAVE;
    tme.hwndTrack = m_hWnd;
    tme.dwHoverTime = HOVER_DEFAULT;
    ::TrackMouseEvent(&tme);

    CWnd::OnMouseMove(nFlags, point);
}

void CToolBoxCtrl::OnLButtonDown(UINT nFlags, CPoint point){
    CToolElement* old_selection = selected_item_;
    selected_item_ = HitTest(point);
    if(selected_item_ == NULL){
        selected_item_ = old_selection;
        return;
    }
    if(!selected_item_->IsEnabled())
        return;
    if(selected_item_ != old_selection){
        selected_item_->Redraw();
        if(old_selection)
            old_selection->Redraw();
    }

    if(callback_){
        callback_->old_value = old_selection;
        callback_->new_value = selected_item_;
        callback_->invoke(callback_);
    }

    if(selected_item_->IsToolTab())
        selected_item_->Expand(!selected_item_->IsExpanded());

    CWnd::OnLButtonDown(nFlags, point);
}

void CToolBoxCtrl::OnLButtonUp(UINT nFlags, CPoint point){
    // TODO: Add your message handler code here and/or call default

    CWnd::OnLButtonUp(nFlags, point);
}

BOOL CToolBoxCtrl::OnEraseBkgnd(CDC* pDC){
    // TODO: Add your message handler code here and/or call default

// 	return CWnd::OnEraseBkgnd(pDC);
    return TRUE;
}

void CToolBoxCtrl::OnMouseLeave(){
    // TODO: Add your message handler code here and/or call default
    if(hover_item_){
        CToolElement* pHover=hover_item_;
        hover_item_=NULL;
        pHover->Redraw();
    }

    CWnd::OnMouseLeave();
}

void CToolBoxCtrl::OnDestroy(){
    while (!tooltabs_list_.IsEmpty()){
        delete tooltabs_list_.RemoveHead();
    }

    selected_item_ = NULL;

    CWnd::OnDestroy();
}
