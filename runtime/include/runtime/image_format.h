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

#ifndef DIP_RT_IMAGE_FORMAT_H_
#define DIP_RT_IMAGE_FORMAT_H_

#include <iostream>
#include <string>

#include "runtime/common.h"

namespace dip{

enum ImageDataType {
    TYPE_RAW_BG8 = 0,
    TYPE_RAW_GR8,
    TYPE_YUV422,
    TYPE_JPEG,
    TYPE_RAW_BG16,
    TYPE_RAW_GR16,
    TYPE_RAW_SMIA10,
    TYPE_RGB24,
    TYPE_RGB32,
    TYPE_YUV420,
    TYPE_YUV444,
    TYPE_RGB565,
    TYPE_RGB656,
    TYPE_GREY,
    TYPE_UNKNOWN
};

static float kDataSizeTable[TYPE_UNKNOWN+1] = {
    1.0f, 
    1.0f, 
    2.0f, 
    1.0f, 
    2.0f, 
    2.0f, 
    1.25f,
    3.0f, 
    4.0f,
    2.0f, 
    3.0f, 
    3.0f, 
    3.0f, 
    1.0f, 
    3.0f  
};

static wstring kTypeStringTable[TYPE_UNKNOWN+1] = {
   _T("RAW_BG8"),
   _T("RAW_GR8"),
   _T("YUV422"),
   _T("JPEG"), 
   _T("RAW_BG16"), 
   _T("RAW_GR16"), 
   _T("RAW_SMIA10"),
   _T("RGB24"),
   _T("RGB32"),
   _T("YUV420"), 
   _T("YUV444"), 
   _T("RGB565"), 
   _T("RG656"),
   _T("GREY"),
   _T("UNKNOWN")
};

static wstring kTypeStringBrevTable[TYPE_UNKNOWN+1] = {
   _T("BG8"),
   _T("GR8"),
   _T(""),
   _T(""), 
   _T("BG16"), 
   _T("GR16"), 
   _T("SMIA10"),
   _T(""),
   _T(""),
   _T(""), 
   _T(""), 
   _T(""), 
   _T(""),
   _T(""),
   _T("UNKNOWN")
};

static inline wstring DataTypeToString(ImageDataType type){
    return kTypeStringTable[type];
}

static inline wstring DataTypeToBrevStr(ImageDataType type){
    return kTypeStringBrevTable[type];
}

static inline ImageDataType StringToDataType(wstring name){
    for(int i = 0; i <= TYPE_UNKNOWN; i++){
        if(kTypeStringTable[i] == name){
            return (ImageDataType) (i);
        }
    }

    return (ImageDataType) (TYPE_UNKNOWN);
}

class ImageFormat {
public:    
    void set_width(int w) { width_ = w; }
    int get_width() { return width_; }

    void set_height(int h) { height_ = h; }
    int get_height()  { return height_; }

    void SetFourChannel(bool flag) { four_channel_ = flag; }
    bool GetFourChannel() { return four_channel_; }

    void SetRawPrefer(bool flag) { this->raw_prefer_ = flag; }
    bool GetRawPrefer() { return this->raw_prefer_; }

    ImageDataType get_data_type() { return this->data_type_; }
    void set_data_type(ImageDataType type) { this->data_type_ = type; }

    int get_data_depth() { return this->data_depth_; }
    void set_data_depth(int d) {  this->data_depth_ = d; }

    int ComputeDataSize(){
        int w = width_ * kDataSizeTable[data_type_];
        w = ((w+3)/4) * 4;
        int sz = w * height_; 
        return sz % 512 ? (sz/512+1) * 512 : sz;
    }

    float bytes_per_pixel(){
        return kDataSizeTable[data_type_];
    }

    ImageFormat(int w=0, int h=0, ImageDataType t=TYPE_RAW_BG8){
        width_    = w;
        height_   = h;
        data_type_ = t;
        four_channel_ = false;
        raw_prefer_ = false;
        data_depth_ = 8;
    }

    bool is_grey(){ 
        return kDataSizeTable[data_type_] < 3;
    }

    bool is_raw(){
        return (data_type_ == TYPE_RAW_BG8 || data_type_ == TYPE_RAW_GR8 ||
                data_type_ == TYPE_RAW_BG16 || data_type_ == TYPE_RAW_GR16 ||
                data_type_ == TYPE_RAW_SMIA10
                );
    }

    bool is_16bit(){
        return (data_type_ == TYPE_RAW_BG16 || data_type_ == TYPE_RAW_GR16);
    }

    bool operator == (const ImageFormat &fmt){
        return (width_       == fmt.width_ 
            && height_       == fmt.height_ 
            && data_type_    == fmt.data_type_ 
            && four_channel_ == fmt.four_channel_ 
            && raw_prefer_   == fmt.raw_prefer_ 
            && data_depth_   == fmt.data_depth_);
    }

    bool operator != (const ImageFormat &fmt){
        return (width_       != fmt.width_ 
            || height_       != fmt.height_ 
            || data_type_    != fmt.data_type_ 
            || four_channel_ != fmt.four_channel_ 
            || raw_prefer_   != fmt.raw_prefer_ 
            || data_depth_   != fmt.data_depth_);
    }

    wstring ToString(){
        wchar_t wh[64];
        swprintf(wh, _T("_%dx%d"), width_, height_);
        wstring tmp = DataTypeToString(data_type_) + wh;
        return tmp;
    }

    
protected:
    int width_;
    int height_;

    ImageDataType data_type_;

    bool four_channel_;
    bool raw_prefer_;
    int  data_depth_;    
private:
  
    friend wostream& operator<<(wostream& os, ImageFormat &fmt);  
};


static wostream& operator<<(wostream& os, ImageFormat &fmt){
    os << kTypeStringTable[fmt.data_type_] <<_T(":") << fmt.width_ <<_T("x") << fmt.height_; 
    return os;
}

struct FormatEntry {
    BYTE          code;
    ImageDataType type;
};

struct FormatCode {
    BYTE  code;
    WORD  width;
    WORD  height;
};

class FormatMap {
public:
    static const wchar_t *FindNameByCode(BYTE code);
    static BYTE FindCodeByName(const wchar_t *name);
private:
    static int m_nEntry;
    static FormatEntry m_pEntry[16];
};

} // namespace dip
#endif