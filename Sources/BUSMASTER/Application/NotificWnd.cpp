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
 * \file      Application/NotificWnd.cpp
 * \brief     This file contain definition of all function of
 * \author    Ravikumar Patil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
// For all standard header file include
#include "StdAfx.h"
// Definition of App class
#include "BUSMASTER.h"
#include "NotificWnd.h"
#include "Common.h"
#include "resource.h"

#define NOTIFIC_WND_CONFIG_SECTION_NAME "NotificWndProperty"
#define TIMER_INVALID       0x0

typedef std::queue<CString> CStringQueue;
static CStringQueue sg_odStringQueue;

extern CCANMonitorApp theApp;

#define def_MAXIMUM_TRACE_LINES             5000

IMPLEMENT_DYNCREATE(CNotificWnd, CMDIChildBase)

CNotificWnd::CNotificWnd():CMDIChildBase( /*NOTIFICATION_WND_PLACEMENT*/(eCONFIGDETAILS)-1 ),
    m_omSizeMaxTxtExtent(0,0)
{
    m_unTimer = TIMER_INVALID;
}

CNotificWnd::~CNotificWnd()
{
}

BEGIN_MESSAGE_MAP(CNotificWnd, CMDIChildBase)
    ON_WM_HELPINFO()
    ON_WM_SIZE()
    ON_MESSAGE(WM_CONFIG_QUESTION, DoConfigOperation)
    ON_WM_CLOSE()
    ON_WM_SHOWWINDOW()
    ON_WM_DESTROY()
    ON_WM_TIMER()
END_MESSAGE_MAP()


/******************************************************************************/
/*  Function Name    :  vSetWindowFont                                        */
/*  Input(s)         :  Reference Object of new font                          */
/*  Output           :                                                        */
/*  Functionality    :  This function will change the font of list box to the */
/*                      the font passed as paramter to the function           */
/*  Member of        :  CNotificWnd                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ravikumar Patil                                       */
/*  Date Created     :  12-Mar-2003                                           */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      19.04.2003, set the font same as replay window        */
/******************************************************************************/
void CNotificWnd::vSetWindowFont()
{
    // Set the font of replay window list box
    LOGFONT LF;
    memset(&LF, 0, sizeof(LF));  //zero out structure
    LF.lfHeight = 14;
    LF.lfWidth  = 0;
    LF.lfItalic = FALSE;
    LF.lfUnderline = FALSE;
    LF.lfStrikeOut = FALSE;
    LF.lfOutPrecision = OUT_CHARACTER_PRECIS;
    LF.lfClipPrecision = CLIP_CHARACTER_PRECIS;
    LF.lfPitchAndFamily  = FIXED_PITCH | FF_SWISS;
    strcpy_s(LF.lfFaceName, LF_FACESIZE, _("Tahoma"));
    LF.lfWeight = FW_NORMAL;

    if (m_omNewFont.CreateFontIndirect(&LF) == TRUE)
    {
        // font setting.
        m_omListBox.SetFont(&m_omNewFont, TRUE);
    }
    else
    {
        AfxMessageBox(_("Font creation unsuccessful"));
    }
}

/******************************************************************************/
/*  Function Name    :  OnCreateClient                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called to create the list box inside */
/*                      the output window                                     */
/*  Member of        :  CNotificWnd                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ravikumar Patil                                       */
/*  Date Created     :  12-Mar-2003                                           */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/*  Modifications    :  Raja N on 20.07.2005, Added code to set window icon   */
/******************************************************************************/
BOOL CNotificWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    //Creation of the listbox window
    CRect omRect;
    this->GetClientRect(omRect);
    m_omListBox.Create(WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL
                       |LBS_HASSTRINGS |LBS_NOTIFY | LBS_WANTKEYBOARDINPUT |
                       LBS_NOINTEGRALHEIGHT | LBS_EXTENDEDSEL,
                       omRect, this, IDC_LSTB_OUTPUT);
    // Set the window icon
    SetIcon(theApp.LoadIcon(IDI_ICO_WRITE), TRUE);

    m_unTimer = SetTimer(0x50, 100, nullptr);

    return CMDIChildBase::OnCreateClient(lpcs, pContext);
}

