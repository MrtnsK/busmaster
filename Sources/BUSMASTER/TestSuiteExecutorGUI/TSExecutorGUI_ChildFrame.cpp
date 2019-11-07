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
 * \file      TSExecutorGUI_ChildFrame.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#pragma once
#include "TSExecutorGUI_stdafx.h"
#include "TSExecutorGUI_ChildFrame.h"
#include "TSExecutorGUI_resource.h"
#include "TestSetupEditorGUI\TSEditorGUI_Definitions.h"
#include ".\tsexecutorgui_childframe.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"
#include "Utility/MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

//TODO::Move To definitions
#define def_INDEX_TESTSUITEIMAGE    0
#define def_INDEX_TESTSETUPIMAGE    1
#define def_INDEX_TESTCASEIMAGE     2
#define def_ID_TESTSUITE            -1
#define WM_EXECUSTIONOVER           0X401
// CTSExecutorChildFrame
extern CTSExecutorChildFrame* g_pomTSExecutorChildWindow;
typedef struct _ExecutionThreadParam
{
    HWND m_hTarget;
    CTSExecutorLIB* m_pouExecutorLib;
} ExecutionThreadParam;


DWORD WINAPI ExecuteThread( LPVOID pParam )
{
    USES_CONVERSION;

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pParam;
    if (pThreadParam == nullptr)
    {
        return ((DWORD)-1);
    }
    CTSExecutorChildFrame* pExecutionFrame = (CTSExecutorChildFrame*)pThreadParam->m_pBuffer;
    if (pExecutionFrame == nullptr)
    {
        return ((DWORD)-1);
    }
    bool bLoopON = true;
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                pExecutionFrame->m_ouTSExecutor.Execute();
                pExecutionFrame->SendMessage(WM_EXECUSTIONOVER, 0, 0);
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            default:
            {

            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return S_OK;
}
IMPLEMENT_DYNCREATE(CTSExecutorChildFrame, CMDIChildWnd)

/******************************************************************************
Function Name  :  CTSExecutorChildFrame
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CTSExecutorChildFrame::CTSExecutorChildFrame(void)
{
    //TODO::Check for appropriate value
    POINT MaxPosition, MinPosition;
    MaxPosition.x = 0;
    MaxPosition.y = 0;
    MinPosition.x = 0;
    MinPosition.y = 0;

    RECT NormalPosition;
    NormalPosition.left = 63;
    NormalPosition.top  = 4;
    NormalPosition.right = 913;
    NormalPosition.bottom = 596;

    m_sTSDefPlacement.length = 44;
    m_sTSDefPlacement.flags = 1;
    m_sTSDefPlacement.showCmd = 1;
    m_sTSDefPlacement.ptMaxPosition = MaxPosition;
    m_sTSDefPlacement.ptMinPosition = MinPosition;
    m_sTSDefPlacement.rcNormalPosition = NormalPosition;
    m_bConnected = false;
}

/******************************************************************************
Function Name  :  ~CTSExecutorChildFrame
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
CTSExecutorChildFrame::~CTSExecutorChildFrame(void)
{
    m_ouExecutionThread.bTerminateThread();
}
BEGIN_MESSAGE_MAP(CTSExecutorChildFrame, CMDIChildWnd)
    ON_WM_DESTROY()
    ON_WM_CREATE()
    ON_MESSAGE(IDM_TESTSUITE_ADD, OnAddTestSetup)
    ON_MESSAGE(IDM_TESTSETUP_DELETE, OnDeleteTestSetup)
    ON_MESSAGE(IDM_TESTSETUP_MODIFY, OnUpdate)
    ON_MESSAGE(IDM_TESTSUITE_EXECUTE, OnExecute)
    ON_MESSAGE(WM_EXECUSTIONOVER, OnExecutionOver)
    ON_WM_CLOSE()
END_MESSAGE_MAP()

/******************************************************************************
Function Name  :  OnDestroy
Input(s)       :  -
Output         :  void
Functionality  :  Handler for WM_DESTROY
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSExecutorChildFrame::OnDestroy(void)
{
    CMDIChildWnd::OnDestroy();
    g_pomTSExecutorChildWindow = nullptr;
}

/******************************************************************************
Function Name  :  OnCreateClient
Input(s)       :  LPCREATESTRUCT lpcs - Create Structure
                  CCreateContext* pContext -  Context
Output         :  BOOL
Functionality  :  Creates the Left and right child views
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
BOOL CTSExecutorChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    BOOL bReturn = m_omSplitterWnd.CreateStatic( this, def_NUM_ROWS_TSEDITOR, def_NUM_COLS_TSEDITOR );
    CSize omSize(def_WIDTH_PANE, def_HEIGHT_PANE);
    m_omSplitterWnd.CreateView(def_ROW_INDEX, def_INDEX_TREEVIEW, RUNTIME_CLASS(CTreeViewEx),omSize, pContext);
    m_omSplitterWnd.CreateView(def_ROW_INDEX, def_INDEX_PROPVIEW, RUNTIME_CLASS(CResultView),omSize, pContext);

    m_odTreeView =  (CTreeViewEx*)m_omSplitterWnd.GetPane(def_ROW_INDEX, def_INDEX_TREEVIEW);
    m_odResultView = (CResultView*)m_omSplitterWnd.GetPane(def_ROW_INDEX, def_INDEX_PROPVIEW);

    CListCtrl& omPropertyList = m_odResultView->GetListCtrl();
    omPropertyList.SetExtendedStyle(omPropertyList.GetExtendedStyle() |LVS_REPORT | LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
    omPropertyList.InsertColumn(0, _("TestCase Name"), LVCFMT_LEFT, 100, -1);
    omPropertyList.InsertColumn(1, _("Execution Step"), LVCFMT_LEFT, 150, -1);
    omPropertyList.InsertColumn(2, _("Result"), LVCFMT_LEFT, 100, -1);

    m_pomImageList = new CImageList();
    m_pomImageList->Create(def_HEIGHT_IMAGE, def_WIDTH_IMAGE, ILC_COLOR32, 3, 3);
    m_pomImageList->SetBkColor(def_COLOR_TREE_BKG);
    CBitmap omBitmap;

    for (int nID = IDI_ICON_TESTCASE; nID <= IDI_ICON_TESTCASE+2; nID++)  // load bitmaps for dog, bird and fish
    {
        HICON hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_TESTCASE));
        DWORD err = GetLastError();
        m_pomImageList->Add(hIcon);
    }


    m_odTreeView->GetTreeCtrl().SetImageList(m_pomImageList, TVSIL_NORMAL);
    return TRUE;
}

/******************************************************************************
Function Name  :  OnCreate
Input(s)       :  LPCREATESTRUCT lpCreateStruct - Create Structure
Output         :  int
Functionality  :
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
int CTSExecutorChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    CString omstrName;
    m_ouTSExecutor.GetTestsuiteName(omstrName);

    m_hParentTreeItem = m_odTreeView->InsertTreeItem(nullptr, omstrName, nullptr,def_INDEX_TESTSUITEIMAGE,
                        def_INDEX_TESTSUITEIMAGE, def_ID_TESTSUITE);

    m_ouExecutionThread.m_pBuffer = this;
    m_ouExecutionThread.m_hActionEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    m_ouExecutionThread.bStartThread(ExecuteThread);
    //SSH + issue#438
    SetIcon(AfxGetApp()->LoadIcon(IDI_APP_ICON), FALSE);
    //SSH -
    return 0;
}



/******************************************************************************
Function Name  :  OnAddTestSetup
Input(s)       :  WPARAM wparam, LPARAM lparam
Output         :  HRESULT
Functionality  :  Adds a Test Setup Using File Browser
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS036
******************************************************************************/
HRESULT CTSExecutorChildFrame::OnAddTestSetup(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
    UNREFERENCED_PARAMETER(wparam);
    DWORD dwID;
    CString omTestSetupName;
    CFileDialog omTestSetupBrowser(TRUE, "*.xml", 0, OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT, szFilter);

    //For Multiple File Select - Hint From Msdn
    const int nSize =  _MAX_PATH  * _MAX_PATH ;  //Total 260 Files
    char* szTempBuf = new char[nSize];
    memset(szTempBuf, 0, sizeof(char) * nSize);
    omTestSetupBrowser.m_ofn.lpstrFile = szTempBuf;
    omTestSetupBrowser.m_ofn.nMaxFile = nSize - 1;

    INT nRetVal = (INT)omTestSetupBrowser.DoModal();
    if(nRetVal == IDOK)
    {
        POSITION pos = omTestSetupBrowser.GetStartPosition();
        while(pos != nullptr)
        {

            CString omstrTestSetup = omTestSetupBrowser.GetNextPathName(pos);
            HRESULT hResult = m_ouTSExecutor.AddTestSetup(omstrTestSetup, dwID);
            if(hResult == S_OK)
            {
                INT nCount;
                m_ouTSExecutor.GetTestSetupCount(nCount);
                bParseTestSetup(nCount-1);
            }
            else
            {
                CString omStrInfo;
                omStrInfo.Format(_("The %s is Invalid File"), omstrTestSetup.GetBuffer(MAX_PATH));
                MessageBox(omStrInfo, _("Error"), MB_OK|MB_ICONERROR);
            }

        }
    }

    delete []szTempBuf;
    return S_OK;
}

/******************************************************************************
Function Name  :  vEnableItem
Input(s)       :  DWORD dwID - ID of Item
                  BOOL& bEnable - Enable Flag
Output         :  void
Functionality  :  Enables or Disables the entity execution according to bEnable
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS038
******************************************************************************/
void CTSExecutorChildFrame::vEnableItem(DWORD dwID, bool& bEnable)
{
    m_ouTSExecutor.EnableItem(dwID, bEnable);
}

/******************************************************************************
Function Name  :  bParseTestSetup
Input(s)       :  INT nIndex
Output         :  BOOL
Functionality  :  Loads The
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
bool CTSExecutorChildFrame::bParseTestSetup(INT nIndex)
{
    bool bResult = false;
    STestSetupInfo sTSInfo;
    if( m_ouTSExecutor.GetTestSetupInfo(nIndex, sTSInfo) == S_OK)
    {
        HTREEITEM hTSItem = m_odTreeView->InsertTreeItem(m_hParentTreeItem, sTSInfo.m_omstrName, nullptr, def_INDEX_TESTSETUPIMAGE,
                            def_INDEX_TESTSETUPIMAGE, sTSInfo.m_dwID);
        HTREEITEM hRoot = m_odTreeView->GetTreeCtrl().GetRootItem();

        m_odTreeView->GetTreeCtrl().SetCheck(hRoot, true);
        m_odTreeView->GetTreeCtrl().SetCheck(hTSItem, sTSInfo.m_bEnable);

        for(INT i = 0; i < sTSInfo.m_nTCCount; i++)
        {
            CBaseEntityTA* pouTCData;
            CTestCaseData ouTCData;
            if(m_ouTSExecutor.GetTestCaseInfo(sTSInfo.m_dwID, i, &pouTCData) == S_OK)
            {
                pouTCData->GetEntityData(TEST_CASE, &ouTCData);
                HTREEITEM hTCItem =  m_odTreeView->InsertTreeItem(hTSItem, ouTCData.m_omTitle, nullptr, def_INDEX_TESTCASEIMAGE,
                                     def_INDEX_TESTCASEIMAGE, pouTCData->GetID());
                m_odTreeView->GetTreeCtrl().SetCheck(hTCItem, pouTCData->bGetEnableStatus());
            }
        }
        bResult = true;
    }
    return bResult;
}

/******************************************************************************
Function Name  :  OnDeleteTestSetup
Input(s)       :  WPARAM wparam - Id of Test Setup
                  LPARAM lparam
Output         :  HRESULT
Functionality  :  Delete The test Setup
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS037
******************************************************************************/
HRESULT CTSExecutorChildFrame::OnDeleteTestSetup(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
    return m_ouTSExecutor.DeleteTestSetup((DWORD)wparam);
}

/******************************************************************************
Function Name  :  OnUpdate
Input(s)       :
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  28/04/2011
Modifications  :
Code Tag       :  CS046
******************************************************************************/
HRESULT CTSExecutorChildFrame::OnUpdate(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
    HRESULT hResult = m_ouTSExecutor.Update((DWORD)wparam);
    if(hResult == S_OK)
    {
        vUpdateTreeView();
    }
    return hResult;
}
/******************************************************************************
Function Name  :  OnExecute
Input(s)       :  WPARAM wparam, LPARAM lparam
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorChildFrame::OnExecute(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
    UNREFERENCED_PARAMETER(wparam);
    m_odTreeView->EnableWindow(FALSE);
    m_ouTSExecutor.SetResultDisplayWnd(&m_odResultView->GetListCtrl());
    m_ouTSExecutor.vSetVersionInfo(m_strVersionInfo);
    SetEvent(m_ouExecutionThread.m_hActionEvent);
    return S_OK;
}
/******************************************************************************
Function Name  :  OnExecutionOver
Input(s)       :  WPARAM wparam, LPARAM lparam
Output         :  HRESULT
Functionality  :
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
HRESULT CTSExecutorChildFrame::OnExecutionOver(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
    UNREFERENCED_PARAMETER(wparam);
    m_odTreeView->EnableWindow(TRUE);
    ResetEvent(m_ouExecutionThread.m_hActionEvent);
    return S_OK;
}

/******************************************************************************
Function Name  :  GetConfigurationData
Input(s)       :  BYTE* pSrcBuffer, UINT unBuffSize
Output         :  HRESULT
Functionality  :  Loads the configuration data
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS039
******************************************************************************/
HRESULT CTSExecutorChildFrame::GetConfigurationData(BYTE*& pDesBuffer, UINT& unBuffSize)
{
    //TestSuite Buffer
    UINT unTSBuffSize = 0;
    BYTE* pTSBuffer;
    m_ouTSExecutor.GetConfigurationData(pTSBuffer, unTSBuffSize);

    //Window position
    WINDOWPLACEMENT wndPlacement;
    GetWindowPlacement(&wndPlacement);

    //TreeView Window Position
    INT nCxCur, nCxMin;
    m_omSplitterWnd.GetColumnInfo(0, nCxCur, nCxMin);

    //TestSuite Buffer + Window position
    unBuffSize = sizeof(WINDOWPLACEMENT) + unTSBuffSize + (2*sizeof(INT));
    pDesBuffer = new BYTE[unBuffSize];
    BYTE* pTemp = pDesBuffer;
    COPY_DATA(pTemp, &wndPlacement, sizeof(WINDOWPLACEMENT));
    COPY_DATA(pTemp, &nCxCur, sizeof(INT));
    COPY_DATA(pTemp, &nCxMin, sizeof(INT));

    COPY_DATA(pTemp, pTSBuffer, unTSBuffSize);

    //Delete The Temporary Info
    delete []pTSBuffer;
    return S_OK;
}

bool CTSExecutorChildFrame::GetConfigurationData(xmlNodePtr& pxmlNodePtr)
{
    WINDOWPLACEMENT wndPlacement;
    GetWindowPlacement(&wndPlacement);

    const char* omcVarChar ;
    pxmlNodePtr = xmlNewNode(nullptr, BAD_CAST DEF_TS_EXECUTOR);
    m_ouTSExecutor.GetConfigurationData(pxmlNodePtr);

    //Window position


    xmlNodePtr pNodeWndPos = xmlNewNode(nullptr, BAD_CAST DEF_WND_POS);
    xmlAddChild(pxmlNodePtr, pNodeWndPos);

    if(IsWindowVisible() == FALSE)
    {
        wndPlacement.showCmd = SW_HIDE;
    }

    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWndPos,wndPlacement);

    //splitter position-------------------------
    INT nCxCur, nCxMin;
    m_omSplitterWnd.GetColumnInfo(0, nCxCur, nCxMin);

    xmlNodePtr pNodeSplitterWnd = xmlNewNode(nullptr, BAD_CAST DEF_SPLITTER_WINDOW);
    xmlAddChild(pxmlNodePtr, pNodeSplitterWnd);

    //<CxIdeal />
    CString  csCxIdeal;
    csCxIdeal.Format("%d", nCxCur );
    omcVarChar = csCxIdeal;
    xmlNodePtr pCxIdeal = xmlNewChild(pNodeSplitterWnd, nullptr, BAD_CAST DEF_CX_IDEAL, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSplitterWnd, pCxIdeal);

    // <CxMin />
    CString  csCxMin;
    csCxMin.Format("%d",nCxMin );
    omcVarChar = csCxMin;
    xmlNodePtr pcsCxMin = xmlNewChild(pNodeSplitterWnd, nullptr, BAD_CAST DEF_CX_MIN, BAD_CAST omcVarChar);
    xmlAddChild(pNodeSplitterWnd, pcsCxMin);
    return true;
}

