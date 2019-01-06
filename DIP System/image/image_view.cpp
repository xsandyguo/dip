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

#include "image/image_view.h"

#include <base/scoped_ptr.h>
#include <ximage.h>
#include "image/common/config_manager.h"
#include "image/common/memdc.h"
#include "image/common/bcmenu.h"
#include "image/image_document.h"
#include "image/ui/DlgHisto.h"
#include "image/ui/DlgFloodFill.h"

#include "ui/main_frame.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CImageView,  CScrollView)

BEGIN_MESSAGE_MAP(CImageView,  CScrollView)
    ON_WM_MOUSEMOVE()
    ON_WM_ERASEBKGND()
    ON_WM_SETFOCUS()
    ON_WM_LBUTTONDOWN()
    ON_WM_CONTEXTMENU()
    ON_WM_LBUTTONUP()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_WM_CHAR()
    ON_WM_RBUTTONDOWN()
    ON_WM_SETCURSOR()

    ON_COMMAND(ID_FILE_PRINT        ,  CScrollView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT ,  CScrollView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW,  CScrollView::OnFilePrintPreview)
    ON_MESSAGE(WM_USER_NEWIMAGE,  OnNewImage)
    ON_MESSAGE(WM_USER_PROGRESS,  OnNewProgress)
END_MESSAGE_MAP()


CImageView::CImageView()
{
    SetScrollSizes(MM_TEXT,  CSize(0,  0));
    
    m_tracker.m_rect = CRect(0, 0, 0, 0);
    m_tracker.m_nStyle = 0;
    m_tracker.m_nStyle |= CRectTracker::dottedLine;
    m_tracker.m_nStyle |= CRectTracker::resizeOutside;
}

CImageView::~CImageView()
{
}

void CImageView::DrawCanvas(CDC* pDC){
    Graphics g(pDC->m_hDC); 	

/*    CRect clipbox;
    GetClientRect(&clipbox);
    Rect rect(clipbox.left,  clipbox.top,  clipbox.Width(),  clipbox.Height());  

    int x,  y;
    int countW = rect.Width / 16 + 1;
    int countH = rect.Height / 8 + 1;
    SolidBrush gray(Color(255,  204,  204,  204));
    SolidBrush white(Color::White);

    for(int j = 0; j < countH; j++){
        for(int i = 0; i < countW; i++){
            x = i * 16 + rect.X;
            y = j * 8 + rect.Y;
            g.FillRectangle(j % 2 == 0? &white : &gray,  Rect(x,  y,  8,  8));			 			 
            x += 8;
            g.FillRectangle(j % 2 == 0? &gray : &white,  Rect(x,  y,  8,  8));
        }
    } */	
    CRect clipbox;
    pDC->GetClipBox(&clipbox);
    Rect rect(clipbox.left,  clipbox.top,  clipbox.Width(),  clipbox.Height());  

    SolidBrush gray(Color(192,  192,  192));
    g.FillRectangle(&gray,  rect);
}

