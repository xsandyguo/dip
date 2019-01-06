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

#include "filter/face_detect.h"

BEGIN_REGISTER_CLASSINFO(FaceDetect,  OcvFilter, \
    "{D95DB2D9-933A-4539-B006-4E94E85D71B9}")
    PATH_PROPERTY(wstring, "file path", get_file_path, set_file_path,\
    "Haarcascade File(*.xml)|*.xml||")
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : »À¡≥ÃΩ≤‚")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO(FaceDetect)

FaceDetect::FaceDetect(){
}

FaceDetect::~FaceDetect(){
}

const char* kCascadeName = "haarcascade_frontalface_alt2.xml"; 

HMODULE GetModuleHandleFromAddress(void* address) {
    MEMORY_BASIC_INFORMATION mbi = {0};
    DWORD result = ::VirtualQuery(address, &mbi, sizeof(mbi));
    ASSERT1(result == sizeof(mbi));
    return static_cast<HMODULE>(mbi.AllocationBase);
}

HMODULE GetCurrentModuleHandle() {
    return GetModuleHandleFromAddress(GetCurrentModuleHandle);
}

bool FaceDetect::Initialize(){
    char path_file[MAX_PATH];

    GetModuleFileNameA(NULL, path_file, sizeof(path_file));
    PathRemoveFileSpecA(path_file);
    PathAppendA(path_file, kCascadeName);

    cascade_.reset(reinterpret_cast<CvHaarClassifierCascade*>(
                   cvLoad(path_file, 0, 0, 0 ))); 
    
    if( !cascade_.get()){ 
        LOG_ERROR(_T("[Could not load classifier cascade]" )); 
        return false;
    } 
    storage_.reset(cvCreateMemStorage(0));     
    
    const IplImage* input = GetInputImage();
    ImageData* data = GetOutDataDirect(0);
    data->set_tagged_data(cvCreateImage(cvGetSize(input), 
                                        input->depth, 
                                        input->nChannels));   

    return true;
}

bool FaceDetect::Terminate(){
    bool ret = OcvFilter::Terminate();

    storage_.reset();
    cascade_.reset();

    return ret;
}

bool FaceDetect::Execute(){ 
    IplImage* out_img = NULL;
    cvCopy(GetInputImage(), out_img = GetOutputImage());

    Detect(out_img);

    return true;
}

void FaceDetect::Detect(IplImage* image ){ 
    const double scale = 1.2; 
    const CvScalar colors[] = { 
        {{0,0,255}},{{0,128,255}},{{0,255,255}},{{0,255,0}}, 
        {{255,128,0}},{{255,255,0}},{{255,0,0}},{{255,0,255}} 
    };

    //Image Preparation 
    // 
    scoped_img gray(cvCreateImage(cvSize(image->width, image->height),
                                         IPL_DEPTH_8U, 1)); 

    scoped_img small_img(cvCreateImage(cvSize(cvRound(image->width / scale),
                                        cvRound(image->height / scale)),
                                        8,
                                        1)); 

    cvCvtColor(image, gray.get(), CV_BGR2GRAY); 
    cvResize(gray.get(), small_img.get(), CV_INTER_LINEAR);
    cvEqualizeHist(small_img.get(), small_img.get());

    //Detect objects if any 
    // 
    cvClearMemStorage(storage_.get());  
    CvSeq* objects = cvHaarDetectObjects(small_img.get(), 
                                         cascade_.get(), 
                                         storage_.get(), 
                                         1.1, 
                                         2, 
                                         0/*CV_HAAR_DO_CANNY_PRUNING*/, 
                                         cvSize(30,30));

    //Loop through found objects and draw boxes around them 
    for(int i = 0; i < (objects? objects->total:0);++i) { 
        CvRect* r = (CvRect*)cvGetSeqElem(objects,i); 
        cvRectangle(image, 
                    cvPoint(r->x * scale, r->y * scale), 
                    cvPoint((r->x+r->width) * scale, (r->y+r->height)*scale),
                    colors[i % 8], 2); 
    } 
}