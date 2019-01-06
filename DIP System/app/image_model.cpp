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

#include "app/image_model.h"

#include <stdlib.h>
#include <crtdbg.h>
#include <ximage.h>
#include <xfile.h>

#include "image/common/config_manager.h"
#include "image/image_document.h"
#include "image/image_view.h"
#include "image/ui/DlgOptions.h"
#include "ui/child_frame.h"

#include "resource.h"

ImageAppModel g_image_model;

const DocType kDocTypes[CMAX_IMAGE_FORMATS] =
{
	{ -1, TRUE, TRUE, _T("Supported files"), _T("*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;*.ppm;*.ras;*.mng;*.jng;*.ska;*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") },
#if CXIMAGE_SUPPORT_BMP
	{ CXIMAGE_FORMAT_BMP, TRUE, TRUE, _T("BMP files"), _T("*.bmp") },
#endif
#if CXIMAGE_SUPPORT_GIF
	{ CXIMAGE_FORMAT_GIF, TRUE, TRUE, _T("GIF files"), _T("*.gif") },
#endif
#if CXIMAGE_SUPPORT_JPG
	{ CXIMAGE_FORMAT_JPG, TRUE, TRUE, _T("JPG files"), _T("*.jpg;*.jpeg") },
#endif
#if CXIMAGE_SUPPORT_PNG
	{ CXIMAGE_FORMAT_PNG, TRUE, TRUE, _T("PNG files"), _T("*.png") },
#endif
#if CXIMAGE_SUPPORT_MNG
	{ CXIMAGE_FORMAT_MNG, TRUE, TRUE, _T("MNG files"), _T("*.mng;*.jng;*.png") },
#endif
#if CXIMAGE_SUPPORT_ICO
	{ CXIMAGE_FORMAT_ICO, TRUE, TRUE, _T("ICO CUR files"), _T("*.ico;*.cur") },
#endif
#if CXIMAGE_SUPPORT_TIF
	{ CXIMAGE_FORMAT_TIF, TRUE, TRUE, _T("TIF files"), _T("*.tif;*.tiff") },
#endif
#if CXIMAGE_SUPPORT_TGA
	{ CXIMAGE_FORMAT_TGA, TRUE, TRUE, _T("TGA files"), _T("*.tga") },
#endif
#if CXIMAGE_SUPPORT_PCX
	{ CXIMAGE_FORMAT_PCX, TRUE, TRUE, _T("PCX files"), _T("*.pcx") },
#endif
#if CXIMAGE_SUPPORT_WBMP
	{ CXIMAGE_FORMAT_WBMP, TRUE, TRUE, _T("WBMP files"), _T("*.wbmp") },
#endif
#if CXIMAGE_SUPPORT_WMF
	{ CXIMAGE_FORMAT_WMF, TRUE, FALSE, _T("WMF EMF files"), _T("*.wmf;*.emf") },
#endif
#if CXIMAGE_SUPPORT_JBG
	{ CXIMAGE_FORMAT_JBG, TRUE, TRUE, _T("JBG files"), _T("*.jbg") },
#endif
#if CXIMAGE_SUPPORT_JP2
	{ CXIMAGE_FORMAT_JP2, TRUE, TRUE, _T("JP2 files"), _T("*.j2k;*.jp2") },
#endif
#if CXIMAGE_SUPPORT_JPC
	{ CXIMAGE_FORMAT_JPC, TRUE, TRUE, _T("JPC files"), _T("*.j2c;*.jpc") },
#endif
#if CXIMAGE_SUPPORT_PGX
	{ CXIMAGE_FORMAT_PGX, TRUE, TRUE, _T("PGX files"), _T("*.pgx") },
#endif
#if CXIMAGE_SUPPORT_RAS
	{ CXIMAGE_FORMAT_RAS, TRUE, TRUE, _T("RAS files"), _T("*.ras") },
#endif
#if CXIMAGE_SUPPORT_PNM
	{ CXIMAGE_FORMAT_PNM, TRUE, TRUE, _T("PNM files"), _T("*.pnm;*.pgm;*.ppm") },
#endif
#if CXIMAGE_SUPPORT_SKA
	{ CXIMAGE_FORMAT_SKA, TRUE, TRUE, _T("SKA files"), _T("*.ska") },
#endif
#if CXIMAGE_SUPPORT_RAW
	{ CXIMAGE_FORMAT_RAW, TRUE, FALSE, _T("RAW files"), _T("*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") }
#endif
};

DlgDataExtInfo dlgDataExtInfos = {0.0f,100.0f,0.0f,100.0f,200,0,true,true,true,false,false};

// CImageApp construction
ImageAppModel::ImageAppModel()
{
    nDocType = 0;
    g_app_models.push_back(this);
}

bool ImageAppModel::Initialize()
{
	doc_template_ = (new CMultiDocTemplate(
		IDR_DEMOTYPE,
		RUNTIME_CLASS(CImageDocument),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CImageView)));

    GetImageConfig().Initialize();

	// Register our Clipboard format name
	m_CF = ::RegisterClipboardFormat(_T("CF_CXIMAGE"));

	return true;;
}

void ImageAppModel::OnFileOpen() 
{
	// prompt the user (with all document templates)
	CString newName;
	int nDocType = -1;
	if (!PromptForFileName(newName, AFX_IDS_OPENFILE,
	  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, &nDocType))
		return; // open cancelled
	
	//OpenDocumentFile(newName);

}

void ImageAppModel::OpenFile(const wchar_t* file_name){
    if(!file_name || !*file_name){
        return;
    }

    CDocument* doc(CreateDocument());
    if(!doc){
        return;
    }

    doc->OnOpenDocument(file_name);
    doc->SetTitle(::PathFindFileName(file_name));    
    doc->UpdateAllViews(NULL);      
}

