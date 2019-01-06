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
#include <windows.h>
#include <atlstr.h>
#include <dshow.h>
#include <qedit.h>
#include <string>

#include <opencv/cv.h>

#include "common/common.h"
#include "filter/video_capture.h"

class DxShowCapture : public VideoCapture {
public:
    DECLEAR_CLASSINFO(DxShowCapture, Source)

    void ShowDevices();
    bool Initialize();
    bool Terminate();
    bool Execute();

    SetMacro(device_name, wstring)
    GetMacro(device_name, wstring)
    SetMacro(show_setting, bool)
    GetMacro(show_setting, bool)

    void set_width(int width);
    int get_width();
    void set_height(int height);
    int get_height();

protected:
    DxShowCapture(void);
    ~DxShowCapture(void);

    HRESULT CreateCapture(IBaseFilter** filter);
    HRESULT OpenCamera();
    HRESULT CreateGraph();
    void GetCaptures(std::vector<CString>* list);
    void ShowSetting();
    HRESULT Take();

private:
    IplImage * frame_;
    int width_;
    int height_;
    long buffer_size_;

    bool show_setting_;

    CComPtr<IGraphBuilder> graph_;
    CComPtr<IBaseFilter> device_filter_;
    CComPtr<IMediaControl> media_control_;
    CComPtr<IBaseFilter> grabber_filter_;
    CComPtr<ISampleGrabber> sample_grabber_;
    CComPtr<IPin> grabber_input_;
    CComPtr<IPin> grabber_output_;
    CComPtr<IPin> camera_output_;
    CComPtr<IMediaEvent> media_event_;
    CComPtr<IBaseFilter> null_filter_;
    CComPtr<IPin> null_input_;

    std::wstring device_name_;
    DISALLOW_EVIL_CONSTRUCTORS(DxShowCapture);
};

