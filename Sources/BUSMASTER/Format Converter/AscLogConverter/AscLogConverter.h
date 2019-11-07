/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      AscLogConverter.h
 * \brief     Descripton of the AscLogConverter class and DLLMain Function.
 * \authors   Venkatanarayana Makam, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Description of the AscLogConverter class.
 */

#pragma once

/* C++ includes */
#include <string>

/* Project includes */
#include "../FormatConverterApp/BaseConverter.h"

using namespace std;

#define ERR_INPUT_FILE_NOTFOUND          (-1)
#define ERR_OUTPUT_FILE_NOTFOUND         (-2)

class CAscLogConverter : public CBaseConverter
{
    string m_omstrConversionStatus;
    HRESULT m_hResult;
public:
    CAscLogConverter(void);
    ~CAscLogConverter(void);
    virtual HRESULT GetInputFileFilters(string&, string& );
    virtual HRESULT GetOutputFileFilters(string&, string& );
    virtual HRESULT ConvertFile(string& chInputFile, string& chOutputFile);
    virtual HRESULT GetConverterName(string& strConverterName);
    virtual HRESULT GetErrorStatus(HRESULT hResult, string& omstrStatus);
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, string& omstrStatus);
    virtual HRESULT GetHelpText(CString& pchHelpText);
    virtual BOOL bHaveOwnWindow();
    virtual HRESULT GettextBusmaster();
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage)
    {
        return S_FALSE;
    };
};
