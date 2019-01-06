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

#include <base/scoped_ptr.h>
#include <runtime/class_info.h>

#include "common/common.h"
#include "common/scoped_ocv.h"
#include "filter/ocvfilter.h"

class MotionDetect : public OcvFilter{
public:
    DECLEAR_CLASSINFO(MotionDetect, OcvFilter)

    bool Execute();
    bool Initialize();
    bool Terminate();
protected:
    MotionDetect();
    ~MotionDetect();

private:
    void Detect(IplImage* source, IplImage* dest);
    scoped_storage storage_; 
    scoped_array<IplImage*> frames_;
    scoped_img history_;
    int max_contour_area_;
    int last_frame_index_;

    DISALLOW_COPY_AND_ASSIGN(MotionDetect);
};