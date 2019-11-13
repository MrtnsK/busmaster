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
 * \file      BUSMASTER.cpp
 * \brief     CCANMonitorApp class implementation file
 * \authors   Amitesh Bharti, Ratnadip Choudhury, Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CCANMonitorApp class implementation file
 */
#include "stdafx.h"
#include "afxwinappex.h"
#include <initguid.h>
#include "HashDefines.h"
#include "common.h"
#include "SectionNames.h"
#include "Datatypes/ProjConfig_DataTypes.h"
#include "BUSMASTER.h"
#include "MainFrm.h"
#include "MessageAttrib.h"
#include "Splash.h"             // splash screen implementation file
#include "Replay/Replay_Extern.h"
#include "ConfigData.h"
//#include "ConfigAdapter.h"
#include "InterfaceGetter.h"

#include "BusmasterDump.h"
#include "BUSMASTER_Interface.h"
#include "BUSMASTER_Interface.c"
#include "../Application/MultiLanguage.h"
#include "Utility\MultiLanguageSupport.h"
#include "AboutDlg.h"
//extern DWORD GUI_dwThread_MsgDisp;
extern BOOL g_bStopSendMultMsg;
extern BOOL g_bStopKeyHandlers;
extern BOOL g_bStopErrorHandlers;
extern BOOL g_bStopSelectedMsgTx;
extern BOOL g_bStopMsgBlockTx;

// To Kill Message Handler Threads
extern void gvStopMessageHandlerThreads();

#define defMAX_RECENTFILE_LIST		5
#define defSECTION_MRU           "MRU List"
#define defSECTION_MRU_FILE		 "MRU File"

