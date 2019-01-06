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

#include "ui/about_dialog.h"

extern const wchar_t* const using_os;

CAboutDialog::CAboutDialog() : CDialogEx(CAboutDialog::IDD){
}

void CAboutDialog::DoDataExchange(CDataExchange* pDX){
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_ABOUT_OPEN_TECH, opensource_);
}

BEGIN_MESSAGE_MAP(CAboutDialog, CDialogEx)
    ON_WM_CREATE()
END_MESSAGE_MAP()

BOOL CAboutDialog::OnInitDialog(){
    __super::OnInitDialog();

    CHARFORMAT format;
    ZeroMemory(&format, sizeof(CHARFORMAT));
    format.cbSize = sizeof(CHARFORMAT);
    format.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE ;
    format.dwEffects &= ~CFM_BOLD;
    format.dwEffects &= ~CFM_ITALIC;
    format.dwEffects &= ~CFM_UNDERLINE;
    format.yHeight = 180;
    _stprintf(format.szFaceName,_T("ËÎÌו"));
    format.crTextColor = RGB(0, 0, 0);
    opensource_.SetWordCharFormat(format);

    opensource_.SetSel(-1, -1);
    opensource_.ReplaceSel(using_os);

    return TRUE;
}

const wchar_t* const using_os = 
_T("OpenCV\n")
_T("Home Page : http://www.opencv.org\n")
_T("Required for : filter\n")
_T("Version : 2.4.4\n")
_T("\n")
_T("Tinyxml\n")
_T("Home Page : http://sourceforge.net/projects/tinyxml/\n")
_T("Required for : dips\n")
_T("Version : 2.6.2         \n")
_T("\n")
_T("cximage\n")
_T("Home Page : http://www.codeproject.com/Articles/1300/CxImage\n")
_T("Version : 6.00\n")
_T("Required for : dips\n")
_T("\n")
_T("ogdf\n")
_T("Home Page : http://www.ogdf.net\n")
_T("Version :  v.2012.07\n")
_T("Required for : dips\n")
_T("\n")
_T("zLib\n")
_T("Home page : http://www.zlib.net/\n") 
_T("Version : 1.2.3\n")
_T("Required for : PNG, TIFF, JBIG.\n")
_T("Patches : none.\n")
_T("\n")
_T("LibTIFF\n")
_T("Home page : http://www.libtiff.org/\n") 
_T("Version : 3.5.7 \n")
_T("Required for : TIFF.\n")
_T("Patches :\n")
_T("\n")
_T("added OJPEG format\n")
_T("LZW compression kit\n")
_T("minor configuration editing\n")
_T("see detatils in libtiff_patches.zip\n")
_T("\n")
_T("LibPNG\n")
_T("Home page : http://www.libpng.org/pub/png/\n") 
_T("Version : 1.2.24\n")
_T("Required for : PNG.\n")
_T("Patches : none.\n")
_T("\n")
_T("LibMNG\n")
_T("Home page : http://www.libpng.org/pub/mng/\n") 
_T("Version : 1.0.10\n")
_T("Required for : MNG, JNG.\n")
_T("Patches : none.\n")
_T("\n")
_T("LibJPEG\n")
_T("Home page : http://www.ijg.org\n") 
_T("Version : 6b \n")
_T("Required for : JPG, TIFF, MNG.\n")
_T("Patches :\n")
_T("\n")
_T("OJPEG support\n")
_T("minor configuration editing\n")
_T("see details in libjpeg_patches.zip\n")
_T("\n")
_T("JBIG-Kit\n")
_T("Home page : http://www.cl.cam.ac.uk/~mgk25/jbigkit/\n") 
_T("Version : 1.6\n")
_T("Required for : JBIG.\n")
_T("Patches : only some missing cast.\n")
_T("\n")
_T("JasPer\n")
_T("Home page : http://www.ece.uvic.ca/~mdadams/jasper/\n") 
_T("Version : 1.900.1\n")
_T("Required for : JP2, JPC, PNM, RAS, PGX.\n")
_T("Patches : only some missing cast and minor configuration editing.\n")
_T("\n")
_T("LibDCR\n")
_T("Home page : http://www.xdp.it/libdcr/\n") 
_T("Version : 0.1.8.81\n")
_T("Required for : RAW, CRW, NEF, CR2, DNG, ORF, ARW, ERF, 3FR, DCR, X3F, MEF, RAF, MRW, PEF, SR2.\n");