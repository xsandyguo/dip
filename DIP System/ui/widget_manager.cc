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

#include "ui/widget_manager.h"

CCommandWindow   * WidgetManager::command_win   = NULL;
CFileView        * WidgetManager::file_win      = NULL;
CThumbnailView   * WidgetManager::thumbnail_win = NULL;
CToolBoxWindow   * WidgetManager::toolbox_win   = NULL;
COutputWindow    * WidgetManager::output_win    = NULL;
CPropertiesWindow* WidgetManager::property_win  = NULL;


void WidgetManager::Register(CFlowDocument* doc){
    command_win->AddObserver(doc);
}

void WidgetManager::Register(CFlowView* view){
    toolbox_win->AddObserver(view);
}

void WidgetManager::Unregister(CFlowDocument* doc){
    command_win->RemoveObserver(doc);
}

void WidgetManager::Unregister(CFlowView* view){
    toolbox_win->RemoveObserver(view);
}

void WidgetManager::Register(CCommandWindow* cmdwin){
    command_win = cmdwin;
}

void WidgetManager::Unresister(CCommandWindow* cmdwin){
    
}

void WidgetManager::Register(CFileView* file){
    file_win = file_win;
}

void WidgetManager::Unregister(CFileView* file){

}

void WidgetManager::Register(CThumbnailView* thumbnail){
    thumbnail_win = thumbnail_win;
}

void WidgetManager::Unregister(CThumbnailView* thumbnail_win){

}

void WidgetManager::Register(CToolBoxWindow* toolbox){
    toolbox_win = toolbox;
}

void WidgetManager::Unregister(CToolBoxWindow* toolbox_win){

}

void WidgetManager::Register(COutputWindow* output){
    output_win = output;
}

void WidgetManager::Unregister(COutputWindow* output_win){

}

void WidgetManager::Register(CPropertiesWindow* property){
    property_win = property;
}

void WidgetManager::Unregister(CPropertiesWindow* property_win){

}