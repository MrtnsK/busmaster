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
 * \file      ReplayManager.cpp
 * \brief     Implementation file for CReplayManager class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CReplayManager class
 */

#include "Replay_stdafx.h"             // For standard includes
#include "Replay_resource.h"
#include "ReplayFile.h"         // For Replay File class declaration
#include "MsgReplayWnd.h"       // For message Window class declaration
#include "ReplayManager.h"      // For Replay Manager class declaration
#include "Utility/RadixEdit.h"         // For Radix edit control used in Cfg dialog
#include "ReplayFileConfigDlg.h"// For Replay File Configuration dialog
#include "ReplayProcess.h"      // For Replay Process class declaration
#include "DataTypes/BaseAppServices.h"
#include "Filter/Filter_extern.h"

// Flag to indicate object creation
BOOL CReplayManager::m_bCreated = FALSE;
// One instance of Replay Manager
// This init statement is placed in CANMonitor.cpp so that this object will be
// created before CCANMonitorApp object. CCANMonitorApp class uses this class
// So this should be created befor CCANMonitorApp object.
CReplayManager CReplayManager::m_ouReplayManager;
static CBaseAppServices* g_pouITracePtr = nullptr;


/*******************************************************************************
  Function Name  : ~CReplayManager
  Description    : Standard Destructor
  Member of      : CReplayManager
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayManager::CReplayManager()
{
    // Indicate object creation
    m_bCreated = TRUE;
    // Init active replay window
    m_pomActiveWindow = nullptr;
    // Connection status
    m_bConnection = FALSE;
    //Window placement invalid value
    m_sWndPlacement.length = 0;
    // Initialize Manager
    vInitReplayManager();

}

/*******************************************************************************
  Function Name  : ~CReplayManager
  Description    : Standard Destructor
  Member of      : CReplayManager
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayManager::~CReplayManager()
{
}

/*******************************************************************************
  Function Name  : ouGetLogManager
  Input(s)       : -
  Output         : CReplayManager - Reference to singleton object
  Functionality  : To get signleton replay manager object
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
CReplayManager& CReplayManager::ouGetReplayManager()
{
    // Return the singleton instance
    return m_ouReplayManager;
}

/*******************************************************************************
  Function Name  : vCopyReplayManager
  Input(s)       : ouDest - Reference to destination object
                   ouSrc - Reference to source object
  Output         : -
  Functionality  : Copies replay manager object
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vCopyReplayManager( CReplayManager& ouDest,
        const CReplayManager& ouSrc ) const
{
    // Clear destination list
    ouDest.m_omReplayFiles.RemoveAll();
    // Copy Replay File List
    int nSize = (int)ouSrc.m_omReplayFiles.GetSize();
    // Iterate through the list and copy
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        ouDest.m_omReplayFiles.Add( ouSrc.m_omReplayFiles[ nIndex ] );
    }
}

/*******************************************************************************
  Function Name  : dShowReplayConfigurationDlg
  Input(s)       : -
  Output         : DWORD - Returns the dialog DOModal return value
  Functionality  : To show replay configuration dialog
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
DWORD CReplayManager::dShowReplayConfigurationDlg(const SFILTERAPPLIED_CAN* psFilterConfigured)
{
    DWORD dwResult;
    // Copy Replay Manager
    CReplayManager ouTemp;
    vCopyReplayManager( ouTemp, *this );
    // Show configuration dialog
    CReplayFileConfigDlg omDlg( ouTemp,  psFilterConfigured);

    dwResult = (DWORD)omDlg.DoModal();
    if( dwResult == IDOK )
    {
        // Copy the user selection
        vCopyReplayManager( *this, ouTemp );
    }
    // Return the result
    return dwResult;
}

/*******************************************************************************
  Function Name  : vInitReplayManager
  Input(s)       : -
  Output         : -
  Functionality  : To initialise replay manager with the default values
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vInitReplayManager()
{
    // Clear all entries in the list
    m_omReplayFiles.RemoveAll();
    // Init active window
    m_pomActiveWindow = nullptr;
    // Remove all entries
    m_omReplayWindowArray.RemoveAll();
    // Clear Replay Process Array
    m_omReplayProcess.RemoveAll();
}

/*******************************************************************************
  Function Name  : vShowInteractiveReplayWindows
  Input(s)       : -
  Output         : -
  Functionality  : To create and show all interactive replay files. Replay
                   window will be created and replay file will be loaded. The
                   new window will be added in to the window list
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vShowInteractiveReplayWindows()
{
    // Get the number of replay files in the list
    int nSize = (int)m_omReplayFiles.GetSize();
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        const CReplayFile& omFile = m_omReplayFiles.ElementAt( nIndex );
        // If it is enabled and interactive
        if( omFile.m_bEnabled == TRUE &&
                omFile.m_bInteractive == TRUE )
        {
            // Create, load and show the window
            CMsgReplayWnd* pWnd =
                new CMsgReplayWnd( omFile, m_sWndPlacement);
            if( pWnd != nullptr )
            {
                m_omReplayWindowArray.Add( pWnd );
                pWnd->bCreateReplayWindow();
                pWnd->bOpenReplayFile(TRUE);
            }
        }
    }
}

/*******************************************************************************
  Function Name  : vStartNonInteractiveReplays
  Input(s)       : -
  Output         : -
  Functionality  : To create and start non interactive replay threads.
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vStartNonInteractiveReplays()
{
    // Get the replay list size
    int nSize = (int)m_omReplayFiles.GetSize();
    //int nIndex;     //nIndex declared outside loop
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        const CReplayFile& omFile = m_omReplayFiles.ElementAt( nIndex );
        // If it is enabled non interactive replay
        if( omFile.m_bEnabled == TRUE &&
                omFile.m_bInteractive == FALSE )
        {
            // Create replay process
            CReplayProcess* pNewProcess = new CReplayProcess( omFile );
            if( pNewProcess != nullptr )
            {
                // Load the replay file
                BOOL bResult = pNewProcess->bOpenReplayFile(FALSE);
                // if loading is success
                if( bResult == TRUE )
                {
                    // check for empty replay file

                    /*if( pNewProcess->dwGetNoOfMsgsInLog()== 0 )
                    {
                        // Show the message in Trace Window
                        CString omStrErr;
                        omStrErr.Format( defSTR_REPLAY_ERROR,
                                         pNewProcess->m_ouReplayFile.m_omStrFileName, // File
                                         defSTR_REPLAY_FILE_EMPTY );
                        vSendToTrace(omStrErr.GetBuffer(MAX_PATH));
                        //gbSendStrToTrace( (char *)omStrErr.operator LPCTSTR());
                        bResult = FALSE;
                    }
                    else
                    {*/
                    // Add the replay process to the list
                    m_omReplayProcess.Add( pNewProcess );
                    //}
                }
                else
                {
                    // Replay file parsing failed
                    // Show Error message
                    CString omStrErr;
                    omStrErr.Format(defSTR_REPLAY_ERROR,
                                    omFile.m_omStrFileName, // File Name
                                    pNewProcess->m_omStrError );    // Error Message
                    vSendToTrace(omStrErr.GetBuffer(MAX_PATH));
                    //gbSendStrToTrace( (char *)omStrErr.operator LPCTSTR() );
                }
                if( bResult == FALSE )
                {
                    // Delete the memory
                    delete pNewProcess;
                }
            }

        }
    }
    // Start all replay threads
    nSize = (int)m_omReplayProcess.GetSize();
    for(int i = 0; i < nSize; i++ )
    {
        EnterCriticalSection(&m_omReplayProcess[i]->m_omCritSecFilter);
        m_omReplayProcess[i]->bStartNIReplay();
        LeaveCriticalSection(&m_omReplayProcess[i]->m_omCritSecFilter);
    }
}

