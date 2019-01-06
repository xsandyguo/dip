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

#include "common/common.h"

#include <opencv/highgui.h>
#include "common/scoped_ocv.h"

#include "filter/video_capture.h"

class DIP_EXPORT CameraCaputre : public VideoCapture {
public:
    DECLEAR_CLASSINFO(CameraCaputre, VideoCapture)

    virtual bool Initialize();
    virtual bool Execute();
    virtual bool Terminate();
    int get_camera();
    void set_camera(int index);

protected:
    CameraCaputre(void);
    virtual ~CameraCaputre(void);
private:
    scoped_capture capture_;
    int camera_index_;

    DISALLOW_EVIL_CONSTRUCTORS(CameraCaputre);
};