/******************************************************************************/
/*  Function Name    :  OnHelpInfo                                            */
/*  Input(s)         :  Pointer to object of structure for help request info. */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_HELPINFO message. This message is sent to   */
/*                      the dialog box during help request from user.         */
/*                                                                            */
/*  Member of        :  CNotificWnd                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ravikumar Patil                                       */
/*  Date Created     :  12-Mar-2003                                           */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/******************************************************************************/
BOOL CNotificWnd::OnHelpInfo(HELPINFO* pHelpInfo)
{
    //theApp.vSetHelpID(pHelpInfo->dwContextId);
    return CMDIChildBase::OnHelpInfo(pHelpInfo);
}

/******************************************************************************/
/*  Function Name    :  OnSize                                                */
/*  Input(s)         :  Type of Resize request, new height and new width      */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work after the       */
/*                      window�s size has changed. List box attached to this  */
/*                      is moved accordingly.                                 */
/*  Member of        :  CNotificWnd                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ravikumar Patil                                       */
/*  Date Created     :  12-Mar-2003                                           */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/******************************************************************************/
void CNotificWnd::OnSize(UINT nType, int cx, int cy)
{
    // Resizing the Listbox when the trace window is resized
    CRect rect;
    CMDIChildBase::OnSize(nType, cx, cy);
    this->GetClientRect(rect);
    m_omListBox.MoveWindow(rect, TRUE);
}

/******************************************************************************/
/*  Function Name    :  OnClose                                               */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework when the     */
/*                      window is about to be closed                          */
/*  Member of        :  CNotificWnd                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ravikumar Patil                                       */
/*  Date Created     :  12-Mar-2003                                           */
/*  Modifications    :                                                        */
/******************************************************************************/
void CNotificWnd::OnClose()
{
    m_sNotificWndParams.m_bSetFlag_Disp = SW_HIDE;
    ShowWindow(SW_HIDE);
}

/******************************************************************************/
/*  Function Name    :  bCreateNotificWindow                                   */
/*  Input(s)         :                                                        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Create Replay window, if it is not created. It uses   */
/*                      default or last stored coordinates                    */
/*  Member of        :  CNotificWnd                                           */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  18.04.2003                                            */
/*  Modifications    :  Raja N on 26.04.2005, Changed the base class to       */
/*                      CMDIChildBase                                         */
/******************************************************************************/
BOOL CNotificWnd::bCreateNotificWindow(CMDIFrameWnd* pomParentWnd)
{
    LPCTSTR strMDIClass = AfxRegisterWndClass(
                              CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                              LoadCursor(nullptr, IDC_ARROW), 0,
                              LoadIcon(nullptr, IDI_WINLOGO));

    BOOL bResult = FALSE;
    // If the co-ordiantes are not correct, calculate the default value
    RECT sToolBarRect      = {0,0,0,0};
    CRect omRect;

    // Get Window rect from Configuration file
    WINDOWPLACEMENT sWinCurrStatus;
    sWinCurrStatus.rcNormalPosition.left = 5;
    sWinCurrStatus.rcNormalPosition.top = 460;
    sWinCurrStatus.rcNormalPosition.right = sWinCurrStatus.rcNormalPosition.left + 810;
    sWinCurrStatus.rcNormalPosition.bottom = sWinCurrStatus.rcNormalPosition.top + 191;

    bResult = CMDIChildBase::Create(strMDIClass, _("Trace Window"),
                                    WS_CHILD /*| WS_VISIBLE*/ | WS_OVERLAPPEDWINDOW,
                                    sWinCurrStatus.rcNormalPosition, pomParentWnd);

    vSetWindowFont();

    return bResult;
}

/******************************************************************************
 Function Name  :   OnShowWindow

 Description    :   Indicates that a window is about to be shown or hidden. The
                    framework calls this member function when the CWnd object
                    is about to be hidden or shown
 Input(s)       :   bShow - TRUE if the window is being shown; it is FALSE if
                    the window is being hidden
                    nStatus - Specifies the status of the window being shown
                    It is 0 if the message is sent because of a ShowWindow
                    member function call; otherwise nStatus is one of the
                    following:
                        SW_PARENTCLOSING - Parent window is closing (being made
                                     iconic) or a pop-up window is being hidden
                        SW_PARENTOPENING - Parent window is opening (being
                                   displayed) or a pop-up window is being shown
 Output         :   -
 Functionality  :   Prior to the window being shown, set the show state and
                    different positions of the window exactly as the same when
                    previous instance of it was destroyed
 Member of      :   CNotificWnd

 Author(s)      :   Amitesh Bharti
 Date Created   :   18.04.2003
 Modifications  :   Raja N on 26.04.2005, Changed the base class to
                    CMDIChildBase
******************************************************************************/
void CNotificWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CMDIChildBase::OnShowWindow(bShow, nStatus);
    GetMDIFrame()->SendMessage(WM_NOTIFICATION_FROM_OTHER, eWINID_TRACEWND, bShow);
    if (bShow)
    {
        m_sNotificWndParams.m_bSetFlag_Disp = SW_SHOW;
    }
    else
    {
        m_sNotificWndParams.m_bSetFlag_Disp = SW_HIDE;
    }
}

