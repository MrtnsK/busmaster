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
 * \file      PPageMessage.cpp
 * \brief     Implementation of CPPageMessage class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CPPageMessage class
 */


// For all standard header file include
#include "stdafx.h"
// Interface file for the application class
#include "BUSMASTER.h"
// Interface file for CRadixEdit class
#include "Utility/RadixEdit.h"
// Interface file for CMsgIDAttr class
#include "MsgIDAttr.h"
// Interface file for CMessageAttrib class
#include "MessageAttrib.h"
//for integer list
#include "HashDefines.h"

#include "MessageList.h"
#include "PPageMessage.h"
#include "MainFrm.h"
#include "common.h"

// Appliction global object
extern CCANMonitorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPPageMessage property page

IMPLEMENT_DYNCREATE(CPPageMessage, CPropertyPage)

CPPageMessage::CPPageMessage() : CPropertyPage(CPPageMessage::IDD)
{
    //{{AFX_DATA_INIT(CPPageMessage)
    //}}AFX_DATA_INIT
}

CPPageMessage::CPPageMessage(BOOL bForDBMsg, UINT* msgIds, unsigned int msgCount, CMessageAttrib* msgAttributes) :
    CPropertyPage(CPPageMessage::IDD,
                  bForDBMsg ? IDS_PPAGE_TITLE_DBMSG : IDS_PPAGE_TITLE_NDBMSG)
{
    m_bForDBMsg    = bForDBMsg;
    m_pRGBColors   = nullptr;
    mUnDBMsgs = msgIds;
    mUnTotalDBMsgs = msgCount;
    mMsgAttributes = msgAttributes;
}

CPPageMessage::~CPPageMessage()
{
    if(m_pRGBColors)
    {
        //delete[] m_pRGBColors;
        free(m_pRGBColors);
        m_pRGBColors = nullptr;
    }
}

void CPPageMessage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPPageMessage)
    DDX_Control(pDX, IDC_LIST_MESSAGE, m_odMsgList);
    DDX_Control(pDX, IDC_BUTTON_REMOVE, m_ctrlRemove);
    DDX_Control(pDX, IDC_BUTTON_ADD, m_ctrlAdd);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPPageMessage, CPropertyPage)
    //{{AFX_MSG_MAP(CPPageMessage)
    ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
    ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
    ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_MESSAGE, OnDblclkListMessage)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MESSAGE, OnItemchangedListMessage)
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_MESSAGE, OnNMCustomdrawListMessage)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPageMessage message handlers


