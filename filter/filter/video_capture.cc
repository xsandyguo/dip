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

#include "filter/video_capture.h"

BEGIN_REGISTER_ABSTRACT_CLASSINFO(VideoCapture, Source, \
    "{97618C26-E447-4753-B365-B4C5AF5C77A1}")

END_REGISTER_CLASSINFO(VideoCapture)

VideoCapture::VideoCapture(){
}

VideoCapture::~VideoCapture(){

}

const ImageData* VideoCapture::GetOutData(int idx){
    const ImageData* data = Source::GetOutData(idx);
    Modified();

    return data;
}