/******************************************************************************
    Function Name    :  vAddString
    Input(s)         :  unParam - Unused
                        lParam - Typecast pointer to the text buffer.
    Output           :  -
    Functionality    :  Adds a text to the list box.
    Member of        :  CNotificWnd
    Friend of        :
    Author(s)        :  Amitesh Bharti
    Date Created     :
    Modified by      :
    Modification     :
/*****************************************************************************/
void CNotificWnd::vAddString(CString omStr)
{
    if (! omStr.IsEmpty())
    {
        if ( m_omListBox.GetCount() > def_MAXIMUM_TRACE_LINES )
        {
            m_omListBox.DeleteString(0);
        }
        m_omListBox.AddString(omStr);
        // Set horizontal extent of the list box to max string available
        // so that user can scroll
        CSize   sz(0,0);
        CDC*  pDC = m_omListBox.GetDC();
        if (pDC != nullptr)
        {
            static INT s_nDx = 0;
            sz = pDC->GetTextExtent(omStr);
            if (sz.cx > s_nDx)
            {
                s_nDx = sz.cx;
                // Set the horizontal extent so every character of all strings
                // can be scrolled to.
                m_omListBox.SetHorizontalExtent(s_nDx);
            }
            m_omListBox.ReleaseDC(pDC);
        }
        int nCount = m_omListBox.GetCount();
        if (nCount > 0)
        {
            m_omListBox.SetCaretIndex(nCount-1);
        }
    }
}

/******************************************************************************
    Function Name    :  vSaveNotificWndConfig
    Input(s)         :  -
    Output           :  -
    Functionality    :  Saves Notification window configuration
    Member of        :  CNotificWnd
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  12.01.2009
    Modification     :
/*****************************************************************************/
void CNotificWnd::vSaveNotificWndConfig()
{
    //UINT unBufferSize = unGetStoreNFBufferSize(); //Get the required size
    //BYTE *SectionBuffer = new BYTE[unBufferSize];
    //if (SectionBuffer != nullptr)
    //{
    //  SaveNFDataIntoBuffer(SectionBuffer);// save the config into buffer
    //  /* Add into a different section NOTIFIC_WND_CONFIG_SECTION_NAME*/
    //  CConfigDetails::ouGetConfigDetailsObject().bSetData((LPVOID)SectionBuffer,
    //                                  unBufferSize, NOTIFIC_WND_CONFIG_SECTION_NAME);
    //  delete[] SectionBuffer;
    //}
}

/******************************************************************************
    Function Name    :  unGetStoreNFBufferSize
    Input(s)         :  -
    Output           :  UINT (size)
    Functionality    :  size of Notification window configuration buffer
    Member of        :  CNotificWnd
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.01.2009
    Modification     :
/*****************************************************************************/
UINT CNotificWnd::unGetStoreNFBufferSize()
{
    //size for windows related things PLACEMENT,FLAG, LOG
    UINT unTotalSize = sizeof(NOTIFICWNDPARAMS);

    return unTotalSize;
}

/**
 * Saves Notification window params into buffer
 *
 * @param[in] DesBuffer Poiter to the buffer of BYTES
 */
void CNotificWnd::SaveNFDataIntoBuffer(BYTE* DesBuffer)
{
    GetWindowPlacement(&m_sNotificWndParams.m_sWndPlacement);
    memcpy(DesBuffer, &m_sNotificWndParams, sizeof(NOTIFICWNDPARAMS));
}

