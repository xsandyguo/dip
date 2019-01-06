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

#include "filter/rgb2gray.h"

BEGIN_REGISTER_CLASSINFO(Rgb2Gray,  OcvFilter, \
    "{67558d21-fcff-45f1-a968-90d7dde456e8}")
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : ²ÊÉ«±ä»Ò½×Í¼")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO(Rgb2Gray)

Rgb2Gray::Rgb2Gray(){
}

Rgb2Gray::~Rgb2Gray(){
}

bool Rgb2Gray::Initialize(){    
    const IplImage* input = GetInputImage();
    if(input->nChannels != 3){
        return false;
    }
    
    ImageData* data = GetOutDataDirect(0);
    data->set_tagged_data(cvCreateImage(cvGetSize(input), 
                            input->depth, 
                            input->nChannels));    

    return false;
}

bool Rgb2Gray::Execute(){    
    cvCvtColor(GetInputImage(), GetOutputImage(), CV_BGR2GRAY);

    return true;
}