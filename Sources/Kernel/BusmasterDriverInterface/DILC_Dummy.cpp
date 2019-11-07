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
 * \file      DIL_Interface/DILC_Dummy.cpp
 * \brief     Source file for dummy DIL functions
 * \authors   Ratnadip Choudhury, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for dummy DIL functions
 */
#include "stdafx.h"
#include "DILC_Dummy.h"
#include "include/Error.h"


HRESULT CDIL_CAN_DUMMY::CAN_PerformInitOperations(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_PerformClosureOperations(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_GetTimeModeMapping(SYSTEMTIME& /*CurrSysTime*/, UINT64& /*TimeStamp*/, LARGE_INTEGER& /*QueryTickCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_ListHwInterfaces(INTERFACE_HW_LIST& /*asSelHwInterface*/, INT& /*nCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_SelectHwInterface(const INTERFACE_HW_LIST& /*asSelHwInterface*/, INT /*nCount*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_DeselectHwInterface(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_DisplayConfigDlg(PSCONTROLLER_DETAILS /*InitData*/, int& /*Length*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_SetConfigData(PSCONTROLLER_DETAILS /*pInitData*/, int /*Length*/)
{
    return WARN_DUMMY_API;
}

/*HRESULT CDIL_CAN_DUMMY::CAN_Connect(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_Disconnect(void)
{
    return WARN_DUMMY_API;
}*/

HRESULT CDIL_CAN_DUMMY::CAN_StartHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_StopHardware(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_GetCurrStatus( STATUSMSG& /*StatusData*/ )
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_SendMsg(DWORD /*dwClientID*/, const STCAN_MSG& /*sCanTxMsg*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_GetLastErrorString(std::string& /*acErrorStr*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_ManageMsgBuf(BYTE /*bAction*/, DWORD /*ClientID*/, CBaseCANBufFSE* /*pBufObj*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_RegisterClient(BOOL /*bRegister*/, DWORD& /*ClientID*/, char* /*pacClientName*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_GetCntrlStatus(const HANDLE& /*hEvent*/, UINT& /*unCntrlStatus*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_GetControllerParams(LONG& /*lParam*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_SetControllerParams(int /* nValue */, ECONTR_PARAM /* eContrparam */)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_SetAppParams(HWND /* hWndOwner */)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_GetErrorCount(SERROR_CNT& /*sErrorCnt*/, UINT /*nChannel*/, ECONTR_PARAM /*eContrParam*/)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_LoadDriverLibrary(void)
{
    return WARN_DUMMY_API;
}

HRESULT CDIL_CAN_DUMMY::CAN_UnloadDriverLibrary(void)
{
    return WARN_DUMMY_API;
}
HRESULT CDIL_CAN_DUMMY::CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount)
{
    return WARN_DUMMY_API;
}