/******************************************************************************
    Function Name    :  DoConfigOperation
    Input(s)         :  -
    Output           :  -
    Functionality    :  This function will be invoked when notification
                        window gets WM_CONFIG_QUESTION message
    Member of        :  CNotificWnd
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.01.2009
    Modification     :
/*****************************************************************************/
LRESULT CNotificWnd::DoConfigOperation(WPARAM WParam, LPARAM LParam)
{
    switch ((UINT)WParam)
    {
        case LOAD_CONFIG :
        {
            vLoadNotificWndConfig();
        }
        break;

        case SAVE_CONFIG :
        {
            vSaveNotificWndConfig();
        }
        break;

        case IS_CONFIG_CHANGED :
        {
            BOOL* bIsChanged = (BOOL*)LParam;
            *bIsChanged = bIsConfigChanged();
        }
        break;

        default :
        {
            ASSERT(false);
        }
    }
    return 0;
}

/**
* \brief         Clears Trace window contents
* \param         void
* \return        void
* \authors       Arunkumar Karri
* \date          17.12.2012 Created
*/
void CNotificWnd ::vClearTraceContents()
{
    /* Clear contents */
    m_omListBox.ResetContent();
}

/******************************************************************************
    Function Name    :  vLoadNotificWndConfig
    Input(s)         :  -
    Output           :  -
    Functionality    :  Loads Notification window configuration
    Member of        :  CNotificWnd
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.01.2009
    Modification     :
/*****************************************************************************/
void CNotificWnd :: vLoadNotificWndConfig()
{
#if 0
    int nBufferCount = 0;
    BYTE* SrcBuffer = nullptr;
    /* Retrieve the section NOTIFIC_WND_CONFIG_SECTION_NAME*/
    CConfigDetails::ouGetConfigDetailsObject().bGetData((void*&)(SrcBuffer),
            nBufferCount,NOTIFIC_WND_CONFIG_SECTION_NAME);
    if (SrcBuffer != nullptr)
    {
        /* Copy the retrieved config data into member variables and apply*/
        CopyNFDataFromBuffer(SrcBuffer);
        delete[] SrcBuffer;
        SrcBuffer = nullptr;
    }
    else //new file : load default values TBD
#endif
    {
        //Load default window placement
        GetWindowPlacement(&m_sNotificWndParams.m_sWndPlacement);
        m_sNotificWndParams.m_sWndPlacement.showCmd &= ~SW_MINIMIZE;
        m_sNotificWndParams.m_sWndPlacement.showCmd |= SW_RESTORE;
        m_sNotificWndParams.m_sWndPlacement.rcNormalPosition.left =
            SetWindowPlacement(&m_sNotificWndParams.m_sWndPlacement);
    }
}

/**
 * Copies Notification window params into buffer
 */
void CNotificWnd::CopyNFDataFromBuffer(BYTE* SrcBuffer)
{
    NOTIFICWNDPARAMS TempParam;
    memcpy(&TempParam, SrcBuffer, sizeof (NOTIFICWNDPARAMS));

    /* Save the data */
    m_sNotificWndParams.m_bSetFlag_Disp = TempParam.m_bSetFlag_Disp;
    m_sNotificWndParams.m_bSetFlag_Log = TempParam.m_bSetFlag_Log;
    m_sNotificWndParams.m_sWndPlacement = TempParam.m_sWndPlacement;
    SetWindowPlacement(&m_sNotificWndParams.m_sWndPlacement);
    ShowWindow(m_sNotificWndParams.m_bSetFlag_Disp);
}

/******************************************************************************
    Function Name    :  bIsConfigChanged
    Input(s)         :  -
    Output           :  boolean variable to indicate change in configuration
    Functionality    :  Saves Notification window configuration
    Member of        :  CNotificWnd
    Friend of        :
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19.01.2009
    Modification     :
/*****************************************************************************/
BOOL CNotificWnd::bIsConfigChanged()
{
    BOOL bReturn = FALSE;
    return bReturn;
}

void CNotificWnd::OnDestroy()
{
    CMDIChildBase::OnDestroy();

    // TODO: Add your message handler code here;
    if (TIMER_INVALID != m_unTimer)
    {
        ::KillTimer(nullptr, m_unTimer);
        m_unTimer = TIMER_INVALID;
    }
}

void CNotificWnd::OnTimer(UINT nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == m_unTimer)
    {
        m_omCriticalSection.Lock();
        while (sg_odStringQueue.empty() == false)
        {
            vAddString(sg_odStringQueue.front());
            sg_odStringQueue.pop();
        }
        m_omCriticalSection.Unlock();
    }

    CMDIChildBase::OnTimer(nIDEvent);
}

void CNotificWnd::vDisplayString(const CString& omStr)
{
    m_omCriticalSection.Lock();
    sg_odStringQueue.push(omStr);
    m_omCriticalSection.Unlock();
}
