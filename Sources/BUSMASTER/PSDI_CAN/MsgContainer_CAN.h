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
 * \file      MsgContainer_CAN.h
 * \brief     Definition of CMsgContainerCAN class
 * \authors   Anish Kumar, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CMsgContainerCAN class
 */

#pragma once

#include "afxtempl.h"
#include "Utility/Utility_Thread.h"
#include "Application/MsgWndHdrInfo.h"
#include "CommonClass/GUI_FormatMsgCAN.h"
#include "include/BaseDefs.h"
//#include "DataTypes/MsgBufAll_DataTypes.h"
#include "MsgBufCANVFSE.h"
#include "MsgBufVVSE.h"
#include "DataTypes/Filter_Datatypes.h"
#include "CommonClass/RefTimeKeeper.h"
#include "CommonClass/MsgContainerBase.h"
#include "BaseDIL_CAN.h"
//#include "DIL_Interface_extern.h"
#include "UDS_Protocol/UDS_Extern.h"
#include "MsgBufFSE.h"

typedef CMsgBufCANVFSE<STCANDATA> CCANBufVFSE;
typedef void (*MSG_RX_CALL_BK)(void* pParam, ETYPE_BUS eBusType);

// To save delta time in case of append mode
typedef struct stCanDataSpl : public STCANDATA
{
    __int64      m_nDeltime;

    stCanDataSpl()
    {
        m_nDeltime = 0;
    }
} STCANDATASPL;

typedef CMsgBufCANVFSE<STCANDATASPL> CMsgBufCANVFSEspl;

class CMsgContainerCAN: public CMsgContainerBase
{
private:
    CMsgBufFSE<STCANDATA>   m_ouMCCanBufFSE;
    CCANBufVFSE             m_ouOWCanBuf;
    CMsgBufCANVFSEspl       m_ouAppendCanBuf;
    SFORMATTEDDATA_CAN      m_sOutFormattedData;
    //STCANDATA               m_sCANReadData;
    STCANDATASPL            m_sCANReadDataSpl;
    CFormatMsgCAN           m_ouFormatCAN;
    DWORD                   m_dwClientId;
    CBaseDIL_CAN*           m_pouDIL_CAN_Interface;
    IBMNetWorkGetService*   mBmNetwork;
private:
    //ReadMcNetData virtual function
    void vProcessNewData(STCANDATA& sCanData);
    void vProcessCurrErrorEntry(const SERROR_INFO& sErrInfo);
    BOOL bIsTransitionInState( UINT unChannel,
                               BYTE byRxError,
                               BYTE byTxError );
    BOOL bTobeBlocked(STCANDATA& sCanData);
    USHORT usProcessCurrErrorEntry(SERROR_INFO& sErrInfo);

    CRITICAL_SECTION m_sCritSecDataSync;
    CRITICAL_SECTION m_omCritSecFilter;
    SFILTERAPPLIED_CAN m_sFilterCAN;
    ERROR_STATE m_eCurrErrorState[ defNO_OF_CHANNELS ];
    HRESULT GetInterPretedSignalList(STCAN_MSG& ouMsg, SSignalInfoArray& ouSSignalInfoArray, bool isHexMode);
    HRESULT GetMessageName(STCANDATA&, CString& msgName, bool formatHexForId);
    HRESULT GetErrorInfo(STCANDATA& sCANMsg, CString& msgName);
public:
    CMsgContainerCAN(void);
    ~CMsgContainerCAN(void);
    void InitTimeParams(void);
    void vRetrieveDataFromBuffer();

public:
    //Exported functions
    BOOL bGetDilInterFace();
    void vInit(void* pParam);
    void vEditClearAll();
    int nGetAppendBufferCount();
    int nGetOWBufferCount();
    BOOL bStartReadThread();
    BOOL bStopReadThread();
    //void vSetRelBaseTime(__int64 nTimeStamp);

    void vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
                                     char* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
                                     BYTE bExprnFlag_Disp);
    HRESULT hReadFromOWBuffer(void* psMsg, __int64 nMapIndex);
    HRESULT hReadFromAppendBuffer(void* psMsg, int nMsgIndex);
    HRESULT hUpdateFormattedMsgStruct(int nListIndex, int& nMsgCode,
                                      BYTE bExprnFlag_Disp, __int64 nTimeOffset = 0);
    void vSetCurrMsgName(CString strMsgNameOrCode);
    void vSetMsgLength(CString strMsgLength);
    void vClearFormattedMsgStruct();
    void vSaveOWandGetDetails(void* pMsg, __int64& dwMapIndex,
                              __int64& dwTimeStamp, UINT& nMsgCode, int& nBufferIndex, EINTERPRET_MODE&);


    //void vSetRxMsgCallBkPtr(MSG_RX_CALL_BK pFuncPtr);
    void SetClientID(DWORD dwClientID);
    void DoSortBuffer(int nField,bool bAscending);
    void GetMapIndexAtID(int nIndex,__int64& nMapIndex);
    HRESULT hToggleDILBufferRead(BOOL bRead);
    HRESULT ApplyFilterScheme(void* pvFilterApplied);
    HRESULT GetFilterScheme(void* pvFilterApplied);
    HRESULT EnableFilterApplied(BOOL bEnable);
    __int64 nCreateMapIndexKey( LPVOID pMsgData );
    //Import Log File.
    HRESULT LoadPage(const unsigned long& nPageNo);
    HRESULT OverwritePage(const unsigned long& nLineNo);
    HRESULT SetIBMNetWorkGetService(void* pBmNetWork);
    HRESULT GetSignalInfoArray(__int64 nMapIndex, SSignalInfoArray& SigInfoArray, bool formatInHex);
    HRESULT GetMessageDetails(__int64 nMapIndex, unsigned int& msgId, CString& msgName, bool formatHexForId);
    HRESULT sendMessage(long long key, bool bOverwriteMode/*has to be removed*/);
    COLORREF getMessageColor(long long key, bool isAppendMode, CMessageAttrib* /*has to be removed*/);
};
