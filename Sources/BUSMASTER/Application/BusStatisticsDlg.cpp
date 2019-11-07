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
 * \file      BusStatisticsDlg.cpp
 * \brief     This file contain the definition of CBusStatisticsDlg class
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CBusStatisticsDlg class
 */
// For all standard header file include
#include "stdafx.h"
// Definition of App class
#include "BUSMASTER.h"
// Definition of CBusStatistics class

// For time manager class
#include "Utility\TimeManager.h"
// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/MsgBufAll_Datatypes.h"
//#include "DataTypes/DIL_Datatypes.h"
#include "BaseDIL_CAN.h"

#include "BusStatisticsDlg.h"
#include ".\busstatisticsdlg.h"
#include "NetworkStatistics.h"

#define  BUS_STATICS_CONFIG_PATH     "//BUSMASTER_CONFIGURATION/Module_Configuration/Bus_Statistics/CAN_Statistics/COLUMN"
/////////////////////////////////////////////////////////////////////////////
// CBusStatisticsDlg dialog
//extern SBUSSTATISTICS g_sBusStatistics[ defNO_OF_CHANNELS];
// For application object
extern CCANMonitorApp theApp;
// DIL CAN interface
extern CBaseDIL_CAN* g_pouDIL_CAN_Interface;

const BYTE BUS_STATS_DLG_VERSION = 0x1;

/******************************************************************************/
/*  Function Name    :  CBusStatisticsDlg                                     */
/*  Input(s)         :  pParent : Pointer to parent window                    */
/*                      CBaseBusStatisticCAN* Pointer to the BusStatistics    */
/*                      Base class                                            */
/*  Output           :                                                        */
/*  Functionality    :  This is the constructor of this class. All data member*/
/*                      will be initialised in this class.                    */
/*  Member of        :  CBusStatisticsDlg                                     */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2003                                            */
/*  Modifications    :  Raja N on 12.12.2004                                  */
/*                      Removed duplicate initialisation by calling common    */
/*                      init function vClearData                              */
/*  Modifications    :  Raja N on 12.12.2004                                  */
/*                      Removed clearing data as data is moved to the class   */
/*                      CStatistics                                           */
/******************************************************************************/
CBusStatisticsDlg::CBusStatisticsDlg(CBaseBusStatisticCAN* pouBSCAN,int nChannelCount)
    : CCommonStatistics(CBusStatisticsDlg::IDD, CAN),
      m_pouBSCAN(pouBSCAN),
      m_omStrBusLoad(""),
      m_omStrPeakBusLoad(""),
      m_omStrAvgBusLoad( "" )

{
    //{{AFX_DATA_INIT(CBusStatisticsDlg)
    //}}AFX_DATA_INIT
    m_nChannelCount = nChannelCount;
}

CBusStatisticsDlg::CBusStatisticsDlg(CBaseBusStatisticCAN* pouBSCAN, CWnd* /* pParent */, int nChannelCount)
    : CCommonStatistics(CBusStatisticsDlg::IDD, CAN),
      m_pouBSCAN(pouBSCAN),
      m_omStrBusLoad(""),
      m_omStrPeakBusLoad(""),
      m_omStrAvgBusLoad( "" )

{
    //{{AFX_DATA_INIT(CBusStatisticsDlg)
    //}}AFX_DATA_INIT
    m_nChannelCount = nChannelCount;
    for( int nChannel = 0; nChannel < m_nChannelCount; nChannel++ )
    {
        SBUSSTATISTICS sBusStatistics;
        m_pouBSCAN->BSC_GetBusStatistics(nChannel, sBusStatistics);
    }
}

/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to exchange and validate      */
/*                         dialog data                                        */
/*                                                                            */
/*  Member of        :  CBusStatisticsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2003                                            */
/*  Modifications    :  Raja N on 12.12.2004                                  */
/*                      Removed DDX mapping of global statistics members as   */
/*                      UI type got changed to List control from read only    */
/*                      text boxes                                            */
/******************************************************************************/
void CBusStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
    CCommonStatistics::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBusStatisticsDlg)
    DDX_Control(pDX, IDC_LIST_STAT, m_omStatList);
    //}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CBusStatisticsDlg, CCommonStatistics)
