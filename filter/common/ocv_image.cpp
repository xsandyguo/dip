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

#include "common/ocv_image.h"

#include "base/debug.h"
#include <opencv/highgui.h>

CV_INLINE CvRect RectToCvRect( RECT sr );
CV_INLINE IplROI RectToROI( RECT r );
CV_INLINE RECT CvRectToRect( CvRect sr );
CV_INLINE RECT NormalizeRect( RECT r );

CV_INLINE RECT NormalizeRect( RECT r ) {
    int t;
    if( r.left > r.right ) {
        t       = r.left;
        r.left  = r.right;
        r.right = t;
    }
    if( r.top > r.bottom ) {
        t        = r.top;
        r.top    = r.bottom;
        r.bottom = t;
    }

    return r;
}

CV_INLINE CvRect RectToCvRect( RECT sr ) {
    sr = NormalizeRect( sr );
    return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );
}

CV_INLINE RECT CvRectToRect( CvRect sr ) {
    RECT dr;
    dr.left   = sr.x;
    dr.top    = sr.y;
    dr.right  = sr.x + sr.width;
    dr.bottom = sr.y + sr.height;

    return dr;
}

CV_INLINE IplROI RectToROI( RECT r ) {
    IplROI roi;
    r           = NormalizeRect( r );
    roi.xOffset = r.left;
    roi.yOffset = r.top;
    roi.width   = r.right - r.left;
    roi.height  = r.bottom - r.top;
    roi.coi     = 0;

    return roi;
}

void  FillBitmapInfo( BITMAPINFO* bmp_info, 
                        int width, 
                        int height, 
                        int bpp,
                        int origin ) {
    ASSERT1( bmp_info && 
            width >= 0 && 
            height >= 0 && 
            (bpp == 8 || bpp == 24 || bpp == 32));

    BITMAPINFOHEADER* header = &(bmp_info->bmiHeader);

    memset( header, 0, sizeof(*header));
    header->biSize        = sizeof(BITMAPINFOHEADER);
    header->biWidth       = width;
    header->biHeight      = origin ? abs(height) : -abs(height);
    header->biPlanes      = 1;
    header->biBitCount    = (unsigned short)bpp;
    header->biCompression = BI_RGB;

    if( bpp == 8 ) {
        RGBQUAD* palette = bmp_info->bmiColors;
        for(int i = 0; i < 256; i++ ) {
            palette[i].rgbBlue = palette[i].rgbGreen 
                = palette[i].rgbRed = (BYTE)i;
            palette[i].rgbReserved = 0;
        }
    }
}
OcvImage::OcvImage() {
    image_ = 0;
}
void OcvImage::Destroy() {
    cvReleaseImage( &image_ );
}

OcvImage::~OcvImage() {
    Destroy();
}

bool  OcvImage::Create( int w, int h, int bpp, int origin ) {
    const unsigned max_img_size = 10000;

    if( (bpp != 8 && bpp != 24 && bpp != 32) ||
        (unsigned)w >=  max_img_size || (unsigned)h >= max_img_size ||
        (origin != IPL_ORIGIN_TL && origin != IPL_ORIGIN_BL)) {
        ASSERT1(0); // most probably, it is a programming error
        return false;
    }

    if( !image_ || Bpp() != bpp || image_->width != w || image_->height != h ) {
        if( image_ && image_->nSize == sizeof(IplImage))
            Destroy();
        /* prepare IPL header */
        image_ = cvCreateImage( cvSize( w, h ), IPL_DEPTH_8U, bpp/8 );
    }

    if( image_ ){
        image_->origin = origin == 0 ? IPL_ORIGIN_TL : IPL_ORIGIN_BL;
    }

    return image_ != 0;
}

void  OcvImage::CopyOf( OcvImage& image, int desired_color ) {
    IplImage* img = image.GetImage();
    if( img ) {
        CopyOf( img, desired_color );
    }
}

void  OcvImage::CopyOf( IplImage* img, int desired_color ) {
    ASSERT1(img);
    ASSERT1(img->imageData);

    if( img->nSize == sizeof(IplImage) ) {
        int color = desired_color;
        CvSize size = cvGetSize( img ); 
        if( color < 0 ){
            color = img->nChannels > 1;
        }

        if( Create( size.width, size.height,
            (!color ? 1 : img->nChannels > 1 ? img->nChannels : 3)*8,
            img->origin )) {
            cvConvertImage( img, image_, 0 );
        }
    }
}

