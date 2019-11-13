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
 * \file      MsgBufferConfigPage.cpp
 * \brief     This file contain definition of all function of
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
#include "stdafx.h"                 // For standard include
#include "BUSMASTER.h"            // For App definition
#include "MsgBufferConfigPage.h"    // For class definition

extern CCANMonitorApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMsgBufferConfigPage property page



/******************************************************************************
 Function Name  :   CMsgBufferConfigPage

 Description    :   Standard default constructor
 Member of      :   CMsgBufferConfigPage
 Functionality  :   Initialises data members

 Author(s)      :   Raja N
 Date Created   :   22.07.2004
 Modifications  :
******************************************************************************/
CMsgBufferConfigPage::CMsgBufferConfigPage(msgBufferSettings* msgBufferSettings) :
    CPropertyPage(CMsgBufferConfigPage::IDD,
                  IDS_PPAGE_TITLE_BUFFER)
{
    mMsgBufferSettings = msgBufferSettings;
    m_unAppendSize = 0;
    m_unOverWriteSize = 0;
    m_unDisplayUpdateRate = 0;
    m_pnBufferSize = nullptr;
}

/******************************************************************************
 Function Name  :   ~CMsgBufferConfigPage

 Description    :   Standard destructor
 Member of      :   CMsgBufferConfigPage

 Author(s)      :   Raja N
 Date Created   :   22.07.2004
 Modifications  :
******************************************************************************/
CMsgBufferConfigPage::~CMsgBufferConfigPage()
{
}

void CMsgBufferConfigPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMsgBufferConfigPage)
    DDX_Text(pDX, IDC_EDIT_APPEND_SIZE, m_unAppendSize);
    DDV_MinMaxUInt(pDX, m_unAppendSize, defMIN_BUFFER_SIZE, defMAX_BUFFER_SIZE);
    DDX_Text(pDX, IDC_EDIT_OVERWRITE_SIZE, m_unOverWriteSize);
    DDV_MinMaxUInt(pDX, m_unOverWriteSize, defMIN_BUFFER_SIZE, defMAX_BUFFER_SIZE);
    DDX_Text(pDX, IDC_EDIT_DISPLAY_UPDATE, m_unDisplayUpdateRate);
    DDV_MinMaxUInt(pDX, m_unDisplayUpdateRate, defMIN_DISPLAY_UPDATE_TIME, defMAX_DISPLAY_UPDATE_TIME);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgBufferConfigPage, CPropertyPage)
    //{{AFX_MSG_MAP(CMsgBufferConfigPage)
    ON_BN_CLICKED(IDC_CBTN_SET_DEFAULT, OnCbtnSetDefault)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgBufferConfigPage message handlers

/*******************************************************************************
 Function Name  : OnOK
 Description    : Virtual Function. This handler will be called when the user
                  selects Ok from the property sheet. This will update the user
                  data in to the configuration class.
 Member of      : CFlexListCtrl
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 31.07.2004, Removed unused local variable
*******************************************************************************/
void CMsgBufferConfigPage::OnOK()
{
    // Update Global Data Here
    UpdateData( TRUE );
    // Check for data boundary condition
    if( m_unAppendSize >= defMIN_BUFFER_SIZE &&
            m_unAppendSize <= defMAX_BUFFER_SIZE &&
            m_unOverWriteSize >= defMIN_BUFFER_SIZE &&
            m_unOverWriteSize <= defMAX_BUFFER_SIZE &&
            m_unDisplayUpdateRate >= defMIN_DISPLAY_UPDATE_TIME &&
            m_unDisplayUpdateRate <= defMAX_DISPLAY_UPDATE_TIME )
    {
        // Append Bufer Size
        mMsgBufferSettings->mAppendSize = m_unAppendSize;
        // Overwrite Bufer Size
        mMsgBufferSettings->mOverWriteSize = m_unOverWriteSize;
        // Display Update Rate
        mMsgBufferSettings->mDisplayUpdateRate = m_unDisplayUpdateRate;
    }
    CPropertyPage::OnOK();
}

/*******************************************************************************
Function Name    : OnInitDialog
Input(s)         :
Output           :
Functionality    : This function will be called during initialization of dialog
                   box. This function will load the initial data from
                   configuration module
Member of        : CMsgBufferConfigPage
Friend of        :  -
Author(s)        : Raja N
Date Created     : 22.07.2004
Modifications    :
*******************************************************************************/
BOOL CMsgBufferConfigPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    // Update the UI controls
    m_unAppendSize = mMsgBufferSettings->mAppendSize;
    m_unOverWriteSize = mMsgBufferSettings->mOverWriteSize;
    m_unDisplayUpdateRate = mMsgBufferSettings->mDisplayUpdateRate;
    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************
Function Name    : OnCbtnSetDefault
Input(s)         : -
Output           : -
Functionality    : This function will be called if the user selects the
                   SetDefault button. This will set the default value of the
                   various parameters
Member of        : CMsgBufferConfigPage
Friend of        :  -
Author(s)        : Raja N
Date Created     : 22.07.2004
Modifications    :
*******************************************************************************/
void CMsgBufferConfigPage::OnCbtnSetDefault()
{
    m_unAppendSize = defDEF_APPEND_BUFFER_SIZE;
    m_unOverWriteSize = defDEF_OVERWRITE_BUFFER_SIZE;
    m_unDisplayUpdateRate = defDEF_DISPLAY_UPDATE_TIME;
    UpdateData( FALSE );
}