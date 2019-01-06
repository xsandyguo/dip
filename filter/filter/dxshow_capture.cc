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

#include "filter/dxshow_capture.h"

#include <algorithm>
#include <atlbase.h>
#include <atlconv.h>
#include <qedit.h>

#include <base/debug.h>
#define RETURN_IF_FAILED(x)\
do { \
    HRESULT hr = x; \
    if(FAILED(hr)){ \
        return hr; \
    } \
} while (0);


#define FREE_MEDIATYPE(mt)\
do { \
    if ((mt).cbFormat != 0){\
        CoTaskMemFree((PVOID)(mt).pbFormat);	\
        (mt).cbFormat = 0;						\
        (mt).pbFormat = NULL;					\
    }											\
    if ((mt).pUnk != NULL){	     				\
        (mt).pUnk->Release();					\
        (mt).pUnk = NULL;						\
    } \
}while(0);		

const int kDefaultPreferWidth = 640;
const int kDefaultPreferHeight = 480;

BEGIN_REGISTER_CLASSINFO(DxShowCapture, Source, \
    "{926A2764-E6D7-4A58-95C8-0B8E318B0F2C}")
    PROPERTY(wstring, "device_name",  get_device_name,  set_device_name)
    PROPERTY(bool  , "show_setting", get_show_setting, set_show_setting)
    PROPERTY(int   , "width",        get_width,        set_width)
    PROPERTY(int   , "height",       get_height,       set_height)

    METHOD_VOID("ShowDevices", DxShowCapture, ShowDevices)
    DESCRIPT("Author : CUST")
    DESCRIPT("Description : DirectShow Capture")
    DESCRIPT("Version : 1.0.0")
END_REGISTER_CLASSINFO(DxShowCapture)

DxShowCapture::DxShowCapture(void)
:width_(kDefaultPreferWidth), 
height_(kDefaultPreferHeight), frame_(NULL), buffer_size_(0){

    std::vector<CString> list;
    GetCaptures(&list);

    if(list.size()){
        device_name_ = list.at(0);
    }    
}

DxShowCapture::~DxShowCapture(void){
}


void DxShowCapture::set_width(int width){
    if(width <= 0){
        LOG_ERROR(_T("[DxShowCapture::set_width][invalid param]"));
        return;
    }
    width_ = width;
    Modified();
}

int DxShowCapture::get_width(){
    return width_;
}

void DxShowCapture::set_height(int height){
    if(height <= 0){
        LOG_ERROR(_T("[DxShowCapture::set_height][invalid param]"));
        return;
    }

    height_ = height;
    Modified();
}

int DxShowCapture::get_height(){
    return height_;
}

void DxShowCapture::ShowDevices(){
    typedef std::vector<CString>::iterator DeviceIter;
    std::vector<CString> devices;
    GetCaptures(&devices);

    for_each(devices.begin(), devices.end(), [&devices] (const CString& name){
        LOG_CMD(_T("Capture Device:%s"), name);
    } );
}

void DxShowCapture::GetCaptures(std::vector<CString>* list) {
    CComPtr<ICreateDevEnum> device_enum;
    CComPtr<IEnumMoniker> moniker_enum; 

    HRESULT hr = device_enum.CoCreateInstance(CLSID_SystemDeviceEnum);
    if (FAILED(hr) || FAILED(device_enum->CreateClassEnumerator(
        CLSID_VideoInputDeviceCategory,
        &moniker_enum, 0))){
            return;
    } 

    CComPtr<IMoniker> moniker;
    VARIANT varName;
    while (moniker_enum->Next(1, &moniker, NULL) == S_OK) {
        CComPtr<IPropertyBag> pPropBag;
        hr = moniker->BindToStorage(0,
                                    0, 
                                    IID_IPropertyBag,
                                    (void**)(&pPropBag));
        moniker = NULL;

        if (FAILED(hr)) {
            continue;  // Skip this one, maybe the next one will work.
        }
        // Find the description or friendly name.
        VariantInit(&varName);
        hr = pPropBag->Read(L"Description", &varName, 0);
        if (FAILED(hr)) {
            hr = pPropBag->Read(L"FriendlyName", &varName, 0);
        }
        if (SUCCEEDED(hr)) {
            list->push_back(CString(varName.bstrVal));
            VariantClear(&varName);
        }
    }
}