/******************************************************************************
Function Name  :  SetConfigurationData
Input(s)       :  BYTE* pSrcBuffer, UINT unBuffSize
Output         :  HRESULT
Functionality  :  Returns the configuration data
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS040
******************************************************************************/
HRESULT CTSExecutorChildFrame::SetConfigurationData(BYTE* pSrcBuffer, UINT unBuffSize)
{
    if(unBuffSize != 0)
    {
        vInitialise();
        WINDOWPLACEMENT wndPlacement;
        COPY_DATA_2(&wndPlacement, pSrcBuffer,sizeof(WINDOWPLACEMENT));
        SetWindowPlacement(&wndPlacement);

        INT nCxCur, nCxMin;
        COPY_DATA_2(&nCxCur, pSrcBuffer, sizeof(INT));
        COPY_DATA_2(&nCxMin, pSrcBuffer, sizeof(INT));
        m_omSplitterWnd.SetColumnInfo(0, nCxCur, nCxMin);
        m_omSplitterWnd.RecalcLayout();

        unBuffSize = unBuffSize - sizeof(WINDOWPLACEMENT) - (2*sizeof(INT));
        m_ouTSExecutor.SetConfigurationData(pSrcBuffer, unBuffSize);

        m_odTreeView->GetTreeCtrl().SetCheck(m_hParentTreeItem, m_ouTSExecutor.m_bTestSuiteStatus);

        //Parse The Tree Control;
        INT nCount;
        m_ouTSExecutor.GetTestSetupCount(nCount);
        for(int i = 0; i < nCount; i++)
        {
            bParseTestSetup(i);
        }
    }
    //New File or when Initialising
    else
    {
        vInitialise();
        WINDOWPLACEMENT omTempWnd = m_sTSDefPlacement;
        omTempWnd.showCmd = SW_HIDE;
        SetWindowPlacement(&omTempWnd);
    }
    return S_OK;
}

