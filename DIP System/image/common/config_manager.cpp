#include "image/common/config_manager.h"

#include <afxwin.h>
#include <tchar.h>

ImageConfig& GetImageConfig(){
    class ImageConfigImpl : public ImageConfig{};
    static ImageConfigImpl impl;

    return impl;
}

ImageConfig::ImageConfig():doc_types_(0){}

void ImageConfig::Initialize(){
    InitProfile();

#ifdef VATI_EXTENSIONS
    InitFilter();
#endif
}

void ImageConfig::InitProfile(){
#ifdef VATI_EXTENSIONS
    // init m_text by a temporary CxImage 
    CxImage *ima = new CxImage;
    ima->InitTextInfo( &TextConfig );
    delete ima;

    //recall last used font data for Text tool
    _stprintf(TextConfig.lfont.lfFaceName, GetProfileString ( _T("TextTool"), _T("lfFaceName"), _T("Times New Roman")) );
    TextConfig.lfont.lfCharSet   = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfCharSet"), EASTEUROPE_CHARSET ) ;
    TextConfig.lfont.lfWeight    = GetProfileInt ( _T("TextTool"), _T("lfWeight"), 0 );
    TextConfig.lfont.lfItalic    = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfItalic"), 0 ); 
    TextConfig.lfont.lfUnderline = (BYTE)GetProfileInt ( _T("TextTool"), _T("lfUnderline"), 0 ); 
    TextConfig.fcolor = GetProfileInt ( _T("TextTool"), _T("fcolor"), RGB( 255,255,160 ));
    TextConfig.bcolor = GetProfileInt ( _T("TextTool"), _T("bcolor"), RGB(   0, 80,160 ));
    TextConfig.opaque = (BYTE)GetProfileInt ( _T("TextTool"), _T("opaque"), 1);
    TextConfig.b_opacity = (float)(GetProfileInt( _T("TextTool"), _T("opacity"), 0 ))/(float)100.;  
    TextConfig.b_round   = (BYTE)GetProfileInt ( _T("TextTool"), _T("roundradius"), 25 );
    TextConfig.smooth    = (BYTE)GetProfileInt ( _T("TextTool"), _T("antialias"), 1 );
#endif

    OptJpegQuality = GetProfileInt(_T("Options"),_T("JpegQualityI"),90) + 0.001f * GetProfileInt(_T("Options"),_T("JpegQualityF"),0);
    OptJpegOptions = GetProfileInt(_T("Options"),_T("JpegOptions"),0);

    OptRawOptions = GetProfileInt(_T("Options"),_T("RawOptions"),0);

    doc_types_ = GetProfileInt(_T("General"),_T("DocType"),0);
}

void ImageConfig::Save(){

#ifdef VATI_EXTENSIONS
    // get the window & save
   WriteProfileInt ( _T("Screen"), _T("maximized"), IsZoomed()) ;
    if ( !IsZoomed() && !IsIconic() )
    {
        CRect rect;
        GetWindowRect ( & rect ) ;

        int xScreen = GetSystemMetrics(SM_CXSCREEN) - 100;
        int yScreen = GetSystemMetrics(SM_CYSCREEN) - 100;
        if (rect.left > xScreen)
            rect.left = xScreen;
        if (rect.top > yScreen)
            rect.top = yScreen;

        WriteProfileInt ( _T("Screen"), _T("left"),   rect.left ) ;
        WriteProfileInt ( _T("Screen"), _T("top"),    rect.top ) ;
        WriteProfileInt ( _T("Screen"), _T("right"),  rect.right ) ;
        WriteProfileInt ( _T("Screen"), _T("bottom"), rect.bottom ) ;
    }
    // save last used font data for Text tool
    WriteProfileString( _T("TextTool"), _T("lfFaceName"), GetImageConfig().TextConfig.lfont.lfFaceName );
    WriteProfileInt( _T("TextTool"), _T("lfCharSet"), GetImageConfig().TextConfig.lfont.lfCharSet );
    WriteProfileInt( _T("TextTool"), _T("lfWeight"), GetImageConfig().TextConfig.lfont.lfWeight );
    WriteProfileInt( _T("TextTool"), _T("lfItalic"), GetImageConfig().TextConfig.lfont.lfItalic );
    WriteProfileInt( _T("TextTool"), _T("lfUnderline"), GetImageConfig().TextConfig.lfont.lfUnderline );
    WriteProfileInt( _T("TextTool"), _T("fcolor"), GetImageConfig().TextConfig.fcolor );
    WriteProfileInt( _T("TextTool"), _T("bcolor"), GetImageConfig().TextConfig.bcolor );
    WriteProfileInt( _T("TextTool"), _T("opacity"), (int)(100.*GetImageConfig().TextConfig.b_opacity) );
    WriteProfileInt( _T("TextTool"), _T("r_radius"), GetImageConfig().TextConfig.b_round );
    WriteProfileInt( _T("TextTool"), _T("antialias"), GetImageConfig().TextConfig.smooth );
    WriteProfileInt( _T("Options") , _T("JpegQualityI"),(int)(GetImageConfig().OptJpegQuality));
    WriteProfileInt( _T("Options") , _T("JpegQualityF"),(int)(0.5f+1000.0f*(GetImageConfig().OptJpegQuality-(int)(GetImageConfig().OptJpegQuality))));
    WriteProfileInt( _T("Options") , _T("JpegOptions"),GetImageConfig().OptJpegOptions);
    WriteProfileInt( _T("Options") , _T("RawOptions"),GetImageConfig().OptRawOptions);
    WriteProfileInt( _T("General") , _T("DocType"),GetImageConfig().GetDocType());
    WriteProfileInt( _T("General") , _T("MenuStyle"),BCMenu::GetMenuDrawMode());
#endif

}

