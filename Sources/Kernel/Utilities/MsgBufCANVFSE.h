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
 * \file      MsgBufCANVFSE.h
 * \brief     Defines and implements the template class for circular queue
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines and implements the template class for circular queue
 */

#pragma once

#include "../BusmasterDriverInterface/include/Error.h"
#include "BaseMsgBufAll.h"
#include "..\..\Busmaster\Application\Hashdefines.h"


const int SIZE_APP_CAN_BUFFER       = 5000;
typedef int( *compForSort )( const void*, const void* );
/* This is the concrete template class of a circular queue where each entry is
of fixed size. Implemented as a template class so as to cater to any data type.
Here SMSGBUFFER is the data type in operation. */
template <typename SMSGBUFFER>
class CMsgBufCANVFSE
{
protected:

    SMSGBUFFER m_asMsgBuffer[SIZE_APP_CAN_BUFFER]; // The data buffer
    CRITICAL_SECTION m_CritSectionForGB;       // To make it thread safe

    int m_nIndexRead;                          // Current read index
    int m_nIndexWrite;                         // Current write index
    int m_nMsgCount;                           // Number of message entries
    int m_nMsgSize;                            /* At the beginning we need to
    store size of a message entry. This information will be used frequently */
    HANDLE m_hNotifyingEvent;                  // Event to be signalled when
    // there is at least one message

public:
    // Short explanation on each member function is present in the base class.
    // That's why information are not repeated unnecessarily.

    CMsgBufCANVFSE();
    ~CMsgBufCANVFSE();

    int ReadFromBuffer(SMSGBUFFER* psMsgBuffer,__int64 nSlotId);
    int ReadFromBuffer(SMSGBUFFER* psMsgBuffer,int nIndex);
    int WriteIntoBuffer(const SMSGBUFFER* psMsg, __int64 nSlotId, int& nIndex);
    int WriteIntoBuffer(const SMSGBUFFER* psMsg);
    int GetBufferLength(void) const;
    void vClearMessageBuffer(void);
    HANDLE hGetNotifyingEvent(void) const;
    void vDoSortBuffer( int nField, bool bAscending );
    void vDoSortIndexMapArray();
    __int64 GetSlotID ( STCANDATA& pDatCAN );
    void nGetMapIndexAtID(int nIndex,__int64& nMapIndex);

    SMSGBUFFER * nGetBuffer ()
    {
        return m_asMsgBuffer;
    }

    

private:
    CMap<__int64, __int64, int, int> m_omIdIndexMap;
};

