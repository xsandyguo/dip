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

#include <vector>

#include <base/scoped_ptr.h>
#include <base/scoped_any.h>
#include <runtime/reflector.h>
#include <runtime/image_window.h>
#include <runtime/synchronized.h>

#include <opencv/cv.h>

#include "common/common.h"
#include "common/ocv_image.h"
#include "common/resource.h"

class BaseWindow {
public:
    BaseWindow() : hwnd_(NULL) { }
    static LRESULT CALLBACK WindowProc(HWND hwnd, 
                                        UINT uMsg, 
                                        WPARAM wParam, 
                                        LPARAM lParam) {
        BaseWindow *me = NULL;
        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            me = (BaseWindow*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)me);

            me->hwnd_ = hwnd;
        } else {
            me = (BaseWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (me){
            return me->HandleMessage(uMsg, wParam, lParam);
        }else{
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    bool CreateWin(PCWSTR lpWindowName,
                    DWORD dwStyle,
                    DWORD dwExStyle = 0,
                    int x           = CW_USEDEFAULT,
                    int y           = CW_USEDEFAULT,
                    int nWidth      = CW_USEDEFAULT,
                    int nHeight     = CW_USEDEFAULT,
                    HWND hWndParent = 0,
                    HMENU hMenu     = 0) {
        WNDCLASS wc = {0};

        wc.lpfnWndProc   = WindowProc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        RegisterClass(&wc);
        hwnd_ = CreateWindowEx(dwExStyle,
                                ClassName(), 
                                lpWindowName, 
                                dwStyle, 
                                x, y, 
                                nWidth, 
                                nHeight, 
                                hWndParent, 
                                hMenu, 
                                GetModuleHandle(NULL), 
                                this);

        return (hwnd_ ? true : false);
    }

    HWND Window() const { return hwnd_; }
protected:
    virtual PCWSTR  ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    HWND hwnd_;
};

enum DisplayMode{
    DM_ORIGINAL_IMAGE,
    DM_WINDOW_FIT_IMAGE,
    DM_IMAGE_FIT_WINDOW
};

class OcvWindow : public ImageWindow, public BaseWindow {
public:
    DECLEAR_CLASSINFO(OcvWindow, ImageWindow)

    virtual bool Initialize();
    virtual bool Execute();
    virtual bool Dispose();

protected:
    bool ShowWindow(int nCmdShow);
    PCWSTR  ClassName() const { return L"OcvWindow";}
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void OnPaint();
    void OnSize();
    void OnCreate();
    void OnTimer(uint32 timer_id);
    void OnClose();
    void OnDispose();

    unsigned int get_width(){ return width_;}
    void set_width(unsigned int width){ width_ = width; OnSize(); }
    unsigned int get_height(){return height_;}
    void set_height(unsigned int height){ height_ = height; OnSize();}

    unsigned int get_refresh_rate(){ return refresh_rate_; }
    void set_refresh_reate(unsigned int refresh_rate){ refresh_rate_ = refresh_rate;}

    DisplayMode get_display_mode(){return display_mode_;}
    void set_display_mode(DisplayMode mode);
    void set_stop_render_if_no_data(bool stop){ stop_render_if_no_data_ = stop; }
    bool will_stop_render_if_no_data(){return stop_render_if_no_data_; }

    size_t GetWindowIndex();
    OcvWindow(void);
    ~OcvWindow(void);

private:

    DisplayMode display_mode_;
    unsigned int refresh_rate_;
    unsigned int width_;
    unsigned int height_;
    bool stop_render_if_no_data_;
    bool is_timer_valid_;

    OcvImage image_;
    scoped_handle sync_close_event_;

    typedef std::vector<OcvWindow*> WindowCollection;
    typedef WindowCollection::iterator WindowIter;

    static LLock windows_lock_;
    static WindowCollection showing_windows_;

    DISALLOW_EVIL_CONSTRUCTORS(OcvWindow);

};

