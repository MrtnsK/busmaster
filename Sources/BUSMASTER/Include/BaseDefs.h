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
 * @file      BaseDefs.h
 * @brief     This contains various basic definitions.
 * @author    Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains various basic definitions.
 */

#pragma once
//#include "ProtocolsDefinitions.h"
//#include "Struct_CAN.h"
//#include "Struct_LIN.h"

#include "CANDriverDefines.h"
#include "LINDriverDefines.h"
#include "FlexRayDriverDefines.h"
#include "CommonDefines.h"
#include "ProtocolsDefinitions.h"
/*Maximum possible length of a CAN*/
#define MAX_DATA_LEN_CAN 8
/*Maximum possible length of a CAN FD*/
#define MAX_DATA_LEN_CAN_FD 64
/*Maximum possible length of a J1939*/
#define MAX_DATA_LEN_J1939 1785
/*Maximum possible length of a MCNET*/
#define MAX_DATA_LEN_MCNET 0x7FFF

#define MAX_DATA_LEN_LIN 8

typedef enum eBUSEVEHANDLER
{
    BUS_CONNECT = 0,
    BUS_PRE_CONNECT,
    BUS_DISCONNECT
};


#define defLINETYPE_SOLID "SOLID"
#define defLINETYPE_DASH "DASH"
#define defLINETYPE_DOT "DOT"
#define defLINETYPE_DASHDOT "DASHDOT"
#define defLINETYPE_DASHDOTDOT "DASHDOTDOT"
#define defLINETYPE_NULL "nullptr"
#define defLINETYPE_XYSTEP "XYSTEP"
#define defLINETYPE_YXSTEP "YXSTEP"
#define defLINETYPE_BARS "BARS"
#define defLINETYPE_STICK "STICK"

enum eProtocol
{
    PROTOCOL_CAN    = 0,
    PROTOCOL_J1939,
    PROTOCOL_UNKNOWN
};



enum ESTATUS_BUS
{
    BUS_PRECONNECT,
    BUS_CONNECTED,
    BUS_IDLE,
    BUS_DISCONNECTED
};


enum eTimerMode
{
    TIME_MODE_RELATIVE = 0,
    TIME_MODE_SYSTEM,
    TIME_MODE_ABSOLUTE
};
typedef eTimerMode ETIMERMODE;

/* enum eFormat
{
    HEXADECIMAL = 0,
    DEC
};
typedef eFormat EFORMAT;
*/
enum eMode
{
    APPEND_MODE = 0,
    OVERWRITE_MODE,
    OVERWRITE_INTERPRET_MODE
};

enum eScroll
{
    SCROLL = 0,
    FREEZE
};

enum eTreeItemStates
{
    TREESTATE_TOGGLE,
    TREESTATE_EXPAND,
    TREESTATE_COLLAPSE
};

enum eFILENAMESUFFIX
{
    SUFFIX_MEASUREMENT,
    SUFFIX_TIME,
    SUFFIX_SIZE,
    SUFFIX_DEFAULT
};
enum eNodeOperations
{
    NODE_ADD,
    NODE_REMOVE,
    NODE_REMOVEALL,
};

const BYTE TYPE_ID_CAN_NONE        = 0x0;
const BYTE TYPE_ID_CAN_STANDARD    = 0x1;
const BYTE TYPE_ID_CAN_EXTENDED    = 0x2;
const BYTE TYPE_ID_CAN_ALL         = 0x3;

const BYTE TYPE_ID_LIN_NONE        = 0x0;
const BYTE TYPE_ID_LIN_STANDARD    = 0x1;
const BYTE TYPE_ID_LIN_ALL         = 0x2;

const BYTE TYPE_MSG_CAN_NONE        = 0x0;
const BYTE TYPE_MSG_CAN_RTR         = 0x1;
const BYTE TYPE_MSG_CAN_NON_RTR     = 0x2;
const BYTE TYPE_MSG_CAN_FD          = 0x4;
const BYTE TYPE_MSG_CAN_ALL         = 0x7;

const BYTE TYPE_MSG_LIN_NONE        = 0x0;

const BYTE TYPE_MSG_LIN_FD          = 0x4;

#define LENGTH_STR_TIMESTAMP_CAN        16
#define LENGTH_STR_DATA_CAN             256
#define LENGTH_STR_ID_CAN               16
#define LENGTH_STR_DESCRIPTION_CAN      256
#define LENGTH_STR_DLC_CAN              3
#define LENGTH_STR_CHANNEL_CAN          4
#define LENGTH_STR_DIRECTION_CAN        4
#define LENGTH_STR_TYPE_CAN             8