BEGIN_MESSAGE_MAP(CCANMonitorApp, CWinApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    /* Standard print setup command */
    ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND_EX_RANGE(ID_FILE_MRU_FILE1, ID_FILE_MRU_FILE16, OnOpenRecentFile)
END_MESSAGE_MAP()

CCANMonitorApp::CCANMonitorApp()
{
    m_pouFlags = nullptr;
    m_bIsMRU_CreatedInOpen = FALSE;
    m_bFromAutomation = FALSE;
    GetCurrentDirectory(MAX_PATH, m_acApplicationDirectory);
    m_bIsConfigFileLoaded = false;
}

CCANMonitorApp theApp;

static const CLSID clsid =
{ 0xc4eff9ad, 0x8f4b, 0x42bf, { 0xb6, 0xf5, 0x2f, 0xb3, 0xcd, 0x70, 0xa2, 0x2b } };

const GUID CDECL BASED_CODE _tlid =
{ 0xf710b25d, 0x7abf, 0x4545, { 0xa9, 0x69, 0x53, 0x74, 0xc3, 0xec, 0x8b, 0x8a } };

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

static HINSTANCE ghLangInst=nullptr;

BOOL CCANMonitorApp::InitInstance()
{
    // Begin of Multiple Language support
    if ( CMultiLanguage::m_nLocales <= 0 )    // Not detected yet
    {
        CMultiLanguage::DetectLangID(); // Detect language as user locale
        CMultiLanguage::DetectUILanguage();    // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH];        // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
    {
        ASSERT(false);
    }
    // Load resource-only language DLL. It will use the languages
    // detected above, take first available language,
    // or you can specify another language as second parameter to
    // LoadLangResourceDLL. And try that first.
    ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (ghLangInst)
    {
        AfxSetResourceHandle( ghLangInst );
    }
    // End of Multiple Language support

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

    //InitCommonControls();
    // START CHANGES MADE FOR AUTOMATION
	CWinAppEx::InitInstance();

    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(_("Fail to Intilaize OLE"));
        return FALSE;
    }
    //CoInitializeEx(nullptr, COINIT_MULTITHREADED );

    CBusmasterDump dump("BUSMASTER");
    // END CHANGES MADE FOR AUTOMATION
    // Enable OLE/ActiveX objects support
    AfxEnableControlContainer();
	InitContextMenuManager();
	InitKeyboardManager();
	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need. DEBUG
//#ifdef _AFXDLL
//    Enable3dControls();         // Call this when using MFC in a shared DLL
//#else
//    Enable3dControlsStatic();   // Call this when linking to MFC statically
//#endif
    // Change the registry key under which our settings are stored.
    // TODO: You should modify this string to be something appropriate
    // such as the name of your company or organization.
    SetRegistryKey("RBEI-ETAS\\BUSMASTER_v3.1.0");
    // START CHANGES MADE FOR AUTOMATION
    COleTemplateServer::RegisterAll();
    // END CHANGES MADE FOR AUTOMATION
    //LoadStdProfileSettings(0); // Load standard INI file options (including MRU)
    // Enable drag/drop open
    // Enable DDE Execute open
    //EnableShellOpen();
    //RegisterShellFileTypes(TRUE);
    // Display splash screen
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);
    short shRegServer = -1;
    short shUnRegServer = -1;

    if (__argc > 1)
    {
        shRegServer = (short) strcmpi(__targv[1],"/regserver");
        shUnRegServer = (short) strcmpi(__targv[1],"/unregserver");
    }

    // Don't display a new MDI child window during startup
    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew
            || cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
    {
        cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
    }

    // START CHANGES MADE FOR AUTOMATION

    if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
    {
        m_bFromAutomation = TRUE;
        //      return TRUE;
    }
    else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
    {
        AfxOleUnregisterTypeLib(LIBID_CAN_MonitorApp);
    }
    else
    {
        COleObjectFactory::UpdateRegistryAll();
        AfxOleRegisterTypeLib(AfxGetInstanceHandle(), LIBID_CAN_MonitorApp);
    }

    if (  shRegServer == 0  || shUnRegServer == 0 ) //If command line argument match
    {
        return FALSE;
    }

    if (!m_bFromAutomation)
    {
        CSplashScreen::ActivateSplashScreen(cmdInfo.m_bShowSplash);
    }

    // Allocate memory for CFlags
    m_pouFlags = &(CFlags::ouGetFlagObj());
    // create main MDI Frame window
    CMainFrame* pMainFrame = new CMainFrame;

    if ( pMainFrame == nullptr )
    {
        ::PostQuitMessage(0);
        return FALSE;
    }

    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
    {
        return FALSE;
    }

    m_pMainWnd = pMainFrame;

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
    {
        return FALSE;
    }

    m_pMainWnd->DragAcceptFiles();
    // show main frame
    m_pMainWnd->ShowWindow(m_nCmdShow);
    m_pMainWnd->UpdateWindow();
    //// Create message window



    if(m_pouMsgSgInactive == nullptr )
    {
        if(m_bFromAutomation==FALSE)
            MessageBox(nullptr,_(MSG_MEMORY_CONSTRAINT),
                       "BUSMASTER", MB_OK|MB_ICONINFORMATION);

        ::PostQuitMessage(0);
    }

    BOOL bResult;
    bResult = m_aomState[UI_THREAD].SetEvent();
    // get the information of the last used configuration file..
    // initialize the flag that indicates if the configuratin file has been
    // loaded..
    m_bIsConfigFileLoaded = FALSE;
    CString ostrCfgFilename = "";

    // If user has double click the .cfg file then assign that file name else
    // read from registry.
    if(cmdInfo.m_strFileName.IsEmpty() == TRUE)
    {
        //ostrCfgFilename =
        //    GetProfileString(_(SECTION), defCONFIGFILENAME, "");
        DWORD dwVal;
        bReadFromRegistry(HKEY_CURRENT_USER, _(SECTION), defCONFIGFILENAME, REG_SZ, ostrCfgFilename, dwVal);
    }
    else
    {
        ostrCfgFilename = cmdInfo.m_strFileName;
    }

    BOOL bValidDir = TRUE;
    CFileFind findFile;
    if (!ostrCfgFilename.IsEmpty() && !findFile.FindFile(ostrCfgFilename))
    {
        DWORD dwErr = GetLastError();
        CString strMsg = "";
        if (dwErr == ERROR_PATH_NOT_FOUND)
        {
            bValidDir = FALSE;
            FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, dwErr, 0, strMsg.GetBuffer(1024), 1024, nullptr);
            AfxMessageBox(strMsg);
        }
    }
    if(ostrCfgFilename.IsEmpty() == FALSE && bValidDir == TRUE)
    {
        bInitialiseConfiguration(m_bFromAutomation);

        // load the configuration information
        if(pMainFrame->nLoadConfigFile(ostrCfgFilename) != defCONFIG_FILE_SUCCESS)
        {
            //m_oConfigDetails.vInitDefaultValues();
            m_ostrConfigFilename = "";
        }
        else
        {
            m_ostrConfigFilename = ostrCfgFilename;
        }

        m_bIsConfigFileLoaded = TRUE;

        //build all nodes -- Node Simulation.
        pMainFrame->BuildAllNodes();

    }
    else
    {
        BOOL bReturn = bInitialiseConfiguration(m_bFromAutomation);

        if(bReturn == FALSE )
        {
            ::PostQuitMessage(0);
        }

        // Load a default database file
        //CStringArray omDatabaseArray;
        //CString omSampleDatabasePath;
        //omSampleDatabasePath.Format("%s\\Samples\\SampleDB.dbf",m_acApplicationDirectory);
        //DWORD dRetVal = pMainFrame->dLoadDataBaseFile(omSampleDatabasePath, FALSE);

        //if (dRetVal == S_OK)
        //{
        //    //omDatabaseArray.Add(omSampleDatabasePath);
        //    //Store in configdetails
        //    //bSetData(DATABASE_FILE_NAME, &omDatabaseArray);
        //    bWriteIntoTraceWnd(_(MSG_DEFAULT_DATABASE));
        //    bWriteIntoTraceWnd(_(MSG_CREATE_UNLOAD_DATABASE));
        //}
        pMainFrame->OnHex_DecButon();           // setting HEX by default
    }

    // ********  Filter workaround  ********
    // Filter list is initialised before msg wnd creation. So update display
    // filter here
    // Update Message Display Filter List
    //::PostThreadMessage(GUI_dwThread_MsgDisp, TM_UPDATE_FILTERLIST, nullptr, nullptr );
    // ********  Filter workaround  ********

    // Start Logging if is enabled
    // Get the Flag Pointer
    CFlags* pomFlag =  pouGetFlagsPtr();

    if( pomFlag != nullptr )
    {
        // Get the Logging Status
        BOOL bLogON = pomFlag->nGetFlagStatus(LOGTOFILE);

        // If it is on then post a message to display thread to start logging
        if(bLogON == TRUE )
        {
            // Start Logging
            //CLogManager::ouGetLogManager().vStartStopLogging( TRUE );
        }
    }
    //pMainFrame->OnHex_DecButon();           // setting HEX by default
    //CExecuteManager::ouGetExecuteManager().vStartDllReadThread();
    return TRUE;
}
void CCANMonitorApp::ReadRecentFileList()
{
	LoadStdProfileSettings( defMAX_RECENTFILE_LIST );
}
void CCANMonitorApp::AddToRecentFileList(LPCTSTR lpszPathName)
{
    if (nullptr == lpszPathName)
    {
        return;
    }
    CString ext = PathFindExtension(lpszPathName);
    ext.MakeLower();
    if (ext == defVALIDEXTN)
    {
        if (nullptr != m_pRecentFileList)
        {
            m_pRecentFileList->Add(lpszPathName);
        }
    }
}

