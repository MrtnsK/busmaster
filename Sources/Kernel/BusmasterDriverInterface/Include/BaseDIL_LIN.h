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
 * \file      BaseDIL_LIN.h
 * \brief     Base class for CDIL_LIN class.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Base class for CDIL_LIN class.
 */
#if !defined BASEDIL_LIN_H__INCLUDED_
#define BASEDIL_LIN_H__INCLUDED_

#include "LINDriverDefines.h"
#include "../../BusmasterDBNetwork/Include/IBMNetWorkGetService.h"
#include "IBusService.h"
class CBaseDIL_LIN : public IBusService
{
public:
    /**
     * Constructor
     */
    CBaseDIL_LIN() {};

    /**
     * Destructor
     */
    ~CBaseDIL_LIN() {};

    /**
     * Based on the parameter this function renders number of the driver interface
     * layers supported or available. If 'bAvailable' is true, this returns number of
     * the DILs implemented; else the list of the DILs supported by the existing
     * license will be returned. If List is NULL, only number is returned.
     */
    virtual DWORD DILL_GetDILList(bool bAvailable, DILLIST* List) = 0;

    /**
     * DILL_SelectDriver selects a driver abstraction layer (DAL). If support for the
     * intended one isn't allowed with the present license, this returns NO_LICENSE.
     * A list for DALs (or bDriverID) is shown below:
     * DRIVER_LIN_STUB, //Simulation engine without the hardware
     * DRIVER_LIN_PEAK_USB, // PEAK-LIN USB hardware
     * DRIVER_LIN_PEAK_PP,  // PEAK-LIN Parallel Port Interface
     * DRIVER_LIN_ICS_NEOVI, // IntrepidCS neoVI hardware
     * DRIVER_LIN_ETAS_BOA.     // ETAS BOA Framework
     */
    virtual HRESULT DILL_SelectDriver(DWORD dwDriverID, HWND hWndParent) = 0;

