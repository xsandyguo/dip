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

#include "app/app_model.h"

#include <afxstr.h>
#include <base/scoped_ptr.h>

class CFlowDocument;
class FlowAppModel : public AppModel{
public:
    FlowAppModel();   
    bool Initialize();
    CString GetFilterStr();
    CDocument* CreateDocument();
    bool IsSupport(const wchar_t* file_ext);
    const CString& GetAcquisitionPath();
    const CString& GetPluginPath();
    void OpenFile(const wchar_t* file_name);

private:
    CString image_acquisition_dir_;
    CString plugin_dir_;
    CString filter_;

    DISALLOW_COPY_AND_ASSIGN(FlowAppModel);
};

extern FlowAppModel g_flow_model;