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

#include "runtime/capture_source.h"

namespace dip{
BEGIN_REGISTER_ABSTRACT_CLASSINFO( CaptureSource, Source, \
    "{0AEC2CD4-3725-4188-A6BE-AF6E43F925CE}")
END_REGISTER_CLASSINFO( CaptureSource)

CaptureSource::CaptureSource(){

}

CaptureSource::~CaptureSource(){

}

void CaptureSource::SetImageDataType(ImageDataType type){
    this->format_.set_data_type(type);
}

void CaptureSource::SetImageWidth(int width){
    this->format_.set_width(width);
}

void CaptureSource::SetImageHeight(int height){
    this->format_.set_height(height);
}

void CaptureSource::SetImageFormat(int w, int h, int type){
    format_.set_width(w);
    format_.set_height(h);
    format_.set_data_type((ImageDataType)type);
}

ImageDataType CaptureSource::GetImageDataType(){
    return this->format_.get_data_type();
}

int CaptureSource::GetImageWidth(){
    return this->format_.get_width();
}

int CaptureSource::GetImageHeight(){
    return this->format_.get_height();
}

void CaptureSource::ClearDumpFile(){

}

bool CaptureSource::ExecuteTask(const wchar_t *taskName, const vector<int> &vals){
    return true;
}

} //nmaespace dip