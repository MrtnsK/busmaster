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
 * \file      OutWnd.cpp
 * \brief     This file contain the definition all member function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
// For all standard header file include
#include "NodeSimEx_stdafx.h"
//definition of CMainFrame class

// Definition of COutWnd class
#include "OutWnd.h"
//To get CoutWnd member of ExecuteManager
#include "ExecuteManager.h"
#include "FileView.h"
#include "HashDefines.h"
#include "GlobalObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// COutWnd

IMPLEMENT_DYNCREATE(COutWnd, CMDIChildBase)
/******************************************************************************/
/*  Function Name    :  COutWnd                                               */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is default constructor                  */
/*                                                                            */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  11.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            */
/*  Modifications    :  Raja N on 26.04.2005                                  */
/*                      Added code to init base class with window identity    */
/******************************************************************************/
COutWnd::COutWnd(ETYPE_BUS eBus, CGlobalObj* pGlobalObj, CExecuteManager* pExecuteMgr) : CMDIChildBase( /*MSG_WND_PLACEMENT*/ OUT_WND_PLACEMENT),
    m_omSizeMaxTxtExtent(0,0)
{
    m_eBus = eBus;
    m_pGlobalObj = pGlobalObj;
    m_pExecuteMgr = pExecuteMgr;
}
/******************************************************************************/
/*  Function Name    :  ~COutWnd                                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is default destructor                   */
/*                                                                            */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
COutWnd::~COutWnd()
{
    m_pExecuteMgr->m_pouBuildProgram->m_podOutWnd = nullptr;
}


BEGIN_MESSAGE_MAP(COutWnd, CMDIChildBase)
    //{{AFX_MSG_MAP(COutWnd)
    ON_WM_DESTROY()
    ON_WM_HELPINFO()
    ON_WM_SIZE()
    ON_LBN_DBLCLK(IDC_LSTB_OUTPUT,OnDbClick)
    ON_WM_SHOWWINDOW()
	
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  bAddString                                            */
/*  Input(s)         :  Array of strings                                      */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function will add strings, stored in CStringArray*/
/*                      passed as parameter, to the list box                  */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            */
/*  Modification By  :  Anish kumar                                           */
/*  Modified On      :  03.11.2006,Setting the font for list box doesn't
                        assign font type to CDC object so for calculating
                        TextExtent first select that font using CDC           */
/******************************************************************************/
BOOL COutWnd::bAddString(CStringArray& omStrArray)
{
    BOOL bReturn     = TRUE;
    INT  nTotalCount = (COMMANINT)omStrArray.GetSize();
    INT  nReturnAdd  = 0;
    //   vResetContent();
    MDIActivate();
    omStrArray.Add(" ");
    INT nCount; //nCount declared outside
    for( nCount=0; nCount<nTotalCount; nCount++)
    {
        nReturnAdd = m_omListBox.AddString(omStrArray.GetAt(nCount));
        if(nReturnAdd<0)
        {
            bReturn = FALSE;
        }
    }
    // Set horizontal extent of the list box to max string availaable
    // so that user can scroll
    CSize   sz(0,0);
    CString omStrText   = "";
    CDC*  pDC = m_omListBox.GetDC();
    //Check for valid dc and then set the horizontal extent
    if( pDC != nullptr)
    {
        TEXTMETRIC   tm;
        pDC->GetTextMetrics(&tm);

        //Select the new font object to calculte the extent
        //because font is set for window not for CDC
        CFont* pOldFont = pDC->SelectObject(&m_omNewFont);
        INT nTotalItem =  m_omListBox.GetCount();
        INT nDx = 0;
        for (nCount = 0; nCount < nTotalItem; nCount++)
        {
            m_omListBox.GetText( nCount, omStrText );
            // remove space
            omStrText.TrimRight();
            sz = pDC->GetTextExtent(omStrText);
            //Add avg char width for avoiding any wrapping up
            sz.cx += tm.tmAveCharWidth;
            if (sz.cx > nDx)
            {
                nDx = sz.cx;
            }
        }
        //release the new font
        pDC->SelectObject(pOldFont);
        m_omListBox.ReleaseDC(pDC);
        // Set the horizontal extent so every character of all strings
        // can be scrolled to.
        m_omListBox.SetHorizontalExtent(nDx);
    }
    else
    {
        bReturn = FALSE;
    }
    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  vSetWindowFont                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will change the font of list box to the */
/*                      the font passed as paramter to the function           */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      19.04.2003, set the font same as replay window        */
/******************************************************************************/
void COutWnd::vSetWindowFont()
{
    // Set the font of replay window list box
    LOGFONT LF;
    memset(&LF, 0, sizeof(LF));  //zero out structure

    LF.lfHeight = 12;
    LF.lfWidth  = 0;
    LF.lfItalic = FALSE;
    LF.lfUnderline = FALSE;
    LF.lfStrikeOut = FALSE;
    LF.lfOutPrecision = OUT_CHARACTER_PRECIS;
    LF.lfClipPrecision = CLIP_CHARACTER_PRECIS;
    LF.lfPitchAndFamily  = FIXED_PITCH | FF_SWISS;
    _tcscpy(LF.lfFaceName, "Courier");

    LF.lfWeight = FW_NORMAL;
    if (m_omNewFont.CreateFontIndirect(&LF) == TRUE)
    {
        // font setting.
        m_omListBox.SetFont(&m_omNewFont,TRUE);
    }
    else
    {
        AfxMessageBox("Font creation unsuccessful");
    }
}
/******************************************************************************/
/*  Function Name    :  vResetContent                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Reset the content of list box to remove all the items */
/*                                                                            */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void COutWnd::vResetContent()
{
    m_omListBox.ResetContent();
}

/******************************************************************************/
/*  Function Name    :  OnCreateClient                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called to create the list box inside */
/*                      the output window                                     */
/*                                                                            */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            */
/*  Modifications    :  Raja N on 26.04.2005, Changed base class to           */
/*                      CMDIChildBase                                         */
/******************************************************************************/
BOOL COutWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    //Creation of the listbox window
    CRect omRect;
    this->GetClientRect(omRect);


    BOOL breturn = m_omListBox.Create(WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL
                                      |LBS_HASSTRINGS |LBS_NOTIFY | LBS_WANTKEYBOARDINPUT |
                                      LBS_NOINTEGRALHEIGHT,
                                      omRect, this, IDC_LSTB_OUTPUT);

    return CMDIChildBase::OnCreateClient(lpcs, pContext);
}
/******************************************************************************/
/*  Function Name    :  OnDestroy                                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work to destroy the  */
/*                      window. The window coordinates are written into .ini  */
/*                      or registry                                           */
/*                                                                            */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            */
/*  Modifications    :  Raja N on 26.04.2005, Changed base class to           */
/*                      CMDIChildBase                                         */
/******************************************************************************/
void COutWnd::OnDestroy()
{
    CMDIChildBase::OnDestroy();
}

