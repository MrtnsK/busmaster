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
 * \file      MsgSignal.h
 * \brief     This header file contains the defintion of class
 * \authors   Amarnath Shastry, Rajesh Kumar 04.03.2003
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include <IO.H>             // i/0 header definitions
#include <stdlib.h>         // standard library definitions
#include "HashDefines.h"    // #defines definitions    
#include "TreeItemParam.h"
#include "Datatypes/MsgSignal_Datatypes.h"
#include <list>
//#include "FibexClass/BasePARSER_FIBEX.h"
//#include "FibexClass.h"
#include "CommonDefines.h"
#include "UIThread.h"
#include "IBMNetWorkGetService.h"

#include "NodeSimEx/NodeSimEx_Struct.h"     //CAPL_DB_NAME_CHANGE
//
//typedef HRESULT (__stdcall* GETPARSER_FIBEX)(void** ppvInterface);
//static GETPARSER_FIBEX pfGetParser_FIBEX;

class CMsgSignal
{
public:
    CMsgSignal(const SDBPARAMS& sDbParams, BOOL bInAutoSrvMode);
    virtual ~CMsgSignal();



public:



    struct sDBFileStruct
    {
        CString m_omStrDatabasePath; // Database path
        UINT m_unMessageCount;       // Message count in the database
        sMESSAGE* m_psMessages;      // Pointer to the array of message structure
        sDBFileStruct* m_psNextDBFileStruct;
    };

private:
    SDBPARAMS m_sDbParams;
    // Attributes
    CStringArray m_omDatabaseNames;
    //This data member will contain number of messages of active database.
    static CUIThread* m_pUIThread;
    UINT m_unMessageCount;
    //This data member will contain pointer to sMESSAGE structure that holds active database information.
    sMESSAGE* m_psMessages;
    // This flag indicates if the database is ACTIVE/IN_ACTIVE
    BOOL m_bIsDatabaseActive;
    CString m_strActiveDBFileName;
    //This is pointer to the list of database structures
    sDBFileStruct* m_psDatbaseStructList;
    //CMap to store the all the messages of all the DBs for faster searches
    CMap <UINT, UINT, sMESSAGE*, sMESSAGE*> m_omMsgDetailsIDMap;
    //For shared CMap access and deletion
    CCriticalSection m_omDBMapCritSec;
    // Is application running in automation server mode
    BOOL m_bAutoServerMode;
    // Functions
public:
    static void vSetTraceWndObj(CUIThread* pUIThread);
    //Reset the content of CMap on DB import or dissociate
    void vResetMsgMapContent();
    // get the signal valid range
    void vGetSignalValidRange(CString omStrMessageCode,
                              CString omStrSgName, CString&   omMinValue, CString& omMaxValue);
    // Checks if the signal value is out of range
    BOOL bIsSignalValueOutofRange( CString omStrMessageCode,
                                   CString omStrSgName,
                                   CString omStrSignalValue);
    // Checks if the Signal value is duplicate
    BOOL bIsDuplicateSignalValue( CString omStrMessageCode,
                                  CString omStrSgName,
                                  CString omStrSignalValue,
                                  CString omStrDescription,
                                  INT nIndex,
                                  BOOL bMode);
    // Checks if the description is duplicate
    BOOL bIsDuplicateValueDescription( CString omStrMessgaeCode,
                                       CString omStrSgName,
                                       CString omStrSignalValue,
                                       CString omStrDescription,
                                       INT nIndex,
                                       BOOL bMode);
    // Returns the signal names and their indexes
    // to be deleted which are defined after
    // the specified message length
    void vGetSigNamesAndIndexTobeDeleted( UINT unMessageLength,
                                          CString omStrMsgName,
                                          CStringArray& omStrList,
                                          UINT unIndexes[]);
    void unListGetMessageIDs(UINT* omListId);
    // Returns whether a signal can have new
    // Signal description and value
    BOOL bItemCanHaveSignalDesc( CString omStrMsgName,
                                 CString omStrSgName,
                                 unsigned __int64 un64ItemCount );
    // Updates signal matrix
    void vUpdateSignalMatrix(const BYTE* pSrcByte,
                             BYTE* pDestByte,
                             UINT unArrayLen,
                             BOOL bUpdateType);


    // Returns the value of "m_bIsDatabaseSaved" flag
    BOOL bGetModifiedFlag();

    // sets the value of "m_bIsDatabaseSaved" flag
    void vSetModifiedFlag(BOOL bSaved);

    // Gives a list of message names
    void omStrListGetMessageNames(CStringList& );
    // Gives a list of message names for editing database
    void omStrListGetMessageNamesInactive(CStringList& );

    // Set and get active DB flag status
    void vSetDBActiveFlagStatus(BOOL bValue);
    BOOL bGetDBAcitveFlagStatus();

    // Get message pointer for given message
    sMESSAGE* psGetMessagePointer( CString strMsgName);
    // Get message pointer for given message if called to change inactive Db
    sMESSAGE* psGetMessagePointerInactive( CString strMsgName);

    // Get message pointer for given message ID
    sMESSAGE* psGetMessagePointer( UINT unMsgID);
    // Get message pointer for given message ID for inactive DB
    sMESSAGE* psGetMessagePointerInactive( UINT unMsgID);
    // Convert Extended To Standard Frame Format
    void vConvertExtendedToStandardFrameFormat(int&);

