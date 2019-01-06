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

#ifndef DIP_RT_IMAGE_DATA_H_
#define DIP_RT_IMAGE_DATA_H_

#include <vector>

#include "runtime/common.h"
#include "runtime/image_format.h"
#include "runtime/synchronized.h"

namespace dip{
class IProcess;

class DIP_EXPORT ImageData{
public:    
    void set_format(int w, int h, ImageDataType type){
        format_.set_width(w);
        format_.set_height(h);
        format_.set_data_type(type);
    }

    void set_format(ImageFormat& format);
    ImageFormat& get_format();

    void set_data_size(int size){ data_size_ = size; }
    int get_data_size(){ return data_size_;}

    bool is_grey() { return format_.is_grey(); }
    bool is_raw() { return format_.is_raw(); }
    int  get_width() { return format_.get_width(); }
    void set_width(int w) { format_.set_width(w); }
    int  get_height() { return format_.get_height(); }
    void set_height(int h) { format_.set_height(h); }
    ImageDataType get_data_type() { return format_.get_data_type(); }
    void set_data_type(ImageDataType type) { format_.set_data_type(type); }

    int get_data_depth() { return format_.get_data_depth(); }
    void set_data_depth(int d) { format_.set_data_depth(d); }
    void SetFourChannel(bool flag) { format_.SetFourChannel(flag); }
    bool GetFourChannel() { return format_.GetFourChannel(); }
    void SetRawPrefer(bool flag) { format_.SetRawPrefer(flag); }
    bool GetRawPrefer() { return format_.GetRawPrefer(); }

    virtual void Initialize();
    BYTE *get_buffer() { return buffer_;}
    bool has_header() { return header_; }

    bool get_header(){ return header_; }
    void set_header(bool header){ header_ = header; }

    Lockable& GetLifeLock() { return lock_; }
    void Lock() { lock_.Lock(); }
    void Unlock() { lock_.Unlock(); }

    uint64 GetModifiedTime() const { return creation_time_.GetModifiedTime(); }
    void Modified(){ creation_time_.Modified(); }

    IProcess* get_owner() const { return owner_; }
    void set_owner(IProcess* owner){ owner_ = owner; }

    void set_tagged_data(void* data){ tagged_data_ = data ;}
    void* get_tagged_data() const {return tagged_data_; }

    ImageData();
    virtual ~ImageData();
protected:

    ImageFormat format_;
    int data_size_;
    BYTE *buffer_;
    bool header_;     
    TimeStamp creation_time_;
    IProcess* owner_;
    void* tagged_data_;

    LLock lock_;

private:
    DISALLOW_EVIL_CONSTRUCTORS(ImageData);
};

} // namespace dip
#endif