/*******************************************************************************
  Function Name  : vSetActiveReplayWindow
  Input(s)       : pomWindow - Active replay window
  Output         : -
  Functionality  : To set the active replay window. To invalidate active window
                   this will be nullptr
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vSetActiveReplayWindow(CMsgReplayWnd* pomWindow)
{
    m_pomActiveWindow = pomWindow;
}

/*******************************************************************************
  Function Name  : vCmdStep
  Input(s)       : -
  Output         : -
  Functionality  : Replay Step Command handler
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vCmdStep()
{
    // Pass it to the active window
    if( m_pomActiveWindow != nullptr )
    {
        m_pomActiveWindow->vCmdStep();
    }
}

/*******************************************************************************
  Function Name  : vCmdSkip
  Input(s)       : -
  Output         : -
  Functionality  : Replay Skip Command handler
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vCmdSkip()
{
    // Pass it to the active window
    if( m_pomActiveWindow != nullptr )
    {
        m_pomActiveWindow->vCmdSkip();
    }
}

/*******************************************************************************
  Function Name  : vCmdSkip
  Input(s)       : -
  Output         : -
  Functionality  : Replay Skip Command handler
  Member of      : CReplayManager
  Author(s)      : Ashwin R Uchil
  Date Created   : 3.1.2013
  Modifications  :
*******************************************************************************/
void CReplayManager::vEnableFilters(BOOL bEnabled)
{
    UINT nCount = (UINT)m_omReplayProcess.GetSize();

    for(int nCnt = 0; nCnt < nCount; nCnt++)
    {
        CReplayProcess* pReplayProcess = nullptr;
        pReplayProcess = m_omReplayProcess.GetAt(nCnt);
        if(pReplayProcess == nullptr)
        {
            return;
        }
        EnterCriticalSection(&pReplayProcess->m_omCritSecFilter);
        pReplayProcess->m_ouReplayFile.m_sFilterApplied.m_bEnabled = bEnabled;
        LeaveCriticalSection(&pReplayProcess->m_omCritSecFilter);
    }

    nCount = (UINT)m_omReplayWindowArray.GetSize();

    for(int nCnt = 0; nCnt < nCount; nCnt++)
    {
        CMsgReplayWnd* pReplayWnd = nullptr;
        pReplayWnd = m_omReplayWindowArray.GetAt(nCnt);
        if(pReplayWnd == nullptr)
        {
            return;
        }
        EnterCriticalSection(&pReplayWnd->m_ouReplayDetails.m_omCritSecFilter);
        pReplayWnd->m_ouReplayDetails.m_ouReplayFile.m_sFilterApplied.m_bEnabled = bEnabled;
        LeaveCriticalSection(&pReplayWnd->m_ouReplayDetails.m_omCritSecFilter);
    }
}