HRESULT CTSExecutorChildFrame::SetConfigurationData(xmlNodePtr pXmlNode)
{
    int nRetValue = S_OK;
    //Test Suite Window Position
    WINDOWPLACEMENT wndPlacement;
    wndPlacement = m_sTSDefPlacement;

    if( nullptr != pXmlNode )
    {
        vInitialise();
        xmlNodePtr pTempNode = nullptr;
        xmlXPathObjectPtr pObjectPtr = nullptr;

        pObjectPtr = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*)"Window_Position");
        if( nullptr != pObjectPtr)
        {
            pTempNode = pObjectPtr->nodesetval->nodeTab[0];
            if( S_FALSE == xmlUtils::ParseWindowsPlacement(pTempNode, wndPlacement) )
            {
                wndPlacement = m_sTSDefPlacement;
            }
            xmlXPathFreeObject(pObjectPtr);
            pObjectPtr = nullptr;
        }

        //Splitter Position
        pObjectPtr = xmlUtils::pGetChildNodes(pXmlNode, (xmlChar*)"Splitter_Window");
        int nCxCur = 0;
        int nCxMin = 0;
        if( nullptr != pObjectPtr)
        {
            pTempNode = pObjectPtr->nodesetval->nodeTab[0];
            if ( S_OK == xmlUtils::ParseSplitterWindow(pTempNode, nCxCur, nCxMin) )
            {
                m_omSplitterWnd.SetColumnInfo(0, nCxCur, nCxMin);
                //Arun
                m_omSplitterWnd.RecalcLayout();
            }
        }

        //Test Suite Executor ConfigData
        m_ouTSExecutor.SetConfigurationData(pXmlNode);

        m_odTreeView->GetTreeCtrl().SetCheck(m_hParentTreeItem, m_ouTSExecutor.m_bTestSuiteStatus);

        CString omStrName;
        m_ouTSExecutor.GetTestsuiteName(omStrName);
        m_odTreeView->GetTreeCtrl().SetItemText(m_hParentTreeItem, omStrName);

        //Parse The Tree Control;
        INT nCount;
        m_ouTSExecutor.GetTestSetupCount(nCount);
        for(int i = 0; i < nCount; i++)
        {
            bParseTestSetup(i);
        }

    }
    if( nullptr == pXmlNode )
    {
        vInitialise();
    }
    SetWindowPlacement(&wndPlacement);

    return S_OK;
}

