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

#include "filter/hsv_space.h"

BEGIN_REGISTER_CLASSINFO(HsvSpace,  OcvFilter, \
    "{dd3f1aab-472f-4d7e-9227-cc50c665a671}")
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : HSV色彩空间")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO(HsvSpace)

HsvSpace::HsvSpace():OcvFilter(1, 3){
}

HsvSpace::~HsvSpace(){
}

bool HsvSpace::Initialize(){
    const IplImage* input = GetInputImage();
    if(input->nChannels != 3){
        return false;
    }
    
    for(int i = 0; i < GetNumOutPins(); ++i){
        ImageData* data = GetOutDataDirect(i);
        data->set_tagged_data(cvCreateImage(cvGetSize(input), 
                            IPL_DEPTH_8U, 
                            1));
    }

    return true;
}


bool HsvSpace::Execute(){    
    IplImage* input  = cvCloneImage(GetInputImage());
    IplImage* hue = GetOutputImage(0); 
    IplImage* sat = GetOutputImage(1);
    IplImage* val = GetOutputImage(2);
        
    cvFlip(input);
    
    cvCvtColor( input, input ,CV_BGR2HSV );  
    cvCvtPixToPlane( input, hue, sat, val, 0 );  

    IplImage* mask = cvCreateImage(cvSize(256, 1), IPL_DEPTH_8U, 3);
    BYTE *mask_buffer = (BYTE*) mask->imageData;
    memset(mask_buffer, 232, mask->imageSize);    //  设置基色值
    for (int i = 0; i <= 180; i++) {                  //  181 种色调模式
        mask_buffer[3 * i]=(BYTE) i;                  //  hue
        //		buf[3*i+1]=255;                     //  Saturation
        //		buf[3*i+2]=255;                     //  Value
    }
    cvCvtColor( mask, mask ,CV_HSV2BGR );

    BYTE* hue_buffer   = (BYTE*) hue->imageData;
    BYTE* color_buffer = (BYTE*) input->imageData;

    for (int i = 0; i < hue->height; i++) {
        for (int j = 0, k = 0; j < hue->width; j++, k += 3) {
            memcpy(&color_buffer[k], &mask_buffer[3 * hue_buffer[j]], 3);  //  逐点查表转换
        }
        color_buffer += input->widthStep; 
        hue_buffer   += hue->widthStep;
    }
        
    return true;
}