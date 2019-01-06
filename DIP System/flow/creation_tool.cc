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

#include "flow/creation_tool.h"

#include <afxwin.h>

CreationTool::CreationTool(IFigureFactory* factory){
    ASSERT1(factory);
    factory_.reset(factory);
}

CreationTool::~CreationTool(){
}

void CreationTool::SetCreationClass(const ClassId& id){
    this->id_ = id;
}

void CreationTool::MouseUp(MouseEventArgs& evt){ 
    __super::MouseUp(evt);

    IFigure* figure = factory_->CreateFigure(id_);
    ASSERT1(figure);

    if(figure){
        Point p = Locate(evt.GetPoint());
        IDrawing* container = get_drawing();

        Rect rc = figure->GetBounds();
        rc.X = p.X; 
        rc.Y = p.Y; 
        figure->SetBounds(rc);
        container->AddFigure(figure); 

        get_view()->ClearSelection();
        //GetView()->AddToSelection(figure);
    }else{
        CString msg;
        msg.Format(_T("Failed to create instance of selected object id:%s"),
                          CString(id_.c_str()));
        AfxMessageBox(msg, MB_OK | MB_ICONERROR);
    }

    this->OnToolDone();
}

