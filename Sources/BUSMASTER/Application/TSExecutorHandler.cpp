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
 * \file      TSExecutorHandler.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include ".\TSExecutorHandler.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "ProtocolsDefinitions.h"
typedef HRESULT (*SHOWTSEXECUTORWINDOW)(void* pParentWnd);
typedef HRESULT (*TSEXECUTORWINDOWSHOWN)();
//typedef HRESULT (*TSEXECUTORGETCONFIGDATA)(BYTE*& pDesBuffer, UINT& nBuffSize);
typedef HRESULT (*TSEXECUTORGETCONFIGDATA)(xmlNodePtr* pxmlNodePtr);
typedef HRESULT (*TSEXECUTORSETCONFIGDATA)(BYTE* pSrcBuffer, UINT nBuffSize);
typedef HRESULT (*TSEXECUTORSETXMLCONFIGDATA)(xmlDocPtr);
typedef HRESULT (*PFTSSTARTSTOPREADTHREAD)(ETYPE_BUS eBus, BOOL bStart);
typedef HRESULT(*PTSDOINITIALIZATION)(ETYPE_BUS eBus);
typedef HRESULT(*PTSDODEINITIALIZATION)(ETYPE_BUS eBus);

typedef HRESULT (*PTSBUSCONNECTED)(BOOL bConnected);
typedef HRESULT (*PTSSETVERSIONINFO)(BYTE bytMajor, BYTE bytMinor, BYTE bytBuild);

SHOWTSEXECUTORWINDOW  pfShowTSExecutorwindow;
TSEXECUTORWINDOWSHOWN pfTSExecutorWindowShown;
TSEXECUTORGETCONFIGDATA pfTSExecutorGetConfigdata;
TSEXECUTORSETCONFIGDATA pfTSExecutorSetConfigdata;
TSEXECUTORSETXMLCONFIGDATA pfTSExecutorSetXMLConfigdata;
PFTSSTARTSTOPREADTHREAD pfTSStartStopReadThread;
PTSDOINITIALIZATION pfTSDoInitialization;
PTSDODEINITIALIZATION pfTSDoDeInitialization;
PTSBUSCONNECTED pfTSBusConnected;
PTSSETVERSIONINFO pfTSSetVersion;

TSExecutorHandler::TSExecutorHandler(void)
{
    m_bConnected = FALSE;
    m_hTSExecutorHandle = nullptr;
}

