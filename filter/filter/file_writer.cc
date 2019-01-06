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

#include "filter/file_writer.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>

namespace dip{
BEGIN_REGISTER_CLASSINFO( FileWriter, Mapper, "{DE7801A0-572F-4DEB-8E7B-3B44B618919F}")
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : Save image to disk")
    DESCRIPT("Version : 1.0.0")

    PATH_PROPERTY(wstring, "file path", get_file_path, set_file_path, "")

END_REGISTER_CLASSINFO( FileWriter)

FileWriter::FileWriter() : Mapper(1),
name_counter_(0),
name_pattern_(L"sample_%d.jpg"){

}

bool FileWriter::Initialize(){
    return true;
}

bool FileWriter::Execute(){
    const ImageData* data = GetInData();
    if(data){
        const IplImage* img = reinterpret_cast<const IplImage*>(
            data->get_tagged_data());

        CStringA path(file_path_.c_str());
        path.AppendFormat(CStringA(name_pattern_.c_str()), name_counter_++);

        return cvSaveImage(path, img) == CV_OK;
    }

    return true;
}


bool FileWriter::Dispose(){
    return true;
}


} // namespace dip