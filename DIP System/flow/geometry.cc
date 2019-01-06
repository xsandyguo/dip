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

#include "flow/geometry.h"

namespace Geometry{

Rect MakeRect(const vector<Point>& points){    
    if(points.size() == 0){
       return kEmptyRect;
    }

    Point min_p = *points.begin();
    Point max_p = min_p;
    for (vector<Point>::const_iterator itor = points.begin();
        itor < points.end(); ++itor){
        Point p = *itor;
        min_p.X = min(min_p.X, p.X);
        min_p.Y = min(min_p.Y, p.Y);

        max_p.X = max(max_p.X, p.X);
        max_p.Y = max(max_p.Y, p.Y);
    }

    return Rect(min_p, Size(max_p.X - min_p.X, max_p.Y - min_p.Y));
}

bool LineContains(const Point& p1, const Point& p2, const Point& p){
    vector<Point> points;
    points.push_back(p1);
    points.push_back(p2);

    Rect r = MakeRect(points);
    r.Inflate(2,2);
    if (! r.Contains(p))
        return false;

    double a, b, x, y;

    if (p1.X == p2.X)
        return (std::abs(p.X - p1.X) < 3);

    if (p1.Y == p2.Y)
        return (std::abs(p.Y - p1.Y) < 3);

    a = (double)(p1.Y - p2.Y) / (double)(p1.X - p2.X);
    b = (double)p1.Y - a * (double)p1.X;
    x = (p.Y - b) / a;
    y = a * p.X + b;

    return (std::min<double>(std::abs(x - p.X), std::abs(y - p.Y)) < 4);

}

inline double BezierFormula(int p0, int p1, int p2, int p3, double t){
    double r = 1.0 - t;
    return p0*r*r*r + 3*p1*t*r*r + 3*p2*t*t*r + p3*t*t*t;
}

//
// check point is where on specified bezier curve. half-find
// @param p0
// @param p1
// @param p2
// @param p3
// @NOTE:
// p0(1-t)³+3p1t(1-t)²+3p2t²(1-t)+p3t³ = p
// where t in range of [0.0, 1.0]
// reference below:
// http://baike.baidu.com/link?url=n7iJBnCBzhkLtffffPQO1Vsx0JFuKN3qRWi4WDXpSR4VJ_y1XN4F6G0xjcpOUF_X
//
// @return
bool IsPointOnBezier(const Point& p, const Point& p0, const Point& p1, 
                     const Point& p2, const Point& p3){
    double left               = 0.0;
    double right              = 1.0;
    const int kTorlance       = 3;
    const double kGranularity = 0.000001;
    int y                     = INT_MAX;

    Rect bounds(MakeRect(p0, p3));
    bounds.Inflate(kTorlance, kTorlance);
    if(!bounds.Contains(p)){
        return false;
    }    

    while(abs(left - right) > kGranularity){
        double middle = (left + right) * 0.5;
        double x = BezierFormula(p0.X, p1.X, p2.X, p3.X, middle);
        if(abs(x - p.X) < 0.5){
            y = static_cast<int>(BezierFormula(p0.Y, p1.Y, p2.Y, p3.Y, middle) + 0.5f);
            break;
        }

        double  lr = BezierFormula(p0.X, p1.X, p2.X, p3.X, left) - p.X;
        double  mr = BezierFormula(p0.X, p1.X, p2.X, p3.X, middle) - p.X;

        if(lr * mr < 0)
            right = middle;
        else
            left = middle;        
    }
    
    return abs(y - p.Y) <= kTorlance;
}

void MakeRoundPath(const Rect& bound, int percentageRounded, GraphicsPath *path){
    int left  = bound.GetLeft();   
    int right = bound.GetRight();   

    int top    = bound.GetTop();   
    int bottom = bound.GetBottom();   

    int offsetX = (right-left)*percentageRounded/100;     
    int offsetY = (bottom-top)*percentageRounded/100;   

    path->AddArc(right-offsetX, top, offsetX, offsetY, 270, 90);
    path->AddArc(right-offsetX, bottom-offsetY, offsetX, offsetY, 0, 90); 
    path->AddArc(left, bottom - offsetY, offsetX, offsetY, 90, 90);   
    path->AddArc(left, top, offsetX, offsetY, 180, 90);   
    path->AddLine(left + offsetX, top, right - offsetX/2, top);   
}   

}