void COutWnd::vCloseOutputWindow()
{
	CMDIChildBase::OnClose();
}


/******************************************************************************/
/*  Function Name    :  OnHelpInfo                                            */
/*  Input(s)         :  Pointer to object of structure for help request info. */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework in response  */
/*                      to the WM_HELPINFO message. This message is sent to   */
/*                      the dialog box during help request from user.         */
/*                                                                            */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  11.03.2002                                            */
/*  Modifications    :  Raja N on 26.04.2005, Changed base class to           */
/*                      CMDIChildBase                                         */
/******************************************************************************/
BOOL COutWnd::OnHelpInfo(HELPINFO* pHelpInfo)
{
    return CMDIChildBase::OnHelpInfo(pHelpInfo);
}
/******************************************************************************/
/*  Function Name    :  OnSize                                                */
/*  Input(s)         :  Type of Resize request, new height and new width      */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work after the       */
/*                      window's size has changed. List box attached to this  */
/*                      is moved accordingly.                                 */

/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  11.03.2002                                            */
/*  Modifications    :  Raja N on 26.04.2005, Changed base class to           */
/*                      CMDIChildBase                                         */
/******************************************************************************/
void COutWnd::OnSize(UINT nType, int cx, int cy)
{
    //Resizing the Listbox wnd when the output window
    //is resized
    CRect rect;
    CMDIChildBase::OnSize(nType, cx, cy);
    this->GetClientRect(rect);
    m_omListBox.MoveWindow(rect,TRUE);
}
/******************************************************************************/
/*  Function Name    :  OnDbClick                                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user double clicks the   */
/*                      list box window. If there is any error line number is */
/*                      taken out from the string and passed to file view     */
/*                      member function as parameter                          */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  12.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            *
    Modification     :  28.01.06 ,Ganapathi hegde,Anish:made changes to open a
                        document if it is not opened and Set it active if it is
                        opened;for multiple document view