bool  OcvImage::Load( const char* filename, int desired_color ){
    IplImage* img = cvLoadImage( filename, desired_color );
    if( !img ){
        return false;
    }

    CopyOf( img, desired_color );
    cvReleaseImage( &img );

    return true;
}
bool  OcvImage::LoadRect( const char* filename,
                            int desired_color, CvRect r ) {
    if( r.width < 0 || r.height < 0 ) return false;

    IplImage* img = cvLoadImage( filename, desired_color );
    if( !img ){
        return false;
    }

    if( r.width == 0 || r.height == 0 ) {
        r.width = img->width;
        r.height = img->height;
        r.x = r.y = 0;
    }
    if( r.x > img->width || r.y > img->height ||
        r.x + r.width < 0 || r.y + r.height < 0 ){
        cvReleaseImage( &img );
        return false;
    }

    /* truncate r to source image */
    if( r.x < 0 ) {
        r.width += r.x;
        r.x = 0;
    }
    if( r.y < 0 ) {
        r.height += r.y;
        r.y = 0;
    }
    if( r.x + r.width > img->width )
        r.width = img->width - r.x;

    if( r.y + r.height > img->height ){
        r.height = img->height - r.y;
    }

    cvSetImageROI( img, r );
    CopyOf( img, desired_color );
    cvReleaseImage( &img );
    return true;
}
bool  OcvImage::Save( const char* filename ) {
    if( !image_ )
        return false;

    cvSaveImage( filename, image_ );

    return true;
}

void  OcvImage::Show( const char* window ) {
    if( image_ ){
        cvShowImage( window, image_ );
    }
}

void  OcvImage::Show( HDC dc, int x, int y, int w, int h, int from_x, int from_y ) {
    ASSERT1(image_);

    if (image_->depth != IPL_DEPTH_8U) {
        return;
    }

    uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
    BITMAPINFO* bmi = (BITMAPINFO*)buffer;
    int bmp_w = image_->width, bmp_h = image_->height;
    FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), image_->origin );

    from_x = MIN( MAX( from_x, 0 ), bmp_w - 1 );
    from_y = MIN( MAX( from_y, 0 ), bmp_h - 1 );
    int sw = MAX( MIN( bmp_w - from_x, w ), 0 );
    int sh = MAX( MIN( bmp_h - from_y, h ), 0 );

    SetDIBitsToDevice(
        dc, x, y, sw, sh, from_x, from_y, from_y, sh,
        image_->imageData + from_y*image_->widthStep,
        bmi, 
        DIB_RGB_COLORS );    
}

void  OcvImage::DrawToHDC( HDC hdc, RECT* rect ) {
    ASSERT1(rect);
    ASSERT1(image_);
    ASSERT1(image_->imageData);

    if (image_->depth != IPL_DEPTH_8U) {
        return;
    }

    uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
    BITMAPINFO* bmi = (BITMAPINFO*)buffer;
    int bmp_w = image_->width, bmp_h = image_->height;
    CvRect roi = cvGetImageROI( image_ );
    CvRect dst = RectToCvRect( *rect );

    if( roi.width == dst.width && roi.height == dst.height ){
        Show( hdc, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y );
        return;
    }

    if( roi.width > dst.width ) {
        SetStretchBltMode(
            hdc,           // handle to device context
            HALFTONE );
    }else {
        SetStretchBltMode(
            hdc,           // handle to device context
            COLORONCOLOR );
    }
    FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), image_->origin );
    ::StretchDIBits(hdc,
                    dst.x, dst.y, dst.width, dst.height,
                    roi.x, roi.y, roi.width, roi.height,
                    image_->imageData, bmi, DIB_RGB_COLORS, SRCCOPY );
    
}

void  OcvImage::Fill( int color ){
    cvSet( image_, cvScalar(color&255,
                            (color>>8)&255, 
                            (color>>16)&255, 
                            (color>>24)&255) );
}

