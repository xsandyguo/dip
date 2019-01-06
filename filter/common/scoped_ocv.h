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

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <base/debug.h>

#include "common/common.h"

template<typename HANDLE_TYPE, 
    void (*DisposeFun)(HANDLE_TYPE**) = NULL >
class scoped_ocv{
public:
    scoped_ocv():handle_(NULL){}

    scoped_ocv(HANDLE_TYPE* handle):handle_(handle){
    }

    virtual ~scoped_ocv(){        
        reset();
    }

    HANDLE_TYPE* operator ->(){
        return handle_;
    }

    HANDLE_TYPE* get(){
        return handle_;
    }
    
    bool operator !(){
        return !handle_;
    }

    void release(){
        handle_ = NULL;
    }
    
    void reset(HANDLE_TYPE* handle = NULL){
        if(handle_){
            ASSERT1(DisposeFun);
            DisposeFun(&handle_);
        }
        
        handle_ = handle;
    }

    HANDLE_TYPE** address(){
        return &handle_;
    }

private:
    HANDLE_TYPE* handle_;
    DISALLOW_EVIL_CONSTRUCTORS(scoped_ocv);
};

#define DEFINE_SCOPED_OCV_OBJECT(NAME, TYPE, RELEASE)\
class scoped_##NAME : public scoped_ocv<TYPE, RELEASE>{\
public:    \
    scoped_##NAME(){}\
    scoped_##NAME(TYPE* handle):scoped_ocv(handle){}\
private:\
    DISALLOW_EVIL_CONSTRUCTORS(scoped_##NAME);\
};

DEFINE_SCOPED_OCV_OBJECT(img    , IplImage, cvReleaseImage)
DEFINE_SCOPED_OCV_OBJECT(capture, CvCapture, cvReleaseCapture)
DEFINE_SCOPED_OCV_OBJECT(storage, CvMemStorage, cvReleaseMemStorage)
DEFINE_SCOPED_OCV_OBJECT(haar   , CvHaarClassifierCascade, cvReleaseHaarClassifierCascade)