#define LENGTH_STR_TIMESTAMP_LIN        16
#define LENGTH_STR_DATA_LIN             256
#define LENGTH_STR_ID_LIN               16
#define LENGTH_STR_DESCRIPTION_LIN      256
#define LENGTH_STR_DLC_LIN              3
#define LENGTH_STR_CHKSUM_LIN           24
#define LENGTH_STR_MSGTYPE_LIN          32
#define LENGTH_STR_CHANNEL_LIN          4
#define LENGTH_STR_DIRECTION_LIN        4
#define LENGTH_STR_TYPE_LIN             8

#define S_MEASUREMENT "_M"
#define S_SIZE "_S"
#define S_TIME "_T"
#define S_DEFAULT "_"

#define M_SUFFIX_FORMAT "_M%d%*s"
#define S_SUFFIX_FORMAT "_S%d%*s"
#define T_SUFFIX_FORMAT "_T%d%*s"

//Import Log file
#define defDEFAULT_PAGE_LENGTH 1000

enum eImportLogError
{
    eImportLogValid,
    eImportLogProtocolMismatch,
    eImportLogFileSizeErr,
    eImportLogInvalid,
    eImportLogTotal
};
const std::string m_omImportLogErr[eImportLogTotal] =
{
    "File Valid",
    "Unable to Load. Protocol Mismatch",
    "Unable to Load. File size exceeds 2GB",
    "Unable to Load. Invalid file.",
};
struct tagFormattedData_CAN
{
    UINT64          m_u64TimeStamp;                 // Time stamp
    DWORD           m_dwMsgID;                      // Message identifier
    EDIRECTION      m_eDirection;                   // Direction (Rx / Tx)
    TYPE_CHANNEL    m_eChannel;                     // Channel
    BYTE            m_byDataLength;                 // Data length count
    BYTE            m_abData[MAX_DATA_LEN_CAN_FD];  // Message data
    BYTE            m_byIDType;                     // Type of the ID (STD or EXTENDED)
    BYTE            m_byMsgType;                    // Type of the message (RTR or NRTR)
    ERROR_STATE    m_eEventType;

    char   m_acMsgDir[LENGTH_STR_DIRECTION_CAN];     // "Tx" or "Rx"
    char   m_acChannel[LENGTH_STR_CHANNEL_CAN];      // "1" or "2"
    char   m_acType[LENGTH_STR_TYPE_CAN];            // s / x / r
    char   m_acDataLen[LENGTH_STR_DLC_CAN];          // DLC always in dec
    char   m_acMsgDesc[LENGTH_STR_DESCRIPTION_CAN];  // Message description

    char   m_acMsgIDHex[LENGTH_STR_ID_CAN];          // Identifier in hex
    char   m_acMsgIDDec[LENGTH_STR_ID_CAN];          // Identifier in dec

    char   m_acDataHex[LENGTH_STR_DATA_CAN];         // Data in hex
    char   m_acDataDec[LENGTH_STR_DATA_CAN];         // Data in dec

    char   m_acTimeAbsReset[LENGTH_STR_TIMESTAMP_CAN];    // Absolute time stamp Reset
    char   m_acTimeAbs[LENGTH_STR_TIMESTAMP_CAN];    // Absolute time stamp
    char   m_acTimeRel[LENGTH_STR_TIMESTAMP_CAN];    // Relative time stamp
    char   m_acTimeSys[LENGTH_STR_TIMESTAMP_CAN];    // System time stamp

    __int64         m_n64MapId;                     // Map id of the message
    COLORREF        m_ColourCode;                   // Colour code associated
};
typedef tagFormattedData_CAN SFORMATTEDDATA_CAN;

enum eMSG_INTERPRETATION_PROPERTY
{
    NON_INTERPRETABLE = 0,
    INTERPRETABLE,
    INTERPRETING,
    MODE_NONE
};
typedef eMSG_INTERPRETATION_PROPERTY EINTERPRET_MODE;