void CImageView::OnDraw(CDC* pDC)
{
    CImageDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    BOOL bPrinting = pDC->IsPrinting();

    scoped_ptr<CMyMemDC> pMemDC;
    if (!bPrinting){
        pMemDC.reset(new CMyMemDC(pDC));
        pDC = pMemDC.get();
    }

    DrawCanvas(pDC);

    CxImage* ima = pDoc->GetImage(); 
    if (!ima) {
        return;
    }

    if (bPrinting) {
        int cxPage = pDC->GetDeviceCaps(HORZRES);
        int cyPage = pDC->GetDeviceCaps(VERTRES);

        int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
        int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
        // Best Fit case: create a rectangle which preserves the aspect ratio
        int cx = ima->GetXDPI() ? (ima->GetWidth()*cxInch)/ima->GetXDPI():ima->GetWidth()*cxInch/96;
        int cy = ima->GetYDPI() ? (ima->GetHeight()*cyInch)/ima->GetYDPI():ima->GetHeight()*cyInch/96;

        CxImage tmp;
        tmp.Copy(*ima);
        RGBQUAD c={255, 255, 255, 0};
        if (tmp.GetTransIndex()>=0) tmp.SetPaletteColor((BYTE)tmp.GetTransIndex(), c);
        tmp.SetTransColor(c);
        tmp.AlphaStrip();
        tmp.Stretch(pDC->GetSafeHdc(),  CRect(100, 100, cx, cy));

        return;
    }     
 
    CRect rect;
    GetClientRect(&rect);
    if (pDoc->GetStretchMode()) {
        ima->Draw(pDC->GetSafeHdc(),  rect, 0, pDoc->GetSmoothMode()!=0);
    } else {
        float zoom=pDoc->GetZoomFactor();
        int width  = (int)(ima->GetWidth()*zoom);
        int height = (int)(ima->GetHeight()*zoom);

        CRect imgRect;
        imgRect.left   = rect.Width() / 2 - width / 2;
        imgRect.top    = rect.Height() / 2 - height / 2;
        imgRect.right  = imgRect.left + width;
        imgRect.bottom = imgRect.top + height;

        if (zoom==1)
            ima->Draw(pDC->GetSafeHdc(),  imgRect);
        else
            ima->Draw(pDC->GetSafeHdc(),  imgRect,  0,  pDoc->GetSmoothMode()!=0);
    }

    if ( m_tracker.m_rect.Width()>0 && m_tracker.m_rect.Height()>0 )
        m_tracker.Draw(pDC);    
}

BOOL CImageView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    pInfo->SetMaxPage(1);
    return DoPreparePrinting(pInfo);
}

void CImageView::OnBeginPrinting(CDC* /*pDC*/,  CPrintInfo* /*pInfo*/)
{
}

void CImageView::OnEndPrinting(CDC* /*pDC*/,  CPrintInfo* /*pInfo*/)
{
}

void CImageView::GetImageXY(CImageDocument *pDoc,  CxImage *ima,  long *x,  long *y)
{
    if (!ima || !pDoc) return;

    CPoint point=GetScrollPosition();
    float fx =(float)(*x + point.x);
    float fy =(float)(*y + point.y);

    fx/=pDoc->GetZoomFactor();
    fy/=pDoc->GetZoomFactor();

    if (pDoc->GetStretchMode())	{
        CRect rect;
        GetClientRect(&rect);
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        fx *= ima->GetWidth()/(float)width;
        fy *= ima->GetHeight()/(float)height;
    }
    *x = (long)fx;
    *y = (long)fy;
}

// CImageView message handlers

