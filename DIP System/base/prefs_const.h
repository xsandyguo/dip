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

#ifndef DIP_FLOWEDITOR_CONSTANTS_H_
#define DIP_FLOWEDITOR_CONSTANTS_H_

#include <tchar.h>
#include <GdiPlus.h>
using Gdiplus::Color;
using Gdiplus::Size;

extern const Color& kPortOutlinColor;
extern const Color& kPortOutlineHintColor;
extern const Color& kPortFillColor;

extern const Color& kLineSelectionColor;
extern const Color& kLineColor;

extern const Color& kNodeFillColor;
extern const Color& kNodeSelectedFillColor;
extern const Color& kNodeSelectedBorderColor;
extern const Color& kNodeBorderColor;

extern const Size&  kConnectorSize;
extern const Size&  kHandleSize;

extern const Color& kHandleFillColor;
extern const Color& kHandleOutlineColor;

extern const Size&  kNodeFigureSize;

extern const Size&  kGridSize;
extern const Color& kGridColor;

extern const Color& kDragShadowColor;

extern const Color& kSelectionAreaFillColor;
extern const Color& kSelectionAreaBorderColor;
extern const Color& kLabelColor;

extern const TCHAR* const kLabelFontName;
extern const unsigned int kLabelFontSize;

extern const Size& kFigureMargin;

#endif