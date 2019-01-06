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

#ifndef DIP_RT_VERSION_H
#define DIP_RT_VERSION_H

#include "runtime/common.h"
#include "runtime/object.h"

namespace dip{

#define DIP_VERSION "1.0.0.1"
#define DIP_MAJOR_VERSION 1
#define DIP_MINOR_VERSION 0
#define DIP_BUILD_VERSION 2
#define DIP_SOURCE_VERSION "Dip version " DIP_VERSION "), Dip source $Revision: 1.0 $, $Date: 2012/12/30 17:21:38 $ (GMT)"

class DIP_EXPORT DipVersion : public Object {
public:
  	DECLEAR_CLASSINFO(DipVersion, Object);   

  // Description: 
  // Return the version of Dip this object is a part of.
  // A variety of methods are included. GetSourceVersion returns a wstring
  // with an identifier which timestamps a particular source tree. 
  static const wchar_t *GetVersion() { return DIP_VERSION; };
  static int GetMajorVersion() { return DIP_MAJOR_VERSION; };
  static int GetMinorVersion() { return DIP_MINOR_VERSION; };
  static int GetBuildVersion() { return DIP_BUILD_VERSION; };
  static const wchar_t *GetSourceVersion() { return DIP_SOURCE_VERSION; };
  
protected:
  DipVersion() {}; //insure constructor/destructor protected
  ~DipVersion() {};
  DipVersion(const DipVersion&) {};
  void operator=(const DipVersion&) {};

};

} // namespace dip
#endif 