bool CCANMonitorApp::bWriteIntoRegistry(HKEY /* hRootKey */, CString strSubKey, CString strName, BYTE bytType, CString strValue, DWORD dwValue)
{
	HKEY hKey;
	DWORD dwDisp = 0;
	LPDWORD lpdwDisp = &dwDisp;
	CString strCompleteSubKey;
	strCompleteSubKey.Format("Software\\RBEI-ETAS\\BUSMASTER_v%d.%d.%d\\%s", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, strSubKey);
	LONG iSuccess = RegCreateKeyEx(HKEY_CURRENT_USER, strCompleteSubKey, 0L, nullptr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, nullptr, &hKey, lpdwDisp);
	LSTATUS ls = 0;
	if (iSuccess == ERROR_SUCCESS)
	{
		if (bytType == REG_SZ)
		{
			ls = RegSetValueEx(hKey, strName.GetBuffer(MAX_PATH), 0L, REG_SZ, (CONST BYTE*) strValue.GetBuffer(MAX_PATH), strValue.GetLength());
		}
		else if (bytType == REG_DWORD)
		{
			ls = RegSetValueEx(hKey, strName.GetBuffer(MAX_PATH), 0L, REG_DWORD, (CONST BYTE*) &dwValue, sizeof(dwValue));
		}
		RegCloseKey(hKey);
		return true;
	}
	else
	{
		return false;
	}
}

