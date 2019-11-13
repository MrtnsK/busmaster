/**********************************************************************************
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
**********************************************************************************/

#include "CommonClass_stdafx.h"
#include "GUI_FormatMsgJ1939.h"
#include "include/Utils_macro.h"
#include "Error.h"

struct sERRORMSGINFO
{
    unsigned short m_usErrorCode; // Error code
    char* m_ptcErorMsg;          // Error message
};

typedef sERRORMSGINFO SERRORMSGINFO;
typedef sERRORMSGINFO* PERRORMSGINFO;
const int ERRORS_DEFINED = 15;

static SERRORMSGINFO sg_asErrorEntry[ERRORS_DEFINED] =
{
    {ERROR_UNKNOWN,       "Unknown Error"},
    {BIT_ERROR_RX,        "Bus Error - Bit Error(Rx)"},
    {CRC_ERROR_RX,        "Bus Error - CRC Error(Rx)"},
    {FORM_ERROR_RX,       "Bus Error - Form Error(Rx)"},
    {STUFF_ERROR_RX,      "Error Frame - Stuff error(Rx)"},
    {OTHER_ERROR_RX,      "Bus Error - Other Error(Rx)"},
    {BIT_ERROR_TX,        "Bus Error - Bit Error(Tx)"},
    {CRC_ERROR_TX,        "Bus Error - CRC Error(Tx)"},
    {FORM_ERROR_TX,       "Bus Error - Form Error(Tx)"},
    {STUFF_ERROR_TX,      "Bus Error - Stuff Error(Tx)"},
    {OTHER_ERROR_TX,      "Bus Error - Other Error(Tx)"},
    {ERROR_DEVICE_BUFF_OVERFLOW,        "Controller Buffer Overflow"},
    {ERROR_DRIVER_BUFF_OVERFLOW,        "Driver Buffer Overflow"},
    {ERROR_APPLICATION_BUFF_OVERFLOW,   "Application Buffer Overflow"},
    {ERROR_DLLMSG_BUFF_OVERFLOW,        "DLL message buffer Overflow"}
};

CFormatMsgJ1939::CFormatMsgJ1939(void)
{
}

CFormatMsgJ1939::~CFormatMsgJ1939(void)
{
}

/*******************************************************************************
  Function Name  : vFormatTime
  Input(s)       : -
  Output         : -
  Functionality  : Format Time
  Member of      : CFormatMsgJ1939
  Author(s)      : Arun kumar K
  Date Created   : 24.01.2011
  Modifications  :
*******************************************************************************/
void CFormatMsgJ1939::vFormatTime(BYTE bExprnFlag,
                                  PSTJ1939_MSG psJ1939BData, __int64 nDeltime,
                                  PSFORMATTEDATA_J1939 psJ1939FData)
{
    BYTE bTmpExprnFlag = bExprnFlag;
    if (IS_TM_ABS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp,
                              psJ1939FData->m_acTimeAbs, sizeof( psJ1939FData->m_acTimeAbs ) );
    }

    if (IS_TM_REL_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_REL(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, nDeltime,
                              psJ1939FData->m_acTimeRel, sizeof( psJ1939FData->m_acTimeRel ) );
    }

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_SYS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, psJ1939BData->m_sMsgProperties.m_un64TimeStamp,
                              psJ1939FData->m_acTimeSys, sizeof( psJ1939FData->m_acTimeSys ) );
    }
}


/*******************************************************************************
  Function Name  : vFormatDataAndId
  Input(s)       : bExprnFlag, psJ1939BData, psJ1939FData
  Output         : -
  Functionality  : Format data and id
  Member of      : CFormatMsgJ1939
  Author(s)      : Arun kumar K
  Date Created   : 27.01.2011
  Modifications  :
*******************************************************************************/
void CFormatMsgJ1939::vFormatDataAndId(BYTE bExprnFlag,
                                       PSTJ1939_MSG psJ1939BData,
                                       PSFORMATTEDATA_J1939 psJ1939FData)
{
    if (IS_NUM_HEX_SET(bExprnFlag))
    {
        sprintf_s(psJ1939FData->m_acPGNHex, FORMAT_PGN_ID_HEX,
                  psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
        sprintf_s(psJ1939FData->m_acMsgIDHex, FORMAT_STR_ID_HEX, psJ1939FData->m_dwMsgID);
        USHORT j  = 0;  //j is declared outside the for loop.
        for (USHORT i = 0; i < psJ1939BData->m_unDLC; i++)
        {
            BYTE CurrDat = psJ1939BData->m_pbyData[i];
            sprintf_s(&(psJ1939FData->m_pcDataHex[j]), (MAX_DATA_LEN_J1939 * 3 + 1)-j, FORMAT_STR_DATA_HEX, CurrDat);
            j += 3;
        }
        psJ1939FData->m_pcDataHex[j] = L'\0';
    }

    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        sprintf_s(psJ1939FData->m_acPGNDec, FORMAT_PGN_ID_DEC,
                  psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN());
        sprintf_s(psJ1939FData->m_acMsgIDDec, FORMAT_STR_ID_DEC, psJ1939FData->m_dwMsgID);
        USHORT j = 0;   // j is declared outside of the for loop
        for (USHORT i = 0; i < psJ1939BData->m_unDLC; i++)
        {
            BYTE CurrDat = psJ1939BData->m_pbyData[i];
            sprintf_s(&(psJ1939FData->m_pcDataDec[j]), (MAX_DATA_LEN_J1939 * 4 + 1)-j , FORMAT_STR_DATA_DEC, CurrDat);
            psJ1939FData->m_pcDataDec[j + 3] = L' ';
            j += 4;
        }
        psJ1939FData->m_pcDataDec[j] = L'\0';
    }
}

