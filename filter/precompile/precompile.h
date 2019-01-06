// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define STRSAFE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS                       1
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES         1
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES       1
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT 1

#include "common/targetver.h"


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#define POINTER_64 __ptr64
#include <windows.h>
#include <opencv/cv.h>


// TODO: reference additional headers your program requires here
