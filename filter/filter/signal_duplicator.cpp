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

#include "filter/signal_duplicator.h"

BEGIN_REGISTER_CLASSINFO(SignalDuplicator, OcvFilter, \
    "{63E945E7-ECF1-4629-8DA0-91A24F6AD572}")
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : ÐÅºÅ¸´ÖÆÆ÷")
    DESCRIPT("Version : 1.0.0")

END_REGISTER_CLASSINFO(SignalDuplicator)

SignalDuplicator::SignalDuplicator():OcvFilter(1, 4){
}

SignalDuplicator::~SignalDuplicator(){}

bool SignalDuplicator::Initialize(){   
    const IplImage* input = GetInputImage();
    for(int i = 0; i < GetNumOutPins(); ++i){
        ImageData* data = GetOutDataDirect(i);
        IplImage* img;
        data->set_tagged_data(img = cvCreateImage(cvGetSize(input), 
                                            input->depth, 
                                            input->nChannels));  
        cvZero(img);
    }

    return true;
}

bool SignalDuplicator::Execute(){
    const IplImage* input = GetInputImage();
    for(int i = 0; i < GetNumOutPins(); ++i){
        cvCopy(input, GetOutputImage(i));
    }

    return true;

}