/*******************************************************************************
  Function Name  : vCmdGo
  Input(s)       : -
  Output         : -
  Functionality  : Replay Go Command handler
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vCmdGo()
{
    // Pass it to the active window
    if( m_pomActiveWindow != nullptr )
    {
        m_pomActiveWindow->vCmdGo();
    }
}

/*******************************************************************************
  Function Name  : vCmdStop
  Input(s)       : -
  Output         : -
  Functionality  : Replay Stop Command handler
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vCmdStop()
{
    // Pass it to the active window
    if( m_pomActiveWindow != nullptr )
    {
        m_pomActiveWindow->vCmdStop();
    }
}

/*******************************************************************************
  Function Name  : bGetUIStateCmdStep
  Input(s)       : -
  Output         : BOOL - TRUE to enable and FALSE to disable
  Functionality  : To get the enable status of Step toolbar button
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
BOOL CReplayManager::bGetUIStateCmdStep()
{
    BOOL bEnable = FALSE;
    // If tool is connected
    if( m_bConnection == TRUE && m_pomActiveWindow != nullptr )
    {
        bEnable = m_pomActiveWindow->bGetUIStateCmdStep();
    }
    return bEnable;
}

/*******************************************************************************
  Function Name  : bGetUIStateCmdSkip
  Input(s)       : -
  Output         : BOOL - TRUE to enable and FALSE to disable
  Functionality  : To get the enable status of Skip toolbar button
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
BOOL CReplayManager::bGetUIStateCmdSkip()
{
    BOOL bEnable = FALSE;
    if( m_bConnection == TRUE && m_pomActiveWindow != nullptr )
    {
        bEnable = m_pomActiveWindow->bGetUIStateCmdSkip();
    }
    return bEnable;
}

/*******************************************************************************
  Function Name  : bGetUIStateCmdGo
  Input(s)       : -
  Output         : BOOL - TRUE to enable and FALSE to disable
  Functionality  : To get the enable status of Go toolbar button
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
BOOL CReplayManager::bGetUIStateCmdGo()
{
    BOOL bEnable = FALSE;
    if( m_bConnection == TRUE && m_pomActiveWindow != nullptr )
    {
        bEnable = m_pomActiveWindow->bGetUIStateCmdGo();
    }
    return bEnable;
}

/*******************************************************************************
  Function Name  : bGetUIStateCmdStop
  Input(s)       : -
  Output         : BOOL - TRUE to enable and FALSE to disable
  Functionality  : To get the enable status of Stop toolbar button
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
BOOL CReplayManager::bGetUIStateCmdStop()
{
    BOOL bEnable = FALSE;
    if( m_bConnection == TRUE && m_pomActiveWindow != nullptr )
    {
        bEnable = m_pomActiveWindow->bGetUIStateCmdStop();
    }
    return bEnable;
}

/*******************************************************************************
  Function Name  : bRemovePointerFromList
  Input(s)       : pWnd - Pointer that is to be removed from the list
  Output         : BOOL - TRUE if found, FALSE otherwise
  Functionality  : To remove the specified replay window pointer from the replay
                   replay window pointer list
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
BOOL CReplayManager::bRemovePointerFromList(CWnd* pWnd)
{
    BOOL bFound = FALSE;
    if( pWnd != nullptr )
    {
        int nSize = (int)m_omReplayWindowArray.GetSize();
        for( int nIndex = 0; bFound == FALSE && nIndex < nSize; nIndex++ )
        {
            if( m_omReplayWindowArray[ nIndex ] == pWnd )
            {
                m_omReplayWindowArray.RemoveAt( nIndex );
                bFound = TRUE;
            }
        }
    }
    return bFound;
}

/*******************************************************************************
  Function Name  : vSetThreadStopSignal
  Input(s)       : -
  Output         : -
  Functionality  : To set the thread stop signal to all replay process
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vSetThreadStopSignal()
{
    // Interactive replays
    int nSize = (int)m_omReplayWindowArray.GetSize();
    int nIndex;
    for( nIndex = 0; nIndex < nSize; nIndex++ )
    {
        m_omReplayWindowArray.ElementAt( nIndex )->bSetThreadStopSignal();
    }
    // Non Interactive replays
    nSize = (int)m_omReplayProcess.GetSize();
    for( nIndex = 0; nIndex < nSize; nIndex++ ) //nIndex declared outside
    {
        m_omReplayProcess.ElementAt( nIndex )->bSetThreadStopSignal();
    }

}

/*******************************************************************************
  Function Name  : vStopReplayThread
  Input(s)       : -
  Output         : -
  Functionality  : To kill all replay process and to clear data structures
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vStopReplayThread()
{
    // Interactive dispalys
    int nSize = (int)m_omReplayWindowArray.GetSize();
    int nIndex;     //nIndex declared outside loop
    for( nIndex = 0; nIndex < nSize; nIndex++ )
    {
        // Use index 0. Because Destroy Window will remove the pointer
        // from the list. So the size of the list will shrink. Use 0
        // to refer the next elements in the list
        m_omReplayWindowArray.ElementAt( 0 )->bStopReplayThread();
        // Destroy the window. This will remove itself from the list
        m_omReplayWindowArray.ElementAt( 0 )->DestroyWindow();
    }
    // Remove elements from the list if any
    m_omReplayWindowArray.RemoveAll();

    // Non Interactive threads
    nSize = (int)m_omReplayProcess.GetSize();
    for( nIndex = 0; nIndex < nSize; nIndex++ )
    {
        m_omReplayProcess.ElementAt( nIndex )->bStopReplayThread();
    }
    // Clear NI Thread Pool
    nSize = (int)m_omReplayProcess.GetSize();
    // Delete memory allocated for NI threads
    for( nIndex = 0; nIndex < nSize; nIndex++ )
    {
        delete m_omReplayProcess.ElementAt( nIndex );
    }
    // Clear NI thread pool
    m_omReplayProcess.RemoveAll();
}

void CReplayManager::vSetConfiguredFilter(const void* pvFilterConfigured)
{
    SFILTERAPPLIED_CAN* psFilterConfigured = (SFILTERAPPLIED_CAN*)pvFilterConfigured;
    UINT unFileCount = (UINT)m_omReplayFiles.GetSize();
    for (UINT i = 0; i < unFileCount; i++)
    {
        CReplayFile& ouRepFile = m_omReplayFiles.ElementAt(i);
        Filter_ReUpdateAppliedFilter(&(ouRepFile.m_sFilterApplied),
                                     psFilterConfigured, CAN);
    }
}

/*******************************************************************************
  Function Name  : vHandleConnectionStatusChange
  Input(s)       : bConnect - Connection status
  Output         : -
  Functionality  : To handle tool connection state change
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vHandleConnectionStatusChange(BOOL bConnect )
{
    // Update Local instance
    m_bConnection = bConnect;
    // Start/Stop Replay Process
    vStartStopReplay( m_bConnection );
}

/*******************************************************************************
  Function Name  : vStartStopReplay
  Input(s)       : bStart - Start/Stop value
  Output         : -
  Functionality  : To start or stop replay threads
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vStartStopReplay( BOOL bStart )
{
    if( bStart == TRUE )
    {
        // Start all non interactive threads
        vStartNonInteractiveReplays();
        // Start all interractive threads
        vShowInteractiveReplayWindows();
    }
    else
    {
        // Stop all interractive  and non interactive threads
        // Signal the thread to stop
        vSetThreadStopSignal();
        // Stop the thread
        vStopReplayThread();
    }
}

/*******************************************************************************
  Function Name  : vGetReplayFileNameList
  Input(s)       : omList - Reference to string array to copy repaly file list
  Output         : -
  Functionality  : Copies the list of replay file names.
  Member of      : CReplayManager
  Author(s)      : Raja N
  Date Created   : 16.7.2005
  Modifications  :
*******************************************************************************/
void CReplayManager::vGetReplayFileNameList(CStringArray& omList)
{
    omList.RemoveAll();
    // Populate the list
    int nSize = (int)m_omReplayFiles.GetSize();
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        CReplayFile& ouFile = m_omReplayFiles.ElementAt( nIndex );
        omList.Add( ouFile.m_omStrFileName );
    }
}

