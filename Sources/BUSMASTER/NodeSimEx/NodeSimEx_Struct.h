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
 * \file      NodeSimEx_Struct.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#if !defined NODESIMEX_STRUCT_H__INCLUDED_
#define NODESIMEX_STRUCT_H__INCLUDED_

typedef int  (*TRANSMIT_MSG)(void*, HMODULE);
typedef bool (*EN_DIS_ABLE_LOG)(bool);
typedef bool (*DISCONNECT_TOOL)(bool);
typedef void (*RESET_CONTROLLER)(bool);
typedef bool (*WRITE_TO_LOG)(char*);

enum
{
    LOG_ENABLE_DISABLE = 0,
    LOG_ENABLE_DISABLE_LIN,
    LOG_ENABLE_DISABLE_FLEXRAY,
    DIS_CONNECT,
    WRITE_TO_LOGFILE,
    RESET_HARDWARE_CONTROLLER,
    WRITE_TO_LOGFILE_J1939,
    WRITE_TO_LOGFILE_LIN,
    LIN_DIS_CONNECT,
};

//USER_DLL MESSAGES
#define WM_FROM_USER_DLL            (WM_USER + 40)
#define WM_GET_ABSOLUTE_TIME         WM_USER + 52
// Definition of PSTOOLBARINFO structure is here
typedef enum
{
    TB_LOAD,
    TB_UNLOAD,
    TB_BUILD,
    TB_BUILDLOAD,
    TB_SAVE_FILE
} E_TB_FN_EDITOR;

typedef enum eUpdateType
{
    UPDATE_ALL = 0,
    UPDATE_DATABASE_MSGS,
    UPDATE_UNIONS_HEADER_FILES
} E_UPDATE_TYPE;

typedef struct tagMsgNameMsgCode
{
    CString         m_omMsgName;
    DWORD           m_dwMsgCode;
    UINT            m_unMsgLen;
    CStringList     m_omSignalNames;


    tagMsgNameMsgCode()
    {
        m_unMsgLen = 8;
        m_dwMsgCode = 0;
        m_omSignalNames.RemoveAll();
    }

    tagMsgNameMsgCode& operator=(tagMsgNameMsgCode& RefObj)
    {
        m_dwMsgCode = RefObj.m_dwMsgCode;
        m_omMsgName = RefObj.m_omMsgName;
        m_unMsgLen  = RefObj.m_unMsgLen;
        m_omSignalNames.RemoveAll();
        m_omSignalNames.AddTail(&(RefObj.m_omSignalNames));

        return *this;
    }
    /* This is done to improve the performance of the Find function */
    BOOL operator==(const tagMsgNameMsgCode& RefObj) const
    {
        return ((RefObj.m_dwMsgCode == m_dwMsgCode) || (RefObj.m_omMsgName == m_omMsgName));
    }

} SMSG_NAME_CODE;

typedef CList<SMSG_NAME_CODE, SMSG_NAME_CODE&> CMsgNameMsgCodeList;
typedef CList<SMSG_NAME_CODE, SMSG_NAME_CODE&> CMsgNameMsgCodeList;
//CAPL_DB_NAME_CHANGE
typedef struct tagDbNameMsg
{
    bool                        m_bAssociated;
    CString                     m_omDbName;
    CMsgNameMsgCodeList         m_oMsgNameMsgCodeList;
    tagDbNameMsg()
    {
        m_omDbName = "";
        m_bAssociated = true;
    }
    tagDbNameMsg& operator=(tagDbNameMsg& RefObj)
    {
        m_omDbName = RefObj.m_omDbName;
        m_oMsgNameMsgCodeList.AddTail(&RefObj.m_oMsgNameMsgCodeList);
        return *this;
    }


} SDB_NAME_MSG;

typedef CList<SDB_NAME_MSG, SDB_NAME_MSG&> CMsgNameMsgCodeListDataBase;

class CBaseAppServices;
typedef struct
{
    HWND                m_hWmdMDIParentFrame;
    //CUIThread*          m_pouTraceWnd;
    //CNetwork_McNet*     m_pNetWorkMcNet;
    CStringArray        m_omAPIList; // Function List
    CStringArray        m_omAPINames;//Function API Names
    CString             m_omObjWrapperName; // Wrapper_<BUS>.o
    CString             m_omStructName; // STCAN_MSG or STMCNET_MSG
    CString             m_omStructFile; // struct_<BUS>.h
    CString             m_omAppDirectory;
    CStringArray        m_omDefinedMsgHeaders; // _unions.h
    CStringArray        m_omErrorHandlerList;
    CMsgNameMsgCodeListDataBase m_odMsgNameMsgCodeListDB;   //CAPL_DB_NAME_CHANGE
    CBaseAppServices*   m_pouITraceWndPtr;
    CMapStringToPtr*    m_podNodeToDllMap;
    //CKeyPanelEntryList* m_podKeyPanelEntryList;
    TRANSMIT_MSG        Send_Msg;
    EN_DIS_ABLE_LOG     EnDisableLog;
    DISCONNECT_TOOL     DisConnectTool;
    RESET_CONTROLLER    RestController;
    WRITE_TO_LOG        WriteToLog;

} S_EXFUNC_PTR, *PS_EXFUNC_PTR;

#endif //NODESIMEX_STRUCT_H__INCLUDED_