bool CCANMonitorApp::bReadFromRegistry(HKEY hRootKey, CString strSubKey, CString strName,  DWORD dwType, CString& strValue , DWORD& dwValue)
{
    DWORD cbData;
    LONG lError = 0;
    HKEY hKey;
    BYTE acRegValue[1024] = {0};
    DWORD dwSize = sizeof(BYTE[1024]) ;

    CString strCompleteSubKey;
    strCompleteSubKey.Format("Software\\RBEI-ETAS\\BUSMASTER_v%d.%d.%d\\%s",VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,strSubKey);
	strValue = "";
    lError = RegOpenKeyEx( hRootKey, strCompleteSubKey, 0, KEY_READ, &hKey);
    // If the registry key open successfully, get the value in "path"
    // sub key
    if(lError==ERROR_SUCCESS)
    {
        if ( dwType == REG_SZ )
        {
            lError = RegQueryValueEx(hKey,strName,0, &dwType, acRegValue,&dwSize);
            strValue.Format("%s", acRegValue);
        }
        else if ( dwType == REG_DWORD )
        {
            lError = RegQueryValueEx(hKey,strName,0, &dwType, (LPBYTE)&dwValue,&cbData);
        }

        RegCloseKey(hKey);
        return true;
    }
    else
    {
        return false;
    }
}

void CCANMonitorApp::WinHelp(DWORD dwData, UINT nCmd)
{
    CWinApp::WinHelp(dwData,  nCmd) ;
}

int CCANMonitorApp::ExitInstance()
{
    unloadInternationalizationLibrary();

    if (ghLangInst)
    {
        FreeLibrary( ghLangInst );
    }


    if (m_pouMsgSignal != nullptr )
    {
        m_pouMsgSignal->bDeAllocateMemory("");
        delete m_pouMsgSignal;
        m_pouMsgSignal = nullptr;
    }

    // if the user directly closes the appln when the database
    // is opened,
    // Delete memory associated with the in-active data structure.
    if ( m_pouMsgSgInactive != nullptr )
    {
        m_pouMsgSgInactive->bDeAllocateMemoryInactive();
        delete m_pouMsgSgInactive;
        m_pouMsgSgInactive = nullptr;
    }

    DWORD dwResult = WaitForSingleObject(m_aomState[UI_THREAD], MAX_TIME_LIMIT);

    switch (dwResult)
    {
        case WAIT_ABANDONED:
            break;

        case WAIT_OBJECT_0:
            break;

        case WAIT_TIMEOUT:
            break;

        default:
            break;
    }

    CoUninitialize();
	return CWinAppEx::ExitInstance();
}


void CCANMonitorApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

/**
 * \brief Return m_pouFlags data member variable value
 * \return Pointer to CFlags
 *
 * Return m_pouFlags data member variable value
 */
CFlags* CCANMonitorApp::pouGetFlagsPtr()
{
    return m_pouFlags;
}

/**
 * \brief assign the value passed as parameter to m_dwHelpID
 *
 * assign the value passed as parameter to m_dwHelpID
 */
void CCANMonitorApp::vSetHelpID(DWORD dwHelpID)
{
    m_dwHelpID = dwHelpID;
}

CWnd* CCANMonitorApp::GetMainWnd()
{
    // TODO: Add your specialized code here and/or call the base class
    return CWinApp::GetMainWnd();
}


/**
 * \brief wrapper around the method CConfigDetails::bSetData(...)
 * \param eParam enumeration denoting the information that
 *   needs to be stored into the config
 * \param lpVoid pointer where the data should be written
 * \return
 *   TRUE, if the CConfigDetails object successfully updates the information.
 *   FALSE, if any error is encountered while updating the information.
 *
 * This is a wrapper around the method
 * CConfigDetails::bSetData(...). It is planned to use
 * only one global object, theApp as provided by the
 * Wizard.
 */
BOOL CCANMonitorApp::bSetData1(eCONFIGDETAILS /*eParam*/, LPVOID /*lpVoid*/)
{
    return FALSE;//m_oConfigDetails.bSetData(eParam, lpVoid);
}