    /**
     * DILL_RegisterClient registers/unregisters the client.
     * bRegister = TRUE for registeration
     * bRegister = FALSE for unregisteration
     * Only registered client's buffer will be updated on receive of a msg in the bus.

     * Following are the return values
     * S_OK -->registeration successful
     * ERR_CLIENT_EXISTS -->Client already registered. Client Id will be stored in ClientId.
     * ERR_NO_CLIENT_EXIST --> No client exists. Usually this happens if we try to remove non existing client.
     * ERR_NO_MORE_CLIENT_ALLOWED -->No more clients are allowed to register.
     */
    virtual HRESULT DILL_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName) = 0;

    /**
     * This function manages the target message buffer list. The two combinations
     * are the following:
     * bAction = MSGBUF_ADD, add pBufObj to the target message buffer list.
     * bAction = MSGBUF_CLEAR, clear the list. pBufObj is ignored.
     * At present maximum number of entries in the list is kept as 8.
     */
    virtual HRESULT DILL_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseLINBufFSE* pBufObj) = 0;

    /**
     * This returns ID of the driver interface layer presently selected.
     */
    virtual DWORD DILL_GetSelectedDriver(void) = 0;

    /**
     * Call for all initialisation operations.
     */
    virtual HRESULT DILL_PerformInitOperations(void) = 0;

    /**
     * Call for all uninitialisation operations
     */
    virtual HRESULT DILL_PerformClosureOperations(void) = 0;

    /**
     * Call this function to get a system time and the time stamp associated with it
     */
    virtual HRESULT DILL_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount) = 0;

    /**
     * Call this function to list the hardware interfaces available and receive
     * user's choice.
     */
    virtual HRESULT DILL_ListHwInterfaces(INTERFACE_HW_LIST& asSelHwInterface, INT& nCount) = 0;

    /**
     * Call this function to select a particular hardware interface.
     */
    virtual HRESULT DILL_SelectHwInterfaces(const INTERFACE_HW_LIST& asSelHwInterface, INT nCount) = 0;

    /**
     * Call this function to deselect the selected hardware interface.
     */
    virtual HRESULT DILL_DeselectHwInterfaces(void) = 0;

    /**
     * Function to display the configuration dialog box for the selected DIL. If
     * the dialog box needs to be displayed been initialised, pass the relevant data
     * as InitData. If it is null, the dialog box is uninitialised. This also contains
     * the user's choice as OUT parameter
     */


    /**
     * To set the configuration data for the currently selected DIL. Caller must
     * release the memory.
     */
    virtual HRESULT DILL_SetConfigData( IBMNetWorkGetService* ouConfig) = 0;
    //virtual HRESULT DILL_SetConfigData(PSCONTROLLER_DETAILS pInitData, int Length) = 0;

    /**
     * Start the controller
     */
    virtual HRESULT DILL_StartHardware(void) = 0;

    virtual HRESULT DILL_PreStartHardware(void) = 0;

    /**
     * Stop the controller
     */
    virtual HRESULT DILL_StopHardware(void) = 0;

    /**
     * Reset Hardware
     */
    virtual HRESULT DILL_ResetHardware(void) = 0;

    /**
    * Reset Hardware
    */
    virtual HRESULT DILL_SetSlaveRespData(const STLIN_MSG stRespMsg) = 0;
    virtual HRESULT DILL_ResetSlaveRespData(void)=0;
    virtual HRESULT DILL_DisableSlaveRespData(DWORD dwClientID, STLIN_MSG& sMessage) = 0;

    virtual HRESULT DILL_GetConfiguration( SCONTROLLER_DETAILS_LIN[], INT& nSize ) = 0;
    /**
     * Send messages
     */
    virtual HRESULT DILL_SendMsg(DWORD dwClientID, STLIN_MSG& sLinTxMsg) = 0;

    /**
    * Call to get descriptive string of the last error occurred
     */
    virtual HRESULT DILL_GetLastErrorString(std::string& acErrorStr) = 0;

    /**
     * Call to get controller status. Caller has to give the handle of a
     * event which will set whenever the controller changes the state.
     * #define defCONTROLLER_ACTIVE                   1
     * #define defCONTROLLER_PASSIVE                  2
     * #define defCONTROLLER_BUSOFF                   3
     */
    virtual HRESULT DILL_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus) = 0;

    /**
     * Call to get Controller parameters. Value will be returned stored in lParam
     * Possible values for E_LIN_CONTR_PARAM are ...
     */
    virtual HRESULT DILL_GetControllerParams( LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam ) = 0;
    /**
     *
     *
     */
    virtual HRESULT DILL_SetControllerParams( int nValue, ECONTR_PARAM eContrparam ) = 0;
    /**
     * Call to Get Error Counts
     */
    virtual HRESULT  DILL_GetErrorCount( SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam ) = 0;


    //Master Mode Commands

    //Schedule table
    /**
     * Register a schedule table to DIL.Return an handle of the table, which can be used later to refer to the table
     *
     **/
    virtual HRESULT DILL_RegisterLinScheduleTable( DWORD& dwClientId, int& nChannel, CSheduleTable ouTable, int& nHandle) = 0;

    /**
     * Deregister a specified schedule table from DIL.
     *
     **/
    virtual HRESULT DIIL_DeRegisterLinScheduleTabel(DWORD& dwClientId, int& nChannel, int& nTableHandle) = 0;

    /**
     * Starts specified schedule table. The currently executing table will be stopped
     *
     **/
    virtual HRESULT DILL_StartLinScheduleTable(DWORD& dwClientId, int& nChannel, int& nTableHandle) = 0;

    /**
     * Enable or Disable a perticular Header transmission int Lin Scheduling table.
     *
     **/
    virtual HRESULT DIIL_EnableLinScheuleCommand(DWORD& dwClientId, int& nChannel, int nTableHandle, int nIndex, bool bEnable) = 0;


    virtual HRESULT DILL_UpdateLinScheduleTable( DWORD& dwClientId, int& nChannel, int& nHandle, CSheduleTable& ouTable ) = 0;

    //Individual commands.
    virtual HRESULT DIIL_RegisterLinHeader(DWORD& dwClientId, int& nChannel, int nId, int nCycleTimer) = 0;
    virtual HRESULT DIIL_DeRegisterLinHeader(DWORD& dwClientId, int& nChannel, int nId) = 0;
};

#endif // BASEDIL_LIN_H__INCLUDED_