void CImageView::OnMouseMove(UINT nFlags,  CPoint point) 
{
    CImageDocument* pDoc = GetDocument();
    CxImage*  ima  = pDoc->GetImage();
    if (!ima)	return;

    // We'll get the RGB values at the point the user selects
    long x = point.x;
    long y = point.y;
    GetImageXY(pDoc,  ima,  &x, &y);

    TCHAR s[80];
    if (ima->IsInside(x, y))	{

        long yflip = ima->GetHeight() - y - 1;
        _stprintf(s, _T("x: %d y: %d  idx: %d"),  x,  y,  ima->GetPixelIndex(x, yflip));
        RGBQUAD rgb=ima->GetPixelColor(x, yflip);
        if (ima->AlphaIsValid()) rgb.rgbReserved=ima->AlphaGet(x, yflip);
        else rgb.rgbReserved=ima->GetPaletteColor(ima->GetPixelIndex(x, yflip)).rgbReserved;
        _stprintf(&s[_tcslen(s)], _T("  RGBA: (%d,  %d,  %d,  %d)"),  rgb.rgbRed,  rgb.rgbGreen,  rgb.rgbBlue,  rgb.rgbReserved);

        //Enable these lines if you want draw over the image	
        //if ((nFlags & MK_LBUTTON)==MK_LBUTTON){
        //	ima->SetPixelColor(x, yflip, RGB(rand()/(RAND_MAX/256), rand()/(RAND_MAX/256), rand()/(RAND_MAX/256)));
        //	Invalidate(0);
        //}
#ifdef VATI_EXTENSIONS
        if (nFlags & MK_RBUTTON && !(nFlags & MK_LBUTTON))
        {
            switch (pDoc->m_tool){
            case 1: // selection
                if ( nFlags & MK_CONTROL ) // CTRL+right button: move selection
                {
                    for (int i=0; i<pDoc->m_NumSel; i++)
                    {
                        pDoc->m_Sel[i].x += (long)(x-m_oldPnt.x);
                        pDoc->m_Sel[i].y += (long)(y-m_oldPnt.y);
                    }
                    m_oldPnt.x = x;
                    m_oldPnt.y = y;
                    //redraw selection
                    CWnd* pFrame=GetParentFrame();
                    RECT rClient;
                    pFrame->GetClientRect(&rClient);
                    pFrame->RedrawWindow( &rClient,  NULL,  RDW_INVALIDATE | RDW_UPDATENOW);
                    DrawSelection();
                }
                else if ( !(nFlags & MK_CONTROL) && pDoc->m_isRectSel && pDoc->m_NumSel==5 )
                {
                    // calculate angle difference looking from rectangle center
                    double d_angle = 
                        atan2( (y - m_orgPnt.y),  (x - m_orgPnt.x) )
                      - atan2( (m_oldPnt.y - m_orgPnt.y),  (m_oldPnt.x - m_orgPnt.x) );
                    m_oldPnt.x = x;
                    m_oldPnt.y = y;
                    Dpoint2d    p;

                    // rotate corner points around center point
                    for(int i=0;i<5;i++)
                    {
                        p.x = m_dpnt[i].x - m_orgPnt.x;
                        p.y = m_dpnt[i].y - m_orgPnt.y;
                        m_dpnt[i].x = p.x*cos(d_angle) - p.y*sin(d_angle) + m_orgPnt.x;
                        m_dpnt[i].y = p.x*sin(d_angle) + p.y*cos(d_angle) + m_orgPnt.y;
                        // write back to selection
                        pDoc->m_Sel[i].x = (long)m_dpnt[i].x ;
                        pDoc->m_Sel[i].y = (long)m_dpnt[i].y ;

                    }
                    // redraw the rectangle
                    CWnd* pFrame=GetParentFrame();
                    RECT rClient;
                    pFrame->GetClientRect(&rClient);
                    pFrame->RedrawWindow( &rClient,  NULL,  RDW_INVALIDATE | RDW_UPDATENOW);
                    DrawSelection();
                }
            break;			
            } // end switch
        } // end if MK_RBUTTON
#endif	

        if (nFlags & MK_LBUTTON)
            switch (pDoc->m_tool){
            case 0: // move
            {
                SetCursor(LoadCursor(0, IDC_SIZEALL));
                CSize sz(GetTotalSize());
                CWnd* pFrame=GetParentFrame();
                RECT rClient;
                pFrame->GetClientRect(&rClient);
                if (sz.cx>rClient.right) SetScrollPos(SB_HORZ, m_RefScroll.x - point.x + m_RefPoint.x); else SetScrollPos(SB_HORZ, 0);
                if (sz.cy>rClient.bottom) SetScrollPos(SB_VERT, m_RefScroll.y - point.y + m_RefPoint.y); else SetScrollPos(SB_VERT, 0);
                Invalidate(0);
                break;
            }
            case 1:	//selection
                SetCursor(LoadCursor(0, IDC_CROSS));

#ifdef VATI_EXTENSIONS
                if ( nFlags & MK_SHIFT )
                {
                    // rectangle selection
                    pDoc->m_isRectSel = 1;
                    // in rectangle mode,  selection array has 0, 1 or 5 points
                    if (!pDoc->m_NumSel) 
                    {
                        pDoc->m_Sel[0].x = x;
                        pDoc->m_Sel[0].y = y;
                        pDoc->m_NumSel = 1;
                    }
                    else // already has at least one corner
                    {
                        pDoc->m_Sel[1].x = x;
                        pDoc->m_Sel[1].y = pDoc->m_Sel[0].y;
                        pDoc->m_Sel[2].x = x;
                        pDoc->m_Sel[2].y = y;
                        pDoc->m_Sel[3].x = pDoc->m_Sel[0].x;
                        pDoc->m_Sel[3].y = y;
                        //close the rectangle:
                        pDoc->m_Sel[4].x = pDoc->m_Sel[0].x;
                        pDoc->m_Sel[4].y = pDoc->m_Sel[0].y;
                        pDoc->m_NumSel = 5;
                    }
                    // delete old rectangle from display:
                    CWnd* pFrame=GetParentFrame();
                    RECT rClient;
                    pFrame->GetClientRect(&rClient);
                    pFrame->RedrawWindow( &rClient,  NULL,  RDW_INVALIDATE | RDW_UPDATENOW);
                    // draw the new rectangle
                    DrawSelection();
                }
                else
#endif
                {
                    //freehand selection (original)
                    float zoom=pDoc->GetZoomFactor();
                    CPoint pos(GetScrollPosition());
                    int i=pDoc->m_NumSel;
                    pDoc->m_Sel[i].x = (long)((point.x + pos.x)/zoom);
                    pDoc->m_Sel[i].y = (long)((point.y + pos.y)/zoom);
                    if (i<(MAX_SEL_POINTS-2)) pDoc->m_NumSel++;
                    DrawSelection();
                }

        } //end switch

    } else _tcscpy(s, _T(" "));
    
    //CStatusBar& statusBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
    //statusBar.SetPaneText(0,  s);
    
    CScrollView::OnMouseMove(nFlags,  point);
}

