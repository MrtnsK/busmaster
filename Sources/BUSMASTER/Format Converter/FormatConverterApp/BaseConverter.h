#pragma once
#include "FormatConverter_stdafx.h"
//#include "GettextBusmaster.h"
#include "../../Utility/MultiLanguageSupport.h"


class CBaseConverter
{
public:
    virtual HRESULT GetInputFileFilters(string&, string& ) = 0;
    virtual HRESULT GetOutputFileFilters(string&, string& ) = 0;
    virtual HRESULT ConvertFile(string& chInputFile, string& chOutputFile) = 0;
    virtual HRESULT GetConverterName(string& strConverterName) = 0;
    virtual HRESULT GetErrorStatus(HRESULT hResult, string& omstrStatus) = 0;
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, string& omstrStatus) = 0;
    virtual HRESULT GetHelpText(CString& pchHelpText)=0;
    virtual BOOL bHaveOwnWindow() = 0;
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage ) = 0;
    virtual HRESULT GettextBusmaster() = 0;
    virtual ~CBaseConverter() {};
};