void CReplayManager::vGetReplayConfigData(BYTE*& pDesBuffer, int& nBuffSize)
{
    UINT unToatalDataSize = sizeof(BYTE);//configuration version;
    int nSize = (int)m_omReplayFiles.GetSize();
    //For storing nSize value
    unToatalDataSize += sizeof(nSize);
    CReplayFile ouFile;
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        ouFile = m_omReplayFiles.ElementAt( nIndex );
        unToatalDataSize += ouFile.unGetConfigSize();
    }
    unToatalDataSize += sizeof (WINDOWPLACEMENT);
    pDesBuffer = new BYTE[unToatalDataSize];
    nBuffSize = unToatalDataSize;
    BYTE* pbyTemp = pDesBuffer;
    //BYTE byVersion = REPLAY_MANAGER_INITIAL_VERSION;
    BYTE byVersion = REPLAY_MANAGER_REPLAY_MSG_ADD;
    COPY_DATA(pbyTemp, &byVersion, sizeof(BYTE));

    COPY_DATA(pbyTemp, &nSize, sizeof(nSize));

    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        ouFile = m_omReplayFiles.ElementAt( nIndex );
        pbyTemp = ouFile.pbySaveConfig(pbyTemp);
    }
    if (m_pomActiveWindow != nullptr)
    {
        m_pomActiveWindow->GetWindowPlacement(&m_sWndPlacement);
    }
    COPY_DATA(pbyTemp, &m_sWndPlacement, sizeof (WINDOWPLACEMENT));
}