/******************************************************************************/
void COutWnd::OnDbClick()
{
    INT nSelectIndex;
    CString omStrSelectedItem;
    nSelectIndex = m_omListBox.GetCurSel();
    if(nSelectIndex!=LB_ERR )
    {
        CString omStrLineNumber = "";
        INT     nIndex          = 0;
        UINT    unLineNumber    = 0;
        char*   pcStopStr       = nullptr;

        m_omListBox.GetText(nSelectIndex,omStrSelectedItem);
        CString omStrFilePath;
        CString omStrFileName;
        omStrFilePath=omStrSelectedItem;
        while(!(nSelectIndex==0||omStrFilePath==defSTR_BUILD_TRACE_LINE_MARK))
        {
            --nSelectIndex;
            m_omListBox.GetText(nSelectIndex,omStrFilePath);
        }
        if(omStrFilePath==defSTR_BUILD_TRACE_LINE_MARK)
        {
            ++nSelectIndex;
            m_omListBox.GetText(nSelectIndex,omStrFilePath);
            int nNameIndex = omStrFilePath.ReverseFind('\\');
            if(nNameIndex != -1)
            {
                //pGetBusSpecificFunctionEditorDoc(omStrFilePath);
                CFunctionEditorDoc* pDoc = m_pGlobalObj->pGetDocPtrOfFile(omStrFilePath);
                if (pDoc != nullptr)
                {
                    //If file is opened then get its frame and activate it
                    {
                        POSITION pos = pDoc->GetFirstViewPosition();
                        if (pos)
                        {
                            pDoc->GetNextView(pos)->GetParentFrame()->ActivateFrame();
                        }
                    }
                }
                else
                {
                    //If file is not opened then open it
                    if ( !m_pGlobalObj->bOpenFunctioneditorfile(omStrFilePath) )
                    {
                        AfxMessageBox("Specified filename not found!",
                                      MB_OK|MB_ICONINFORMATION);
                    }
                }

                // Find the ':' to get the number after second ':'
                nIndex = omStrSelectedItem.Find(":");
                if(nIndex!=-1)
                {

                    omStrLineNumber = omStrSelectedItem.Right(
                                          omStrSelectedItem.GetLength()-nIndex-1);
                    nIndex          = omStrLineNumber.Find(":");
                    omStrLineNumber = omStrLineNumber.Right(
                                          omStrLineNumber.GetLength()-nIndex-1);

                    omStrLineNumber.TrimLeft();
                    omStrLineNumber.TrimRight();
                    omStrLineNumber = omStrLineNumber.SpanExcluding(":");
                    unLineNumber    = _tcstol((LPCTSTR)omStrLineNumber,
                                              &pcStopStr,10);
                    // Call this function only if the  line number is valid
                    if(unLineNumber!=0)
                    {
                        CFileView* pFileView = m_pGlobalObj->podGetFileViewPtr();
                        if(pFileView != nullptr)
                        {
                            pFileView->vDisplayWarningLineNumber(OUTWND,unLineNumber);
                        }
                    }
                }
                else
                {
                    nIndex = omStrSelectedItem.Find(":");
                    if(nIndex!=-1)
                    {
                        nIndex          = omStrSelectedItem.Find(":");
                        omStrLineNumber = omStrSelectedItem.Right(
                                              omStrSelectedItem.GetLength()-nIndex-1);

                        omStrLineNumber.TrimLeft();
                        omStrLineNumber.TrimRight();
                        omStrLineNumber = omStrLineNumber.
                                          SpanExcluding("\t ");
                        unLineNumber    = _tcstol((LPCTSTR)omStrLineNumber,
                                                  &pcStopStr,
                                                  10);
                        if(unLineNumber!=0)
                        {
                            CFileView* pFileView = m_pGlobalObj->podGetFileViewPtr();

                            if(nullptr != pFileView)
                            {
                                pFileView->vDisplayWarningLineNumber(OUTWND,
                                                                     unLineNumber);
                            }
                        }
                    }
                }
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  bCreateReplayWindow                                   */
/*  Input(s)         :                                                        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Create Replay window, if it is not created. It uses   */
/*                      default or last stored coordinates                    */
/*  Member of        :  COutWnd                                               */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  18.04.2003                                            */
/*  Modification By  :  Raja N                                                */
/*  Modified On      :  26.04.2005, Added code to get window position data    */
/*                      from configuration module by using the window identity*/
/******************************************************************************/
BOOL COutWnd::bCreateOutputWindow()
{

    BOOL bReturn = 0;
    // If the co-ordiantes are not correct, calculate the default value
    // Get Window rect from Configuration file
    WINDOWPLACEMENT sWinPlacement;
    m_pGlobalObj->bGetDefaultValue(OUT_WND_PLACEMENT, sWinPlacement);
    sWinPlacement.showCmd = SW_SHOW;

    bReturn     = CMDIChildBase::Create( nullptr,
                                         "Output Window",
                                         WS_CHILD   |
                                         WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                                         sWinPlacement.rcNormalPosition,
                                         nullptr,
                                         nullptr);

    vSetWindowFont();
    SendMessage(WM_NCPAINT, 1, 0);

    return bReturn;
}
