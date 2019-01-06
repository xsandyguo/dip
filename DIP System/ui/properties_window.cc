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

#include "ui/properties_window.h"
#include <afxstr.h>

#include "resource.h"
#include "base/debug.h"
#include "ui/main_frame.h"
#include "app/app.h"

CPropertiesWindow::CPropertiesWindow()
:binding_figure_(NULL){
}

CPropertiesWindow::~CPropertiesWindow(){
}

BEGIN_MESSAGE_MAP(CPropertiesWindow, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_EXPAND_ALL              , OnExpandAllProperties)
    ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL    , OnUpdateExpandAllProperties)
    ON_COMMAND(ID_SORTPROPERTIES          , OnSortProperties)
    ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

void CPropertiesWindow::AdjustLayout(){
    if (GetSafeHwnd() == NULL){
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = toolbar_.CalcFixedLayout(FALSE, TRUE).cy;

    toolbar_.SetWindowPos(NULL, 
        rectClient.left, 
        rectClient.top, 
        rectClient.Width(), 
        cyTlb,
        SWP_NOACTIVATE | SWP_NOZORDER);
    property_grid_.SetWindowPos(NULL,
        rectClient.left, 
        rectClient.top + cyTlb, 
        rectClient.Width(),
        rectClient.Height() -(cyTlb), 
        SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWindow::OnCreate(LPCREATESTRUCT lpCreateStruct){
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // 创建组合:
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER 
        | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    if (!property_grid_.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2)){
        TRACE0("未能创建属性网格\n");
        return -1;      // 未能创建
    }

    InitPropList();

    toolbar_.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
    toolbar_.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 已锁定*/);
    toolbar_.CleanUpLockedImages();
    toolbar_.LoadBitmap(IDB_PROPERTIES_HC, 0, 0, TRUE /* 锁定*/);

    toolbar_.SetPaneStyle(toolbar_.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    toolbar_.SetPaneStyle(toolbar_.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC
        | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    toolbar_.SetOwner(this);

    // 所有命令将通过此控件路由，而不是通过主框架路由:
    toolbar_.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();
    return 0;
}

void CPropertiesWindow::OnSize(UINT nType, int cx, int cy){
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CPropertiesWindow::OnExpandAllProperties(){
    property_grid_.ExpandAll();
}

void CPropertiesWindow::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */){
    int i = 2;
}

void CPropertiesWindow::OnSortProperties(){
    property_grid_.SetAlphabeticMode(!property_grid_.IsAlphabeticMode());
}

void CPropertiesWindow::OnUpdateSortProperties(CCmdUI* pCmdUI){
    pCmdUI->SetCheck(property_grid_.IsAlphabeticMode());
}

void CPropertiesWindow::InitPropList(){
    SetPropListFont();

    property_grid_.EnableHeaderCtrl(FALSE);
    property_grid_.EnableDescriptionArea();
    property_grid_.SetVSDotNetLook();
    property_grid_.MarkModifiedProperties();
}

void CPropertiesWindow::BindObjectProperties(){
    CreateProperty();
    property_grid_.Invalidate(TRUE);
}

void CPropertiesWindow::CreateProperty(){
    if(!binding_figure_){
        property_grid_.RemoveAll();
        return;
    }

    Object& object = binding_figure_->GetFilter().GetObject();
    Class* type = Reflector::GetClass(&object);

    vector<dip::Property*> properties;
    type->EnumProperties(properties);
   
    for(vector<dip::Property*>::iterator itor = properties.begin();
        itor != properties.end();
        ++itor){
        dip::Property* property = *itor;
        dip::VariantData* data = property->Get(&object);

        CMFCPropertyGridProperty* item = NULL;
        CString description(property->GetDescription().c_str());
        CString display_name(property->GetDisplayName().c_str());
        
        if(property->IsPath()){          
            ASSERT1(data->GetType() == DipString);
            
            CString file_name((const wchar_t*)(*data)); 
            CString postfix(property->GetPostfixes().c_str());

            if(!postfix.GetLength()){
                item = new CMFCPropertyGridFileProperty(display_name, 
                                                        _T(""), 
                                                        (DWORD_PTR)property,
                                                        description);
            }else{
                item = new CMFCPropertyGridFileProperty(display_name, 
                                                        true,
                                                        file_name,
                                                        NULL,
                                                        0, 
                                                        postfix, 
                                                        description,
                                                        (DWORD_PTR)property);
            }
        }else {
            _variant_t var;
            if(!ConvertTo(&var, *data)){
                LOG_ERROR(_T("[CPropertiesWindow::CreateProperty][ConvertTo to _variant_t failed]"));
                continue;
            }

            item = new CMFCPropertyGridProperty(display_name,
                                                var,
                                                description,
                                                (DWORD_PTR)property);
            if(data->GetType() == DipEnum){
                EnumBase* enum_value = *data;
                const vector<wstring>& names =  enum_value->GetEnumNames();

                 for (EnumBase::NameIter itor = names.begin(); 
                    itor != names.end();
                    ++itor) {
                    item->AddOption(CString((*itor).c_str()));
                }
                item->AllowEdit(FALSE);
            }
        }      

        ASSERT1(item);

#ifdef DEBUG
        if(data->GetType() != DipBool)
#endif
        {
            item->AllowEdit(!property->IsReadOnly());
        }

        property_grid_.AddProperty(item);
    }    
}

bool CPropertiesWindow::ConvertTo(_variant_t* var, const VariantData& src){   
    ASSERT1(var);

    switch(src.GetType()){
    case DipString:
        *var = _variant_t((wchar_t*)src);break;
    case DipInt:
        *var = _variant_t((int)src);break;
    case DipUInt:
        *var = _variant_t((unsigned int)src);break;
    case DipLong:
        *var = _variant_t((long)src);break;
    case DipULong:
        *var = _variant_t((unsigned long)src);break;
    case DipShort:
        *var = _variant_t((short)src);break;
    case DipUShort:
        *var = _variant_t((unsigned short)src);break;
    case DipChar:
        *var = _variant_t((char)src);break;
    case DipUChar:
        *var = _variant_t((unsigned char)src);break;
    case DipDouble:
        *var = _variant_t((double)src);break;
    case DipFloat:
        *var = _variant_t((float)src);break;
    case DipBool:
        *var = _variant_t((bool)src);break;
    case DipEnum:
        *var = _variant_t(((EnumBase&)src).ToString().c_str());break;
    default:
        ASSERT1(false);
        LOG_ERROR(_T("[CPropertiesWindow::ConvertTo][Not support property type %u]"),
            src.GetType());
        return false;
    }

    return true;
}

bool CPropertiesWindow::ConvertTo(VariantData* var, const _variant_t& src){
    ASSERT1(var);
    EnumBase* enumValue = NULL;
    switch(var->GetType()){
    case DipString:
        *var = (wchar_t*)((_bstr_t)src);break;
    case DipInt:
        *var = (int)src;break;
    case DipUInt:
        *var = (unsigned int)src;break;
    case DipLong:
        *var = (long)src;break;
    case DipULong:
        *var = (unsigned long)src;break;
    case DipShort:
        *var = (short)src;break;
    case DipUShort:
        *var = (unsigned short)src;break;
    case DipChar:
        *var = (char)src;break;
    case DipUChar:
        *var = (unsigned char)src;break;
    case DipDouble:
        *var = (double)src;break;
    case DipFloat:
        *var = (float)src;break;
    case DipBool:
        *var = (bool)src;break;
    case DipEnum:
        enumValue = *var;
        enumValue->SetFromString((wchar_t*)((_bstr_t)src));
        break;    
    default:
        ASSERT1(false);
        LOG_ERROR(_T("[CPropertiesWindow::ConvertTo][Not support property type %u]"),
            var->GetType());
        return false;
    }

    return true;
}


LRESULT CPropertiesWindow::OnPropertyChanged(WPARAM wParam, LPARAM lParam) {  
    CMFCPropertyGridProperty* grid = reinterpret_cast<CMFCPropertyGridProperty*>(lParam);
    ASSERT1(grid);

    Object* obj = &GetBindObject();
    ASSERT1(obj);

    Property* prop        = reinterpret_cast<Property*>(grid->GetData());
    CString property_name = grid->GetName();  
    _variant_t newVal     = grid->GetValue();   

    VariantData data(*prop->Get(obj));    
    VERIFY1(ConvertTo(&data, newVal));
    prop->Set(obj, &data);
    
    // See if set successfully.
    _variant_t var_t;
    ConvertTo(&var_t, data);
    grid->SetValue(var_t);

    return 0;
}

void CPropertiesWindow::OnSetFocus(CWnd* pOldWnd){
    CDockablePane::OnSetFocus(pOldWnd);
    property_grid_.SetFocus();
}

void CPropertiesWindow::OnSettingChange(UINT uFlags, LPCTSTR lpszSection){
    CDockablePane::OnSettingChange(uFlags, lpszSection);
    SetPropListFont();
}

void CPropertiesWindow::SetPropListFont(){
    ::DeleteObject(prop_list_.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info;
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;

    prop_list_.CreateFontIndirect(&lf);

    property_grid_.SetFont(&prop_list_);
}

void CPropertiesWindow::SelectionChanged(FigureSelectionEventArgs& evt){
    ProcessFigure* figure = NULL;
    std::vector<IFigure*>& figures = evt.GetNewSelection();
    if(figures.size()){      
        figure = dynamic_cast<ProcessFigure*>(figures.at(0));
    }
    if(figure != binding_figure_){
        binding_figure_ = figure;

        BindObjectProperties();
    }
}

void CPropertiesWindow::FigureRemoved(DrawingEventArgs& evt){
    ProcessFigure* process = dynamic_cast<ProcessFigure*>(evt.GetTargetFigure());
    if(process == binding_figure_){
        binding_figure_ = NULL;

        BindObjectProperties();
    }
}

void CPropertiesWindow::FigureAdded(DrawingEventArgs& evt){

}