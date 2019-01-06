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

#include "filter/camera_capture.h"

#include <base/debug.h>
#include <opencv/cv.h>

BEGIN_REGISTER_CLASSINFO(CameraCaputre, Source, \
    "{4661463D-82E4-4F20-98A6-AA268AD33DF4}")

    PROPERTY(int, "camera index", get_camera, set_camera)

    DESCRIPT("Author : CUST")
    DESCRIPT("Description : CameraCaputre")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO(CameraCaputre)

CameraCaputre::CameraCaputre(void) :
capture_(NULL),
camera_index_(0){
}

CameraCaputre::~CameraCaputre(void) {
}


int CameraCaputre::get_camera(){
    return camera_index_;
}

void CameraCaputre::set_camera(int index){
    if(camera_index_ == index){
        return;
    }

    if(index < 0){
        LOG_ERROR(_T("[Index can't be less than 0]"));
        return;
    }

    camera_index_ = index;

    Modified();
}

bool CameraCaputre::Initialize(){
    capture_.reset(cvCaptureFromCAM(camera_index_));
    if(!capture_){
        LOG_ERROR(_T("[cvCaptureFromCAM(%d) failed.]"), 
                    camera_index_);
        return false;
    }

    return true;
}

bool CameraCaputre::Execute(){
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

bool CameraCaputre::Terminate(){
    if(!Source::Terminate()){
        return false;
    }

    ASSERT1(capture_);    
    capture_.reset(NULL);
    return true;
}