bool ImageAppModel::IsSupport(const wchar_t* file_ext){
    CString ext_name(file_ext);
    ext_name.MakeLower();

    for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
        if (kDocTypes[i].bRead){
            CString ext(kDocTypes[i].ext);
            if(ext.Find(ext_name) != -1)
                return true;
        }
    }
    return false;
}

CString ImageAppModel::GetFilterStr(){
    return GetFileTypes(TRUE);
}

// prompt for file name - used for open and save as
// static function called from app
BOOL ImageAppModel::PromptForFileName(CString& fileName, UINT /*nIDSTitle*/, 
	DWORD dwFlags, BOOL open, int* pType)
{
	CFileDialog dialog(open);
	CString title;
	if (open) title=_T("Open image file"; else title="Save image file");

	dialog.m_ofn.Flags |= dwFlags;

	int doc_type = (pType != NULL) ? *pType : CXIMAGE_FORMAT_BMP;
	if (doc_type==0) doc_type=1;

	int nIndex = GetIndexFromType(doc_type, open);
	if (nIndex == -1) nIndex = 0;

	dialog.m_ofn.nFilterIndex = nIndex +1;
	// strDefExt is necessary to hold onto the memory from GetExtFromType
	CString strDefExt = GetExtFromType(doc_type).Mid(2,3);
	dialog.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter = GetFileTypes(open);
    strFilter.Replace(L'|', NULL);

    dialog.m_ofn.lpstrFilter = strFilter;
    dialog.m_ofn.lpstrTitle  = title;
    dialog.m_ofn.lpstrFile   = fileName.GetBuffer(_MAX_PATH);

	BOOL bRet = (dialog.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	if (bRet){
		if (pType != NULL){
			int nIndex = (int)dialog.m_ofn.nFilterIndex - 1;
			ASSERT(nIndex >= 0);
			*pType = GetTypeFromIndex(nIndex, open);
		}
	}
	return bRet;
}

int ImageAppModel::GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog ? kDocTypes[i].bRead : kDocTypes[i].bWrite){
			if (kDocTypes[i].nID == nDocType) return nCnt;
			nCnt++;
		}
	}
	return -1;
}

int ImageAppModel::GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog ? kDocTypes[i].bRead : kDocTypes[i].bWrite){
			if (nCnt == nIndex)
//              return i; // PJO - Buglet ?
                return kDocTypes[i].nID;
			nCnt++;
		}
	}
	ASSERT(FALSE);
	return -1;
}

CString ImageAppModel::GetExtFromType(int nDocType)
{
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (kDocTypes[i].nID == nDocType)
			return kDocTypes[i].ext;
	}
	return CString(_T(""));
}

CString ImageAppModel::GetDescFromType(int nDocType)
{
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (kDocTypes[i].nID == nDocType)
			return kDocTypes[i].description;
	}
	return CString(_T(""));
}

BOOL ImageAppModel::GetWritableType(int nDocType)
{
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (kDocTypes[i].nID == nDocType)
			return kDocTypes[i].bWrite;
	}
	return FALSE;
}

CString ImageAppModel::GetFileTypes(BOOL bOpenFileDialog)
{
	CString str;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog && kDocTypes[i].bRead){
			str.AppendFormat(_T("%s (%s)|%s|"), kDocTypes[i].description, 
                kDocTypes[i].ext, kDocTypes[i].ext);
        } else if (!bOpenFileDialog && kDocTypes[i].bWrite) {
            str.AppendFormat(_T("%s (%s)|%s|"), kDocTypes[i].description, 
                kDocTypes[i].ext, kDocTypes[i].ext);
		}
	}
	return str;
}

void ImageAppModel::OnWindowCloseall() 
{
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition(); 
    while(pos) { 
        CDocTemplate* pTemplate = AfxGetApp()->GetNextDocTemplate(pos); 
        POSITION pos2 = pTemplate->GetFirstDocPosition(); 
        while(pos2) { 
            CDocument* pDocument; 
			if ((pDocument=pTemplate->GetNextDoc(pos2)) != NULL) 
				pDocument->OnCloseDocument();  
		}
	}
}


void ImageAppModel::OnCximageOptions() 
{
	CxImage* image = new CxImage();

	if (image==NULL) return;

	DlgOptions dlg;
	dlg.m_jpeg_quality = GetImageConfig().OptJpegQuality;
	dlg.m_xres = image->GetXDPI();
	dlg.m_yres = image->GetYDPI();

#if CXIMAGE_SUPPORT_TIF
	dlg.m_Opt_tif = image->GetCodecOption(CXIMAGE_FORMAT_TIF);
#endif
#if CXIMAGE_SUPPORT_GIF
	dlg.m_Opt_gif = image->GetCodecOption(CXIMAGE_FORMAT_GIF);
#endif
#if CXIMAGE_SUPPORT_PNG
	dlg.m_Opt_png = image->GetCodecOption(CXIMAGE_FORMAT_PNG);
#endif

	dlg.m_Opt_jpg = GetImageConfig().OptJpegOptions;
	dlg.m_Opt_raw = GetImageConfig().OptRawOptions;
	dlg.m_exif = 0;
	if (dlg.DoModal()==IDOK){
#ifdef VATI_EXTENSIONS
		GetImageConfig().OptJpegQuality = dlg.m_jpeg_quality;
		GetImageConfig().OptJpegOptions = dlg.m_Opt_jpg;
		GetImageConfig().OptRawOptions  = dlg.m_Opt_raw;
#endif
	}

	delete image;
}

CDocument* ImageAppModel::CreateDocument(){
    return doc_template_->OpenDocumentFile(NULL); 
}