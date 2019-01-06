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

#include "filter/ocvfilter.h"

BEGIN_REGISTER_ABSTRACT_CLASSINFO(OcvFilter,  Filter, \
    "{2329b6ff-129d-4846-bbb0-50cda6e4a3f7}")

END_REGISTER_CLASSINFO(OcvFilter)


OcvFilter::OcvFilter(int in_num, int out_num)
    : Filter(in_num, out_num){

}

OcvFilter::~OcvFilter(){
    IplImage* image = NULL;

    for(int i = 0 ; i < GetNumOutPins(); ++i){
        ImageData* data = GetOutDataDirect(i);
        ASSERT1(data);
        
        void* tag = data->get_tagged_data();
        if(tag){
            delete tag;
            data->set_tagged_data(NULL);
        }
    }
}

bool OcvFilter::Terminate(){
    bool ret = Filter::Terminate();

    for(int i = 0; i < GetNumOutPins(); ++i){
        ImageData* data = GetOutDataDirect(i);
        IplImage* img = NULL;
        if(data && (img = reinterpret_cast<IplImage*>(
                data->get_tagged_data()))){
            cvReleaseImage(&img);
            data->set_tagged_data(NULL);
        }
    }

    return ret;
}

const IplImage* OcvFilter::GetInputImage(int index){
    const ImageData* data = GetInData(index);    

    if(data){
        return reinterpret_cast<const IplImage*>(data->get_tagged_data());
    }

    return NULL;
}

IplImage* OcvFilter::GetOutputImage(int index){
    ImageData* data = GetOutDataDirect(index);
    return (IplImage*)data->get_tagged_data();
}