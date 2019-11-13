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
 * \file      ValueDescriptionDlg.cpp
 * \brief     This file contain definition of all function of
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
#include "stdafx.h"                 // Standard header
#include "BUSMASTER.h"            // App class header
#include "ValueDescriptionDlg.h"    // Class definition is here 
#include "MainFrm.h"                // Hex validation definition is here
#include "HashDefines.h"            // All hash defines sre here

extern CCANMonitorApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CValueDescriptionDlg dialog
/******************************************************************************/
/*  Function Name    :  CValueDescriptionDlg                                  */
/*  Input(s)         :  eMODES eMode,
                        CString omStrMsgName,
                        CString omStrSgName,
                        CWnd* pParent                                         */
/*  Output           :                                                        */
/*  Functionality    :  Constructor                                           */
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :  11-12-2002, Amarnath S                                */
/*                      Initialised members                                   */
/******************************************************************************/
CValueDescriptionDlg::CValueDescriptionDlg(const SDBPARAMS& sDbParams,
        eMODES eMode,
        CString omStrMessageCode,
        CString omStrSgName,
        INT nIndex,
        CWnd* pParent /*=nullptr*/)
    : CDialog(CValueDescriptionDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CValueDescriptionDlg)
    m_omStrDescriptor = "";
    m_omStrValue = "";
    m_bIsCanceled = FALSE;
    m_omStrMessageCode = omStrMessageCode;
    m_omStrSgName = omStrSgName;
    m_bMode = eMode;
    m_nIndex = nIndex;
    //}}AFX_DATA_INIT
    bSignalType = "B";
    m_omStrPrevDesc         = "";
    m_omStrPrevSignalVal    = "";
    m_sDbParams             = sDbParams;
}


void CValueDescriptionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CValueDescriptionDlg)
    DDX_Text(pDX, IDC_EDIT_VALUE_DESC, m_omStrDescriptor);
    DDX_Text(pDX, IDC_EDIT_VAL, m_omStrValue);
    DDX_Control(pDX, IDC_EDIT_VAL, m_ounSigValue);
    DDV_MaxChars(pDX, m_omStrValue, 20);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CValueDescriptionDlg, CDialog)
    //{{AFX_MSG_MAP(CValueDescriptionDlg)
    ON_BN_CLICKED(IDC_CBTN_OK, OnClickedOk)
    ON_BN_CLICKED(IDC_CBTN_CANCEL, OnCancel)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/******************************************************************************/
