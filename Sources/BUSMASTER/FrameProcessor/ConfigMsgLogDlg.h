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
 * \file      ConfigMsgLogDlg.h
 * \brief     This is the class definition file for log configuration
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This is the class definition file for log configuration
 */

#pragma once
#include "FrameProcessor\IFrameProcessor_Common.h"
#include "resource.h"
#include "Utility/RadixEdit.h"
typedef enum ECONTROLTYPE
{
    EDITCTRL = 0, COMBOBOX, RADIOBUTTON, BUTTON, CHECKBOX, STATICTEXT,
};



class CActionFlag
{
public:

    static const BYTE ACTIONVAL_RESET  = 0x00;
    static const BYTE ENABLE_CTRL      = 0x01;
    static const BYTE CLEAR_CTRL       = 0x02;

    CActionFlag();
    ~CActionFlag();

    // Query functions
    static BOOL bCtrlToBeEnabled(BYTE bFlag)
    {
        return ((bFlag & ENABLE_CTRL) == ENABLE_CTRL);
    };

    static BOOL bCtrlToBeCleared(BYTE bFlag)
    {
        return ((bFlag & CLEAR_CTRL) == CLEAR_CTRL);
    };

    // Value setting functions
    static BYTE bResetActionFlag(void)
    {
        return ACTIONVAL_RESET;
    };

    static BYTE bSetEnableCtrlFlag(BYTE bFlag)
    {
        return (bFlag | ENABLE_CTRL);
    };

    static BYTE bSetClearCtrlFlag(BYTE bFlag)
    {
        return (bFlag | CLEAR_CTRL);
    };
};
// CConfigMsgLogDlg dialog
class CConfigMsgLogDlg : public CDialog
{
    DECLARE_DYNAMIC(CConfigMsgLogDlg)

public:
    CConfigMsgLogDlg(IFrameProcessor_Common* baseLogger, BOOL& bLogOnConnect, CWnd* pParent);
    virtual ~CConfigMsgLogDlg();
    enum { IDD = IDD_DLG_CONFIG_MSG_LOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()

public:
    // Init Log File Control
    virtual BOOL OnInitDialog();
    void vSetLogFileONOFF(BOOL bLogON);

protected:
    IFrameProcessor_Common* mFrameProcessor;
    int m_nLogIndexSel;

    CComboBox m_omComboChannel;
    CComboBox m_omComboTimeMode;
    CButton m_ChkbResetTimeStamp;
    CListCtrl m_omListLogFiles;
    CRadixEdit m_odStartMsgID;
    CRadixEdit m_odStopMsgID;
    CButton m_ChkbEnableLogOnConnect;
    UINT m_unChannelCount;
    CString m_strCurrWndText;
private:
    //Advanced Log variables
    int m_nMeasurement;
    int m_nTime;
    int m_nSize;

    int m_nFileSize;
    int m_nFileCount;
    int m_nTimeHrs;
    int m_nTimeMin;

    // Dialog control variables


    // To indicate programmed UI update
    BOOL m_bUpdatingUI;


    BOOL m_bEditingON;
    BOOL m_bLogON;
    UINT m_unDispUpdateTimerId;

    BOOL& m_bLogOnConnect;

    CButton m_ChkLogOnMeasurement;
    CButton m_ChkLogOnSize;
    CButton m_ChkLogOnTime;

    void EnablingMaxNumEdit();
    void EnablingAdvSettings(BOOL);

private:
    void vEnableDisableControls(BOOL);
    SLOGTRIGGER GetLogTriggerFromGUI(void);
    void vCreateFileList(void);
    void vEnableDisableControl(int nControlID, ECONTROLTYPE eCtrlType, BOOL Enable);
    void vUpdateControl(int nControlID, ECONTROLTYPE eCtrlType, BYTE bAction);
    void SetGUIFromTimeMode(ETIMERMODE eTimeMode);
    void SetGUIFromChannel(TYPE_CHANNEL CurrChannel);
    CString GetUniqueLogFilePath(void);
    void vUpdate_GUI_From_Datastore(USHORT usIndex);
    void vUpdate_Datastore_From_GUI(USHORT usIndex, int CtrlID);
    BOOL FoundInLogList(CString omFullPath, CString omFileName);
    void AddNewItem_GUI(SLOGINFO sLogStructNew, int Index);


    // Abstract helper functions
    USHORT GetLoggingBlockCount(void);
    HRESULT GetLoggingBlock(USHORT ushBlk, SLOGINFO& sLogObject);
    HRESULT SetLoggingBlock(USHORT ushBlk, const SLOGINFO& sLogObject);
    HRESULT AddLoggingBlock(const SLOGINFO& sLogObject);
    HRESULT RemoveLoggingBlock(USHORT ushBlk);

    virtual std::string GetDefaultLogFileName() = 0;
    virtual HRESULT HandleFilterSettings() = 0;
    virtual void InitialiseWindow() = 0;

protected:
    afx_msg void OnBnClickedCbtnAddlog(void);
    afx_msg void OnBnClickedCbtnLogFilePath(void);
    afx_msg void OnBnClickedCbtnRemovelog(void);
    afx_msg void OnLvnItemchangedLogBlockLst(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNumRButtonClick(UINT);
    afx_msg void OnFileRButtonClick(UINT);
    afx_msg void OnChkbTriggerClick(UINT);
    afx_msg void OnSelChangeComb(UINT);
    afx_msg void OnStartStopMsgIDEnChange(UINT);
    afx_msg void OnBnClickedLogFilter(void);

    afx_msg void OnBnClickedLogOnMeasurement(void);
    afx_msg void OnBnClickedLogOnSize(void);
    afx_msg void OnBnClickedLogOnTime(void);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnTimer(UINT nIDEvent);

    afx_msg void OnEnChngLogFileSize();
    afx_msg void OnEnChngLogFileTimeHrs();
    afx_msg void OnEnChngLogFileTimeMins();
    afx_msg void OnEnChngLogFilesAllowed();
    afx_msg void OnEnChngLogComment();

protected:
    CString m_omControlParam;
    CString m_omControlParam2;

public:
    afx_msg void OnBnClickedOk();
    void vAddSuffixToLogFileName(eFILENAMESUFFIX eFileNameSuffix);
    void vRemoveSuffixFromLogFileName(eFILENAMESUFFIX eFileNameSuffix);
    void vResetAdvLogParameters();

};
