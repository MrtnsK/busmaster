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
 * \file      SignalWatchDefs.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#define WM_REMOVE_SIGNAL            WM_USER + 41
#define WM_ADDDEL_SIGNAL            WM_USER + 42
#define defSTR_PHYSICAL_COLUMN      "Physical Value"
#define defSTR_RAW_COLUMN           "Raw Value"

#define defSW_LIST_COLUMN_COUNT     4
#define defSTR_SW_MSG_NAME          "Message"
#define defSTR_SW_MSG_COL           0
#define defSTR_SW_SIG_NAME          "Signal"
#define defSTR_SW_SIG_COL           1
#define defSTR_SW_RAW_VALUE         defSTR_RAW_COLUMN
#define defSTR_SW_RAW_VAL_COL       3
#define defSTR_SW_PHY_VALUE         defSTR_PHYSICAL_COLUMN
#define defSTR_SW_PHY_VAL_COL       2
#define defCOLOR_WHITE              RGB(255,255,255)
#define defSTR_SIGNAL_WATCH_FMT     " %-2s %-16s-> %-14s %-10s[%s]"



#define defSTR_MSG_SIG_SEPERATER " -> "
#define defSTR_FORMAT_SW_LIST    "%s%s%s"
#define defSIZE_OF_IMAGE         25
#define defSTR_SW_DELETE_ERROR   "Error while deleting temporary list!!!"
#define defSTR_SW_DELETE_SIG_ERROR  "Error while deleting Signal %s from Message %s !!!"
#define defSTR_SW_DELETE_SIG_MSGID_ERROR "Error while deleting Signal %s from Message ID: %x !!!"
#define defSTR_SW_PARSE_ERROR    "Parse Error in %s "
#define defSTR_SW_SIGNAL_DETAIL_ERROR   "Signal Not Found in the Database!!\nPlease remove the Signal"
#define defSTR_SW_SIGNAL_GENERIC_ERROR  "Error showing Signal Details!!"
#define defSTR_MSG_ID_IN_HEX          "[0x%x]"
#define defMSGID_EXTENDED       'x'
#define defMSGID_NAME_DELIMITER  "["
#define defMSGID_NAME_START_CHAR '['
#define defMSG_NAME_END_CHAR     ']'

#define defSTR_FORMAT_DISPLAY_DEC    "%-60s   [%16I64d]"
#define defSTR_FORMAT_DISPLAY_HEX    "%-60s   [%16I64X]"
#define defSTR_FORMAT_OL_INTRP   "         %-20s %-20s"
#define defSTR_FORMAT_PHY_VALUE  "%.3f"
#define defSTR_FORMAT_PHY_VALUE_WITH_UNIT  "%16s %s"

#define defSIGNAL_ICON_SIZE               16