BOOL CImageView::OnEraseBkgnd(CDC* /*pDC*/) 
{
    return 1;
}

void CImageView::OnSetFocus(CWnd* pOldWnd) 
{
    CScrollView::OnSetFocus(pOldWnd);
    
    CImageDocument* pDoc = GetDocument();
    if (pDoc) pDoc->UpdateStatusBar();
}

void CImageView::OnLButtonDown(UINT nFlags,  CPoint point) 
{
    CImageDocument* pDoc = GetDocument();
    if (pDoc) {
        switch(pDoc->m_tool){
        case 0: // select
        case 1:
            {
#ifdef VATI_EXTENSIONS
                if ( nFlags & MK_SHIFT )
                    pDoc->m_isRectSel = 1;
                else
                    pDoc->m_isRectSel = 0;
#endif
                CxImage* ima = pDoc->GetImage();
                if (ima) {
                    m_RefScroll = GetScrollPosition();
                    m_RefPoint.x = point.x;
                    m_RefPoint.y = point.y;
                }
            }
            break;
        case 2: // zoom
            if (!pDoc->GetWaitingClick()) PostMessage(WM_COMMAND, ID_VIEW_ZOOMIN);
            break;
        case 4: // tracker
            {
                if (m_tracker.HitTest(point) < 0)
                {
                    CRectTracker track;
                    if (track.TrackRubberBand(this,  point,  true)) {
                        track.m_rect.NormalizeRect();
                        m_tracker.m_rect = track.m_rect;
                        SetImageRectSelection(pDoc, &(m_tracker.m_rect));
                    } else {
                        m_tracker.m_rect = CRect(0, 0, 0, 0);
                    }
                } else {
                    if (m_tracker.Track(this,  point,  true)){
                        m_tracker.m_rect.NormalizeRect();
                        SetImageRectSelection(pDoc, &(m_tracker.m_rect));
                    } else {
                        m_tracker.m_rect = CRect(0, 0, 0, 0);
                    }
                }
                OnUpdate(0, 0, 0);
            }
            break;
        case 3: //text
            {
                //pDoc->m_tool=-1;
                CxImage* ima = pDoc->GetImage();
                if (ima){
                    pDoc->SubmitUndo();
                    long x = point.x;
                    long y = point.y;
                    GetImageXY(pDoc,  ima,  &x, &y);
#ifndef VATI_EXTENSIONS
                    RGBQUAD c = ima->RGBtoRGBQUAD(pDoc->m_color);
                    c.rgbReserved=255;
                    ima->DrawString(0, x, y, pDoc->m_text, c, 
                                pDoc->m_font.lfFaceName, 
                                pDoc->m_font.lfHeight, 
                                pDoc->m_font.lfWeight, 
                                pDoc->m_font.lfItalic, 
                                pDoc->m_font.lfUnderline, 
                                true);
#else
                    ima->DrawStringEx(0, x, y, &GetImageConfig().TextConfig );
#endif
                }
                Invalidate(0);
            }
            break;
        case 5: //flood fill
            {
                CxImage* ima = pDoc->GetImage();
                if (ima){
                    DlgFloodFill* pDlg = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_pDlgFlood;
                    if (pDlg){
                        pDlg->UpdateData(1);
                        GetImageConfig().FloodColor = ima->RGBtoRGBQUAD(pDlg->m_color);
                        GetImageConfig().FloodTolerance = pDlg->m_tol;
                        GetImageConfig().FloodOpacity = pDlg->m_opacity;
                        GetImageConfig().FloodSelect = pDlg->m_select;
                    }
                    pDoc->SubmitUndo();
                    long x = point.x;
                    long y = point.y;
                    GetImageXY(pDoc,  ima,  &x, &y);
                    long yflip = ima->GetHeight() - y - 1;
                    ima->FloodFill(x, yflip, GetImageConfig().FloodColor, 
                                    GetImageConfig().FloodTolerance, GetImageConfig().FloodOpacity, 
                                    GetImageConfig().FloodSelect!=0);
                    Invalidate(0);
                }
            }
            break;
        }

        if (pDoc->GetWaitingClick()){
            pDoc->SetWaitingClick(0);
            CxImage* ima = pDoc->GetImage();
            if (ima) {
                long x = point.x;
                long y = point.y;
                GetImageXY(pDoc,  ima,  &x, &y);
                if (ima->IsInside(x, y))	{
                    pDoc->SubmitUndo();
                    long yflip = ima->GetHeight() - y - 1;
                    ima->SetTransIndex(ima->GetPixelIndex(x, yflip));
                    // <DP> RGB transparency
                    ima->SetTransColor(ima->GetPixelColor(x, yflip));
                    pDoc->UpdateAllViews(NULL);
                }
            }
        }
#if CXIMAGE_DEMO_SELECT
         else {
            KillTimer(1);
            pDoc->m_NumSel=0;
            m_SelShift=0;
        }
#endif
    }
    CScrollView::OnLButtonDown(nFlags,  point);
}