/**
 * \brief wrapper around CConfigDetails::bGetData(...)
 * \param eParam enumeration denoting the information that
 *   needs to be obtained from the config
 * \param lpVoid source pointer for data
 * \return
 *   TRUE, if the CConfigDetails object successfully obtains the information.
 *   FALSE, if any error is encountered while obtaining the information.
 *
 * This is a wrapper around the method
 * CConfigDetails::bGetData(...). It is planned to use
 * only one global object, theApp as provided by the
 * Wizard.
 */
BOOL CCANMonitorApp::bGetData1(eCONFIGDETAILS /*eParam*/, LPVOID* /*lpData*/)
{
    return FALSE;//m_oConfigDetails.bGetData(eParam, lpData);
}

/**
 * \brief wrapper around CConfigDetails::bRelease(...)
 * \param eParam enumeration denoting the section for which
 *   the memory should be released
 * \param lpDataPtr pointer that should be released
 *
 * This is a wrapper around the method
 * CConfigDetails::bRelease(...). It is planned to use
 * only one global object, theApp as provided by the
 * Wizard.
 */
void CCANMonitorApp::vRelease1(eCONFIGDETAILS /*eParam*/, LPVOID* /*lpDataPtr*/)
{
    //m_oConfigDetails.vRelease(eParam, lpDataPtr);
}

void CCANMonitorApp::vSetFileStorageInfo(CString oCfgFilename)
{
    USES_CONVERSION;
    DATASTORAGEINFO stempDataInfo;
    FILESTORAGEINFO FileStoreInfo;
    strcpy_s(FileStoreInfo.m_FilePath, MAX_PATH, oCfgFilename.GetBuffer(MAX_PATH));
    stempDataInfo.FSInfo = &FileStoreInfo;
    stempDataInfo.m_Datastore = FILEMODE;
    CConfigData::ouGetConfigDetailsObject().SetConfigDatastorage(&stempDataInfo);
    CConfigData::ouGetConfigDetailsObject().vSetCurrProjName(_(DEFAULT_PROJECT_NAME));
    CMainFrame* pMainFrame = static_cast<CMainFrame*> (m_pMainWnd);

    if (pMainFrame != nullptr)
    {
        pMainFrame->vPushConfigFilenameDown(oCfgFilename);
    }
}

void CCANMonitorApp::GetLoadedConfigFilename(CString& roStrCfgFile)
{
    roStrCfgFile = m_ostrConfigFilename;
}

BOOL CCANMonitorApp::OnOpenRecentFile(UINT nID)
{
	CMainFrame* pMainFrame = static_cast<CMainFrame*> (m_pMainWnd);
	int index = nID - ID_FILE_MRU_FILE1;
	if (index >= 0 && index < m_pRecentFileList->m_nSize)
	{
		pMainFrame->OnClickMruList(m_pRecentFileList->m_arrNames[index]);
	}
	return TRUE;
}
/**
 * \brief message handler for ID_FILE_OPEN
 *
 * This is a message handler for ID_FILE_OPEN
 * This function will be called when user selects
 * File -> Function Editor -> Open menu option.
 * Displays open file dialog initialised with *.c filter,
 * and the previous selected C file.
 * Opens the selected file if the file is found and
 * saves back the selected file into the configuration
 */
void CCANMonitorApp::OnFileOpen()
{
    // Display open dialog box with *.c filter
    // and select the C file by default
    CFileDialog fileDlg( TRUE,      // Open File dialog
                         "c",       // Default Extension,
                         m_omMRU_C_FileName,
                         OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                         _("C File(s)(*.c)|*.c||"),
                         nullptr );
    // Set Title
    fileDlg.m_ofn.lpstrTitle  = _("Select BUSMASTER Source Filename...");

    if ( IDOK == fileDlg.DoModal() )
    {
        CString strExtName  = fileDlg.GetFileExt();
        CString omStrNewCFileName   = fileDlg.GetPathName();

        if ( omStrNewCFileName.ReverseFind('.') )
        {
            omStrNewCFileName = omStrNewCFileName.
                                Left( omStrNewCFileName.ReverseFind('.') + 1);
            omStrNewCFileName.TrimRight();
            omStrNewCFileName += strExtName;
        }

        // file-attribute information
        struct _finddata_t fileinfo;

        // Check if file exists
        if (_findfirst( omStrNewCFileName.GetBuffer(MAX_PATH), &fileinfo) != -1L)
        {
            // Now open the selected file
            CMainFrame* pMainFrame = static_cast<CMainFrame*> (m_pMainWnd);
            if(pMainFrame != nullptr)
            {
                GetICANNodeSim()->FE_OpenFunctioneditorFile(omStrNewCFileName, pMainFrame->GetSafeHwnd(),
                        pMainFrame->m_sExFuncPtr[CAN]);
            }
            // Save the selected filename into the configuration details
            // if it is is not the same C file

            // Since opening of the document
            // loads another menu,
            // the menu created for the MRU
            // would have destroyed. So create
            // the same.
            // And this should be created only once.
            
        }
        else
        {
            MessageBox(nullptr,_("Specified filename not found!"),
                       "BUSMASTER",MB_OK|MB_ICONINFORMATION);
        }
    }
}

