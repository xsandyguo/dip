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
#include "base/basictypes.h"

#include <Windows.h>
#include <vector>

#include "framework/framework.h"

namespace Geometry{

inline Point Center(const Rect& r){
    return Point(r.X + r.Width/2, r.Y + r.Height/2);        
}

Rect MakeRect(const vector<Point>& points);

inline Rect MakeRect(const Point& p0, const Point& p1){
    return Rect(std::min<int>(p0.X, p1.X), std::min<int>(p0.Y, p1.Y),
        abs(p0.X - p1.X), abs(p0.Y - p1.Y));
}

bool LineContains(const Point& p1, const Point& p2, const Point& p);

inline Rect As(const CRect& rc){
    return Rect(rc.left, rc.top, rc.Width(), rc.Height());
}

inline Rect As(const RectF& rc){
    return Rect((int)rc.GetLeft(), (int)rc.GetTop(), (int)rc.Width, (int)rc.Height);
}

inline CRect As(const Rect& rc){
    return CRect(rc.X, rc.Y, rc.X + rc.Width, rc.Y + rc.Height);
}

inline CPoint As(const Point& p){
    return CPoint(p.X, p.Y);
}

inline Point As(const CPoint& p){
    return Point(p.x, p.y);
}

inline bool Contains(const RectF& bound, const Point& p){
    PointF pf(static_cast<float>(p.X),  static_cast<float>(p.Y));
    return !!bound.Contains(pf);
}
    
//
// check point is where on specified bezier curve. half-find
// @param p0
// @param p1
// @param p2
// @param p3
// @NOTE:
// p0.Xt³+p1.Xt²+p2.Xt+p3.X = x
// p0.Yt³+p1.Yt²+p2.Yt+p3.Y = y
// where t in range of [0.0, 1.0]
// reference below:
// http://baike.baidu.com/link?url=n7iJBnCBzhkLtffffPQO1Vsx0JFuKN3qRWi4WDXpSR4VJ_y1XN4F6G0xjcpOUF_X
//
// @return
bool IsPointOnBezier(const Point& p, const Point& p0, const Point& p1, 
                        const Point& p2, const Point& p3);

void MakeRoundPath(const Rect& bound, int percentageRounded, GraphicsPath *path);
}