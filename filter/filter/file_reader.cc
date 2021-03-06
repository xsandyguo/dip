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

#include "filter/file_reader.h"

#include <atlstr.h>
#include <opencv/cv.h>

namespace dip{

BEGIN_REGISTER_CLASSINFO( FileReader, Source, \
    "{47BADB9C-50BF-466E-B0B4-FE4E4ACCC3F6}")
    PATH_PROPERTY(wstring, "file path", get_file_path, set_file_path,\
    "Png File(*.png)|*.png|jpeg (*.jpg;*.jpeg;*.jpe)|*.jpg;*.jpeg;*.jpe|bmp (*.bmp)|*.bmp||")
    
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : FileReader")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO( FileReader)

FileReader::FileReader(){
}

FileReader::~FileReader(){
}

bool FileReader::Execute(){
    if(!::PathFileExists(file_path_.c_str())){
        LOG_ERROR(_T("[FileReader::Initialize][File %s doesn't existed.]"), 
            file_path_);
        return false;
    }

    CStringA path(file_path_.c_str());
    image_.reset(cvLoadImage(path)); 

    if(!image_){
        LOG_ERROR(_T("[FileReader::Execute][cvLoadImage failed.]"));
        return false;
    }

    ImageData* data = GetOutDataDirect(0);
    ASSERT1(data);
    data->set_tagged_data(image_.get());

    return true;
}

} // namespace dip