BEGIN_MESSAGE_MAP(CBusStatisticsDlg, CCommonStatistics)
    //{{AFX_MSG_MAP(CBusStatisticsDlg)
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_DISPLAY_MESSAGE, vUpdateFields)
    ON_WM_SIZE()
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  vUpdateFields                                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will uodate the dialog controls with the*/
/*                      data stored in its corresponding data member. Some    */
/*                      will be calculated before updating.                   */
/*                                                                            */
/*  Member of        :  CBusStatisticsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2003                                            */
/*  Modifications    :  Amitesh Bharti, 13.08.2004                            */
/*                      Bus load and peak bus load is displayed.              */
/*  Modifications    :  Raja N on 12.12.2004, Modified function to compute new*/
/*                      stat parameters and to populate new list control UI.  */
/*                      also added check to update the UI only if the dialog  */
/*                      is visible. Otherwise only do computation             */
/*  Modifications    :  Raja N on 14.03.2005, Removed computation part. Now   */
/*                      CStatistics class will do computation and this function
/*                      will only update UI with the computed values          */
/******************************************************************************/
LRESULT CBusStatisticsDlg::vUpdateFields(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    // Update the list control only if the dialog is visible
    if( IsWindowVisible() == TRUE )
    {
        for( int nChannel = 0; nChannel < m_nChannelCount; nChannel++ )
        {
            SBUSSTATISTICS sBusStatistics;
            m_pouBSCAN->BSC_GetBusStatistics(nChannel, sBusStatistics);
            // Update Window Text
            CString omDispText;
            int nIndex = 0;
            // Message Total
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unTotalMsgCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Total Message Rate
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unMsgPerSecond );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Total Errors
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unErrorTotalCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Error Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dErrorRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Bus Load
            // Format to show only two decimal digit.
			if (theApp.pouGetFlagsPtr()->nGetFlagStatus(CONNECTED) == TRUE)
			{
            m_omStrBusLoad.Format( defSTR_FORMAT_BUS_LOAD,
                                   sBusStatistics .m_dBusLoad );
            m_omStatList.SetItemText(nIndex, nChannel + 1, m_omStrBusLoad);
			}
            nIndex++;
            // Peak Load
            m_omStrPeakBusLoad.Format( defSTR_FORMAT_BUS_LOAD,
                                       sBusStatistics.m_dPeakBusLoad );
            m_omStatList.SetItemText(nIndex, nChannel + 1, m_omStrPeakBusLoad);
            nIndex++;
            // Avarage Load
            m_omStrAvgBusLoad.Format( defSTR_FORMAT_BUS_LOAD,
                                      sBusStatistics.m_dAvarageBusLoad );
            m_omStatList.SetItemText(nIndex, nChannel + 1, m_omStrAvgBusLoad);
            nIndex++;

            // For Tx Message Title
            nIndex++;

            // Tx Total
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unTotalTxMsgCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Std Msg
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unTxSTDMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Std Msg Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dTxSTDMsgRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Exd Msg
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unTxEXTDMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Exd Msg Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dTxEXTMsgRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Std. RTR
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unTxSTD_RTRMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Exd. RTR
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unTxEXTD_RTRMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Error
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unErrorTxCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Error Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dErrorTxRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;

            nIndex++;

            // Rx Total
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unTotalRxMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Std Msg
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unRxSTDMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Std Msg Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dRxSTDMsgRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Exd Msg
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unRxEXTDMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Exd Msg Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dRxEXTMsgRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Std. RTR
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unRxSTD_RTRMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Exd. RTR
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unRxEXTD_RTRMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Error
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unErrorRxCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Error Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dErrorRxRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Increment for the title Status
            nIndex++;

            // Controller status
            switch( sBusStatistics.m_ucStatus )
            {
                case defCONTROLLER_ACTIVE:
                {
                    omDispText = _(defSTR_ACTIVE_STATE);
                }
                break;
                case defCONTROLLER_PASSIVE:
                {
                    omDispText = _(defSTR_PASSIVE_STATE);
                }
                break;
                case defCONTROLLER_BUSOFF:
                {
                    omDispText = _(defSTR_BUSOFF_STATE);
                }
                break;
                default:
                {
                    omDispText = _(defSTR_UNKNOWN_STATE);
                }
            }
            // Set controller state
            m_omStatList.SetItemText( nIndex, nChannel + 1, omDispText );
            nIndex++;
            // Set Tx Error Counter value
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_ucTxErrorCounter );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;

            // Set Tx Peak Error Counter value
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_ucTxPeakErrorCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;

            // Set Rx Error Counter value
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_ucRxErrorCounter );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;

            // Set Rx Peak Error Counter value
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_ucRxPeakErrorCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
        }
    }
    return 0;
}