HRESULT DxShowCapture::CreateCapture(IBaseFilter** filter){
    ASSERT1(device_name_.size());

    CComPtr<ICreateDevEnum> device_enum;
    CComPtr<IEnumMoniker> moniker_enum;

    HRESULT hr = device_enum.CoCreateInstance(CLSID_SystemDeviceEnum);
    if (FAILED(hr) || FAILED(device_enum->CreateClassEnumerator(
        CLSID_VideoInputDeviceCategory,
        &moniker_enum, 0))){
            return hr;
    } 

    CComPtr<IMoniker> moniker;
    VARIANT var_name;
    while (moniker_enum->Next(1, &moniker, NULL) == S_OK) {
        CComPtr<IPropertyBag> pPropBag;
        hr = moniker->BindToStorage(0,
                                    0, 
                                    IID_IPropertyBag,
                                    (void**)(&pPropBag));
        if (FAILED(hr)) { 
            // Skip this one, maybe the next one will work.
            moniker = NULL;
            continue; 
        }

        // Find the description or friendly name.
        VariantInit(&var_name);
        hr = pPropBag->Read(L"Description", &var_name, 0);
        if (FAILED(hr)) {
            hr = pPropBag->Read(L"FriendlyName", &var_name, 0);
        }
        if (SUCCEEDED(hr)) {
            CString dev_name(var_name.bstrVal);
            CString name(device_name_.c_str());

            if(name == dev_name){
                return moniker->BindToObject(NULL, 
                                            NULL, 
                                            IID_IBaseFilter, 
                                            (void**)filter);
            }
        }
    }

    return hr;
}

bool DxShowCapture::Execute(){
    HRESULT hr = Take();
    if(FAILED(hr)){
        return false;
    }
    
    ImageData* data = GetOutDataDirect(0);
    ASSERT1(data);
    data->set_tagged_data(frame_);

    return true;
}

bool DxShowCapture::Initialize(){
    HRESULT hr;
    if(FAILED(hr = OpenCamera())){
        LOG_ERROR(_T("[DxShowCapture::Initialize falied][0x%x]"), hr);
    }

    return SUCCEEDED(hr);
}

bool DxShowCapture::Terminate() {   
    bool ok = Source::Terminate();

    media_control_->Stop();

    graph_          = NULL;
    device_filter_  = NULL;
    media_control_  = NULL;
    grabber_filter_ = NULL;
    sample_grabber_ = NULL;
    grabber_input_  = NULL;
    grabber_output_ = NULL;
    camera_output_  = NULL;
    media_event_    = NULL;
    null_filter_    = NULL;
    null_input_     = NULL;

    if (frame_){
        cvReleaseImage(&frame_);
    }

    width_       = kDefaultPreferWidth;
    height_      = kDefaultPreferHeight;
    buffer_size_ = 0;

    return ok;
}

void DxShowCapture::ShowSetting(){
    CComPtr<ISpecifyPropertyPages> property_page;
    HRESULT hr = camera_output_.QueryInterface(&property_page);
    if (SUCCEEDED(hr)){
        PIN_INFO pin_info;
        camera_output_->QueryPinInfo(&pin_info);

        CAUUID caguid;
        property_page->GetPages(&caguid);

        OleCreatePropertyFrame(NULL, 0, 0,
                                L"Property Sheet", 1,
                                (IUnknown **)&(camera_output_.p),
                                caguid.cElems,
                                caguid.pElems,
                                0, 0, NULL);
        CoTaskMemFree(caguid.pElems);
        pin_info.pFilter->Release();
    }
}

