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

#ifndef DIP_RT_CAPTURE_SOURCE__
#define DIP_RT_CAPTURE_SOURCE__
#include <string>

#include "runtime/common.h"
#include "runtime/source.h"
#include "runtime/image_data.h"

namespace dip{

class DIP_EXPORT CaptureSource : public Source {
public:
    DECLEAR_ABSTRACT_CLASSINFO(CaptureSource, Source)

    virtual void SetImageDataType(ImageDataType type);	
    virtual void SetImageWidth(int width);	
    virtual void SetImageHeight(int height);	
    virtual void SetImageFormat(int w, int h, int type);	
    virtual void SetImageFormat(ImageFormat fmt) = 0;
    virtual ImageFormat GetOutFormat() { return format_; }

    virtual ImageDataType GetImageDataType();	
    virtual int GetImageWidth();	
    virtual int GetImageHeight();	
    virtual wstring GetSensorName() = 0;
    virtual bool IsSupportFPGA(){ return false; }
    virtual bool Is16Bit() = 0;	
    virtual bool SendSetting(wchar_t *szFile) = 0;	
    virtual unsigned short ReadRegister(unsigned short addr) = 0;	
    virtual bool WriteRegister(unsigned short addr, BYTE val) = 0;	
    virtual bool DumpRegister(WORD addrMin, WORD addrMax, wchar_t *szFile) = 0;
    virtual bool Scan() = 0;
    virtual bool Detect()=0;

    virtual bool LockDevice() { return true; }
    virtual bool UnlockDevice() { return true; }	
    virtual void ClearDumpFile();
    virtual bool ExecuteTask(const wchar_t *taskName, const vector<int> &vals);

protected:
    CaptureSource();
    virtual ~CaptureSource();

    ImageFormat    format_;
private:
    DISALLOW_EVIL_CONSTRUCTORS(CaptureSource);
};

} // namspace dip
#endif