void CImageView::OnLButtonUp(UINT nFlags,  CPoint point) 
{
    CImageDocument* pDoc = GetDocument();

    if (pDoc && pDoc->m_tool == 1){
        CxImage* ima=pDoc->GetImage();
        if (ima){
            int i=pDoc->m_NumSel;
            if (i>2){
#ifdef VATI_EXTENSIONS
                if ( !pDoc->m_isRectSel )
                {
                    pDoc->m_Sel[i].x = pDoc->m_Sel[0].x;
                    pDoc->m_Sel[i].y = pDoc->m_Sel[0].y;
                    pDoc->m_NumSel++;
                }
#else
                pDoc->m_Sel[i].x = pDoc->m_Sel[0].x;
                pDoc->m_Sel[i].y = pDoc->m_Sel[0].y;
                pDoc->m_NumSel++;
#endif
                SetTimer(1, 333, NULL);
                
                long i;
                long n=pDoc->m_NumSel;
                POINT* p = (POINT*)malloc(n*sizeof(POINT));
                long h=ima->GetHeight();
                for(i=0;i<n;i++){
                    p[i].x = pDoc->m_Sel[i].x;
                    p[i].y = h-1-pDoc->m_Sel[i].y;
                }

                pDoc->Stopwatch(0);

                ima->SelectionClear();
                ima->SelectionAddPolygon(p, n);

                pDoc->Stopwatch(1);
                pDoc->UpdateStatusBar();

                free(p);
            } else {
                ima->SelectionDelete();
            }
            Invalidate(0);
        }
    }

    CScrollView::OnLButtonUp(nFlags,  point);
}

