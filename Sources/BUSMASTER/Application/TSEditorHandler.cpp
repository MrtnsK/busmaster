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
 * \file      TSEditorHandler.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include ".\tseditorhandler.h"

typedef HRESULT (*SHOWTSEDITORWINDOW)(void* pParentWnd);
typedef HRESULT (*SETTSDILINTERFACEPTR)(void* ptrDILIntrf);
typedef HRESULT (*POSTMESSAGETOTSWND)(UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT (*TSEDITORWINDOWSHOWN)();
typedef HRESULT (*TSEDITORLOADTESTSETUPFILE)(CString omFilePath);
typedef HWND (*TSEDITORHWND)();
//typedef HRESULT (*TSEDITORGETCONFIGDATA)(BYTE*& pDesBuffer, UINT& nBuffSize);
typedef HRESULT (*TSEDITORGETCONFIGDATA)(xmlNodePtr* pxmlNodePtr);
typedef HRESULT (*TSEDITORSETCONFIGDATA)(BYTE* pSrcBuffer, UINT nBuffSize);
typedef HRESULT (*TSEDITORSETXMLCONFIGDATA)(xmlDocPtr pDoc);


SHOWTSEDITORWINDOW  pfShowTSEditorwindow;
SETTSDILINTERFACEPTR  pfSetTSDILInterfacePtr;
POSTMESSAGETOTSWND  pfPostMessageToTSWnd;
TSEDITORWINDOWSHOWN pfTSEditorWindowShown;
TSEDITORLOADTESTSETUPFILE pfTSEditorLoadTestSetupFile;
TSEDITORHWND pfTSEditorHwnd;
TSEDITORGETCONFIGDATA pfTSEditorGetConfigdata;
TSEDITORSETCONFIGDATA pfTSEditorSetConfigdata;
TSEDITORSETXMLCONFIGDATA pfTSEditorSetXMLConfigdata;


TSEditorHandler::TSEditorHandler(void)
{
    m_hTSEditorHandle = nullptr;
}

TSEditorHandler::~TSEditorHandler(void)
{
    if ( m_hTSEditorHandle != nullptr )
    {
        FreeLibrary(m_hTSEditorHandle);
    }
}
void TSEditorHandler::vLoadTSEditor_DLL()
{
    if ( m_hTSEditorHandle != nullptr )
    {
        FreeLibrary(m_hTSEditorHandle);
        m_hTSEditorHandle = nullptr;
    }
    m_hTSEditorHandle = LoadLibrary(def_STR_TESTSETUPEDITORDLL);
    vloadFuncPtrAddress();
}
void TSEditorHandler::vInitializeFuncPtrs()
{
    pfShowTSEditorwindow = nullptr;
    pfSetTSDILInterfacePtr = nullptr;
    pfPostMessageToTSWnd = nullptr;
    pfTSEditorWindowShown = nullptr;
    pfTSEditorLoadTestSetupFile = nullptr;
    pfTSEditorHwnd = nullptr;
    pfTSEditorGetConfigdata = nullptr;
    pfTSEditorSetConfigdata = nullptr;
    pfTSEditorSetXMLConfigdata = nullptr;
}
void TSEditorHandler::vloadFuncPtrAddress()
{
    pfShowTSEditorwindow = (SHOWTSEDITORWINDOW)GetProcAddress(m_hTSEditorHandle, "TS_vShowTSEditorWindow");
    pfSetTSDILInterfacePtr = (SETTSDILINTERFACEPTR)GetProcAddress(m_hTSEditorHandle, "TS_vSetDILInterfacePtr");
    pfPostMessageToTSWnd = (POSTMESSAGETOTSWND)GetProcAddress(m_hTSEditorHandle, "TS_vPostMessageToTSWnd");
    pfTSEditorWindowShown = (TSEDITORWINDOWSHOWN)GetProcAddress(m_hTSEditorHandle, "TS_hTSEditorWindowShown");
    pfTSEditorLoadTestSetupFile = (TSEDITORLOADTESTSETUPFILE)GetProcAddress(m_hTSEditorHandle, "TS_hLoadTestSetupFile");
    pfTSEditorHwnd = (TSEDITORHWND)GetProcAddress(m_hTSEditorHandle, "hGetHwnd");
    pfTSEditorGetConfigdata = (TSEDITORGETCONFIGDATA)GetProcAddress(m_hTSEditorHandle, "TSE_hGetConfigurationData");
    pfTSEditorSetConfigdata = (TSEDITORSETCONFIGDATA)GetProcAddress(m_hTSEditorHandle, "TSE_hSetConfigurationData");
    pfTSEditorSetXMLConfigdata = (TSEDITORSETXMLCONFIGDATA)GetProcAddress(m_hTSEditorHandle, "TSE_hSetXmlConfigurationData");
}
void TSEditorHandler::vShowTSEditorWindow(void* pParentWnd)
{
    if(pfShowTSEditorwindow != nullptr)
    {
        pfShowTSEditorwindow(pParentWnd);
    }
}
void TSEditorHandler::vLoadTestSetupFile(CString omFilePath)
{
    if(pfTSEditorLoadTestSetupFile != nullptr)
    {
        pfTSEditorLoadTestSetupFile(omFilePath);
    }
}
HWND TSEditorHandler::vGetSafeHWnd()
{
    HWND hWnd;
    if(pfTSEditorHwnd != nullptr)
    {
        hWnd = pfTSEditorHwnd();
        return hWnd;
    }
    else
    {
        return nullptr;
    }
}

void TSEditorHandler::vGetConfigurationData(BYTE*& /* pDesBuffer */, UINT& /* unBuffSize */)
{
}

void TSEditorHandler::vGetConfigurationData(xmlNodePtr& pxmlNodePtr)
{
    if(pfTSEditorLoadTestSetupFile != nullptr)
    {
        pfTSEditorGetConfigdata(&pxmlNodePtr);
    }
}

void TSEditorHandler::vSetConfigurationData(BYTE*& pSrcBuffer, UINT& unBuffSize)
{
    if(pfTSEditorSetConfigdata!= nullptr)
    {
        pfTSEditorSetConfigdata(pSrcBuffer, unBuffSize);
    }
}
void TSEditorHandler::vSetConfigurationData(xmlDocPtr pXmlDoc)
{
    if(pfTSEditorSetXMLConfigdata!= nullptr)
    {
        pfTSEditorSetXMLConfigdata(pXmlDoc);
    }
}
