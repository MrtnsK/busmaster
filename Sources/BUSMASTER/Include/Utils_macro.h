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
 * \file      Utils_macro.h
 * \brief     This contains various necessary macro definitions.
 * \authors   Ratnadip Choudhury, Anish Kumar, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains various necessary macro definitions.
 */

#pragma once

#define FORMAT_STR_ID_HEX       "0x%03X"
#define FORMAT_STR_ID_DEC       "%04d"
#define FORMAT_STR_DATA_HEX     "%02X "
#define FORMAT_STR_DATA_DEC     "%03d"
#define FORMAT_STR_CHECKSUM_HEX "0x%02X "
#define FORMAT_STR_CHECKSUM_DEC "%03d"
#define FORMAT_STR_HEADER_CRC_HEX           "0x%04X"
#define FORMAT_STR_HEADER_CRC_DEC           "%04d"
#define FORMAT_STR_HEX          "0x%02X"
#define FORMAT_STR_DEC          "%02d"
#define FORMAT_PGN_ID_HEX       "0x%06X"
#define FORMAT_PGN_ID_DEC       "%06d"

#define BIT_TM_ABS              0x01
#define BIT_TM_REL              0x02
#define BIT_TM_SYS              0x04
#define BIT_NUM_HEX             0x08
#define BIT_NUM_DEC             0x10
#define BIT_TM_ABS_RES          0x20

#define SET_TM_ABS(Flag)        (Flag |= BIT_TM_ABS)
#define SET_TM_REL(Flag)        (Flag |= BIT_TM_REL)
#define SET_TM_SYS(Flag)        (Flag |= BIT_TM_SYS)
#define SET_NUM_HEX(Flag)       (Flag |= BIT_NUM_HEX)
#define SET_NUM_DEC(Flag)       (Flag |= BIT_NUM_DEC)
#define SET_TM_ABS_RES(Flag)    (Flag |= BIT_TM_ABS_RES)

#define IS_TM_ABS_SET(Flag)     (Flag & BIT_TM_ABS)
#define IS_TM_REL_SET(Flag)     (Flag & BIT_TM_REL)
#define IS_TM_SYS_SET(Flag)     (Flag & BIT_TM_SYS)
#define IS_NUM_HEX_SET(Flag)    (Flag & BIT_NUM_HEX)
#define IS_NUM_DEC_SET(Flag)    (Flag & BIT_NUM_DEC)
#define IS_TM_ABS_RES(Flag)    (Flag & BIT_TM_ABS_RES)

#define CLEAR_EXPR_FLAG(Flag)   (Flag &= 0x0)

#define DISP_MODE_OVER          0x20
#define DISP_MODE_APPEND        0x40
#define DISP_MODE_INTRP         0x80
const BYTE BITS_DISP            = (DISP_MODE_OVER | DISP_MODE_APPEND |
                                   DISP_MODE_INTRP);
const BYTE BITS_TM              = (BIT_TM_ABS | BIT_TM_REL | BIT_TM_SYS);
const BYTE BITS_NUM             = (BIT_NUM_HEX | BIT_NUM_DEC);
const BYTE BITS_EXPRN           = (BITS_TM | BITS_NUM);

//addded by bhaskar
#define CLEAR_MONITOR_FLAG(Flag)   (Flag &= 0x0)
#define BIT_FRAME_MON_SET        0x1
#define BIT_PDU_MON_SET          0x2
#define SET_FRAME_MONITOR_SET(Flag)    (Flag |= BIT_FRAME_MON_SET)
#define SET_PDU_MONITOR_SET(Flag)    (Flag |= BIT_PDU_MON_SET)
#define IS_FRAME_MONITOR_SET(Flag)    (Flag & BIT_FRAME_MON_SET)
#define IS_PDU_MONITOR_SET(Flag)    (Flag & BIT_PDU_MON_SET)

/*#define SET_FRAME_MONITOR_SET()    {monitorFlag |= BIT_FRAME_MON_SET;}
#define SET_PDU_MONITOR_SET()    {monitorFlag |= BIT_PDU_MON_SET;}
#define CLEAR_FRAME_MONITOR_SET()    {monitorFlag &= BIT_PDU_MON_SET;}
#define CLEAR_PDU_MONITOR_SET()    {monitorFlag &= BIT_FRAME_MON_SET;}
#define GET_MONITOR_FLAG(Flag) {Flag = monitorFlag;}
//#define IS_FRAME_MONITOR_SET(Flag)    (Flag &= monitorFlag)
//#define IS_PDU_MONITOR_SET(Flag)    (Flag &= monitorFlag)*/

//const BYTE BITS_MON_MSG              = ( BIT_TM_ABS | BIT_TM_REL | BIT_TM_SYS | BIT_NUM_HEX | BIT_NUM_DEC |BIT_TM_ABS_RES | DISP_MODE_APPEND | DISP_MODE_INTRP);
//static BYTE monitorFlag =0;
//end bhaskar

#define CLEAR_EXPR_NUM_BITS(Flag)    (Flag &= (~BITS_NUM))
#define CLEAR_EXPR_DISP_BITS(Flag)   (Flag &= (~BITS_DISP))
#define CLEAR_EXPR_TM_BITS(Flag)     (Flag &= (~BITS_TM))

// In any scenario these two bits are mutually exclusive, as far as the
// current implementation is concerned.
#define SET_MODE_OVER(Flag)     (Flag = CLEAR_EXPR_DISP_BITS(Flag) | DISP_MODE_OVER)
#define SET_MODE_APPEND(Flag)   (Flag = CLEAR_EXPR_DISP_BITS(Flag) | DISP_MODE_APPEND)
#define SET_MODE_INTRP(Flag)    (Flag = CLEAR_EXPR_DISP_BITS(Flag) | DISP_MODE_INTRP)

#define IS_MODE_OVER(Flag)      (Flag & DISP_MODE_OVER)
#define IS_MODE_APPEND(Flag)    (Flag & DISP_MODE_APPEND)
#define IS_MODE_INTRP(Flag)     (Flag & DISP_MODE_INTRP)

// To copy the data and advance the pointer of the target data stream
#ifndef COPY_DATA
#define COPY_DATA(TgtStream, SrcStream, TotBytes) {memcpy(TgtStream, SrcStream, TotBytes); TgtStream += TotBytes;}
#endif

// To copy the data and advance the pointer of the source data stream
#ifndef COPY_DATA_2
#define COPY_DATA_2(TgtStream, SrcStream, TotBytes) {memcpy(TgtStream, SrcStream, TotBytes); SrcStream += TotBytes;}
#endif

#define UNUSED_INTENTIONALLY(X)    (X)

#define DELETE_PTR(PtrVal)      {                                       \
        if (nullptr != PtrVal)                 \
        {                                   \
            delete PtrVal; PtrVal = nullptr;   \
        }                                   \
    }

#define DELETE_ARRAY(PtrVal)    {                                       \
        if (nullptr != PtrVal)                 \
        {                                   \
            delete[] PtrVal; PtrVal = nullptr; \
        }                                   \
    }
