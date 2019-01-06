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

#include "filter/edge_detect.h"

#include "common/scoped_ocv.h"

static wstring OperatorTypeString[] = {
    _T("Sobel"),
    _T("Laplace")
};

BEGIN_REGISTER_CLASSINFO(EdgeDetect,  OcvFilter, \
    "{d7165518-b503-4202-b17f-cb45421ef8f4}")

    LABELED_ENUM_PROPERTY(OperatorType, "Operator", get_operator_type, 
                          set_operator_type, OT_Sobel, OT_Lapalase, OperatorTypeString)

    DESCRIPT("Author : CUST")
    DESCRIPT("Description : Sobel¡¢Laplace ±ßÔµ¼ì²â")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO(EdgeDetect)

EdgeDetect::EdgeDetect():operator_type_(OT_Sobel){}

EdgeDetect::~EdgeDetect(){}

bool EdgeDetect::Initialize(){
    const IplImage* input = GetInputImage();
    ImageData* data = GetOutDataDirect(0);
    data->set_tagged_data(cvCreateImage(cvGetSize(input), 
        input->depth, 
        input->nChannels));    

    return true;
}

void EdgeDetect::ApplyOperator(IplImage* src, IplImage* store){
    if(operator_type_ == OT_Sobel){
        cvSobel(src, store, 1, 1, 3);
    }else{
        ASSERT1(operator_type_ == OT_Lapalase);

        cvLaplace(src, store, 3);
    }
}

bool EdgeDetect::Execute() {
    scoped_img planes[3];
    scoped_img work_image(cvCloneImage(GetInputImage(0)));
    scoped_img out(cvCreateImage(cvGetSize(work_image.get()), IPL_DEPTH_16S, 1));

    if (work_image->nChannels == 1) {
        ApplyOperator(work_image.get(), out.get());
        cvConvertScaleAbs(out.get(), GetOutDataDirect(0), 1, 0 );
    }else {
        for (int i = 0; i < 3; i++) {
            planes[i].reset(cvCreateImage(cvGetSize(work_image.get()),
                            IPL_DEPTH_8U, 1));
        }

        cvCvtPixToPlane(work_image.get(), planes[0].get(), 
                        planes[1].get(), planes[2].get(), 0);
        for (int i = 0; i < 3; i++) {
            ApplyOperator(planes[i].get(), out.get());
            cvConvertScaleAbs(out.get(), planes[i].get(), 1, 0 );
        }
        cvCvtPlaneToPix(planes[0].get(), planes[1].get(),
                        planes[2].get(), 0, GetOutDataDirect(0));
    }

    return true;
}