BOOL ImageConfig::WriteProfileString(LPCTSTR lpszSection,
                                            LPCTSTR lpszEntry, 
                                            LPCTSTR lpszDefault ){
    return AfxGetApp()->WriteProfileString(lpszSection, lpszEntry, lpszDefault);
}

CString ImageConfig::GetProfileString(LPCTSTR lpszSection, 
                                             LPCTSTR lpszEntry, 
                                             LPCTSTR lpszDefault){
    return AfxGetApp()->GetProfileString(lpszSection, lpszEntry, lpszDefault);
}

UINT ImageConfig::GetProfileInt(LPCTSTR lpszSection, 
                                       LPCTSTR lpszEntry, 
                                       int nDefault){
    return AfxGetApp()->GetProfileInt(lpszSection, lpszEntry, nDefault);
}

BOOL ImageConfig::WriteProfileInt(LPCTSTR lpszSection, 
                                         LPCTSTR lpszEntry, 
                                         int nValue){
    return AfxGetApp()->WriteProfileInt(lpszSection, lpszEntry, nValue);
}

void ImageConfig::InitFilter(){
    FloodColor.rgbBlue     = 255;
    FloodColor.rgbGreen    = 255;
    FloodColor.rgbRed      = 255;
    FloodColor.rgbReserved = 0;
    FloodTolerance         = 0;
    FloodOpacity           = 255;
    FloodSelect            = 0;

    int i;
    for(i=0;i<25;i++) Filters.Kernel5x5[i]=1;
    for(i=0;i<9;i++) Filters.Kernel3x3[i]=1;
    Filters.kSize    = 3;
    Filters.kBias    = 0;
    Filters.kDivisor = 9;

    Filters.RotateAngle    = 12.345f;
    Filters.RotateMethod   = 1;
    Filters.RotateOverflow = 0;
    Filters.RotateKeepsize = 1;

    Filters.ResampleSizemode  = 1;
    Filters.ResampleFactor    = 2;
    Filters.ResampleW         = 100;
    Filters.ResampleH         = 100;
    Filters.ResampleKeepRatio = 1;
    Filters.ResampleMethod    = 2;

    Filters.SkewPivotX = 0;
    Filters.SkewPivotY = 0;
    Filters.SkewX      =  5.5f;
    Filters.SkewY      = -5.0f;
    Filters.SkewInterp = 1;

    Filters.DitherMethod = 0;

    Filters.ThreshLevel = 128;
    Filters.ThreshPreserveColors = 0;

    Filters.ColorMode            = 0;
    Filters.ColorHSL.rgbBlue     = 50;
    Filters.ColorHSL.rgbGreen    = 12;
    Filters.ColorHSL.rgbRed      = 50;
    Filters.ColorHSL.rgbReserved = 0;
    Filters.ColorBlue            = -50;
    Filters.ColorGreen           = 0;
    Filters.ColorRed             = 50;
    Filters.ColorSolarLevel      = 128;
    Filters.ColorSolarLink       = 1;

    Filters.GammaLevel = 1.23f;
    Filters.GammaR     = 0.9f;
    Filters.GammaG     = 1.1f;
    Filters.GammaB     = 1.2f;
    Filters.GammaLink  = 0;

    Filters.IncBppBPP         = 24;
    Filters.DecBppBPP         = 4;
    Filters.DecBppPalMethod   = 1;
    Filters.DecBppErrDiff     = 0;
    Filters.DecBppLimitColors = 0;
    Filters.DecBppMaxColors   = 256;

    Filters.CanvasMode        = 0;
    Filters.CanvasW           = 800;
    Filters.CanvasH           = 600;
    Filters.CanvasLeft        = 20;
    Filters.CanvasRight       = 20;
    Filters.CanvasTop         = 10;
    Filters.CanvasBottom      = 10;
    Filters.CanvasCenterH     = 1;
    Filters.CanvasCenterV     = 1;
    Filters.CanvasKeepRatio   = 0;
    Filters.CanvasUseImageBkg = 0;
    Filters.CanvasBkg         = RGB(255,255,255);

    Filters.ShadowX         = -5;
    Filters.ShadowY         = 5;
    Filters.ShadowR         = 7;
    Filters.ShadowColor     = RGB(0,0,0);
    Filters.ShadowBkg       = RGB(255,255,255);
    Filters.ShadowIntensity = 128;
    Filters.ShadowRelative  = 0;
}