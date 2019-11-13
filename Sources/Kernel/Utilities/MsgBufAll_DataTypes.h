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
 * \file      MsgBufAll_DataTypes.h
 * \brief     Defines concrete class for data types of different buses.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines concrete class for data types of different buses.
 */

#pragma once

#include "include/Struct_CAN.h"
#include "include/Struct_LIN.h"
#include "include/BaseDefs.h"
#include "DataTypes/J1939_DataTypes.h"
#include "Base_FlexRay_Buffer.h"

#include "MsgBufFSE.h"
#include "MsgBufVFSE.h"

typedef CBaseMsgBufFSE<STCANDATA> CBaseCANBufFSE;
typedef CMsgBufFSE<STCANDATA> CCANBufFSE;
typedef CMsgBufFSE<STLINDATA> CLINBufFSE;
typedef CMsgBufVFSE<STJ1939_MSG> CJ1939BufVFSE;
typedef CBaseMsgBufFSE<STLINDATA> CBaseLINBufFSE;
//typedef CMsgBufFSE<STLINDATA> CLINBufFSE;
//typedef CMsgBufCANVFSE<STCANDATA> CCANBufCANVFSE;
typedef CBaseMsgBufFSE<s_FLXMSG> CBaseFLEXBufFSE;
typedef CMsgBufFSE<s_FLXMSG>     CFLEXBufFSE;