void CReplayManager::vGetReplayConfigData(xmlNodePtr pxmlNodePtr)
{
    int nSize = (int)m_omReplayFiles.GetSize();
    CReplayFile ouFile;
    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        xmlNodePtr pNodeReplay = xmlNewNode(nullptr, BAD_CAST DEF_REPLAY);
        xmlAddChild(pxmlNodePtr, pNodeReplay);
        ouFile = m_omReplayFiles.ElementAt( nIndex );
        ouFile.pbySaveConfig(pNodeReplay);
    }
}
void CReplayManager::vSetReplayConfigData(BYTE* pSrcBuffer, int /*nBuffSize*/)
{
    //Before loading another config initialize the display
    vStopReplayThread();
    vInitReplayManager();
    if (nullptr != pSrcBuffer)
    {
        //Now load new config
        BYTE byVersion = 0;
        COPY_DATA_2(&byVersion, pSrcBuffer, sizeof(BYTE));

        int nSize;
        COPY_DATA_2(&nSize, pSrcBuffer, sizeof(nSize));

        CReplayFile ouFile;
        for( int nIndex = 0; nIndex < nSize; nIndex++ )
        {
            pSrcBuffer = ouFile.pbyLoadConfig(pSrcBuffer, byVersion);
            m_omReplayFiles.Add(ouFile);
        }
        if (pSrcBuffer != nullptr)
        {
            COPY_DATA_2(&m_sWndPlacement, pSrcBuffer, sizeof(WINDOWPLACEMENT));
        }
    }
}
void CReplayManager::vSetReplayConfigData(xmlDocPtr pDoc)
{
    vStopReplayThread();
    vInitReplayManager();
    if (nullptr != pDoc)
    {
        //Now load new config
        xmlChar* pXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_Replay/Replay";
        xmlXPathObjectPtr pPathObject = xmlUtils::pGetNodes(pDoc, pXpath);
        xmlNodePtr pNode = nullptr;
        if( nullptr != pPathObject )
        {
            xmlNodeSetPtr pNodeSet = pPathObject->nodesetval;
            if( nullptr != pNodeSet)
            {
                INT nSize = pNodeSet->nodeNr;
                CReplayFile ouFile;
                for( int nIndex = 0; nIndex < nSize; nIndex++ )
                {
                    ouFile.nLoadXMLConfig(pNodeSet->nodeTab[nIndex]->children);
                    m_omReplayFiles.Add(ouFile);//Duplicate FileNames Allowed
                }
            }
        }
        //COPY_DATA_2(&m_sWndPlacement, pSrcBuffer, sizeof(WINDOWPLACEMENT));
    }
}
void CReplayManager::vAddReplayFile(CReplayFile& ouFile)
{
    int nCount = m_omReplayFiles.GetSize();
    BOOL bDuplicate = FALSE;
    if(nCount == 0 )
    {
        m_omReplayFiles.Add(ouFile);
    }
    else
    {
        for(int i = 0; i < nCount; i++)
        {
            CReplayFile& ouReplayFile = m_omReplayFiles.GetAt(i);
            CString omStrFileName1, omStrFileName2;
            ouReplayFile.GetFileName(omStrFileName1);
            ouFile.GetFileName(omStrFileName2);
            if(omStrFileName1 == omStrFileName2)
            {
                bDuplicate = TRUE;
            }
        }
        if(bDuplicate == FALSE)
        {
            m_omReplayFiles.Add(ouFile);
        }
    }
}

//BOOL CReplayManager::bIsReplayConfigChanged()
//{
//    BOOL bReturn = TRUE;
/**************Get the old buffer pointer and then proceed *********/
//    BYTE* pOldConfigBuff = nullptr;
/**************Get the old buffer pointer and then proceed *********/
// Populate the list
//    int nRplayCnt = 0;
//    memcpy(&nRplayCnt, pOldConfigBuff, sizeof(nRplayCnt));
//    int nSize = (int)m_omReplayFiles.GetSize();
//    if (nSize == nRplayCnt)
//    {
//        for( int nIndex = 0; nIndex < nSize; nIndex++ )
//        {
//            CReplayFile& ouFile = m_omReplayFiles.ElementAt( nIndex );
//            bReturn = ouFile.bisConfigChanged(pOldConfigBuff);
//        }
//    }
//    else
//    {
//        bReturn = FALSE;
//
//    return bReturn;
//}

void CReplayManager::vSetTraceObjPtr( PVOID pvObj)
{
    g_pouITracePtr = (CBaseAppServices*) pvObj;
}

void CReplayManager::vSendToTrace(char* pcString)
{
    if (nullptr != g_pouITracePtr)
    {
        g_pouITracePtr->bWriteToTrace(pcString);
    }
}