// FormatConverter.h : main header file for the PROJECT_NAME application
//

#pragma once
#include "stdafx.h"
#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "FormatConverter_Resource.h"       // main symbols


// CFormatConverterApp:
// See FormatConverter.cpp for the implementation of this class
//

class CFormatConverterApp : public CWinApp
{
public:
    CFormatConverterApp();

    // Overrides
public:
    virtual BOOL InitInstance();

    // Implementation

    DECLARE_MESSAGE_MAP()
};

extern CFormatConverterApp theApp;