void CImageView::OnUpdate(CView* pSender,  LPARAM lHint,  CObject* pHint) 
{
    switch (lHint)
    {
    case WM_USER_NEWIMAGE:
        {
            m_tracker.m_rect = CRect(0, 0, 0, 0);

            CImageDocument* pDoc = GetDocument();
            CxImage*  ima  = pDoc->GetImage();
            if (ima) {
                int px=GetScrollPos(SB_HORZ);
                int py=GetScrollPos(SB_VERT);
                CSize sz(GetTotalSize());
                int x=(int)(ima->GetWidth()*pDoc->GetZoomFactor());
                int y=(int)(ima->GetHeight()*pDoc->GetZoomFactor());
                SetScrollSizes(MM_TEXT, 	CSize(x, y));
                CSize sz2(GetTotalSize());

                CWnd* pFrame=GetParentFrame();
                RECT rClient;
                pFrame->GetClientRect(&rClient);

                if (sz.cx!=0 && sz.cy!=0){
                    if (x>rClient.right) SetScrollPos(SB_HORZ, sz2.cx*px/sz.cx); else  SetScrollPos(SB_HORZ, 0);
                    if (y>rClient.bottom) SetScrollPos(SB_VERT, sz2.cy*py/sz.cy); else SetScrollPos(SB_VERT, 0);
                }      

                if (!ima->SelectionIsValid()) KillTimer(1);

#ifdef VATI_EXTENSIONS
                ima->SetJpegQualityF(GetImageConfig().OptJpegQuality);
  #if CXIMAGE_SUPPORT_JPG
                ima->SetCodecOption(GetImageConfig().OptJpegOptions, CXIMAGE_FORMAT_JPG);
  #endif
  #if CXIMAGE_SUPPORT_RAW
                ima->SetCodecOption(GetImageConfig().OptRawOptions, CXIMAGE_FORMAT_RAW);
  #endif
#endif

                CMainFrame* pMain = (CMainFrame*) AfxGetMainWnd();
                if (pMain->m_HistoBar.IsWindowVisible()){
                    pDoc->m_hmax=ima->Histogram(pDoc->m_hr, pDoc->m_hg, pDoc->m_hb, pDoc->m_hgray);
                    pMain->m_HistoBar.Invalidate();
                } else {
                    pDoc->m_hmax = 0;
                }
            }

            break;
        }
    default:
        {
            CImageDocument* pDoc = GetDocument();
            if (pDoc){
                CxImage*  ima  = pDoc->GetImage();
                if (ima){
                    if (pDoc->GetStretchMode()) SetScrollSizes(MM_TEXT, 	CSize(0, 0));
                    else SetScrollSizes(MM_TEXT, CSize((int)(ima->GetWidth()*pDoc->GetZoomFactor()), 
                                                      (int)(ima->GetHeight()*pDoc->GetZoomFactor())));
                }
            }
        }
    }
    CScrollView::OnUpdate(pSender,  lHint,  pHint);
}

