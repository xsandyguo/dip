#pragma once

#include <windows.h>
#include <afxstr.h>

#include <base/basictypes.h>

struct FiltersConfig{
    long	Kernel5x5[25];
    long	Kernel3x3[9];
    long	kSize;
    long	kBias;
    long	kDivisor;

    float	RotateAngle;
    long	RotateMethod;
    long	RotateOverflow;
    BYTE	RotateKeepsize;

    BYTE	ResampleSizemode;
    float	ResampleFactor;
    long	ResampleW;
    long	ResampleH;
    BYTE	ResampleKeepRatio;
    BYTE	ResampleMethod;

    long	SkewPivotX;
    long	SkewPivotY;
    float	SkewSlopeX;
    float	SkewSlopeY;
    float	SkewX;
    float	SkewY;
    BYTE	SkewInterp;

    BYTE	DitherMethod;

    BYTE	ThreshLevel;
    BYTE	ThreshPreserveColors;

    BYTE	ColorMode;
    RGBQUAD	ColorHSL;
    long	ColorRed;
    long	ColorGreen;
    long	ColorBlue;
    BYTE	ColorSolarLevel;
    BYTE	ColorSolarLink;

    float	GammaLevel;
    float	GammaR;
    float	GammaG;
    float	GammaB;
    BYTE	GammaLink;

    BYTE	IncBppBPP;
    BYTE	DecBppBPP;
    BYTE	DecBppPalMethod;
    BYTE	DecBppErrDiff;
    BYTE	DecBppLimitColors;
    long	DecBppMaxColors;

    BYTE	CanvasMode;
    long	CanvasW;
    long	CanvasH;
    long	CanvasLeft;
    long	CanvasRight;
    long	CanvasTop;
    long	CanvasBottom;
    BYTE	CanvasCenterH;
    BYTE	CanvasCenterV;
    BYTE	CanvasKeepRatio;
    BYTE	CanvasUseImageBkg;
    COLORREF CanvasBkg;

    long	ShadowX;
    long	ShadowY;
    BYTE	ShadowR;
    COLORREF ShadowColor;
    COLORREF ShadowBkg;
    BYTE	ShadowIntensity;
    BYTE	ShadowRelative;
} ;

class ImageConfig{
public:
    float OptJpegQuality;
    int OptJpegOptions;
    int OptRawOptions;

#ifdef VATI_EXTENSIONS
    // for text placement
    CxImage::CXTEXTINFO  TextConfig;
#endif
    FiltersConfig Filters;
    RGBQUAD FloodColor;
    BYTE	FloodTolerance;
    BYTE	FloodOpacity;
    BYTE	FloodSelect;
    
    int GetDocType();
    void SetDocType(int type){ doc_types_ = type;}
    void Initialize();
    void Save();

protected:
    ImageConfig();

private:
    void InitProfile();
    void InitFilter();

    BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault);
    CString GetProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault = NULL);
    UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault);
    BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue);

    int doc_types_;
    DISALLOW_COPY_AND_ASSIGN(ImageConfig);
};

ImageConfig& GetImageConfig();