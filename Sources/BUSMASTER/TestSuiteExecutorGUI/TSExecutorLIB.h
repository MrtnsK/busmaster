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
 * \file      TSExecutorLIB.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

//TODO::revise the inclusion
#include "Include/Utils_macro.h"
#include "Error.h"
#include "TestSetupEditorLib/TestSetupEntity.h"
#include "TSExecutorResult.h"
#include "Utility/XMLUtils.h"

typedef struct _STestSetupInfo
{
    DWORD m_dwID;
    CString m_omstrName;
    bool m_bEnable;
    INT m_nTCCount;
    //Result;
} STestSetupInfo;
typedef struct tagTestSuiteConfigInfo
{
    std::string m_strFileName;
    bool m_bEnable;
    std::list<int> m_nListSelctedCases;
} sTestSuiteConfigInfo;
typedef CList<CTestSetupEntity, CTestSetupEntity&> CTestSetupEntiyList;

class CTSExecutorLIB
{
    CTestSetupEntiyList m_ouTestSetupEntityList;
    CString m_omstrTestSuiteName;
    ETYPE_BUS m_eBusType;
    CResultGenerator m_ouResult;
    CListCtrl* m_ompResultDisplayWnd;
public:
    bool m_bTestSuiteStatus;
    CTSExecutorLIB(void);
    HRESULT SelectBus(ETYPE_BUS eCurrBus);
    HRESULT SetTestsuiteName(CString& omName);
    HRESULT GetTestsuiteName(CString& omName);
    HRESULT AddTestSetup(CString omFilePath, DWORD& dwID);
    HRESULT GetTestSetupName(DWORD dwID, CString& omName);
    HRESULT UpdateTestSetup( DWORD dwID, CString omFilePath);
    HRESULT DeleteTestSetup( DWORD dwID);
    HRESULT Update( DWORD dwID);
    HRESULT EnableTestSetup( DWORD dwID, bool bEnable);
    HRESULT RepositionTestSetup( DWORD dwID, DWORD dwIDPreceding);
    HRESULT GetTestcaseCount( DWORD dwID, UINT& unTotal);
    HRESULT GetTestcaseCount( INT nIndex, UINT& unTotal);
    HRESULT GetTestCaseInfo( DWORD dwID, UINT unIndex, CBaseEntityTA** pTCInfo);
    HRESULT EnableTestCase( DWORD dwID, UINT unIndex, bool bEnable);
    HRESULT GetConfigurationData(BYTE*& pDesBuffer, UINT& unBuffSize);
    HRESULT GetConfigurationData(xmlNodePtr pxmlNodePtr);
    HRESULT SetConfigurationData(BYTE* pDesBuffer, UINT unBuffSize);
    HRESULT SetConfigurationData(xmlNodePtr pXmlNode);
    HRESULT EnableItem(DWORD dwID, bool& bEnable);
    HRESULT RemoveAllItems(void);
    HRESULT GetTestSetupEnableStatus(DWORD dwID, bool& bEnable);
    HRESULT GetTestSetupInfo(INT nIndex, STestSetupInfo& sTSInfo);
    HRESULT GetTestSetupCount(INT& nCount);
    HRESULT Execute( /*PFCALLBACKRESULTTC pfResultTC*/);
    HRESULT SetResultDisplayWnd(CListCtrl* pWnd);
    void vSetVersionInfo(CString& omStrVersionInfo);
    virtual ~CTSExecutorLIB(void);
private:
    bool bExecuteTestSetup(CTestSetupEntity& pEntiy );
    bool bExecuteTestCase(CBaseEntityTA* pTCEntity, CResultTc& ouTestCaseResult);
    int nParseTestSuite(xmlNodePtr pNode, sTestSuiteConfigInfo& sConfigInfo);
};
