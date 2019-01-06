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

#include "ui/command_window.h"
#include "ui/file_view.h"
#include "ui/thumbnail_view.h"
#include "ui/toolbox_window.h"
#include "ui/output_window.h"
#include "ui/properties_window.h"

#include "flow/ui/flow_document.h"
#include "flow/ui/flow_view.h"

// Mediator pattern
class WidgetManager{
public:
    static void Register(CFlowDocument* doc);
    static void Unregister(CFlowDocument* doc);

    static void Register(CFlowView* view);
    static void Unregister(CFlowView* view);
    
    static void Register(CCommandWindow* cmdwin);
    static void Unresister(CCommandWindow* cmdwin);

    static void Register(CFileView* file_win);
    static void Unregister(CFileView* file_win);

    static void Register(CThumbnailView* thumbnail_win);
    static void Unregister(CThumbnailView* thumbnail_win);

    static void Register(CToolBoxWindow* toolbox_win);
    static void Unregister(CToolBoxWindow* toolbox_win);

    static void Register(COutputWindow* output_win);
    static void Unregister(COutputWindow* output_win);

    static void Register(CPropertiesWindow* property_win);
    static void Unregister(CPropertiesWindow* property_win);

private:
    static CCommandWindow* command_win;
    static CFileView* file_win;
    static CThumbnailView* thumbnail_win;
    static CToolBoxWindow* toolbox_win;
    static COutputWindow* output_win;
    static CPropertiesWindow* property_win;

    DISALLOW_EVIL_CONSTRUCTORS(WidgetManager);
};