/*******************************************************************************
  Function Name  : usProcessCurrErrorEntry
  Input(s)       : SERROR_INFO& sErrorInfo
  Output         : Returns Error ID as USHORT.
  Functionality  : Processes the current Error entry and returns the Error code.
  Member of      : CFormatMsgJ1939
  Author(s)      : Arun kumar K
  Date Created   : 24.01.2011
  Modifications  :
*******************************************************************************/
USHORT CFormatMsgJ1939::usProcessCurrErrorEntry(SERROR_INFO& sErrorInfo)
{
    // Get the Error code
    USHORT usErrorID;

    if (sErrorInfo.m_ucErrType == ERROR_BUS)
    {
        // Update Statistics information
        usErrorID = sErrorInfo.m_ucReg_ErrCap /*& 0xE0*/;
    }
    else if (sErrorInfo.m_ucErrType == ERROR_WARNING_LIMIT_REACHED)
    {
        usErrorID = ERROR_UNKNOWN;
    }
    else if (sErrorInfo.m_ucErrType == ERROR_INTERRUPT)
    {
        usErrorID = ERROR_UNKNOWN;
    }
    else
    {
        usErrorID = sErrorInfo.m_ucErrType;
    }

    return usErrorID;
}

/**
 * \brief  Format Current Error Entry
 * \return Error Name
 *
 * Gets the pointer to Message data Buffer from PSDI_CAN DLL
 * and updates the same in List Control.
 */
char* CFormatMsgJ1939::vFormatCurrErrorEntry(USHORT usErrorID)
{
    BOOL bErrProcessed = FALSE;
    int nCount = 0;

    while ((nCount < ERRORS_DEFINED) && (bErrProcessed == FALSE))
    {
        if (usErrorID == sg_asErrorEntry[nCount].m_usErrorCode)
        {
            bErrProcessed = TRUE;
            return sg_asErrorEntry[nCount].m_ptcErorMsg;
        }
        nCount++;
    }
    return nullptr;
}

/*******************************************************************************
  Function Name  : vFormatJ1939DataMsg
  Input(s)       : psJ1939BData, psJ1939FData, bExprnFlag_Log
  Output         : -
  Functionality  : Format J1939 data bytes
  Member of      : CFormatMsgJ1939
  Author(s)      : Arun kumar K
  Date Created   : 27.01.2011
  Modifications  :
*******************************************************************************/
void CFormatMsgJ1939::vFormatJ1939DataMsg(PSTJ1939_MSG psJ1939BData, __int64 nDeltime,
        PSFORMATTEDATA_J1939 psJ1939FData,
        BYTE bExprnFlag_Log)
{
    psJ1939FData->m_dwMsgID = psJ1939BData->m_sMsgProperties.m_uExtendedID.m_unExtID;

    GetMessageTypeStr(psJ1939BData->m_sMsgProperties.m_eType, psJ1939FData->m_acMsgType);

    TYPE_CHANNEL CurrChannel = psJ1939BData->m_sMsgProperties.m_byChannel;
    if ((CurrChannel >= CHANNEL_CAN_MIN) && (CurrChannel <= CHANNEL_CAN_MAX ))
    {
        sprintf(psJ1939FData->m_acChannel, "%d", CurrChannel);
    }

    if (IS_NUM_HEX_SET(bExprnFlag_Log))
    {
        sprintf_s(psJ1939FData->m_acSrcHex, FORMAT_STR_DATA_HEX, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
        sprintf_s(psJ1939FData->m_acDestHex, FORMAT_STR_DATA_HEX, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
    }

    if (IS_NUM_DEC_SET(bExprnFlag_Log))
    {
        sprintf_s(psJ1939FData->m_acSrcDec, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress);
        sprintf_s(psJ1939FData->m_acDestDec, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific);
    }

    sprintf_s(psJ1939FData->m_acPriority, FORMAT_STR_DATA_DEC, psJ1939BData->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority);


    if (DIR_RX == psJ1939BData->m_sMsgProperties.m_eDirection)
    {
        psJ1939FData->m_acMsgDir[0] = L'R';
    }
    else if (DIR_TX == psJ1939BData->m_sMsgProperties.m_eDirection)
    {
        psJ1939FData->m_acMsgDir[0] = L'T';
    }
    else
    {
        ASSERT(false);
    }
    psJ1939FData->m_acMsgDir[1] = L'x';
    psJ1939FData->m_acMsgDir[2] = L'\0';
    _itoa_s(psJ1939BData->m_unDLC, psJ1939FData->m_acDataLen, LEN_STR_DLC_J1939, 10);
    vFormatTime(bExprnFlag_Log, psJ1939BData, nDeltime, psJ1939FData);
    vFormatDataAndId(bExprnFlag_Log, psJ1939BData, psJ1939FData);
}