/**
 * \brief Displays message for the error code
 *
 * Displays an appropriate message for the error code
 * passed to this method. The message is user friendly.
 */
void CCANMonitorApp::vDisplayConfigErrMsgbox(UINT unErrorCode,
        BOOL bOperation)
{
    CString omStrSuffixMessage("");

    if ( bOperation == defCONFIG_FILE_LOADING )
    {
        omStrSuffixMessage = _(" while loading.\nOperation unsuccessful.");
    }
    else if ( bOperation == defCONFIG_FILE_SAVING )
    {
        omStrSuffixMessage = _(" while saving.\nOperation unsuccessful.");
    }
    else
    {
        omStrSuffixMessage = _(".\nOperation unsuccessful.");
    }

    // Get actual error message
    switch(unErrorCode)
    {
        case defCONFIG_FILE_ERROR:
        {
            m_omConfigErr = _("File error occured");
        }
        break;

        case defCONFIG_FILE_NOT_FOUND:
        {
            m_omConfigErr = _("Configuration file not found");
        }
        break;

        case defCONFIG_FILE_OPEN_FAILED:
        {
            m_omConfigErr = _("Unable to open configuration file");
        }
        break;

        case defCONFIG_FILE_READ_FAILED:
        {
            m_omConfigErr = _("Unable to read configuration file");
        }
        break;

        case defCONFIG_FILE_WRITE_FAILED:
        {
            m_omConfigErr = _("Unable to write into configuration file");
        }
        break;

        case defCONFIG_FILE_CLOSE_FAILED:
        {
            m_omConfigErr = _("Unable to close configuration file successfully");
        }
        break;

        case defCONFIG_FILE_INVALID_FILE_EXTN:
        {
            m_omConfigErr = _("Invalid configuration file extension found");
        }
        break;

        case defCONFIG_PATH_NOT_FOUND:
        {
            m_omConfigErr = _("Configuration file path not found");
        }
        break;

        case defCONFIG_FILE_ACCESS_DENIED:
        {
            m_omConfigErr = _("Configuration file access was denied");
        }
        break;

        case defCONFIG_FILE_HANDLE_INVALID:
        {
            m_omConfigErr = _("Invalid file handle obtained");
        }
        break;

        case defCONFIG_DRIVE_NOT_FOUND:
        {
            m_omConfigErr = _("Specified drive not found");
        }
        break;

        case defCONFIG_FILE_CORRUPT:
        {
            m_omConfigErr = _("An attempt\
 to edit the file has been made from outside the application.\n\
Corrupt configuration file found");
        }
        break;

        case defCONFIG_FILE_HDR_CORRUPT:
        {
            m_omConfigErr = _("Corrupt configuration file header found");
        }
        break;

        default:
        {
            m_omConfigErr = _("Unknown error encountered");
        }
        break;
    }

    m_omConfigErr += omStrSuffixMessage;

    //UINT unMsgboxType = MB_OK | MB_ICONEXCLAMATION;
    if(m_bFromAutomation==FALSE)
    {
        bWriteIntoTraceWnd(m_omConfigErr.GetBuffer(MAX_PATH));
        //MessageBox(nullptr, m_omConfigErr, _("BUSMASTER"), unMsgboxType);
    }
}


