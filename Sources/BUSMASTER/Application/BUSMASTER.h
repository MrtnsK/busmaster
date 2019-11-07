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
 * @brief This header file for CCANMonitorApp class
 * @authors Amitesh Bharti, Ratnadip Choudhury, Anish kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file for CCANMonitorApp class
 */

#pragma once

#include "resource.h"       // main symbols
#include "MsgSignal.h"      // Added by ClassView
#include "Flags.h"      // For CFlags
#include "Struct.h" // Added by ClassView
#include "Utility/MultiLanguageSupport.h"

class CCANMonitorApp : public CWinAppEx
{
    DECLARE_MESSAGE_MAP()

public:
    CCANMonitorApp();

    
    /** wrapper around CConfigDetails::vRelease(..) */
    void vRelease1(eCONFIGDETAILS eParam, LPVOID* lpDataPtr);

    /** wrapper around CConfigDetails::bGetData(..) */
    BOOL bGetData1(eCONFIGDETAILS  eParam, LPVOID* lpData);

    /** wrapper around CConfigDetails::bSetData(..) */
    BOOL bSetData1(eCONFIGDETAILS  eParam, LPVOID lpVoid);

    /** To get Default window size */
    BOOL bGetDefaultValue( eCONFIGDETAILS eParam, WINDOWPLACEMENT& sPosition );

    
    CEvent m_aomState[defEVENT_TOTAL];

    void GetLoadedConfigFilename(CString& roStrCfgFile);
    bool bReadFromRegistry(HKEY hRootKey, CString strSubKey, CString strName,  DWORD dwType, CString& strValue , DWORD& dwValue);
	bool bWriteIntoRegistry(HKEY /* hRootKey */, CString strSubKey, CString strName, BYTE bytType, CString strValue, DWORD dwValue = 0);
	void ReadRecentFileList();
	
    void AddToRecentFileList(LPCTSTR lpszPathName);

    CMsgSignal* m_pouMsgSignal;
    CMsgSignal* m_pouMsgSgInactive;

    /**  Specifies the application directory */
    CHAR m_acApplicationDirectory[MAX_PATH];

    void vSetHelpID(DWORD dwHelpID);

    CFlags* pouGetFlagsPtr();
    BOOL m_bFromAutomation;

    virtual BOOL InitInstance();
    virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
    virtual int ExitInstance();
    virtual CWnd* GetMainWnd();

    afx_msg void OnAppAbout();
    afx_msg void OnFileOpen();
    afx_msg void OnFileNew();

	afx_msg BOOL OnOpenRecentFile(UINT nID);
    INT COM_nSaveConfiguration(CString omStrCfgFilename);
    BOOL bWriteIntoTraceWnd(char* omText, BOOL bTrace = TRUE);

private:
    BOOL bInitialiseConfiguration(BOOL bFromCom);
    void vSetFileStorageInfo(CString oCfgFilename);
    void vDisplayConfigErrMsgbox(UINT unErrorCode, BOOL bOperation);

    CString m_ostrConfigFilename;
    CString m_omConfigErr;
    BOOL m_bIsConfigFileLoaded;
    DWORD m_dwHelpID;
    CFlags* m_pouFlags;
    BOOL m_bIsMRU_CreatedInOpen;
    sTOOLBAR_BUTTON_STATUS m_sToolbarInfo;
    CString m_omMRU_C_FileName;
    WINDOWPLACEMENT m_sNotificWndPlacement;
};