/*  Function Name    :  OnClickedOk                                           */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Updates the dialog values to the data structure
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :  11-12-2002, Amarnath S                                */
/*                      Changes done to validate member variables             */
/*  Modifications    :  20-05-2003, Amitesh Bharti                            */
/*                      Pass the index for the case where descriptor is edited*/
/*                      to function for validating signal descriptor value and*/
/*                      signal descriptor.                                    */
/*  Modifications    :  10.03.2004, Raja N                                    */
/*                      Modified to refer inactive data structure for editor  */
/*                      purpose.                                              */
/******************************************************************************/
void CValueDescriptionDlg::OnClickedOk()
{
    BOOL bIsUpdated = UpdateData(TRUE);

    if(bIsUpdated == FALSE)
    {
        return;
    }

    BOOL bRetValue = TRUE;
    BOOL bModifiedFlag = FALSE;
    // Validate data
    if ( m_omStrDescriptor.IsEmpty() == TRUE )
    {
        AfxMessageBox(_("Signal Description cannot be emtpy!"),MB_OK);
        GetDlgItem(IDC_EDIT_VALUE_DESC)->SetFocus();
        bRetValue = FALSE;
    }

    if ( bRetValue == TRUE)
    {
        if ( m_omStrValue.IsEmpty() == TRUE )
        {
            AfxMessageBox(_("Signal value cannot be emtpy!"),MB_OK);
            GetDlgItem(IDC_EDIT_VAL)->SetFocus();
            bRetValue = FALSE;
        }

        if ( bRetValue == TRUE )
        {
            // Get appropriate msg structure ptr
            CMsgSignal* pTempMsgSg = m_sDbParams.m_pouMsgSignalActiveDB;

            if ( pTempMsgSg != nullptr )
            {
                // Check if description is duplicate
                if ( m_omStrPrevDesc != m_omStrDescriptor )
                {
                    if(pTempMsgSg->bIsDuplicateValueDescription(m_omStrMessageCode,
                            m_omStrSgName,
                            m_omStrValue,
                            m_omStrDescriptor,
                            m_nIndex,
                            m_bMode))
                    {
                        AfxMessageBox(_("Signal Descriptor already exists!"),MB_OK);
                        GetDlgItem(IDC_EDIT_VALUE_DESC)->SetFocus();
                        bRetValue = FALSE;
                    }
                }

                // Check if signal value is duplicate
                if ( bRetValue == TRUE )
                {
                    if ( m_omStrPrevSignalVal  != m_omStrValue )
                    {
                        if ( pTempMsgSg->bIsDuplicateSignalValue(m_omStrMessageCode,
                                m_omStrSgName,
                                m_omStrValue,
                                m_omStrDescriptor,
                                m_nIndex,
                                m_bMode))
                        {
                            AfxMessageBox(_("Signal Value already exists!"), MB_OK);
                            GetDlgItem(IDC_EDIT_VAL )->SetFocus();
                            bRetValue = FALSE;
                        }
                    }
                }

                // Check if the signal value is within the range
                if ( bRetValue == TRUE )
                {
                    if (pTempMsgSg->bIsSignalValueOutofRange(m_omStrMessageCode,
                            m_omStrSgName,
                            m_omStrValue ))
                    {
                        AfxMessageBox( _(MSG_SG_VAL_OUT_OF_RANGE), MB_OK);
                        GetDlgItem(IDC_EDIT_VAL )->SetFocus();
                        bRetValue = FALSE;
                    }

                    //check the value is changed or not
                    if (bIsDataModified()&&( bRetValue == TRUE ))
                    {
                        bModifiedFlag = TRUE;
                        // Update the desc into the data structure
                        CSignalDescVal* pNew = new CSignalDescVal;
                        //                      CMainFrame* pMainFrame =
                        //                          static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

                        if (pNew != nullptr)
                        {
                            CString omStrPrevDesc = "";
                            if ( MD_ADD == m_bMode )
                            {
                                // new desc and val hence allocate memory
                                pTempMsgSg->bAddSgDescVal( m_omStrMessageCode,
                                                           m_omStrSgName );
                            }
                            //                              pNew->m_n64SignalVal = _strtoi64( m_omStrValue, nullptr, 10);
                            if(bSignalType.CompareNoCase(defSIGNED_INT) == 0)
                            {

                                if(theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX))
                                {
                                    // Format the min value
                                    pNew->m_DescValue.n64Value = _strtoi64( m_omStrValue, nullptr, 16);
                                }
                                else
                                {
                                    pNew->m_DescValue.n64Value = _strtoi64( m_omStrValue, nullptr, 10);
                                }

                            }
                            else
                            {
                                if(theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX))
                                {
                                    // Format the min value
                                    pNew->m_DescValue.n64Value = _strtoi64( m_omStrValue, nullptr, 16);
                                }
                                else
                                {
                                    pNew->m_DescValue.n64Value = _strtoi64( m_omStrValue, nullptr, 10);
                                }
                            }

                            //                              // convert string to int
                            //                              pNew->m_n64SignalVal = pMainFrame->nConvertStringToInt( m_omStrValue );
                            pNew->m_omStrSignalDescriptor = m_omStrDescriptor;

                            if (MD_EDIT == m_bMode)
                            {
                                m_omStrPrevDesc = m_omStrDescriptor;
                            }
                            // update the edited values to the data structure
                            if ( !pTempMsgSg->bUpdateSgDescVal( m_omStrMessageCode,
                                                                m_omStrSgName,
                                                                m_omStrPrevDesc,
                                                                pNew ))
                            {
                                AfxMessageBox(_("Could not update..."),
                                              MB_OK|MB_ICONINFORMATION);
                                bRetValue = FALSE;
                            }
                            // clean up
                            delete pNew;
                            pNew = nullptr;
                        }
                    }
                }
            }
        }
    }

    if(bRetValue == TRUE)//no error occurs
    {
        if(bModifiedFlag == TRUE)//some modification done
        {
            CDialog::OnOK();
        }
        else //no modification
        {
            CDialog::OnCancel();
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnCancel                                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Sets a flag and calls the base class function
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
void CValueDescriptionDlg::OnCancel()
{
    m_bIsCanceled = TRUE;

    CDialog::OnCancel();
}
/******************************************************************************/
/*  Function Name    :  OnInitDialog                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Initialises the dialog members                        */
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  27.02.2002                                            */
/*  Modifications    :  11-12-2002, Amarnath S                                */
/*                      Initialised members in edit mode                      */
/******************************************************************************/
BOOL CValueDescriptionDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // Get appropriate msg structure ptr
    CMsgSignal* pTempMsgSg = m_sDbParams.m_pouMsgSignalActiveDB;

    CString strMinVal = "";
    CString strMaxVal = "" ;
    if ( pTempMsgSg != nullptr )
    {
        pTempMsgSg->vGetSignalValidRange(m_omStrMessageCode,
                                         m_omStrSgName, strMinVal, strMaxVal);

    }

    if(theApp.pouGetFlagsPtr()->nGetFlagStatus(HEX))
    {
        m_ounSigValue.vSetBase(BASE_HEXADECIMAL);
        m_ounSigValue.vSetBase(BASE_HEXADECIMAL);

        CString strRange;
        strRange.Format(_("(Hexadecimal range %s To %s)"), strMinVal, strMaxVal);

        GetDlgItem(IDC_STAT_RANGE)->SetWindowText(strRange);
    }
    else
    {
        m_ounSigValue.vSetBase(BASE_DECIMAL);
        m_ounSigValue.vSetBase(BASE_DECIMAL);

        CString strRange;
        strRange.Format(_("(Decimal range %s To %s)"), strMinVal, strMaxVal);

        GetDlgItem(IDC_STAT_RANGE)->SetWindowText(strRange);
    }



    if ( m_bMode == MD_EDIT )
    {
        m_omStrPrevDesc         = m_omStrDescriptor;
        m_omStrPrevSignalVal    = m_omStrValue;
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
/******************************************************************************/
/*  Function Name    :  PreTranslateMessage                                   */
/*  Input(s)         :  MSG* pMsg                                             */
/*  Output           :                                                        */
/*  Functionality    :  Process hexadecimal characters                        */
/*                      in signal value edit control                          */
/*  Member of        :  CValueDescriptionDlg                                  */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  11.12.2002                                            */
/*  Modifications    :  23-10-2003, Amitesh Bharti                            */
/*                      Remove condition for not processing space.            */
/******************************************************************************/
BOOL CValueDescriptionDlg::PreTranslateMessage(MSG* pMsg)
{
    // Do not process non-hexadecimal characters
    // in signal value edit control
    BOOL bSkip = FALSE;
    CEdit* omEditCtrlName   = (CEdit*) GetDlgItem(IDC_EDIT_VAL);
    CEdit* omEditFocusName  = (CEdit*) GetFocus();
    if ( pMsg->message == WM_CHAR )
    {
        if ( omEditCtrlName == omEditFocusName )
        {
            int nStart, nEnd;
            omEditCtrlName->GetSel(nStart, nEnd);
            if (nStart == 0 &&
                    pMsg->wParam == MINUS_SIGN)
            {
                bSkip = FALSE;
            }
            else if ( ( pMsg->wParam >= 0x61 && pMsg->wParam<=0x66 )|| // A-F
                      ( pMsg->wParam >= 0x41 && pMsg->wParam<=0x46 )||// a-f
                      ( pMsg->wParam >= '0' && pMsg->wParam <='9' ) ||// 0-9
                      pMsg->wParam == 0x08 )// BackSpace
            {
                bSkip = FALSE;
            }
            else
            {
                bSkip = TRUE;
            }
        }
    }
    if ( bSkip == FALSE )
    {
        bSkip = CDialog::PreTranslateMessage(pMsg);
    }

    return bSkip;
}
//KSS
BOOL CValueDescriptionDlg::bIsDataModified()
{
    BOOL bDataChanged = FALSE;
    if (m_omStrPrevDesc != m_omStrDescriptor ||
            m_omStrPrevSignalVal != m_omStrValue)
    {
        bDataChanged = TRUE;
    }
    return bDataChanged;
}
//KSS