/**
 * \brief message handler for ID_FILE_NEW
 *
 * This is a message handler for ID_FILE_NEW
 * This function will be called when user selects
 * File -> Function Editor -> New  menu option.
 * Creates the MRU menu only once
 */
void CCANMonitorApp::OnFileNew()
{
    BOOL bOneChildWndOpen = FALSE;
    bOneChildWndOpen = m_pouFlags->nGetFlagStatus(FUNCEDITOR);

    if(bOneChildWndOpen != TRUE )
    {
        struct _tfinddata_t fileinfo;
        // Find if the current directory has some .c file already created.
        // Check if it has name "NewEdn" if yes, the new file name will be
        // NewEdx where value of x = n+1;
        char cBuffer[_MAX_PATH];
        CString omStrCFileName;
        CString strFilePath;
        _getcwd( cBuffer, _MAX_PATH );
        BOOL bStop = FALSE;
        UINT unCount = 0;

        while (bStop == FALSE)
        {
            omStrCFileName.Format("%s%d%s", "NewEd", ++unCount, ".c");

            // Search for the file name and if it is not present, set
            // the flag to TRUE to break the loop.
            if (_tfindfirst( omStrCFileName.GetBuffer(MAX_PATH), &fileinfo) == -1L)
            {
                strFilePath = cBuffer ;
                strFilePath += "\\"+ omStrCFileName ;
                bStop = TRUE;
            }
        }

        CMainFrame* pMainFrame = static_cast<CMainFrame*> (m_pMainWnd);
        if(pMainFrame != nullptr)
        {
            GetICANNodeSim()->FE_OpenFunctioneditorFile(strFilePath, pMainFrame->GetSafeHwnd(),
                    pMainFrame->m_sExFuncPtr[CAN]);
        }
        //CWinApp::OnFileNew();
        // Since creating of the document
        // loads another menu,
        // the menu created for the MRU
        // would have destroyed. So create
        // the same.
        // And this should be created only once.

        
    }
}


/**
 * \brief initialises user selection
 * \return TRUE or FALSE
 *
 * This method will initialise user selection from
 * a configuration module to respective module.
 */
