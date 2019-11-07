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
 * \file      MsgSignal_Datatypes.h
 * \brief     Definition file for signal watch data types.
 * \author    Pradeep Kadoor
 * \author    GT-Derka
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for signal watch data types.
 */

#pragma once

//#include "Include/BaseDefs.h"
#include "ProtocolsDefinitions.h"
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include "../Include/BaseDefs.h"


class CMsgSignal;

//DB params
typedef struct _tagDBPARAMS
{
    ETYPE_BUS       m_eBus;
    CString         m_omBusName;
    CString         m_omDBPath;
    CString         m_omIdFieldName;
    UINT            m_unMaxMsgLen;
    CMsgSignal*     m_pouMsgSignalActiveDB;
    CMsgSignal*     m_pouMsgSignalImportedDBs;
} SDBPARAMS;

static SDBPARAMS sg_asDbParams[BUS_TOTAL] =
{
    {CAN,     "CAN",    "", "Message ID :",  8,0,0},
    {MCNET,   "MCNET",  "", "Message Code :",  MAX_DATA_LEN_MCNET,0,0},
    {J1939,   "J1939",  "", "PGN :",  MAX_DATA_LEN_J1939,0,0},
};

enum eWAVEFORMTYPE
{
    eWave_NONE = 0,
    eWave_SINE,
    eWave_TRIANGLE,
    eWave_COS,
    eWave_SAWTOOTH
};

enum eTIMINGS
{
    UNDEFINED_TIMING = 0,
    CYCLIC_TIMING,
    EVENT_CONTROLLED_TIMING,
    REQUEST_CONTROLLED_TIMING,
    RELATIVELY_SCHEDULED_TIMING,
    ABSOLUTELY_SCHEDULED_TIMING,
    IDENTIFIER
};

struct sINTERPRETSIGNALINFO
{
    CString m_omStrSignalName;
    __int64 m_un64RawValue;
    double m_dPhyValue;
    CString m_omStrSignalDescValue;
    CString m_omStrUnit;
    unsigned char   m_ucSigLength;

    sINTERPRETSIGNALINFO(): m_omStrSignalName(""),
        m_un64RawValue(0),
        m_dPhyValue(0),
        m_omStrSignalDescValue(""),
        m_omStrUnit(""),
        m_ucSigLength(0)
    {
        // Do Additional Init here
    }
};

typedef sINTERPRETSIGNALINFO SINTERPRETSIGNALINFO;
typedef SINTERPRETSIGNALINFO* PINTERPRETSIGNALINFO;

typedef CArray<SINTERPRETSIGNALINFO,SINTERPRETSIGNALINFO> CSignalInfoArray;

typedef union _SIG_VALUE
{
    __int64  n64Value;
    unsigned __int64  un64Value;
} SIG_VALUE;

class CSignalDescVal
{
public:
    CSignalDescVal();
    ~CSignalDescVal();
    CSignalDescVal& operator=(const CSignalDescVal& RefObj);
    void vClearNext(void);

    CString m_omStrSignalDescriptor;                // Name of the Signal ID
    SIG_VALUE m_DescValue;
    CSignalDescVal* m_pouNextSignalSignalDescVal;   // Next Signal desc and value
};


struct sWaveformInfo
{
    eWAVEFORMTYPE m_eSignalWaveType;
    float m_fAmplitude;   // Maximum amplitude
    float m_fFrequency;
    float m_fGranularity; // Of each time amplitude calculation. Value depends
    // on the sampling frequency.

    sWaveformInfo();

    sWaveformInfo& operator=(const sWaveformInfo& RefObj);
    static CString omGetWaveformName(eWAVEFORMTYPE eWaveform);
};

struct sSigWaveMap
{
    CString m_omSigName;
    sWaveformInfo sWaveInfo;

    sSigWaveMap();

    sSigWaveMap& operator=(const sSigWaveMap& RefObj);
    BOOL operator==(const sSigWaveMap& RefObj) const;
};

typedef CList<sSigWaveMap, sSigWaveMap&> CSigWaveMapList;

struct SSigGeneration
{
    UINT m_nMsgID;
    float m_fDefaultAmplitude;
    CSigWaveMapList m_omSigWaveMapList;

    SSigGeneration();
    ~SSigGeneration();

    void Reset(void);
    BOOL operator ==(const SSigGeneration& RefObj) const;
    SSigGeneration& operator=(const SSigGeneration& RefObj);
};

typedef CList<SSigGeneration, SSigGeneration&> CSigGenerationInfoList;

struct sSIGNALS
{
    CString m_omStrSignalName;      // Signal Name
    UINT    m_unStartByte;          // Signal definition start byte in Msg, Zero index
    UINT    m_unSignalLength;       // Signal Length, One indexed
    BYTE    m_byStartBit;           // Signal start bit, Zero indexed
    BYTE    m_bySignalType;         // Signal Type

    SIG_VALUE m_SignalMinValue;
    SIG_VALUE m_SignalMaxValue;

    float       m_fSignalFactor;    // Signal Multiplication Factor
    float       m_fSignalOffset;    // Signal Offset value.
    CString m_omStrSignalUnit;      // Signal Measurement Unit
    eEndianess m_eFormat;         // Signal endianness.

    CSignalDescVal*  m_oSignalIDVal;// Signal Type ID and value List
    sSIGNALS* m_psNextSignalList;   // Next Signal

    // Call this function to set the data bits of a signal with the given value
    static void vSetSignalValue(sSIGNALS* pouCurrSignal, UCHAR aucData[8],
                                UINT64 u64SignVal);
    // Call this function to get the bit mask for a signal. In a bit mask all
    // bits petaining to but this signal is 0.
    static UINT64 un64GetBitMask(sSIGNALS* pouCurrSignal);
};

struct sMESSAGE
{
    CString m_omStrMessageName; // Message Name
    UINT m_unMessageCode;       // Message Code
    BYTE m_byMessageChannel;    // Channel on which message is transported
    UINT m_unNumberOfSignals;   // Number Of Signals a Message has
    UINT m_unMessageLength;     // Message Length
    BOOL m_bMessageFrameFormat; // Message Frame Format
    sSIGNALS* m_psSignals;      // Signal associated with Message
    BYTE m_bySignalMatrix[1785];   // Matrix to validate overlapping signals
    int m_nMsgDataFormat;       // 1-Intel, 0-Motorola
};

typedef struct tagSSIGNALINFO
{
    CString m_omSigName;
    CString m_omRawValue;
    CString m_omEnggValue;
    CString m_omUnit;
    CString m_msgName;
} SSignalInfo;

typedef CArray<SSignalInfo, SSignalInfo> SSignalInfoArray;


typedef struct tagSMSGENTRY
{
    sMESSAGE*  m_psMsg;
    tagSMSGENTRY*    m_psNext;

    tagSMSGENTRY();
    ~tagSMSGENTRY();

    static void vClearMsgList(tagSMSGENTRY*& psMsgRoot);
    static void vClearSignalList(sSIGNALS* psSignals);

    static sMESSAGE* psCopyMsgVal(sMESSAGE* psMsg);
    static sSIGNALS* psCopySignalVal(sSIGNALS* psSignal);

    static BOOL bUpdateMsgList(tagSMSGENTRY*& Root, sMESSAGE* psMsg);
    static sSIGNALS* psCopySignalList(sSIGNALS* psSignal);
    static BOOL bGetMsgPtrFromMsgId(const tagSMSGENTRY* psRoot,UINT unMsgId, sMESSAGE*& pMsg);


} SMSGENTRY;