/******************************************************************************
Function Name  :  vInitialise
Input(s)       :  -
Output         :  void
Functionality  :  Initialises the Executor
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :  CS043
******************************************************************************/
void CTSExecutorChildFrame::vInitialise(void)
{
    m_odTreeView->GetTreeCtrl().DeleteAllItems();
    m_ouTSExecutor.RemoveAllItems();

    CString omstrName;
    m_ouTSExecutor.GetTestsuiteName(omstrName);

    m_hParentTreeItem = m_odTreeView->InsertTreeItem(nullptr, omstrName, nullptr,def_INDEX_TESTSUITEIMAGE,
                        def_INDEX_TESTSUITEIMAGE, def_ID_TESTSUITE);
    m_ouTSExecutor.m_bTestSuiteStatus = FALSE;
}

/******************************************************************************
Function Name  :  vSetBusStatus
Input(s)       :  BOOL bConnected
Output         :  void
Functionality  :  Sets The Bus Status
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSExecutorChildFrame::vSetBusStatus(bool bConnected)
{
    m_bConnected = bConnected;
}

/******************************************************************************
Function Name  :  vSetBUSMASTERVersionInfo
Input(s)       :  CString strVersion
Output         :  void
Functionality  :  Sets The Bus Status
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSExecutorChildFrame::vSetBUSMASTERVersionInfo(CString strVersion)
{
    m_strVersionInfo = strVersion;
}

/******************************************************************************
Function Name  :  bGetBusStatus
Input(s)       :  -
Output         :  BOOL
Functionality  :  Return The Bus Status
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  07/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
bool CTSExecutorChildFrame::bGetBusStatus(void)
{
    bool bEnable = true;
    INT nCount;
    m_ouTSExecutor.GetTestSetupCount(nCount);
    if( (m_bConnected == false) || (nCount <=0))
    {
        bEnable = false;
    }
    return bEnable;
}

/******************************************************************************
Function Name  :  unRepisitonEntry
Input(s)       :
Output         :  UINT
Functionality  :
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  11/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
UINT CTSExecutorChildFrame::unRepisitonEntry(DWORD dwRepositionItemID, DWORD dwInsertAfterItemID)
{
    HRESULT hResult = m_ouTSExecutor.RepositionTestSetup(dwRepositionItemID, dwInsertAfterItemID);
    if(hResult == S_OK)
    {
        vUpdateTreeView();
    }
    return hResult;
}

/******************************************************************************
Function Name  :  vUpdateTreeView
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  11/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSExecutorChildFrame::vUpdateTreeView(void)
{

    INT nCount;
    CString omStrTestSuiteName;
    m_ouTSExecutor.GetTestsuiteName(omStrTestSuiteName);
    BOOL bCheck = m_odTreeView->bIsItemChecked(m_hParentTreeItem);
    m_odTreeView->GetTreeCtrl().DeleteAllItems();
    m_hParentTreeItem = m_odTreeView->InsertTreeItem(nullptr, omStrTestSuiteName, nullptr, 0, 0, def_ID_TESTSUITE);
    m_ouTSExecutor.GetTestSetupCount(nCount);
    for(int i = 0; i < nCount; i++)
    {
        bParseTestSetup(i);
    }
    m_odTreeView->GetTreeCtrl().Expand(m_hParentTreeItem, TVE_EXPAND);

    m_odTreeView->GetTreeCtrl().SetCheck(m_hParentTreeItem, bCheck);
}

/******************************************************************************
Function Name  :  OnClose
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CTSExecutorChildFrame
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  28/04/2011
Modifications  :
Code Tag       :
******************************************************************************/
void CTSExecutorChildFrame::OnClose()
{
    WINDOWPLACEMENT wndPlcmnt;

    GetWindowPlacement(&wndPlcmnt);

    wndPlcmnt.showCmd = SW_HIDE;

    SetWindowPlacement(&wndPlcmnt);
    CWnd* pActivateWnd =   GetNextWindow();
    pActivateWnd->SetForegroundWindow();
    pActivateWnd->SetFocus();
    ShowWindow(SW_HIDE);
}
