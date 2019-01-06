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
#include "base/basictypes.h"

#include <opencv/cv.h>

class DIP_EXPORT  OcvImage {
public:
    OcvImage();
    virtual ~OcvImage();

    /* Create image (BGR or grayscale) */
    virtual bool  Create( int width, int height, int bits_per_pixel, int image_origin = 0 );

    /* Load image from specified file */
    virtual bool  Load( const char* filename, int desired_color = 1 );

    /* Load rectangle from the file */
    virtual bool  LoadRect( const char* filename,
                            int desired_color,
                            CvRect r );

    virtual bool  LoadRect( const char* filename,
                            int desired_color, 
                            RECT r ) {
        return LoadRect( filename, desired_color,
            cvRect( r.left, r.top, r.right - r.left, r.bottom - r.top ));
    }

    /* Save entire image to specified file. */
    virtual bool  Save( const char* filename );

    /* Get copy of input image ROI */
    virtual void  CopyOf( OcvImage& image, int desired_color = -1 );
    virtual void  CopyOf( IplImage* img, int desired_color = -1 );
    IplImage* GetImage() { return image_; };
    virtual void  Destroy(void);

    /* width and height of ROI */
    int Width() { return !image_ ? 0 : !image_->roi ? image_->width : image_->roi->width; };
    int Height() { return !image_ ? 0 : !image_->roi ? image_->height : image_->roi->height;};
    int Bpp() { return image_ ? (image_->depth & 255)*image_->nChannels : 0; };
    virtual void  Fill( int color );

    /* draw to highgui window */
    virtual void  Show( const char* window );

    /* draw part of image to the specified DC */
    virtual void  Show( HDC dc, int x, int y, int width, int height,
                        int from_x = 0, int from_y = 0 );

    /* draw the current image ROI to the specified rectangle of the destination DC */
    virtual void  DrawToHDC( HDC hDCDst, RECT* pDstRect );
    bool IsValid() {return image_ != NULL;}

private:
    IplImage*  image_;

    DISALLOW_EVIL_CONSTRUCTORS(OcvImage);
};