    // Convert Standard To Extended Frame Format
    void vConvertStandardToExtendedFrameFormat(int&);

    // Delete signal description and value
    BOOL bDeleteSgDescVal( int nIndexOfItem,
                           CString strMsgName,
                           CString strSgName,
                           CString omStrDesc);

    // Update signal description and value
    BOOL bUpdateSgDescVal( CString strMsgCode,
                           CString strSgName,
                           CString omStrPrevDesc,
                           CSignalDescVal*);

    // Add signal description and value
    BOOL bAddSgDescVal( CString strMsgCode, CString strSgName);

    // Updates signal details
    void vUpdateSignalDetails ( CString strMsgName,
                                CString omStrSignalName,
                                sSIGNALS*);

    // Deletes signal
    BOOL bDeleteSignalFromMsg( int nIndexOfWhichItem,
                               CString strMsgName,
                               CString omStrSignalName);

    // Add signal
    BOOL bAddSignalToMsg( CString strMsgName );

    // Delete message
    BOOL bDeleteMsg( CString );

    // Update message details
    BOOL bUpdateMsg( eMSG_CHANGES_IN eNum, CString, sMESSAGE* );

    // Add message
    BOOL bAddMsg();

    // Return no of messages
    UINT unGetNumerOfMessages();

    // Save into DB file
    BOOL bWriteIntoDatabaseFileFromDataStructure( CString strFileName, eProtocol eProtocolName = PROTOCOL_CAN);

    // Read from the DB file
    BOOL bFillDataStructureFromDatabaseFile( CString strFileName, eProtocol eProtocolName = PROTOCOL_CAN);
    // Get all signal names
    void vGetSignalNames( UINT unMsgID, CStringList& strSignalList);

    // Get index from message name
    int nGetMessageIndexFromMsgName( CString strMsgName);

    // Create new DB
    BOOL bCreateDataBase(CString strFilename);

    // Return message name from code
    CString omStrGetMessageNameFromMsgCode( UINT unMsgCode);

    // Return message name from code when called from Inactive Database
    CString omStrGetMessageNameFromMsgCodeInactive( UINT unMsgCode);

    BOOL bMessageLengthFromMsgCode(UINT unMsgCode, CString& omMsgLength);
    // Get message code from name
    int nGetMessageCode( CString strMsgName);

    // Free reserved memory
    BOOL bDeAllocateMemory(CString strDBName);
    //Delete the memory allocated to a particular Database
    BOOL bDeAllocateDBMemory(sDBFileStruct* psDatbaseStructList);
    //Delete the memory allocated to a Inactive Database
    BOOL bDeAllocateMemoryInactive();
    // Return the array of DB file
    void vGetDataBaseNames(CStringArray* pastrDBnames);
    void vGetRelativeDataBaseNames(std::string& omStrBasePath, CStringArray* pastrDBnames);
    void vSetDataBaseNames(const CStringArray* pastrDBnames);
    void bAddDbNameEntry(const CString& omDbFileName);
    // Validate dupliacte start bit value
    BOOL bIsDuplicateSignalStartBitValue( CString omStrMsgName,
                                          UINT unByteNum,
                                          UINT unSignalLength,
                                          UINT unStartBitValue,
                                          BOOL bDataFormat);

    // Validate duplicate message code
    BOOL bIsDuplicateMessageCode( CString omStrMsgName,
                                  UINT unMsgCode);

    // Validate dupliacte signal name
    BOOL bIsDuplicateSignalName( CString omStrMsgName, CString omStrSgName);

    // Validate dupliacte message name
    BOOL bIsDuplicateMessageName( int nMsgId,CString strMsgName);


    static BOOL bCalcBitMaskForSig(BYTE* pbyMaskByte, UINT unArrayLen,
                                   UINT nByteNum, UINT nBitNum, UINT nLength, eEndianess bDataFormat );
    static BOOL bValidateSignal(UINT nDLC, UINT nByteNum, UINT nBitNum,
                                UINT nLength, eEndianess bDataFormat );

    //CAPL_DB_NAME_CHANGE
    //fill database list
    BOOL bFillDbStructure(CMsgNameMsgCodeListDataBase& odMsgNameMsgCodeListDB);


private:
    BOOL bFormSigNameAndLength( UINT* punLength,
                                UINT* punStartBit,
                                CStringArray& omStrArraySigName,
                                const INT nIndex );
    BOOL bSortSignalStartBitAscend(UINT* punSigStartBit ,UINT unCount);
    BOOL m_bIsDatabaseSaved;

    // Reserves memory for new message
    BOOL bAllocateMemory(BOOL nMode);

    // Validate Db file
    BOOL bValidateDatabaseFile(CString strFileName);

    BOOL bInsertBusSpecStructures(CStdioFile& omHeaderFile,
                                  CString& omStrcommandLine,
                                  CStringArray& omStrArraySigName,
                                  sMESSAGE* pMsg);
    static void vWriteTextToTrace();
    BOOL bFormSigNameAndLengthJ1939(const UINT* punStartBit,
                                    CStringArray& omStrArraySigName,
                                    const INT nIndex );
};

void vReplaceChar(char str[], char chOld, char chNew);
