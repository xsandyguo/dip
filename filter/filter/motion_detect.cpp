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

#include "filter/motion_detect.h"

#include <windows.h>

BEGIN_REGISTER_CLASSINFO(MotionDetect,  OcvFilter, \
    "{3FBD18C1-D684-40CB-BDAF-E07DECD29101}")
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : ÔË¶¯¸ú×Ù")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO(MotionDetect)

const double kMotionDurationTime    = 0.5; 
const int kMaxContinusFrame         = 3;

MotionDetect::MotionDetect():OcvFilter(1, 2),
    max_contour_area_(1000),
    last_frame_index_(0){
}

MotionDetect::~MotionDetect(){
}

bool MotionDetect::Initialize(){
    storage_.reset(cvCreateMemStorage(0));     
    const IplImage* input = GetInputImage();

    CvSize dim = cvSize(input->width, input->height);
    frames_.reset(new IplImage* [kMaxContinusFrame * sizeof(IplImage*)]);
    ZeroMemory(frames_.get(), kMaxContinusFrame * sizeof(IplImage*));

    for(int i = 0; i < kMaxContinusFrame; i++) {
        frames_[i] = cvCreateImage(dim, IPL_DEPTH_8U, 1);
        cvZero( frames_[i] );
    }

    history_.reset(cvCreateImage(dim, IPL_DEPTH_32F, 1));
    cvZero(history_.get()); 

    for(int i = 0; i < GetNumOutPins(); ++i){
        ImageData* data = GetOutDataDirect(i);
        IplImage* img;
        data->set_tagged_data(img = cvCreateImage(cvGetSize(input), 
                                            input->depth, 
                                            i > 0 ? 1 : input->nChannels));  
        cvZero(img);
    }

    return true;
}

bool MotionDetect::Terminate(){
    bool ret = OcvFilter::Terminate();

    for(int i = 0; i < kMaxContinusFrame; i++) {
        cvReleaseImage(&frames_[i]);
    }

    storage_.reset();
    frames_.reset();
    history_.reset();

    return ret;
}

bool MotionDetect::Execute(){ 
    IplImage* out0 = NULL;
    cvCopy(GetInputImage(), out0 = GetOutputImage());    

    Detect(out0, GetOutputImage(1));

    return true;
}

void MotionDetect::Detect(IplImage* source, IplImage* dest) {
    double timestamp = clock() / 100.; 
    CvSize dim = cvSize(source->width, source->height);
    scoped_img pyr(cvCreateImage(cvSize((dim.width & -2) /2,
                                         (dim.height & -2) / 2), 
                                  IPL_DEPTH_8U,
                                  1));   

    cvCvtColor(source, frames_[last_frame_index_], CV_BGR2GRAY);

    int idx1, idx2;
    idx1 = last_frame_index_;
    // index of (last - (N-1))th frame
    idx2 = (last_frame_index_ + 1) % kMaxContinusFrame; 
    last_frame_index_ = idx2;

    IplImage* diff = frames_[idx2];
    cvAbsDiff(frames_[idx1], frames_[idx2], diff);

    cvThreshold(diff, diff, 30, 255, CV_THRESH_BINARY);
    cvUpdateMotionHistory(diff, history_.get(), timestamp, kMotionDurationTime);

    cvConvert(history_.get(), dest);
    cvSmooth(dest, dest, CV_MEDIAN, 3, 0, 0, 0);

    cvPyrDown(dest, pyr.get(), CV_GAUSSIAN_5x5);    
    cvDilate(pyr.get(), pyr.get(), 0, 1);  
    cvPyrUp(pyr.get(), dest, CV_GAUSSIAN_5x5);
   
    // Create dynamic structure and sequence.
    CvSeq *seq = cvCreateSeq(CV_SEQ_ELTYPE_POINT, 
                             sizeof(CvSeq),
                             sizeof(CvPoint),
                             storage_.get());
    cvFindContours(dest,
                   storage_.get(),
                   &seq,
                   sizeof(CvContour),
                   CV_RETR_TREE);

    // drawing contours
    for(; seq; seq = seq->h_next) {
        CvRect r = ((CvContour*)seq)->rect;
        if(r.height * r.width > max_contour_area_){
            cvRectangle(source, 
                        cvPoint(r.x,r.y),
                        cvPoint(r.x + r.width, r.y + r.height),
                        CV_RGB(255, 0, 0),
                        1,
                        CV_AA,
                        0);
        }
    }
}