struct tagFormattedData_LIN
{
    UINT64          m_u64TimeStamp;                 // Time stamp
    DWORD           m_dwMsgID;                      // Message identifier
    EDIRECTION      m_eDirection;                   // Direction (Rx / Tx)
    TYPE_CHANNEL    m_eChannel;                     // Channel
    BYTE            m_byDataLength;                 // Data length count
    BYTE            m_abData[MAX_DATA_LEN_LIN];  // Message data
    BYTE            m_byIDType;                     // Type of the ID (MSG OR EVENT)
    BYTE            m_byMsgType;                    // Type of the message (RTR or NRTR)
    BYTE            m_byChecksum;                   // Checksum of Lin frame
    BYTE            m_byChecksumType;               // Checksum Type (0 - Classical / 1 - Enhanced)
    eLinBusEventType         m_eEventType;

    char   m_acMsgDir[LENGTH_STR_DIRECTION_LIN];     // "Tx" or "Rx"
    char   m_acChannel[LENGTH_STR_CHANNEL_LIN];      // "1" or "2"
    char   m_acType[LENGTH_STR_MSGTYPE_LIN];            // s / x / r
    char   m_acDataLen[LENGTH_STR_DLC_LIN];          // DLC always in dec
    char   m_acMsgDesc[LENGTH_STR_DESCRIPTION_LIN];  // Message description
    char   m_acChecksum[LENGTH_STR_CHKSUM_LIN];

    char   m_acMsgIDHex[LENGTH_STR_ID_LIN];          // Identifier in hex
    char   m_acMsgIDDec[LENGTH_STR_ID_LIN];          // Identifier in dec

    char   m_acDataHex[LENGTH_STR_DATA_LIN];         // Data in hex
    char   m_acDataDec[LENGTH_STR_DATA_LIN];         // Data in dec

    char   m_acTimeAbsReset[LENGTH_STR_TIMESTAMP_LIN];    // Absolute time stamp Reset
    char   m_acTimeAbs[LENGTH_STR_TIMESTAMP_LIN];    // Absolute time stamp
    char   m_acTimeRel[LENGTH_STR_TIMESTAMP_LIN];    // Relative time stamp
    char   m_acTimeSys[LENGTH_STR_TIMESTAMP_LIN];    // System time stamp

    __int64         m_n64MapId;                     // Map id of the message
    COLORREF        m_ColourCode;                   // Colour code associated
};
typedef tagFormattedData_LIN SFORMATTEDDATA_LIN;

struct sWMUpdatePtrPara
{
    int              m_nListIndex;
    EINTERPRET_MODE  m_eInPretMode;
    BOOL             m_bResult;
};
typedef sWMUpdatePtrPara SWMUPDATEPTRPARA;
typedef sWMUpdatePtrPara* PSWMUPDATEPTRPARA;

#define MAX_MSG_WND_COL_CNT              12

struct sMsgWndHdrCol
{
    BYTE m_byTimePos;
    BYTE m_byCodeNamePos;
    BYTE m_byIDPos;
    BYTE m_byRxTxPos;
    BYTE m_byMsgTypePos;
    BYTE m_byFrameTypePos;
    BYTE m_bySlotTypePos;
    BYTE m_byDLCPos;
    BYTE m_byDataPos;
    BYTE m_byChannel;
    BYTE m_byPGNPos;
    BYTE m_bySrcPos;
    BYTE m_byDestPos;
    BYTE m_byPriorityPos;
    BYTE m_byCycleNoPos;
    BYTE m_byChecksumPos;

    sMsgWndHdrCol()
    {
        m_byTimePos     = (BYTE) -1;
        m_byCodeNamePos = (BYTE) -1;
        m_byIDPos       = (BYTE) -1;
        m_byRxTxPos     = (BYTE) -1;
        m_byMsgTypePos  = (BYTE) -1;
        m_byFrameTypePos= (BYTE) -1;
        m_bySlotTypePos = (BYTE) -1;
        m_byDLCPos      = (BYTE) -1;
        m_byDataPos     = (BYTE) -1;
        m_byChannel     = (BYTE) -1;
        m_byPGNPos      = (BYTE) -1;
        m_bySrcPos      = (BYTE) -1;
        m_byDestPos     = (BYTE) -1;
        m_byPriorityPos = (BYTE) -1;
        m_byCycleNoPos  = (BYTE) -1;
        m_byChecksumPos = (BYTE) -1;
    }
};
typedef sMsgWndHdrCol SMSGWNDHDRCOL;




//Import Log file
#define defDEFAULT_PAGE_LENGTH 1000
enum eMSG_WND_PROPERTY
{
    NUMERIC = 0x1,
    TIME_MODE = 0x2,
    DISPLAY_MODE = 0x4
};