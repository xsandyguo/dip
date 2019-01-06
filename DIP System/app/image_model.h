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

#include <atlstr.h>
#include <base/basictypes.h>
#include <base/scoped_ptr.h>

#include "app/app_model.h"

class ImageAppModel : public AppModel
{
public:
	ImageAppModel();
    CDocument* CreateDocument();
    virtual bool Initialize();
    CString GetFilterStr();
    void OpenFile(const wchar_t* file_name);
    bool IsSupport(const wchar_t* file_ext);

	BOOL PromptForFileName(CString& fileName, UINT nIDSTitle, 
        DWORD dwFlags, BOOL bOpenFileDialog, int* pType=NULL);
    BOOL GetWritableType(int nDocType);
	int GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	int GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
	CString GetExtFromType(int nDocType);
	CString GetDescFromType(int nDocType);
	CString GetFileTypes(BOOL bOpenFileDialog);
	inline UINT	  GetCF() { return m_CF; }
    
	afx_msg void OnFileOpen();
	afx_msg void OnImageFromBitmap();
	afx_msg void OnWindowCloseall();
	afx_msg void OnCximageOptions();
    
private:
    int nDocType;
    UINT	m_CF;

    DISALLOW_COPY_AND_ASSIGN(ImageAppModel);
};

struct DocType
{
public:
    int nID;
    BOOL bRead;
    BOOL bWrite;
    const TCHAR* description;
    const TCHAR* ext;
};

struct DlgDataExtInfo
{
    float	fYmin;
    float	fYmax;
    float	fXmin;
    float	fXmax;
    BYTE	nThresh;
    COLORREF cColor;
    bool	bMinmax;
    bool	bAvg;
    bool	bDetect;
    bool	bLogXaxis;
    bool	bLogYaxis;
};

extern ImageAppModel g_image_model;



