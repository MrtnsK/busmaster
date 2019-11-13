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
 * \file      MsgSignalDBWnd.cpp
 * \brief     This file contain definition of all function of
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "stdafx.h"             // Contains standard include header files.
#include "BUSMASTER.h"        // App class header file
#include "MsgSignalDBWnd.h"     // Class defintion included here
#include "MainFrm.h"            // Pointer to this class is defined here
#include "MsgSgTreeView.h"      // Forms the left pane
//#include "MsgSgDetView.h"       // Forms the right pane
#include "Flags.h"              // DBOPEN flag to be set/reset is defined here
#include "common.h"
extern CCANMonitorApp theApp;

SDBPARAMS CMsgSgTreeView::sm_sDbParams = sg_asDbParams[CAN];
SDBPARAMS CMsgSgDetView::sm_sDbParams  = sg_asDbParams[CAN];
/////////////////////////////////////////////////////////////////////////////
// CMsgSignalDBWnd

IMPLEMENT_DYNCREATE(CMsgSignalDBWnd, CMDIChildWnd)
/******************************************************************************/
/*  Function Name    :  CMsgSignalDBWnd                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CMsgSignalDBWnd                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :
/******************************************************************************/
CMsgSignalDBWnd::CMsgSignalDBWnd()
{
    m_sDbParams = sg_asDbParams[CAN];
    m_bSplitWndCreated = FALSE; // Splitter not created yet!
}
CMsgSignalDBWnd::CMsgSignalDBWnd(const SDBPARAMS& sDbParams)
{
    m_sDbParams = sDbParams;
    m_bSplitWndCreated = FALSE; // Splitter not created yet!
}
/******************************************************************************/
/*  Function Name    :  ~CMsgSignalDBWnd                                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Destructor
/*  Member of        :  CMsgSignalDBWnd                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

CMsgSignalDBWnd::~CMsgSignalDBWnd()
{
}


BEGIN_MESSAGE_MAP(CMsgSignalDBWnd, CMDIChildWnd)
    //{{AFX_MSG_MAP(CMsgSignalDBWnd)
    ON_WM_CLOSE()
    ON_MESSAGE(WM_SAVE_DBJ1939, OnSaveDBJ1939)
    ON_MESSAGE(WM_UPDATE_DATABASEEDITOR, UpdateSignalDetails)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgSignalDBWnd message handlers
/******************************************************************************/
/*  Function Name    :  PreCreateWindow                                   */
/*                                                                            */
/*  Input(s)         :  CREATESTRUCT& cs                                      */
/*  Output           :                                                        */
/*  Functionality    :  Window style specified here
/*  Member of        :  CMsgSignalDBWnd                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

BOOL CMsgSignalDBWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_OVERLAPPEDWINDOW;

    return CMDIChildWnd::PreCreateWindow(cs);
}

/******************************************************************************/
/*  Function Name    :  OnCreateClient                                        */
/*                                                                            */
/*  Input(s)         :  LPCREATESTRUCT lpcs, CCreateContext* pContext         */
/*  Output           :  BOOL, True or False                                   */
/*  Functionality    :  This function will create a vertical splitter
                        and associates 2 views for each pane.
/*  Member of        :  CMsgSignalDBWnd                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :
/*  Modification     :  Amitesh Bharti on 22.07.2004                          */
/*                      Modified to set the focus to the first database       */
/*                      message entry in the tree view                        */
/*  Modifications    :  Raja N on 20.07.2005, Added code to set window icon   */
/******************************************************************************/

BOOL CMsgSignalDBWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
                                     CCreateContext* pContext)
{
    // Set Icon for DB Window
    SetIcon( theApp.LoadIcon( IDI_ICO_DB_EDITOR ), TRUE );

    // Create Static Splitter Window with 1 ROW and 2 COLUMNS
    m_bSplitWndCreated  =
        m_omSplitterWnd.CreateStatic(   this,                   // Parent Frame Window
                                        SPLT_ONE_ROWS,          // #Rows
                                        SPLT_ONE_COLS,          // #Columns
                                        WS_CHILD    |
                                        WS_VISIBLE |
                                        WS_BORDER,              // Window Style
                                        AFX_IDW_PANE_FIRST);    // Splitter NOT Nested


    // Get size of Frame wnd
    CSize om_Size(0,0);

    vCalculateSplitterPosition(om_Size);
    if ( TRUE == m_bSplitWndCreated )
    {
        // Create the Right Pane for static splitter window
        CMsgSgDetView::sm_sDbParams = m_sDbParams;
        m_bSplitWndCreated  =
            m_omSplitterWnd.CreateView( FIRST_ROW,                      // #Row
                                        SECOND_COL,                     // #Column
                                        RUNTIME_CLASS(CMsgSgDetView),   // View associated with
                                        om_Size,                        // Sizeof Pane
                                        pContext);
    }

    if ( TRUE == m_bSplitWndCreated )
    {
        // Create the Left Pane for static splitter window
        CMsgSgTreeView::sm_sDbParams = m_sDbParams;
        m_bSplitWndCreated  =
            m_omSplitterWnd.CreateView( FIRST_ROW,                      // #Row
                                        FIRST_COL,                      // #Column
                                        RUNTIME_CLASS(CMsgSgTreeView),  // View associated with
                                        om_Size,                        //CSize( 350,100 ),                 // Sizeof Pane
                                        pContext);
    }
    CString omTitle = _("DatabaseEditor - ");
    omTitle += m_sDbParams.m_omBusName;
    SetWindowText(omTitle.GetBuffer(MAX_PATH));
    if (m_sDbParams.m_eBus == J1939)
    {
        CMsgSignalDBWnd::sm_bValidJ1939Wnd = TRUE;
    }
    return m_bSplitWndCreated;
}
/******************************************************************************/
/*  Function Name    :  vCalculateSplitterPosition                            */
/*                                                                            */
/*  Input(s)         :  CSize &cSize                                          */
/*  Output           :  CSize &cSize                                                      */
/*  Functionality    :  This function will calculate the position of the splitter
                        given the size of the window
/*  Member of        :  CMsgSignalDBWnd                                      */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :
/******************************************************************************/

