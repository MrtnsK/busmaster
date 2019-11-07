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
 * \file      TSDefinitions.cpp
 * \brief     Implementation of the Class SInfo
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the Class SInfo
 */

///////////////////////////////////////////////////////////
//  TSDefinitions.cpp
//  Implementation of the Class SInfo
//  Created on:      07-Jan-2011 2:30:36 PM
///////////////////////////////////////////////////////////
#include "TestSetupEditorLib_stdafx.h"
#include "TSDefinitions.h"



/******************************************************************************
Function Name  :  SInfo
Input(s)       :
Output         :  -
Functionality  :
Member of      :  SInfo
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
SInfo::SInfo()
{

}


/******************************************************************************
Function Name  :  ~SInfo
Input(s)       :
Output         :  -
Functionality  :
Member of      :  SInfo
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
SInfo::~SInfo()
{
}

/******************************************************************************
Function Name  :  vInitialise
Input(s)       :
Output         :  void
Functionality  :
Member of      :  SInfo
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void SInfo::vInitialise(CString omCategory, CString omValue)
{
    m_omCategory = omCategory;
    m_omValue = omValue;
}


/******************************************************************************
Function Name  :  tagUSIGNALVALUE
Input(s)       :
Output         :  -
Functionality  :
Member of      :  tagUSIGNALVALUE
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
tagUSIGNALVALUE::tagUSIGNALVALUE()
{
}

/******************************************************************************
Function Name  :  SReportFile
Input(s)       :
Output         :  -
Functionality  :
Member of      :  SReportFile
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       : CS026
******************************************************************************/
SReportFile::SReportFile()
{
    m_eTimeMode = REL;
    m_eType = TXT;
    m_omPath = "";
}

/******************************************************************************
Function Name  :  ~SReportFile
Input(s)       :
Output         :  -
Functionality  :
Member of      :  SReportFile
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
SReportFile::~SReportFile()
{
}

/******************************************************************************
Function Name  :  vInitialise
Input(s)       :
Output         :  void
Functionality  :
Member of      :  SReportFile
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  14/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void SReportFile::vInitialise(eTYPE_REPORT eType, CString omPath, eTIME_MODE eTimeMode)
{
    m_eType = eType;
    m_eTimeMode = eTimeMode;
    m_omPath = omPath;
}

/******************************************************************************
Function Name  :  strCopyBSTRToCString
Input(s)       :  CComVariant& varSrc
Output         :  CString
Functionality  :  Copies BSTR to CString
Member of      :  -
Friend of      :  -
Author(s)      :  Arunkumar K
Date Created   :  20/07/2011
Modifications  :
Codetag        :
******************************************************************************/
CString strCopyBSTRToCString(CComVariant& varSrc)
{
    CString strDest ;
    char tChar = 0;
    _bstr_t bstrNodeValue(varSrc.bstrVal);
    ULONG ulLen = bstrNodeValue.length();

    for (unsigned int i = 0; i< ulLen*2; i+=2)
    {
        tChar = (char)*(varSrc.pcVal+i);
        strDest.Insert(i/2, tChar);
    }

    tChar = 0;
    strDest.Insert(ulLen, tChar);
    return strDest;
}