HRESULT DxShowCapture::CreateGraph(){
    // Create the Filter Graph Manager.
    RETURN_IF_FAILED(graph_.CoCreateInstance(CLSID_FilterGraph));
    RETURN_IF_FAILED(graph_.QueryInterface(&media_control_));
    RETURN_IF_FAILED(graph_.QueryInterface(&media_event_));

    RETURN_IF_FAILED(null_filter_.CoCreateInstance(CLSID_NullRenderer));
    RETURN_IF_FAILED(graph_->AddFilter(null_filter_, L"NullRenderer"));
    RETURN_IF_FAILED(grabber_filter_.CoCreateInstance(CLSID_SampleGrabber));
    RETURN_IF_FAILED(grabber_filter_.QueryInterface(&sample_grabber_));

    AM_MEDIA_TYPE media_type;
    ZeroMemory(&media_type, sizeof(AM_MEDIA_TYPE));
    media_type.majortype  = MEDIATYPE_Video;
    media_type.subtype    = MEDIASUBTYPE_RGB24;
    media_type.formattype = FORMAT_VideoInfo; 
    RETURN_IF_FAILED(sample_grabber_->SetMediaType(&media_type));
    FREE_MEDIATYPE(media_type);

    graph_->AddFilter(grabber_filter_, _T("Grabber"));

    RETURN_IF_FAILED(CreateCapture(&device_filter_));
    RETURN_IF_FAILED(graph_->AddFilter(device_filter_, NULL));

    CComPtr<IEnumPins> pin_enum;
    device_filter_->EnumPins(&pin_enum); 
    RETURN_IF_FAILED(pin_enum->Reset());
    RETURN_IF_FAILED(pin_enum->Next(1, &camera_output_, NULL)); 

    pin_enum = NULL;
    RETURN_IF_FAILED(grabber_filter_->EnumPins(&pin_enum));
    RETURN_IF_FAILED(pin_enum->Reset());
    RETURN_IF_FAILED(pin_enum->Next(1, &grabber_input_, NULL)); 

    pin_enum = NULL;
    RETURN_IF_FAILED(grabber_filter_->EnumPins(&pin_enum));
    RETURN_IF_FAILED(pin_enum->Reset());
    pin_enum->Skip(1);
    RETURN_IF_FAILED(pin_enum->Next(1, &grabber_output_, NULL)); 

    pin_enum = NULL;
    RETURN_IF_FAILED(null_filter_->EnumPins(&pin_enum));
    RETURN_IF_FAILED(pin_enum->Reset());
    RETURN_IF_FAILED(pin_enum->Next(1, &null_input_, NULL));

    return S_OK;
}

HRESULT DxShowCapture::OpenCamera(){   
    if(!device_name_.size()){
        LOG_ERROR(_T("[DxShowCapture::OpenCamera][No device detected.]"));
        return E_FAIL;
    }
    
    RETURN_IF_FAILED(CreateGraph());

    CComPtr<IAMStreamConfig> stream_config; 
    RETURN_IF_FAILED(camera_output_.QueryInterface(&stream_config));   

    AM_MEDIA_TYPE* type = NULL;    
    RETURN_IF_FAILED(stream_config->GetFormat(&type));
    if ( type->formattype == FORMAT_VideoInfo) {   
        VIDEOINFOHEADER* info_header    = (VIDEOINFOHEADER*)type->pbFormat;   
        info_header->bmiHeader.biWidth  = width_;   
        info_header->bmiHeader.biHeight = height_;   
        stream_config->SetFormat(type);
    }   
    FREE_MEDIATYPE(*type);
    
    RETURN_IF_FAILED(graph_->Connect(camera_output_, grabber_input_));
    RETURN_IF_FAILED(graph_->Connect(grabber_output_, null_input_));

    RETURN_IF_FAILED(sample_grabber_->SetBufferSamples(TRUE));
    RETURN_IF_FAILED(sample_grabber_->SetOneShot(TRUE));

    AM_MEDIA_TYPE media_type;
    RETURN_IF_FAILED(sample_grabber_->GetConnectedMediaType(&media_type));
    VIDEOINFOHEADER *video_header = 
        reinterpret_cast<VIDEOINFOHEADER*>(media_type.pbFormat);

    width_  = video_header->bmiHeader.biWidth;
    height_ = video_header->bmiHeader.biHeight;

    return S_OK;
}

HRESULT DxShowCapture::Take() {
    long event_code;
    long buffer_size = 0;

    RETURN_IF_FAILED(media_control_->Run());
    // TODO:
    media_event_->WaitForCompletion(INFINITE, &event_code);    
    RETURN_IF_FAILED(sample_grabber_->GetCurrentBuffer(&buffer_size, NULL));

    //if the buffer size changed
    if (buffer_size != buffer_size_) {
        if (frame_){
            cvReleaseImage(&frame_);
        }
        buffer_size_ = buffer_size;
        frame_ = cvCreateImage(cvSize(width_, height_), IPL_DEPTH_8U, 3);
    }

    return sample_grabber_->GetCurrentBuffer(&buffer_size_, 
                                            (long*)frame_->imageData);
}