void CImageView::OnContextMenu(CWnd* /*pWnd*/,  CPoint point) 
{
    CImageDocument* pDoc = GetDocument();
    if (pDoc && pDoc->m_tool==2) return;
#ifdef VATI_EXTENSIONS
    // suppress context menu if right-click function was called just now
    if ( m_suppressContextMenu == true )
        return;
#endif
    static UINT toolbars[]={
        IDR_MAINFRAME, 
        IDR_TOOLBAR1, 
    };
    BCMenu menu;
    menu.LoadMenu(IDM_CXIMAGE1);
    menu.LoadToolbars(toolbars, 2);
    CMenu *pContextMenu=menu.GetSubMenu(0);
    pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}

LRESULT CImageView::OnNewImage(WPARAM /*wParam*/,  LPARAM /*lParam*/)
{
    CImageDocument* pDoc = GetDocument();
    pDoc->UpdateAllViews(0, WM_USER_NEWIMAGE);
    pDoc->UpdateStatusBar();
    return 0;
}

LRESULT CImageView::OnNewProgress(WPARAM wParam,  LPARAM /*lParam*/)
{
    //CStatusBar& statusBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetStatusBar();
    //CString s;
    //s.Format(_T("%d %%"), (int)wParam);
    //statusBar.SetPaneText(2,  s);
    //statusBar.Invalidate(1);

/*	CProgressCtrl& progressBar = ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->GetProgressBar();
    progressBar.SetPos((int)wParam);
    progressBar.Invalidate(1);*/

    return 0;
}

void CALLBACK LineDDAProc(int x,  int y,  CImageView *pview)
{
    CDC* pDC = pview->GetDC();
    if (pDC) {
        pview->m_SelCount++;
        if (pview->m_SelCount <= 4)	pDC->SetPixelV(x, y, RGB(255, 255, 255));
        else pDC->SetPixelV(x, y, RGB(0, 0, 0));
        if (pview->m_SelCount == 8) pview->m_SelCount=0;
    }
    pview->ReleaseDC(pDC); //<jh>
}

void CImageView::DrawSelection()
{
    CImageDocument* pDoc = GetDocument();
    if (pDoc->m_NumSel>2){
        CPoint pos(GetScrollPosition());
        float zoom=pDoc->GetZoomFactor();
        m_SelCount=m_SelShift;
//		pDC->MoveTo(zoom*pDoc->m_Sel[0].x - pos.x, zoom*pDoc->m_Sel[0].y - pos.y);
        for(int i=1;i<pDoc->m_NumSel;i++){
//			pDC->LineTo(zoom*pDoc->m_Sel[i].x - pos.x, zoom*pDoc->m_Sel[i].y - pos.y);
            LineDDA((int)(zoom*pDoc->m_Sel[i-1].x - pos.x), 
                    (int)(zoom*pDoc->m_Sel[i-1].y - pos.y), 
                    (int)(zoom*pDoc->m_Sel[i].x - pos.x), 
                    (int)(zoom*pDoc->m_Sel[i].y - pos.y), 
                    (LINEDDAPROC)LineDDAProc, (LPARAM)this);
        }
    }
}

void CImageView::OnDestroy() 
{
    CScrollView::OnDestroy();
    ((CMainFrame*)AfxGetMainWnd())->m_HistoBar.Invalidate();
    KillTimer(1);
    KillTimer(2);
}

void CImageView::OnTimer(UINT nIDEvent) 
{
    if (nIDEvent == 1) {
        m_SelShift++;
        if (m_SelShift==8) m_SelShift=0;
        DrawSelection();
    }

    if (nIDEvent == 2) {
        CImageDocument* pDoc = GetDocument();
        if (pDoc) {
            pDoc->PlayNextFrame();
        }
    }
    CScrollView::OnTimer(nIDEvent);
}

