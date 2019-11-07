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
 * \file      FormatMsgCAN.cpp
 * \brief     Source of CFormatMsgCAN class.
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source of CFormatMsgCAN class.
 */

#include "../FrameProcessor_stdafx.h"
#include "FormatMsgCAN.h"
#include "include/Utils_macro.h"
#include "Error.h"

CFormatMsgCAN::CFormatMsgCAN(CRefTimeKeeper& ouRefTimeKeeper) :
    CFormatMsgCommon(ouRefTimeKeeper)
{
}

CFormatMsgCAN::~CFormatMsgCAN(void)
{
}

void CFormatMsgCAN::vFormatTime(BYTE bExprnFlag,
                                SFORMATTEDDATA_CAN* CurrDataCAN)
{
    BYTE bTmpExprnFlag = bExprnFlag;
    if (IS_TM_ABS_RES(bExprnFlag))//for Absolute reset timestamp
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS_RES(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataCAN->m_u64TimeStamp,
                              CurrDataCAN->m_acTimeAbsReset, sizeof(CurrDataCAN->m_acTimeAbsReset));
    }
    if (IS_TM_ABS_SET(bExprnFlag))//for Absolute non-reset timestamp
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataCAN->m_u64TimeStamp,
                              CurrDataCAN->m_acTimeAbs, sizeof(CurrDataCAN->m_acTimeAbs));
    }

    if (IS_TM_REL_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_REL(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataCAN->m_u64TimeStamp,
                              CurrDataCAN->m_acTimeRel, sizeof(CurrDataCAN->m_acTimeRel));
    }

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_SYS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataCAN->m_u64TimeStamp,
                              CurrDataCAN->m_acTimeSys, sizeof(CurrDataCAN->m_acTimeSys));
    }
}


void CFormatMsgCAN::vFormatDataAndId(BYTE bExprnFlag,
                                     SFORMATTEDDATA_CAN* CurrDataCAN)
{
    if (IS_NUM_HEX_SET(bExprnFlag))
    {
        sprintf(CurrDataCAN->m_acMsgIDHex, FORMAT_STR_ID_HEX, CurrDataCAN->m_dwMsgID);

        int i, j;
        for (i = 0, j = 0; i < CurrDataCAN->m_byDataLength; i++)
        {
            BYTE CurrDat = CurrDataCAN->m_abData[i];
            sprintf(&(CurrDataCAN->m_acDataHex[j]), FORMAT_STR_DATA_HEX, CurrDat);
            j += 3;
        }
        CurrDataCAN->m_acDataHex[j] = L'\0';
    }

    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        sprintf(CurrDataCAN->m_acMsgIDDec, FORMAT_STR_ID_DEC, CurrDataCAN->m_dwMsgID);

        int i, j;
        for (i = 0, j = 0; i < CurrDataCAN->m_byDataLength; i++)
        {
            BYTE CurrDat = CurrDataCAN->m_abData[i];
            sprintf(&(CurrDataCAN->m_acDataDec[j]), FORMAT_STR_DATA_DEC, CurrDat);
            j += 4;
            CurrDataCAN->m_acDataDec[j-1] = L' ';
        }
        CurrDataCAN->m_acDataDec[j] = L'\0';
    }
}