void CMsgSignalDBWnd::vCalculateSplitterPosition(CSize& cSize)
{
    RECT sRect;

    // Get its size
    GetWindowRect( &sRect );

    // Calculate splitter position
    cSize.cx = sRect.right / 3;
    cSize.cy = sRect.bottom / 4;

}
/******************************************************************************/
/*  Function Name    :  OnClose                                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called by the frame work whenever
                        user click on "X" (close)button. This will delete
                        the memory allocated for in-active DB opend in the
                        editor.Also, prompts for saving for any change in the
                        DB editor.

/*  Member of        :  CMsgSignalDBWnd                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  15.02.2002                                            */
/*  Modification Date:  25.03.2002
/*  Modified By      :  Amarnath Shastry                                      */
/*  Modification     :  Raja N on 10.03.2004                                  */
/*                      Modified to get refer inactive database structure for */
/*                      editor operation. changed the close operation as per  */
/*                      this new change                                       */
/******************************************************************************/

void CMsgSignalDBWnd::OnClose()
{
    // Get active frame
    CMainFrame* pFrame =
        static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

    if (pFrame != nullptr)
    {
        // Reset the flag to indicate the closing of database window
        if (m_sDbParams.m_eBus == CAN)
        {
            pFrame->SendMessage(IDM_CONFIGURE_DATABASE_CLOSE, 0, 0);
        }
        else if (m_sDbParams.m_eBus == J1939)
        {
            pFrame->SendMessage(ID_DATABASE_CLOSE, 0, 0);
        }
    }
}

LRESULT CMsgSignalDBWnd::OnSaveDBJ1939(WPARAM /* wParam */, LPARAM /*lParam*/)
{
    // Get active frame
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    if (pFrame != nullptr)
    {
        // Get appropriate data structure
        CMsgSignal* pTempMsgSg = m_sDbParams.m_pouMsgSignalActiveDB;
        pFrame->podGetMsgSgDetView(m_sDbParams.m_eBus);
        if (pTempMsgSg->bGetModifiedFlag() == FALSE)
        {
            vSaveModifiedDBs(pTempMsgSg);
        }
    }

    return 0;
}

void CMsgSignalDBWnd::vSaveModifiedDBs(CMsgSignal* pTempMsgSg)
{
    // Get active frame
    if (pTempMsgSg == nullptr )
    {
        return;
    }

    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    eProtocol eProtocolName = PROTOCOL_CAN;

    if(m_sDbParams.m_eBus == J1939)
    {
        eProtocolName = PROTOCOL_J1939;
    }
    else if(m_sDbParams.m_eBus == CAN)
    {
        eProtocolName = PROTOCOL_CAN;
    }
    if (nullptr != pTempMsgSg)
    {
        pTempMsgSg->bWriteIntoDatabaseFileFromDataStructure(m_sDbParams.m_omDBPath, eProtocolName);
    }

    // Check if the modified file is being loaded or not.
    //If yes then prompt the user whether he wants to
    //import it or not.
    CStringArray omImportedDBNames;
    CMsgSignal* m_ppsMSTemp = m_sDbParams.m_pouMsgSignalImportedDBs;

    if (m_ppsMSTemp != nullptr)
    {
        m_ppsMSTemp->vGetDataBaseNames(&omImportedDBNames);
        for (INT nDBCount = 0; nDBCount < omImportedDBNames.GetSize();
                nDBCount++)
        {
            if (m_sDbParams.m_omDBPath ==
                    omImportedDBNames.GetAt(nDBCount))
            {
                CString omText;
                omText.Format(_( "File  \"%s\"  has been modified which is currently being loaded.\nDo you want to re-import it to reflect the changes?"),
                              m_sDbParams.m_omDBPath);
                if (MessageBox(omText, "BUSMASTER", MB_ICONQUESTION | MB_YESNO) == IDYES)
                {
                    switch (m_sDbParams.m_eBus)
                    {
                        case CAN:
                        {
                            pFrame->dLoadDataBaseFile(m_sDbParams.m_omDBPath, FALSE);
                        }
                        break;
                        case J1939:
                        {
                            pFrame->dLoadJ1939DBFile(m_sDbParams.m_omDBPath, FALSE);
                        }
                        break;
                    };
                }
            }
        }
    }
    //Checking ends
    // Set the modified flag as saved
    if (nullptr != pTempMsgSg)
    {
        pTempMsgSg->vSetModifiedFlag(TRUE);
    }
}

void CMsgSignalDBWnd::vSetDBName(CString& omDBName)
{
    m_sDbParams.m_omDBPath = omDBName;
}

CWnd* CMsgSignalDBWnd::GetWorkingView()
{
    CWnd* pWnd = m_omSplitterWnd.GetActivePane();

    return pWnd;
}

LRESULT CMsgSignalDBWnd::UpdateSignalDetails(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    CMsgSgDetView* pView = (CMsgSgDetView*)m_omSplitterWnd.GetPane(FIRST_ROW,SECOND_COL);
    if(pView)
    {
        pView->UpdateSignalDetails();
    }
    return S_OK;
}