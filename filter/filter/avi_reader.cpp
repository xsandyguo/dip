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

#include "filter/avi_reader.h"

#include <atlstr.h>
#include <opencv/cv.h>

namespace dip{

BEGIN_REGISTER_CLASSINFO( AVIReader, VideoCapture, \
    "{47BADB9C-50BF-466E-B0B4-FE4E4ACCC3F6}")
    PATH_PROPERTY(wstring, "file path", get_file_path, set_file_path,\
    "Mpeg File(*.mpeg)|*.mpeg;*.mpg|AVI File (*.avi)|*.avi||")
    
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : AVIReader")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO( AVIReader)

AVIReader::AVIReader(){
}

AVIReader::~AVIReader(){
}


bool AVIReader::Initialize(){
    CStringA file(file_path_.c_str());
    capture_.reset(cvCaptureFromAVI(file));
    if(!capture_){
        LOG_ERROR(_T("[cvCaptureFromAVI(%s) failed.]"), file_path_.c_str());
        return false;
    }

    return true;
}

bool AVIReader::Execute(){
    ASSERT1(capture_.get());

    IplImage* frame = cvQueryFrame(capture_.get());
    if(!frame){
        LOG_ERROR(_T("[Query frame from capture failed.]"));
        return false;
    }

    ImageData* data = GetOutDataDirect(0);
    ASSERT1(data);
    data->set_tagged_data(frame);

    return true;
}

bool AVIReader::Terminate(){
    capture_.reset();

    return true;
}

} // namespace dip