/*******************************************************************************
  Function Name  : OnInitDialog
  Input(s)       : -
  Output         : FALSE - If focus is set to Any UI control explicitly
  Functionality  : Initialises dialog's UI components
  Member of      : CBusStatisticsDlg
  Author(s)      : Raja N
  Date Created   : 12/12/2004
  Modifications  : Raja N on 14.03.2005
                   Modified init to support multi channel information and added
                   rows for new statistics parameters
*******************************************************************************/
BOOL CBusStatisticsDlg::OnInitDialog()
{
    CCommonStatistics::OnInitDialog();

    SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_NETWORKSTAT), TRUE);
    
    // Init Item color and Heading color variables
    int nHeadingColor, nItemColor;
    nHeadingColor = defHEADING_COLOR;
    nItemColor = defITEM_COLOR;

    int nIndex = 0;

    // Insert Columns
    m_omStatList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );
    m_omStatList.InsertColumn( 0, _(defSTR_COL_PARAMETER) );
    m_omStatList.SetColumnWidth( 0, defPARAMETER_COL_WIDTH );

    for( nIndex = 0; nIndex < m_nChannelCount; nIndex++)
    {
        CString cs;
        cs.Format( defSTR_CHANNEL_NAME_FORMAT, defSTR_CHANNEL_NAME, nIndex + 1);
        m_omStatList.InsertColumn( nIndex + 1, cs, LVCFMT_CENTER );
        m_omStatList.SetColumnWidth( nIndex + 1, defVALUE_COL_WIDTH );
    }

    CRect cr;
    // Get Window Rectangle
    GetWindowRect(&cr);
    // Resize the window.
    // Dialog Width = Width of Parameter Col +
    //                Width of Value * Number of Channels +
    //                Dialog Border offset
    cr.right = cr.left + defPARAMETER_COL_WIDTH + defDIALOG_BORDER_OFFSET +
               ( defVALUE_COL_WIDTH * 2 );
    // Resize the dialog width
    MoveWindow(&cr);
    // Get list boundary
    m_omStatList.GetWindowRect(&cr);
    // Dialog Width = Width of Parameter Col +
    //                Width of Value * Number of Channels +
    //                List Border offset
    cr.right = cr.left + defPARAMETER_COL_WIDTH + defLIST_BORDER_OFFSET +
               ( defVALUE_COL_WIDTH * 2 );
    // Convert to client offset
    ScreenToClient(&cr);
    // Set the boundary for list control
    m_omStatList.MoveWindow(&cr);
    // Insert Rows
    CString omStrInitValue = "";
    // Init Item Index
    nIndex = 0;

    // Total Message count
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_MSG_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Message Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_MSG_RATE) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Errors
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_ERR_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Error Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_ERR_RATE));
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Network Load
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_LOAD) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Peak network load
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_PEAK_LOAD) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Insert Avarage Bus Load
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_AVG_LOAD) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // "Transmitted' heading
    m_omStatList.InsertItem(nIndex, _(defSTR_TX_HEADING) );
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;
    // Total Tx Message
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx Std Message
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_STD_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Tx Std Message Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_STD_RATE) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx Extended Message
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_EXD_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Tx Extended Message Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_EXD_RATE) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx stanard RTR Message
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_STD_RTR) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx Extended RTR Message
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_EXD_RTR) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx Errors Total
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_ERR_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Tx Error Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_ERR_RATE) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // "Received" Heading
    m_omStatList.InsertItem(nIndex, _(defSTR_RX_HEADING));
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;
    // Total Rx Messages
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Standard Messages
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_STD_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Rx Standard Message Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_STD_RATE) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Extended Messages
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_EXD_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Rx Extended Message Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_EXD_RATE) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Standard RTR Messages
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_STD_RTR) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Extended RTR Messages
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_EXD_RTR) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Errors
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_ERR_TOTAL) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Rx Error Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_ERR_RATE) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Controller Status Heading
    m_omStatList.InsertItem(nIndex, _(defSTR_STATUS_HEADING) );
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;

    // Insert Controller status
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_CONTROLLER_STATUS) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Tx Error Counter
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_ERROR_COUNT) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Peak Tx Error Counter
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_PEAK_TX_ERROR_COUNT) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Rx Error counter
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_ERROR_COUNT) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Peak Rx Error Counter
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_PEAK_RX_ERROR_COUNT) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    GetWindowPlacement(&sm_sBusSerializationData.m_sDefaultBusStatsDlgCoOrd);

    if(sm_sBusSerializationData.m_sBusStatsDlgCoOrd.rcNormalPosition.left == 0)
    {
        sm_sBusSerializationData.m_sBusStatsDlgCoOrd = sm_sBusSerializationData.m_sDefaultBusStatsDlgCoOrd;
    }

    // Set the focus to the list
    m_omStatList.SetFocus();

    hSetConfigData();

    return FALSE;
}

/*******************************************************************************
  Function Name  : OnEraseBkgnd
  Input(s)       : pDC - Device context to the Screen
  Output         : -
  Functionality  : This will return simply as the whole dialog area is occupied
                   by the list control. This will avoid flickering
  Member of      : CBusStatisticsDlg
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  :
*******************************************************************************/
BOOL CBusStatisticsDlg::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}


/*******************************************************************************
  Function Name  : OnSize
  Input(s)       : UINT nType, int cx, int cy
  Output         : -
  Functionality  : This function Makes the List control to the same size as
                   that of dialog whenever dialog window is resized.
  Member of      : CBusStatisticsDlg
  Author(s)      : ArunKumar K
  Date Created   : 24.3.2011
  Modifications  :
*******************************************************************************/
void CBusStatisticsDlg::OnSize(UINT nType, int cx, int cy)
{
    CCommonStatistics::OnSize(nType, cx, cy);

    if(m_omStatList.m_hWnd)
    {
        CRect omClientRect;
        GetClientRect(&omClientRect);
        m_omStatList.MoveWindow(&omClientRect);
    }
}

