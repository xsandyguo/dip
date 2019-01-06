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

#include "filter/ocv_window.h"

#include <algorithm>
#include <opencv/highgui.h>
#include <GdiPlus.h>

#include <runtime/process.h>

#include "common/resource.h"

using namespace Gdiplus;
using namespace dip;

static wstring DisplayModeString[] = {
    _T("original image"),
    _T("window fit image"),
    _T("image fit window")
};

BEGIN_REGISTER_CLASSINFO(OcvWindow, ImageWindow, \
    "{c69f2646-5bf6-4cb0-a382-450aed4ad346}")
    PROPERTY(unsigned int, "refresh rate", get_refresh_rate, set_refresh_reate)
    PROPERTY(unsigned int, "width", get_width, set_width)
    PROPERTY(unsigned int, "height", get_height, set_height)
    PROPERTY(bool, "stop_render_if_no_data", will_stop_render_if_no_data, set_stop_render_if_no_data)

    LABELED_ENUM_PROPERTY(DisplayMode, "display mode", get_display_mode, 
                          set_display_mode,
                          DM_ORIGINAL_IMAGE, DM_IMAGE_FIT_WINDOW, 
                          DisplayModeString) 

    DESCRIPT("Author : CUST")
    DESCRIPT("Description : OcvWindow")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO(OcvWindow)

static const int32 kDefaultWidth  = 300;
static const int32 kDefaultHeight = 400;

OcvWindow::WindowCollection OcvWindow::showing_windows_;
LLock OcvWindow::windows_lock_;

OcvWindow::OcvWindow(void)
    :ImageWindow(1),
    refresh_rate_(40),
    width_(kDefaultWidth),
    height_(kDefaultHeight),
    display_mode_(DM_IMAGE_FIT_WINDOW),
    stop_render_if_no_data_(true),
    is_timer_valid_(false){
}

OcvWindow::~OcvWindow(void) {
}

bool OcvWindow::Execute(){
    ShowWindow(SW_SHOW);
    return true;
}

bool OcvWindow::Dispose(){
    ASSERT1(hwnd_);

    ::PostMessage(hwnd_, WM_CLOSE, 0, 0);
    if(!valid(sync_close_event_)){
        reset(sync_close_event_, ::CreateEvent(NULL, false, false, NULL));
        ASSERT1(valid(sync_close_event_));
    }

    return ::WaitForSingleObject(get(sync_close_event_), 5000)
           == WAIT_OBJECT_0;
}

void OcvWindow::OnDispose(){
    ::SetEvent(get(sync_close_event_));

    __mutexScope(windows_lock_);
    WindowIter pos = std::find(showing_windows_.begin(), 
                               showing_windows_.end(), 
                               this);

    if(pos != showing_windows_.end()){
        showing_windows_.erase(pos);
    }
}

void OcvWindow::OnClose(){
    if(is_timer_valid_){
        ::KillTimer(hwnd_, IDT_UPDATE_INTERVAL);
        is_timer_valid_ = false;
    }

    if(!::DestroyWindow(hwnd_)){
        LOG_ERROR(_T("[OcvWindow::Dispose][DestroyWindow failed][%u]"), GetLastError());
    }
}

void OcvWindow::OnCreate(){
    UINT_PTR ret;
    if(!(ret = ::SetTimer(hwnd_, IDT_UPDATE_INTERVAL, refresh_rate_, NULL))){
        LOG_ERROR(_T("[SetTimer failed][%u]"), GetLastError());
    }else{
        is_timer_valid_ = true;
        __mutexScope(windows_lock_);
        showing_windows_.push_back(this);
    }
}

void OcvWindow::OnTimer(uint32 timer_id){
    if(timer_id != IDT_UPDATE_INTERVAL){
        return;
    }
            
    const ImageData* data = GetInData(0);  
    IplImage* img = data ? (IplImage*)data->get_tagged_data() : NULL;
    if(!img){
        LOG_WARNING(_T("[OcvWindow::OnTimer][No image data available!][%s]"),
                    GetObject().get_name().c_str());
        if(stop_render_if_no_data_){
            ::KillTimer(hwnd_, IDT_UPDATE_INTERVAL);
            is_timer_valid_ = false;
        }
        return;
    }

    image_.CopyOf(img);    

    RECT rc;
    ::GetClientRect(hwnd_, &rc);
    ::InvalidateRect(hwnd_, &rc, true);
}

LRESULT OcvWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        OnCreate();
        return 0;
    case WM_PAINT:
        OnPaint();
        return 0;
    case WM_CLOSE:
        OnClose();
        return 0;
    case WM_SIZE:
        OnSize();
        return 0;
    case WM_TIMER:
        OnTimer(wParam);
    case WM_DESTROY:
        OnDispose();
        return 0;
    }
    return DefWindowProc(hwnd_, uMsg, wParam, lParam);
}

bool OcvWindow::ShowWindow(int nCmdShow){
    ::ShowWindow(hwnd_, nCmdShow);
    return !!::UpdateWindow(hwnd_);
}

size_t OcvWindow::GetWindowIndex(){
    __mutexScope(windows_lock_);
    
    WindowIter pos = std::find(showing_windows_.begin(), 
                               showing_windows_.end(), 
                               this);
    return showing_windows_.end() - pos;
}

void OcvWindow::OnSize(){
    RECT rc;
    ::GetWindowRect(::GetDesktopWindow(), &rc);
    
    int width  = 0; 
    int height = 0; 

    if(image_.IsValid() && display_mode_ == DM_WINDOW_FIT_IMAGE){
        width = image_.Width();
        height = image_.Height();
    }else{
        width = width_;
        height = height_;
    }
    int x = (rc.right - rc.left) / 2 - (width * GetWindowIndex()) / 2;    
    int y = (rc.bottom - rc.top) / 2 - height / 2;

    SetWindowPos(hwnd_, HWND_TOPMOST, x, y, width, height, SWP_SHOWWINDOW); 
}

void OcvWindow::set_display_mode(DisplayMode mode){
    display_mode_ = mode;
    
    OnSize();
}

void OcvWindow::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(hwnd_, &ps);

    RECT rc;
    if(image_.IsValid()){
        rc.left   = 0;
        rc.top    = 0;

        if(display_mode_ == DM_ORIGINAL_IMAGE){
            rc.right  = image_.Width();
            rc.bottom = image_.Height();
        }else{
            RECT client;
            ::GetClientRect(hwnd_, &client);
            rc.right = client.right - client.left;
            rc.bottom = client.bottom - client.top;
        }

        image_.DrawToHDC(hdc, &rc);
        //OnSize();
    }else{
        TextOut(hdc, 0, 0, _T("No data available yet!"), 15); 
    }
    ::EndPaint(hwnd_, &ps);    
}

bool OcvWindow::Initialize(){
    return  CreateWin(_T("OcvWindow"), WS_POPUPWINDOW | WS_CAPTION);
}