BOOL CCANMonitorApp::bInitialiseConfiguration(BOOL bFromCom)
{
    BOOL bReturn = TRUE;
    CMainFrame* pMainFrame = static_cast<CMainFrame*> (m_pMainWnd);

    if(pMainFrame != nullptr )
    {
        BOOL bIsDatabaseFoundInConfigFile = FALSE;

        if(m_pouMsgSignal != nullptr)
        {
            m_pouMsgSignal->bDeAllocateMemory("");
        }
        else
        {
            m_pouMsgSignal = new CMsgSignal(sg_asDbParams[CAN], m_bFromAutomation);
        }

        if ( m_pouMsgSignal != nullptr )
        {
            //Get the Database names
            CStringArray aomOldDatabases;
            //To keep all the files which are successfully imported
            CStringArray aomNewDatabases;
            aomNewDatabases.RemoveAll();
            m_pouMsgSignal->vGetDataBaseNames(&aomOldDatabases);
            int nFileCount = aomOldDatabases.GetSize();

            if(nFileCount == 0)
            {
                bIsDatabaseFoundInConfigFile = FALSE;
                // Reset corresponding flag
                m_pouFlags->vSetFlagStatus( SELECTDATABASEFILE, FALSE );
            }
            else
            {
                CString omStrDatabase;
                int nDatabaseNotFound = 0;

                for(int nCount = 0; nCount < nFileCount; nCount++)
                {
                    omStrDatabase  = aomOldDatabases.GetAt(nCount);

                    if (omStrDatabase.IsEmpty())
                    {
                        nDatabaseNotFound++;
                        aomOldDatabases.RemoveAt(nCount);
                        --nCount;
                        --nFileCount;
                    }
                    else
                    {
                        bIsDatabaseFoundInConfigFile = TRUE;
                        // Check if the file really exists
                        struct _finddata_t fileinfo;

                        if (_findfirst(omStrDatabase.GetBuffer(MAX_PATH) ,&fileinfo) == -1L)
                        {
                            CString omStrMsg = _("Database File: ");
                            omStrMsg += omStrDatabase;
                            omStrMsg += _(" not found!");

                            if(bFromCom==FALSE)
                            {
                                MessageBox(nullptr,omStrMsg,"BUSMASTER",MB_OK|MB_ICONERROR);
                            }

                            // Remove the file name from configuration file.
                            nDatabaseNotFound++;
                            aomOldDatabases.RemoveAt(nCount);
                            --nCount;
                            --nFileCount;
                        }
                        else
                        {
                            // Reset corresponding flag
                            m_pouFlags->vSetFlagStatus( SELECTDATABASEFILE, TRUE );
                            m_pouMsgSignal->
                            bFillDataStructureFromDatabaseFile(omStrDatabase, PROTOCOL_UNKNOWN);
                            pMainFrame->vPopulateJ1939PGNList();
                            aomNewDatabases.Add(omStrDatabase);
                        }
                    }
                }

                if(nDatabaseNotFound > 0)
                {
                    BYTE* pbyConfigData = nullptr;
                    UINT unSize = 0;
                    unSize += (sizeof (UINT) + ((sizeof(char) *MAX_PATH) * aomNewDatabases.GetSize()));
                    pbyConfigData = new BYTE[unSize];
                    BYTE* pbyTemp = pbyConfigData;
                    UINT nCount = 0;
                    COPY_DATA(pbyTemp, &nCount, sizeof(UINT));

                    for (UINT i = 0; i < nCount; i++)
                    {
                        char acName[MAX_PATH] = { '\0' };
                        CString omDBName = aomNewDatabases.GetAt(i);
                        strcpy_s(acName, MAX_PATH, omDBName.GetBuffer(MAX_PATH));
                        COPY_DATA(pbyTemp, acName, sizeof(char) * MAX_PATH);
                    }

                    CConfigData::ouGetConfigDetailsObject().bSetData(pbyTemp, unSize, SectionName[DATABASE_SECTION_ID]);
                    delete[] pbyConfigData;
                    pbyConfigData = nullptr;
                }

                if(aomNewDatabases.GetSize()== 0)
                {
                    // Reset the flag and prompt user of file not in disk.
                    m_pouFlags->vSetFlagStatus( SELECTDATABASEFILE, FALSE );
                }
            }
        }
        else
        {
            if(bFromCom==FALSE)
                // Display a message and quit the application
                MessageBox(nullptr,
                           _(MSG_MEMORY_CONSTRAINT),
                           "BUSMASTER",
                           MB_OK|MB_ICONINFORMATION);

            bReturn = FALSE;
        }

        //Finally load the default configuration
        pMainFrame->nLoadConfigFile("");
    }

    return bReturn;
}

/**
 * \brief calls Config Details class member
 * \param eParam Window Identity
 * \param sPosition Reference to Window Placement Structure
 *
 * This function will call Config Details class member to get
 * default window size and position.
 */
BOOL CCANMonitorApp::bGetDefaultValue(eCONFIGDETAILS /*eParam*/,
                                      WINDOWPLACEMENT& /*sPosition*/)
{
    return FALSE;//m_oConfigDetails.bGetDefaultValue( eParam, sPosition );
}


/**
 * \brief writes the text into trace window
 * \param omText Text to be displayed in trace window
 * \return TRUE - Success, FALSE - Failure
 *
 * This function will write the text into trace window.
 * Since this function is asynchronous, caller should not immediately
 * deallocate the omText.
 */
BOOL CCANMonitorApp::bWriteIntoTraceWnd(char* omText, BOOL bTrace)
{
    if(bTrace == FALSE)
    {
        return FALSE;
    }

    BOOL bResult = FALSE;
    CMainFrame* pMainFrame = static_cast<CMainFrame*> (m_pMainWnd);

    if (pMainFrame != nullptr)
    {
        pMainFrame->SendMessage(IDM_TRACE_WND);

        if (pMainFrame->m_podUIThread != nullptr)
        {
            pMainFrame->m_podUIThread->PostThreadMessage(WM_WRITE_TO_TRACE, 0, (LPARAM)omText);
            bResult = TRUE;
        }
    }

    return bResult;
}

INT CCANMonitorApp::COM_nSaveConfiguration(CString omStrCfgFilename)
{
    vSetFileStorageInfo(omStrCfgFilename);
    CConfigData::ouGetConfigDetailsObject().vSaveConfigFile();
    return defCONFIG_FILE_SUCCESS;
}