TSExecutorHandler::~TSExecutorHandler(void)
{
    if ( m_hTSExecutorHandle != nullptr )
    {
        FreeLibrary(m_hTSExecutorHandle);
    }
}
void TSExecutorHandler::vLoadTSExecutor_DLL()
{
    if ( m_hTSExecutorHandle != nullptr )
    {
        FreeLibrary(m_hTSExecutorHandle);
        m_hTSExecutorHandle = nullptr;
    }
    m_hTSExecutorHandle = LoadLibrary(def_STR_TESTSIUTEEXECUTORDLL);
    vloadFuncPtrAddress();
}
void TSExecutorHandler::vInitializeFuncPtrs()
{
    pfShowTSExecutorwindow = nullptr;
    pfTSExecutorWindowShown = nullptr;
    pfTSExecutorGetConfigdata = nullptr;
    pfTSExecutorSetConfigdata = nullptr;
    pfTSExecutorSetXMLConfigdata = nullptr;
    pfTSStartStopReadThread = nullptr;
    pfTSDoInitialization = nullptr;
    pfTSBusConnected = nullptr;
    pfTSSetVersion = nullptr;
}
void TSExecutorHandler::vloadFuncPtrAddress()
{
    pfShowTSExecutorwindow = (SHOWTSEXECUTORWINDOW)GetProcAddress(m_hTSExecutorHandle, "TS_vShowTSExecutorWindow");
    pfTSExecutorWindowShown = (TSEXECUTORWINDOWSHOWN)GetProcAddress(m_hTSExecutorHandle, "TS_hTSEexecutorWindowShown");
    pfTSExecutorGetConfigdata = (TSEXECUTORGETCONFIGDATA)GetProcAddress(m_hTSExecutorHandle, "TS_hGetConfigurationData");
    pfTSExecutorSetConfigdata = (TSEXECUTORSETCONFIGDATA)GetProcAddress(m_hTSExecutorHandle, "TS_hSetConfigurationData");
    pfTSExecutorSetXMLConfigdata = (TSEXECUTORSETXMLCONFIGDATA)GetProcAddress(m_hTSExecutorHandle, "TS_hSetXMLConfigurationData");
    pfTSStartStopReadThread = (PFTSSTARTSTOPREADTHREAD)GetProcAddress(m_hTSExecutorHandle, "TS_StartStopReadThread");
    pfTSDoInitialization = (PTSDOINITIALIZATION)GetProcAddress(m_hTSExecutorHandle, "TS_DoInitialization");
    pfTSDoDeInitialization = (PTSDODEINITIALIZATION)GetProcAddress(m_hTSExecutorHandle, "TS_DoDeInitialization");


    pfTSBusConnected = (PTSBUSCONNECTED)GetProcAddress(m_hTSExecutorHandle, "TS_BUSConnected");
    pfTSSetVersion = (PTSSETVERSIONINFO)GetProcAddress(m_hTSExecutorHandle, "TS_SetBUSMASTERVersionInfo");
}
void TSExecutorHandler::vShowTSExecutorWindow(void* pParentWnd)
{
    if(pfShowTSExecutorwindow != nullptr)
    {
        pfShowTSExecutorwindow(pParentWnd);
        pfTSBusConnected(m_bConnected);
    }
}

void TSExecutorHandler::vGetConfigurationData(BYTE*& /* pDesBuffer */, UINT& /* unBuffSize */)
{
}

void TSExecutorHandler::vGetConfigurationData(xmlNodePtr& pxmlNodePtr)
{
    if(pfTSExecutorGetConfigdata != nullptr)
    {
        pfTSExecutorGetConfigdata(&pxmlNodePtr);
    }
}

void TSExecutorHandler::vSetConfigurationData(BYTE*& pSrcBuffer, UINT& unBuffSize)
{
    if(pfTSExecutorSetConfigdata!= nullptr)
    {
        pfTSExecutorSetConfigdata(pSrcBuffer, unBuffSize);
    }
}
void TSExecutorHandler::vSetConfigurationData(xmlDocPtr pDoc)
{
    if( nullptr != pfTSExecutorSetXMLConfigdata )
    {
        pfTSExecutorSetXMLConfigdata(pDoc);
    }
}
void TSExecutorHandler::vStartStopReadThread(ETYPE_BUS eBus, BOOL bStart)
{
    if(pfShowTSExecutorwindow != nullptr)
    {
        pfTSStartStopReadThread(eBus, bStart);
    }
}
void TSExecutorHandler::vDoInitailization(ETYPE_BUS eBus)
{
    if(pfTSDoInitialization != nullptr)
    {
        pfTSDoInitialization(eBus);
    }
}
void TSExecutorHandler::vDoDeInitailization(ETYPE_BUS eBus)
{
    if (pfTSDoDeInitialization != nullptr)
    {
        pfTSDoDeInitialization(eBus);
    }
}
void TSExecutorHandler::vBusConnected(BOOL bConnected)
{
    if(pfTSBusConnected != nullptr)
    {
        m_bConnected = bConnected;
        pfTSBusConnected(m_bConnected);
    }
}
void TSExecutorHandler::vSetTSEVersionInfo(BYTE bytMajor, BYTE bytMinor, BYTE bytBuild)
{
    if(pfTSSetVersion != nullptr)
    {
        pfTSSetVersion(bytMajor, bytMinor, bytBuild);
    }
}