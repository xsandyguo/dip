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

#include "app/flow_model.h"

#include <runtime/reflector.h>

#include "base/constants.h"
#include "flow/ui/flow_document.h"
#include "flow/ui/flow_view.h"
#include "resource.h"
#include "ui/child_frame.h"

FlowAppModel g_flow_model;

FlowAppModel::FlowAppModel()
    :filter_(L"DIP System Files (*.dips)|*.dips"){
    g_app_models.push_back(this);
}

bool FlowAppModel::Initialize(){
    doc_template_ = new CMultiDocTemplate(IDR_DIPFlowTYPE,
                                              RUNTIME_CLASS(CFlowDocument),
                                              RUNTIME_CLASS(CChildFrame),
                                              RUNTIME_CLASS(CFlowView)); 

    dip::Reflector::Load((const wchar_t*)GetPluginPath());
    return true;
}

CString FlowAppModel::GetFilterStr(){
    return filter_;
}

bool FlowAppModel::IsSupport(const wchar_t* file_ext){
    return _tcsicmp(file_ext, _T("dips")) == 0;
}

CDocument* FlowAppModel::CreateDocument(){
    return doc_template_->OpenDocumentFile(NULL);
}

void FlowAppModel::OpenFile(const wchar_t* file_name){
    CMultiDocTemplate& doct = *doc_template_;
    POSITION pos = doct.GetFirstDocPosition();
    if(pos){
        CDocument* doc = doct.GetNextDoc(pos);
        if(AfxMessageBox(L"一个文档已经打开，继续需要关闭当前文档？",
            MB_OKCANCEL|MB_ICONQUESTION)
            == IDOK){
                doc->OnCloseDocument();
        }else{
            return;
        }
    }

    CDocument* doc(CreateDocument());
    if(!doc){
        return;
    }
    
    if(file_name && *file_name){
        doc->OnOpenDocument(file_name);
        doc->SetTitle(::PathFindFileName(file_name));
    }else{
        CString s;
        s.Format(_T("Untitled Flow"));
        doc->SetTitle(s);
    }
       
    doc->UpdateAllViews(NULL);      
}


const CString& FlowAppModel::GetPluginPath(){
    if(plugin_dir_.IsEmpty()){
        CPath path;
        GetModuleFileName(NULL, path.m_strPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
        path.m_strPath.ReleaseBuffer();
        path.RemoveFileSpec();
        path.Append(kPluginDirName);
        plugin_dir_ = path.m_strPath;
    }

    return plugin_dir_;
}

const CString& FlowAppModel::GetAcquisitionPath(){
    if(image_acquisition_dir_.IsEmpty()){
        CPath path;
        GetModuleFileName(NULL, path.m_strPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
        path.m_strPath.ReleaseBuffer();
        path.RemoveFileSpec();
        path.Append(kAcquisitionDirName);
        image_acquisition_dir_ = path.m_strPath;
    }

    return image_acquisition_dir_;
}