void CImageView::OnRButtonDown(UINT nFlags,  CPoint point) 
{
    CImageDocument* pDoc = GetDocument();
    if (pDoc) {
        switch(pDoc->m_tool){
        case 1:
            {
#ifdef VATI_EXTENSIONS
            m_suppressContextMenu = false; //default: Context Menu can be displayed on ButtonUp
            CxImage *ima = pDoc->GetImage();
            if ( !(nFlags & MK_CONTROL) && pDoc->m_isRectSel && pDoc->m_NumSel == 5 && ima )
            {
                // calculate origo
                m_orgPnt.x = (pDoc->m_Sel[0].x + pDoc->m_Sel[1].x)/2;
                m_orgPnt.y = (pDoc->m_Sel[0].y + pDoc->m_Sel[3].y)/2;
                for(int i=0;i<5;i++)
                {
                    // copy corner coordinates to double-prec array
                    m_dpnt[i].x = pDoc->m_Sel[i].x ;
                    m_dpnt[i].y = pDoc->m_Sel[i].y ;
                }

                // get real coo from point
                long x = point.x;
                long y = point.y;
                GetImageXY(pDoc,  ima,  &x, &y);

                // initialise old point var.
                m_oldPnt.x = x;
                m_oldPnt.y = y;
                m_suppressContextMenu = true; // ButtonUp should not invoke context menu
            }
            else if ( nFlags & MK_CONTROL && ima /* && pDoc->m_isRectSel && pDoc->m_NumSel == 5 */)
            {
                // get real coo from point
                long x = point.x;
                long y = point.y;
                GetImageXY(pDoc,  ima,  &x, &y);

                // initialise old point var.
                m_oldPnt.x = x;
                m_oldPnt.y = y;
                m_suppressContextMenu = true; // ButtonUp should not invoke context menu
            }
#endif
            }
            break;
        case 2:
            if (!pDoc->GetWaitingClick()) PostMessage(WM_COMMAND, ID_VIEW_ZOOMOUT);
            return;
            break;
#ifdef VATI_EXTENSIONS
        default:
            m_suppressContextMenu = false; //default: Context Menu can be displayed on ButtonUp
#endif
        } // end switch
    } // end if pDoc
    
    CScrollView::OnRButtonDown(nFlags,  point);
}

void CImageView::OnActivateView(BOOL bActivate,  CView* pActivateView,  CView* pDeactiveView) 
{
    if (bActivate){
        ((CMainFrame*)AfxGetMainWnd())->m_HistoBar.Invalidate();
    }
    
    CScrollView::OnActivateView(bActivate,  pActivateView,  pDeactiveView);
}

BOOL CImageView::OnSetCursor(CWnd* pWnd,  UINT nHitTest,  UINT message) 
{
    if (m_tracker.SetCursor( pWnd,  nHitTest )) return TRUE;
    return CScrollView::OnSetCursor(pWnd,  nHitTest,  message);
}

bool CImageView::SetImageRectSelection(CImageDocument *pDoc, CRect *rect)
{
    if (pDoc==0)
        return false;

    CxImage* ima = pDoc->GetImage();

    if (ima==0)
        return false;

    long x, y;
    RECT rect_img;
    x = rect_img.left = rect->left;
    y = rect_img.top = rect->top;
    GetImageXY(pDoc,  ima,  &x, &y);
    rect_img.top = ima->GetHeight() - 1 - y;
    rect_img.left = x;

    x = rect_img.right = rect->right;
    y = rect_img.bottom = rect->bottom;
    GetImageXY(pDoc,  ima,  &x, &y);
    rect_img.bottom = ima->GetHeight() - 1 - y;
    rect_img.right = x;

    ima->SelectionClear();
    ima->SelectionAddRect(rect_img);

    return true;
}

CImageDocument* CImageView::GetDocument() // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageDocument)));
    return (CImageDocument*)m_pDocument;
}