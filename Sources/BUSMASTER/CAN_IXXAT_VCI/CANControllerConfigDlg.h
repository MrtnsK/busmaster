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
 * @file
 *  CANControllerConfigDlg.h
 *
 * @brief
 *  Implements the configuration dialog to select the baud rate of the CAN controller.
 *  It's a MFC dialog.
 */

#pragma once

#include <string>
#include "afxwin.h"
#include "DIL_Interface\IChangeRegisters.h"
#include <map>

/**
 * @def IXXAT_NUM_OF_CIA_ENTRIES
 *
 * @brief
 *  The number of entries for the baud rate list combo box.
 *  All CiA baud rates and the "user defined" entry.
 */
#define IXXAT_NUM_OF_CIA_ENTRIES 10


/**
 * @def IXXAT_LAST_ENTRY_BAUD_LIST
 *
 * @brief
 *  The index number of the last combo box entry.
 *
 */
#define IXXAT_LAST_ENTRY_BAUD_LIST 9

/**
 * @class CCANControllerConfigDlg
 *
 * @brief
 *  MFC-Dialog for setting the CAN controller baud rate.
 *
 */
class CCANControllerConfigDlg : public CDialog, public IChangeRegisters //CPropertyPage
{
public:
	int InvokeAdavancedSettings(PSCONTROLLER_DETAILS pControllerDetails, UINT nCount,UINT );
	DOUBLE vValidateBaudRate(DOUBLE dbaudrate,int,UINT );
    DECLARE_DYNAMIC(CCANControllerConfigDlg)

public:
    CCANControllerConfigDlg(std::string omBaudRate, int iBTRRegisters, CWnd* pParent /*=nullptr*/);
    virtual ~CCANControllerConfigDlg();

    // Dialog Data
    enum { IDD = IDD_IXXAT_CAN_CONFIG_DLG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonCancfgOk();
    CComboBox m_comboBoxCiABaudSelection;

protected:

    /**
     * @struct  sCiaBaud
     *
     * @brief
     *    A structur to fill with the CiA and the user defined
     *    baud values.
     *
     */
    struct sCiaBaud
    {
        std::string strName;
        DWORD dwBaud;
        DWORD dwBTR0;
        DWORD dwBTR1;
    };
    sCiaBaud m_asBaudList[IXXAT_NUM_OF_CIA_ENTRIES];  ///< List of as baud entries.

	std::map<int, int> m_mapBaudRateToCiaBaudIndex;
	DWORD m_dwBaudRate; //To store the baud rate selected by the user in channel selection dialog
    DWORD m_dwBTR0; ///< The class member for the bit timing register 0
    DWORD m_dwBTR1; ///< The class member for the bit timing register 1
    std::string m_strSelectedBaudName; ///< The selected baud rate as string
    BOOL   m_bSuppressUpdateCalculation;  ///< internal flag to prevent problems while changing the combox box selection
	BOOL	m_bDialogCancel;
	SCONTROLLER_DETAILS m_asDummyControllerDetails[CHANNEL_ALLOWED];
    void FillBaudStruct();
    void UpdateBTRFields(int iIndex);
    int  GetListIndexFromBTRRegisters();
    void UpdateComboBoxIndexFromEditFields();
	void SelectComboBaudRate(); //To set the value in the CANControllerConfigDlg which is set by user in Channel Selection Dlg
public:
    CEdit m_editBTR0; ///< A corresponing member for the edit box bit timing register 0
    CEdit m_editBTR1; ///< A corresponing member for the edit box bit timing register 0
    afx_msg void OnCbnSelendokComboCiaBaudSelection();
    int GetBitTimingValue();
    std::string GetBitTimingName();
    afx_msg void OnBnClickedButtonCancel();
    afx_msg void OnEnChangeEditBitreg0();
    afx_msg void OnEnChangeEditBitreg1();
};