/******************************************************************************
  Function Name    :  CMsgBufCANVFSE
  Input(s)         :  -
  Output           :  -
  Functionality    :  Standard constructor
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER>
CMsgBufCANVFSE<SMSGBUFFER>::CMsgBufCANVFSE()
{
    m_nMsgSize = sizeof(SMSGBUFFER);
    InitializeCriticalSection(&m_CritSectionForGB);
    vClearMessageBuffer();
    m_hNotifyingEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
}

/******************************************************************************
  Function Name    :  ~CMsgBufCANVFSE
  Input(s)         :  -
  Output           :  -
  Functionality    :  Destructor
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER>
CMsgBufCANVFSE<SMSGBUFFER>::~CMsgBufCANVFSE()
{
    CloseHandle(m_hNotifyingEvent);
    m_hNotifyingEvent = nullptr;
    DeleteCriticalSection(&m_CritSectionForGB);
}

/******************************************************************************
  Function Name    :  vClearMessageBuffer
  Input(s)         :  void
  Output           :  void
  Functionality    :  Clears the message buffer and resets the indices.
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
vClearMessageBuffer(void)
{
    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);
    memset((BYTE*) m_asMsgBuffer, 0, SIZE_APP_CAN_BUFFER * m_nMsgSize);
    m_omIdIndexMap.RemoveAll();
    m_nIndexRead = 0;
    m_nIndexWrite = 0;
    m_nMsgCount = 0;
    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);
}

/******************************************************************************
  Function Name    :  ReadFromBuffer
  Input(s)         :  psMsg - The target message entry. An [out] parameter.
  Output           :  EMPTY_APP_BUFFER if buffer is empty; else CALL_SUCCESS.
  Functionality    :  Reads a message entry and advances the read index. On
                      successful reading operation the present entry is
                      invalidated to make room for a new entry.
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* psMsg, int nIndex)
{
    HRESULT nResult = S_OK;

#ifdef _DEBUG
    ASSERT(psMsg != nullptr);
    ASSERT(!(m_nIndexRead > SIZE_APP_CAN_BUFFER));
#endif

    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);

    // Check entry indexed by m_nIndexRead. If this particular entry
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else if (nIndex >= SIZE_APP_CAN_BUFFER)
    {
        nResult = ERR_INVALID_INDEX;
    }
    else
    {
        INT TempIndex = m_nIndexRead + nIndex;
        if (m_nIndexRead + nIndex >= SIZE_APP_CAN_BUFFER)
        {
            TempIndex -= SIZE_APP_CAN_BUFFER;
        }
        *psMsg = m_asMsgBuffer[TempIndex];
    }
    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);

    return nResult;
}

template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::ReadFromBuffer(SMSGBUFFER* psMsg, __int64 nSlotId)
{
    HRESULT nResult = CALL_SUCCESS;
    int nIndex;

#ifdef _DEBUG
    ASSERT(psMsg != nullptr);
    ASSERT(!(m_nIndexRead > SIZE_APP_CAN_BUFFER));
#endif

    // Lock the buffer
    EnterCriticalSection(&m_CritSectionForGB);

    // Check entry indexed by m_nIndexRead. If this particular entry
    if (m_nMsgCount == 0)
    {
        nResult = EMPTY_APP_BUFFER;
    }
    else
    {
        if (m_omIdIndexMap.Lookup(nSlotId, nIndex))
        {
            *psMsg = m_asMsgBuffer[nIndex];
        }
        else
        {
            nResult = ERR_INVALID_SLOT;
        }
    }
    // Unlock the buffer
    LeaveCriticalSection(&m_CritSectionForGB);

    return nResult;
}


/******************************************************************************
  Function Name    :  WriteIntoBuffer
  Input(s)         :  psMsg - The source message entry. An [in] parameter.
  Output           :  ERR_FULL_APP_BUFFER if buffer is full; else CALL_SUCCESS.
  Functionality    :  Writes a message entry and advances the write index.
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::WriteIntoBuffer(const SMSGBUFFER* psMsg,
        __int64 nSlotId, int& nIndex)

{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != nullptr);
    ASSERT(!(m_nIndexWrite > SIZE_APP_CAN_BUFFER));
#endif

    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    if (m_nMsgCount == SIZE_APP_CAN_BUFFER) // Check for buffer overflow
    {
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        if (m_omIdIndexMap.Lookup(nSlotId, nIndex))
        {
            m_asMsgBuffer[nIndex] = *psMsg;
        }
        else
        {
            nIndex = m_nMsgCount;
            m_asMsgBuffer[m_nMsgCount] = *psMsg;
            m_omIdIndexMap[nSlotId] = m_nMsgCount;
            ++m_nMsgCount;
        }
        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer

    return nResult;
}
template <typename SMSGBUFFER>
int CMsgBufCANVFSE<SMSGBUFFER>::WriteIntoBuffer(const SMSGBUFFER* psMsg)

{
    int nResult = CALL_SUCCESS;

#ifdef _DEBUG
    ASSERT(psMsg != nullptr);
    ASSERT(!(m_nIndexWrite > SIZE_APP_CAN_BUFFER));
#endif

    EnterCriticalSection(&m_CritSectionForGB); // Lock the buffer

    if (m_nMsgCount == SIZE_APP_CAN_BUFFER) // Check for buffer overflow
    {
        m_asMsgBuffer[m_nIndexRead] = *psMsg;
        m_nIndexRead++;
        if (m_nIndexRead == SIZE_APP_CAN_BUFFER)
        {
            m_nIndexRead = 0;
        }
        nResult = ERR_FULL_APP_BUFFER;
    }
    else
    {
        m_asMsgBuffer[m_nMsgCount] = *psMsg;
        ++m_nMsgCount;
        SetEvent(m_hNotifyingEvent);
    }

    LeaveCriticalSection(&m_CritSectionForGB); // Unlock the buffer

    return nResult;
}
/******************************************************************************
  Function Name    :  GetBufferLength
  Input(s)         :  void
  Output           :  Number of message entries (int)
  Functionality    :  Returns the number of unread entries in the queue.
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER> int CMsgBufCANVFSE<SMSGBUFFER>::
GetBufferLength(void) const
{
    return m_nMsgCount;
}

/******************************************************************************
  Function Name    :  hGetNotifyingEvent
  Input(s)         :  void
  Output           :  The notifying event handle (HANDLE)
  Functionality    :  Returns handle of the event that gets signalled when
                      a message entry is added.
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  1.12.2009
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER> HANDLE CMsgBufCANVFSE<SMSGBUFFER>::
hGetNotifyingEvent(void) const
{
    return m_hNotifyingEvent;
}

/******************************************************************************
  Function Name    :  vDoSortBuffer
  Input(s)         :  nField - The field to be used as the sorting key.
  Output           :  -
  Functionality    :  Reorders the list according to the sorting key specified.
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Ratnadip Choudhury
  Date Created     :  21-06-2010
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
vDoSortBuffer( int /*nField*/, bool /*bAscending*/ )
{
    //SMSGBUFFER::vSetSortField(nField);
    //SMSGBUFFER::vSetSortAscending(bAscending);
    //qsort((void*) m_asMsgBuffer, (size_t) GetBufferLength(),
    //    sizeof( SMSGBUFFER ), SMSGBUFFER::DoCompareIndiv );
    ////After sorting Start index has to be reset
    //m_nIndexRead = 0;

    //if ( m_stlIdIndexMap.size() == 0 )
    //{
    //    return;
    //}

    //vDoSortIndexMapArray();
}
/******************************************************************************
  Function Name    :  vDoSortIndexMapArray
  Input(s)         :
  Output           :  -
  Functionality    :  Reorders the Index Map Array according to the order specified.
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Arunkumar K
  Date Created     :  28-06-2010
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
vDoSortIndexMapArray()
{
    for(int nCnt = 0; nCnt< m_omIdIndexMap.GetCount(); nCnt++)
    {
        __int64 nSlotID = GetSlotID(m_asMsgBuffer[nCnt]);
        m_omIdIndexMap.SetAt(nSlotID, nCnt);
    }
}


/******************************************************************************
Function Name    :  GetSlotID
Input(s)         :  pDataCAN - The field data to be used for displaying sorted data.
Output           :  -
Functionality    :  Gets the index of the message from the CAN index map.
Member of        :  CMsgBufCANVFSE
Friend of        :  -
Author(s)        :  Mandar M Apte
Date Created     :  12-01-2017
Modification date:
Modification By  :
******************************************************************************/
template <typename SMSGBUFFER>
__int64 CMsgBufCANVFSE<SMSGBUFFER>::GetSlotID ( STCANDATA& pDatCAN )
{
    STCAN_MSG& sMsg = pDatCAN.m_uDataInfo.m_sCANMsg;
    // Form message to get message index in the CMap
    int nMsgID = MAKE_RX_TX_MESSAGE ( sMsg.m_unMsgID,
                                      IS_RX_MESSAGE ( pDatCAN.m_ucDataType ) );

    nMsgID = MAKE_DEFAULT_MESSAGE_TYPE ( nMsgID );
    // For extended message
    if ( sMsg.m_ucEXTENDED )
    {
        nMsgID = MAKE_EXTENDED_MESSAGE_TYPE ( nMsgID );
    }
    // Apply Channel Information
    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE ( nMsgID,
                                                          sMsg.m_ucChannel );
    return n64MapIndex;
}

/******************************************************************************
  Function Name    :  nGetMapIndexAtID
  Input(s)         :  nIndex - The Index at which the SlotID needs to be pickef from.
  Output           :  -
  Functionality    :  Returns the Slot ID of the index specified in m_omIdIndexMap.
  Member of        :  CMsgBufCANVFSE
  Friend of        :  -
  Author(s)        :  Arunkumar K
  Date Created     :  28-06-2010
  Modification date:
  Modification By  :
******************************************************************************/
template <typename SMSGBUFFER> void CMsgBufCANVFSE<SMSGBUFFER>::
nGetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    POSITION pos = m_omIdIndexMap.GetStartPosition();
    int nLocalIndex=0;
    __int64 nSlotID;
    while( pos != nullptr )
    {
        m_omIdIndexMap.GetNextAssoc(pos, nSlotID, nLocalIndex);
        if(nIndex == nLocalIndex)
        {
            nMapIndex = nSlotID;
            return;
        }
    }
}