void CFormatMsgCAN::vFormatCANDataMsg(STCANDATA* pMsgCAN,
                                      SFORMATTEDDATA_CAN* CurrDataCAN,
                                      BYTE bExprnFlag_Log)
{
    if (RX_FLAG == pMsgCAN->m_ucDataType)
    {
        CurrDataCAN->m_eDirection = DIR_RX;
        CurrDataCAN->m_acMsgDir[0] = L'R';
        CurrDataCAN->m_eEventType = ERROR_INVALID;
    }
    else if (TX_FLAG == pMsgCAN->m_ucDataType)
    {
        CurrDataCAN->m_eDirection = DIR_TX;
        CurrDataCAN->m_acMsgDir[0] = L'T';
        CurrDataCAN->m_eEventType = ERROR_INVALID;
    }
    else
    {
        std::string strErrMsg;
        ERROR_STATE eErrType;
        vFormatErrMsg(pMsgCAN->m_uDataInfo.m_sErrInfo, eErrType);
        CurrDataCAN->m_eEventType = eErrType;
        ASSERT(false);
    }

    CurrDataCAN->m_eChannel = pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucChannel;
    if ((CurrDataCAN->m_eChannel >= CHANNEL_CAN_MIN) && (CurrDataCAN->m_eChannel <= CHANNEL_CAN_MAX ))
    {
        sprintf(CurrDataCAN->m_acChannel, "%d", CurrDataCAN->m_eChannel);
    }

    if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucEXTENDED != 0)
    {
        CurrDataCAN->m_byIDType = TYPE_ID_CAN_EXTENDED;
        CurrDataCAN->m_acType[0] = L'x';
    }
    else
    {
        CurrDataCAN->m_byIDType = TYPE_ID_CAN_STANDARD;
        CurrDataCAN->m_acType[0] = L's';
    }

    if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucRTR != 0)
    {
        CurrDataCAN->m_byMsgType = TYPE_MSG_CAN_RTR;
        CurrDataCAN->m_acType[1] = L'r';
    }
    else
    {
        CurrDataCAN->m_byMsgType = TYPE_MSG_CAN_NON_RTR;
        CurrDataCAN->m_acType[1] = L'\0';
    }

    _itot(pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucDataLen, CurrDataCAN->m_acDataLen, 10);
    _tcscpy(CurrDataCAN->m_acMsgDesc,  "Description");

    CurrDataCAN->m_u64TimeStamp = pMsgCAN->m_lTickCount.QuadPart;
    CurrDataCAN->m_dwMsgID = pMsgCAN->m_uDataInfo.m_sCANMsg.m_unMsgID;
    CurrDataCAN->m_byDataLength = pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucDataLen;
    memcpy(CurrDataCAN->m_abData, pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucData,
           CurrDataCAN->m_byDataLength);

    vFormatTime(bExprnFlag_Log, CurrDataCAN);
    vFormatDataAndId(bExprnFlag_Log, CurrDataCAN);
}

void CFormatMsgCAN::vFormatErrMsg(SERROR_INFO sErrInfo, ERROR_STATE& eErrType)
{
    eErrType = ERROR_ACTIVE;
    USHORT usErrorID = ERROR_UNKNOWN;
    if (sErrInfo.m_ucErrType == ERROR_WARNING_LIMIT_REACHED)
    {
        usErrorID = sErrInfo.m_ucErrType;
        eErrType = ERROR_WARNING_LIMIT;
    }
    else
    {
        if (sErrInfo.m_ucErrType == ERROR_BUS)
        {
            // Update Statistics information
            usErrorID = sErrInfo.m_ucReg_ErrCap /*& 0xE0*/;
            // Create Channel ID & Error code word
        }
        else if (sErrInfo.m_ucErrType == ERROR_INTERRUPT)
        {
            usErrorID = sErrInfo.m_ucErrType;
        }

        ERROR_STATE bErrTransState = ERROR_INVALID;
        if(bIsTransitionInState(sErrInfo.m_ucChannel, sErrInfo.m_ucRxErrCount, sErrInfo.m_ucTxErrCount, bErrTransState))
        {
            if (usErrorID == STUFF_ERROR_RX)
            {
                eErrType = ERROR_FRAME;
            }
            else
            {
                eErrType = bErrTransState;
            }
        }
    }
}

BOOL CFormatMsgCAN::bIsTransitionInState(UINT /* unChannel */, BYTE byRxError, BYTE byTxError, ERROR_STATE& eErrState)
{
    BOOL bIsTransition = FALSE;
    // Based on the value of transmit and receive error counter decide
    // the current error state

    if ((byTxError <= 127) && (byRxError <= 127))
    {
        // Error Active Mode
        if (eErrState != ERROR_ACTIVE)
        {
            bIsTransition = TRUE;
            eErrState = ERROR_ACTIVE;
        }
    }
    else if (byTxError == 255)
        // The sudden shift to the third state is to avoid
        // "else if ((byTxError > 127) || (byRxError > 127))"
    {
        // Bus off
        if (eErrState != ERROR_BUS_OFF)
        {
            bIsTransition = TRUE;
            eErrState = ERROR_BUS_OFF;
        }
    }
    else
    {
        // Error passive
        if (eErrState != ERROR_PASSIVE)
        {
            bIsTransition = TRUE;
            eErrState = ERROR_PASSIVE;
        }
    }

    return bIsTransition;
}