BOOL CPPageMessage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO: Add extra initialization here
    if (m_bForDBMsg)
    {
        m_ctrlAdd.ShowWindow(SW_HIDE);
        m_ctrlRemove.ShowWindow(SW_HIDE);
    }

    m_odMsgList.SetExtendedStyle(m_odMsgList.GetExtendedStyle()
                                 | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

    RECT Rect;
    m_odMsgList.GetWindowRect(&Rect);

    int nWidth = (Rect.right - Rect.left - 4) / 2;
    m_odMsgList.InsertColumn(0, _("Message ID"), LVCFMT_LEFT, nWidth);
    m_odMsgList.InsertColumn(1, _("Message Text"), LVCFMT_LEFT, nWidth);


    BOOL bAnyMsgEntered = (nInitialiseMsgLCtrl(mUnTotalDBMsgs, mUnDBMsgs) > 0);

    GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(bAnyMsgEntered);
    GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(bAnyMsgEntered);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPPageMessage::bIsMsgIDPresent(UINT* punMsgIDBuffer, UINT unTotalMsgs,
                                    UINT unMsgID) const
{
    BOOL bResult = FALSE;

    if ((unTotalMsgs > 0) && (punMsgIDBuffer != nullptr))
    {
        UINT unCount = 0;
        while ((unCount < unTotalMsgs) && (bResult != TRUE))
        {
            if (punMsgIDBuffer[unCount] == unMsgID)
            {
                bResult = TRUE;
            }
            unCount++;
        }
    }

    return bResult;
}

int CPPageMessage::nInitialiseMsgLCtrl(UINT unTotalDBMsgs, UINT* punDBMsgIDs)
{
    int nMessagesAdded = 0;

    SCanIDList* psList = nullptr;

    int nEntries = mMsgAttributes->nGetTotalIDs();

    if (nEntries > 0)
    {
        psList = new SCanIDList[nEntries];
    }

    if (psList != nullptr)
    {
        mMsgAttributes->nGetMsgAttribs(psList);
        int nLastItem = 0;

        m_pRGBColors = (COLORREF*)malloc(nEntries * sizeof(COLORREF));
        for (int i = 0; i < nEntries; i++)
        {
            m_sNewItem = *(psList + i);
            BOOL bToEnter = !(bIsMsgIDPresent(punDBMsgIDs, unTotalDBMsgs,
                                              m_sNewItem.nCANID) ^ m_bForDBMsg);
            if (bToEnter)
            {
                nLastItem = nEnterMessageAttrib(m_sNewItem, nLastItem);
                nLastItem++;
                nMessagesAdded++;
            }
        }
        delete[] psList;
        psList = nullptr;
    }

    return nMessagesAdded;
}

int CPPageMessage::nEnterMessageAttrib(const SCanIDList& sMsgAttrib, int nItem)
{
    int nResult;

    // Copy message ID
    sprintf_s(m_acMsgEntry, 128, "  %X", sMsgAttrib.nCANID);
    nResult = m_odMsgList.InsertItem(LVIF_STATE | LVIF_TEXT| LVIF_PARAM, nItem,
                                     m_acMsgEntry, LVIS_FOCUSED | LVIS_SELECTED, 0x7, 0, sMsgAttrib.Colour);

    if (nResult != -1)
    {
        // Copy message Name
        size_t CharToCopy = sMsgAttrib.omCANIDName.GetLength() + 1;
        if (CharToCopy > 128)
        {
            CharToCopy = 128;
        }

        strncpy_s(m_acMsgEntry, 128, (LPCTSTR) sMsgAttrib.omCANIDName, CharToCopy);

        if (m_odMsgList.SetItemText(nItem, 1, m_acMsgEntry) == FALSE)
        {
            nResult = -1;
        }

        if(m_pRGBColors)
        {
            m_pRGBColors[nResult] = sMsgAttrib.Colour;
        }
    }
    return nResult;
}

void CPPageMessage::OnButtonAdd()
{
    CMsgIDAttr odMsgDlg(mMsgAttributes);
    if (odMsgDlg.DoModal() == IDOK)
    {
        m_sNewItem.nCANID = odMsgDlg.m_nID;
        m_sNewItem.omCANIDName = odMsgDlg.m_omStrMsg;
        m_sNewItem.Colour = odMsgDlg.m_sColour;

        /* Addition of the new message attribute must be always successful
        because the validation is already done */
        mMsgAttributes->nAddNewAttrib(m_sNewItem);

        if(m_pRGBColors == nullptr)
        {
            m_pRGBColors = (COLORREF*)malloc(sizeof(COLORREF));
        }
        else
        {
            m_pRGBColors = (COLORREF*)realloc(m_pRGBColors, (m_odMsgList.GetItemCount()+1)* sizeof(COLORREF));
        }

        nEnterMessageAttrib(m_sNewItem, m_odMsgList.GetItemCount());
    }
}

void CPPageMessage::OnButtonEdit()
{
    int nCurrSel = m_odMsgList.GetNextItem(-1, LVNI_SELECTED);

    if (nCurrSel >= 0)
    {

        CString omARow = m_odMsgList.GetItemText(nCurrSel, 0);

        UINT unMsgID;
        sscanf_s((LPCTSTR) omARow, "%X", &unMsgID);
        mMsgAttributes->nGetAttrib(unMsgID, m_sNewItem);

        CMsgIDAttr odMsgDlg(mMsgAttributes);
        odMsgDlg.m_nID = m_sNewItem.nCANID;
        odMsgDlg.m_sColour = m_sNewItem.Colour;
        odMsgDlg.m_omStrMsg = m_sNewItem.omCANIDName;
        odMsgDlg.m_bForEdit = true;
        odMsgDlg.m_bDBMessage = m_bForDBMsg;
        odMsgDlg.m_nPreviousID = m_sNewItem.nCANID;
        if (odMsgDlg.DoModal() == IDOK)
        {
            m_sNewItem.omCANIDName = odMsgDlg.m_omStrMsg;
            m_sNewItem.nCANID = odMsgDlg.m_nID;
            m_sNewItem.Colour = odMsgDlg.m_sColour;

            if(odMsgDlg.m_nPreviousID == m_sNewItem.nCANID) //if ID is not changed then just modify the current one
            {
                mMsgAttributes->nModifyAttrib(m_sNewItem);
            }
            else    //if the ID has changed then first remove the structure with old ID and then save the new ID
            {
                int nResult = mMsgAttributes->nRemoveAttrib(odMsgDlg.m_nPreviousID);
                if (nResult == 0)
                {
                    mMsgAttributes->nAddNewAttrib(m_sNewItem);
                }
            }

            m_odMsgList.DeleteItem(nCurrSel);
            nEnterMessageAttrib(m_sNewItem, nCurrSel);

            m_odMsgList.EnsureVisible(nCurrSel, FALSE);
        }
    }
}

void CPPageMessage::OnButtonRemove()
{
    int nCurrSel = m_odMsgList.GetNextItem(-1, LVNI_SELECTED);

    while (nCurrSel >= 0)
    {
        CString omARow = m_odMsgList.GetItemText(nCurrSel, 0);
        UINT unMsgID;
        sscanf_s((LPCTSTR) omARow, "%X", &unMsgID);
        mMsgAttributes->nGetAttrib(unMsgID, m_sNewItem);

        /* CString omWarningMsg;
         omWarningMsg.Format(
             "Do you want to delete entry for Message ID 0x%X ?",
             m_sNewItem.nCANID);
         if (AfxMessageBox(omWarningMsg, MB_YESNO | MB_ICONQUESTION) == IDYES)*/
        mMsgAttributes->nRemoveAttrib(m_sNewItem.nCANID);
        m_odMsgList.DeleteItem(nCurrSel);
        nCurrSel = m_odMsgList.GetNextItem(-1, LVNI_SELECTED);
    }
}

void CPPageMessage::OnDblclkListMessage(NMHDR* pNMHDR, LRESULT* pResult)
{
    if (pNMHDR->idFrom == IDC_LIST_MESSAGE)
    {
        OnButtonEdit();
    }

    *pResult = 0;
}

void CPPageMessage::OnCancel()
{
    mMsgAttributes->vDoRollback();

    CPropertyPage::OnCancel();
}

void CPPageMessage::OnOK()
{
    mMsgAttributes->vDoCommit();
    mMsgAttributes->vSaveMessageAttribData();
    CPropertyPage::OnOK();
}

void CPPageMessage::OnItemchangedListMessage(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // IDE generated code - commented because it isn't needed at present.
    //NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // If there is a selected item, then it makes sense to keep the "Edit" and
    // "Remove" buttons in enabled state.
    BOOL bToEnable = ((m_odMsgList.GetNextItem(-1, LVNI_SELECTED)) >= 0);

    CButton* pomButton = (CButton*) GetDlgItem(IDC_BUTTON_EDIT);
    if (pomButton != nullptr)
    {
        pomButton->EnableWindow(bToEnable);
    }

    if (m_bForDBMsg == FALSE)
    {
        pomButton = (CButton*) GetDlgItem(IDC_BUTTON_REMOVE);
        if (pomButton != nullptr)
        {
            pomButton->EnableWindow(bToEnable);
        }
    }

    *pResult = 0;
}

///*******************************************************************************
//  Function Name  : OnNMCustomdrawListMessage
//  Description    :
//  Member of      : CPPageMessage
//  Functionality  : Setting the Color for each Message Entry
//  Author(s)      : ArunKumar K
//  Date Created   : 09.07.2010
//  Modifications  :
//*******************************************************************************/
void CPPageMessage::OnNMCustomdrawListMessage(NMHDR* pNMHDR, LRESULT* pResult)
{
    //LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
    // TODO: Add your control notification handler code here

    if (pNMCD->nmcd.dwDrawStage == CDDS_PREPAINT)
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
    {
        if(m_pRGBColors)
        {
            pNMCD->clrText = m_pRGBColors[pNMCD->nmcd.dwItemSpec];
        }
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
    {

        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == (CDDS_ITEMPOSTPAINT | CDDS_SUBITEM))
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT)
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREERASE)
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_ITEMPOSTERASE)
    {
        *pResult = CDRF_DODEFAULT;
    }
    else if (pNMCD->nmcd.dwDrawStage == CDDS_SUBITEM )
    {
        *pResult = CDRF_DODEFAULT;
    }
    else
    {
        *pResult = CDRF_DODEFAULT;
    }
}
///*******************************************************************************
//  Function Name  : vSetMsgIDList
//  Description    :
//  Member of      : CPPageMessage
//  Functionality  : Provides the list of messages ID contained in the m_odMsgList
//                 to CanMonitor class
//  Author(s)      : Anish
//  Date Created   : 14.12.2006
//  Modifications  :
//*******************************************************************************/
//void CPPageMessage::vSetMsgIDList()
//{
//  CString omStrTemp ;
//  theApp.m_omListOfNDBDisplayMsgID.RemoveAll();
//  int nItem = m_odMsgList.GetItemCount();
//  for(int nTemp = 0 ; nTemp < nItem ; nTemp++)
//  {
//      omStrTemp = m_odMsgList.GetItemText(nTemp,0);
//      theApp.m_omListOfNDBDisplayMsgID.AddHead(strtol(omStrTemp,nullptr,16));
//  }
//}