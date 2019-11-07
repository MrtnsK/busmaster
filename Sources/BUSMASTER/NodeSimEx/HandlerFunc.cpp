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
 * \file      HandlerFunc.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "NodeSimEx_stdafx.h"
#include "HandlerFunc.h"
#include "ExecuteManager.h"
#include "AppServicesCAN.h"
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

CRITICAL_SECTION g_omDllMsgCritiSec;

/******************************************************************************
    Function Name    :  unKeyHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined key
                        handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        user-defined key handler function.
                        - User will be notified status of transmission.
    Member of        :  CExecuteFunc
    Friend of        :      -
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  06.11.2002
    Modifications    :  Anish kumar
                        30.12.05 moved to CExecuteFunc ,refer CEx members
******************************************************************************/
DWORD WINAPI unKeyHandlerProc(LPVOID pParam)
{
    if (pParam != nullptr)
    {
        PSEXECUTE_KEY_HANDLER psExecuteKeyHandler =
            (PSEXECUTE_KEY_HANDLER) pParam;
        // There is no memory allocation is this thread. So initialise it to nullptr
        psExecuteKeyHandler->m_pCExecuteFunc->m_asUtilThread[defKEY_HANDLER_THREAD].
        m_pvThread = nullptr;
        // Reset the event signaled
        psExecuteKeyHandler->m_pCExecuteFunc->
        m_aomState[defKEY_HANDLER_THREAD ].ResetEvent();

        try
        {
            psExecuteKeyHandler->
            m_pFKeyHandler(psExecuteKeyHandler->m_ucKeyValue);
        }
        catch(...)
        {
            CHAR acError[256];
            sprintf( acError,
                     _(defSTR_ERROR_IN_KEY_PROG),
                     psExecuteKeyHandler->m_ucKeyValue );
            // Display the error information in the Trace window
            gbSendStrToTrace(acError);
        }

        //       psExecuteKeyHandler = nullptr;
        psExecuteKeyHandler->m_pCExecuteFunc->
        m_asUtilThread[defKEY_HANDLER_THREAD].m_pvThread = nullptr;
        // Reinitialise the Thread handle before terminating it.
        psExecuteKeyHandler->m_pCExecuteFunc->
        m_asUtilThread[defKEY_HANDLER_THREAD].m_hThread = nullptr;
        // Set the event to indicate termination of this thread.
        psExecuteKeyHandler->m_pCExecuteFunc->
        m_aomState[defKEY_HANDLER_THREAD].SetEvent();
        delete psExecuteKeyHandler;
    }
    return 0;
}
UINT unErrorHandlerProcLin(LPVOID pParam)
{
    if (pParam != nullptr)
    {
        PSEVENTHANDLERLIN pErrorHandler = (PSEVENTHANDLERLIN) pParam; //ell2kor
        // There is no memory allocation is this thread. So initialise it to nullptr
        pErrorHandler->m_pCExecuteFunc->
        m_asUtilThread[defERROR_HANDLER_THREAD].m_pvThread = nullptr;
        // Reset the event signaled
        pErrorHandler->m_pCExecuteFunc->
        m_aomState[defERROR_HANDLER_THREAD].ResetEvent();
        try
        {
            switch ( pErrorHandler->m_ouLinEventInfo.m_eEventType )
            {
                case EVENT_LIN_ERRCRC:
                {
                    SEVENT_CHECKSUM_LIN ouCrcInfo;
                    ouCrcInfo.m_ucChannel = pErrorHandler->m_ouLinEventInfo.m_ucChannel;
                    ouCrcInfo.m_ucCrc = pErrorHandler->m_ouLinEventInfo.m_ucCrc;
                    ouCrcInfo.m_ucId = pErrorHandler->m_ouLinEventInfo.m_ucId;
                    ouCrcInfo.m_ulTime = (ULONG)pErrorHandler->m_ouLinEventInfo.m_ulTimeStamp;
                    pErrorHandler->m_pfEventHandlersLin(ouCrcInfo);
                }
                break;
                case EVENT_LIN_ERRNOANS:
                {
                    SEVENT_SLAVE_NORESP_LIN ouSlaveInfo;
                    ouSlaveInfo.m_ucChannel = pErrorHandler->m_ouLinEventInfo.m_ucChannel;
                    ouSlaveInfo.m_ucId = pErrorHandler->m_ouLinEventInfo.m_ucId;
                    ouSlaveInfo.m_ulTime = (ULONG)pErrorHandler->m_ouLinEventInfo.m_ulTimeStamp;
                    pErrorHandler->m_pfEventHandlersLin(ouSlaveInfo);
                }
                break;
                case EVENT_LIN_ERRSYNC:
                {
                    SEVENT_SYNC_LIN ouSyncLin;
                    ouSyncLin.m_ucChannel = pErrorHandler->m_ouLinEventInfo.m_ucChannel;
                    ouSyncLin.m_ulTime = (ULONG)pErrorHandler->m_ouLinEventInfo.m_ulTimeStamp;
                    pErrorHandler->m_pfEventHandlersLin(ouSyncLin);
                }
                break;
            }
        }
        catch(...)
        {
            // Display the error information in the Trace window
            gbSendStrToTrace(_(defSTR_ERROR_IN_ERR_PROG));
        }
        // Reinitialise the Thread handle before terminating it.
        pErrorHandler->m_pCExecuteFunc->m_asUtilThread[defERROR_HANDLER_THREAD].m_hThread = nullptr;


        // Set the event to indicate termination of this thread.
        pErrorHandler->m_pCExecuteFunc->
        m_aomState[defERROR_HANDLER_THREAD].SetEvent();
    }
    return 0;
}

/******************************************************************************
    Function Name    :  unErrorHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined error
                        handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        user-defined error handler function.
                        - User will be notified status of transmission.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  25.02.2003
    Modifications    :
******************************************************************************/
UINT unErrorHandlerProc(LPVOID pParam)
{
    if (pParam != nullptr)
    {
        PSERRORHANDLER pErrorHandler = (PSERRORHANDLER) pParam; //ell2kor
        // There is no memory allocation is this thread. So initialise it to nullptr
        pErrorHandler->m_pCExecuteFunc->
        m_asUtilThread[defERROR_HANDLER_THREAD].m_pvThread = nullptr;
        // Reset the event signaled
        pErrorHandler->m_pCExecuteFunc->
        m_aomState[defERROR_HANDLER_THREAD].ResetEvent();
        try
        {
            pErrorHandler->m_pFErrorHandlers(pErrorHandler->sErrorVal); //ell2kor
        }
        catch(...)
        {
            // Display the error information in the Trace window
            gbSendStrToTrace(_(defSTR_ERROR_IN_ERR_PROG));
        }
        // Reinitialise the Thread handle before terminating it.
        pErrorHandler->m_pCExecuteFunc->
        m_asUtilThread[defERROR_HANDLER_THREAD].m_hThread = nullptr;
        // Set the event to indicate termination of this thread.
        pErrorHandler->m_pCExecuteFunc->
        m_aomState[defERROR_HANDLER_THREAD].SetEvent();
    }
    return 0;
}

/******************************************************************************
    Function Name    :  unEventHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined event
                        handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        user-defined event handler function.
                        - User will be notified status of transmission.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Pradeep Kadoor
    Date Created     :  08.02.2011
    Modifications    :
******************************************************************************/
UINT unEventHandlerProc(LPVOID pParam)
{
    PSEVENTHANDLER pEventHandler = (PSEVENTHANDLER) pParam;
    pEventHandler->m_pCExecuteFunc->
    m_asUtilThread[defEVENT_HANDLER_THREAD].m_hThread = AfxGetThread();
    if (pParam != nullptr)
    {
        // PSEVENTHANDLER pEventHandler = (PSEVENTHANDLER) pParam; //ell2kor
        // There is no memory allocation is this thread. So initialise it to nullptr
        pEventHandler->m_pCExecuteFunc->
        m_asUtilThread[defEVENT_HANDLER_THREAD].m_pvThread = nullptr;
        // Reset the event signaled
        pEventHandler->m_pCExecuteFunc->
        m_aomState[defEVENT_HANDLER_THREAD].ResetEvent();
        try
        {
            if (pEventHandler->m_byEventType == 0x0)
            {
                //call the user written function from the dll
                pEventHandler->m_pFEventHandlers(pEventHandler->m_unPGN, pEventHandler->m_bySrc,
                                                 pEventHandler->m_byDest, pEventHandler->m_bSuccess);
            }
            else if (pEventHandler->m_byEventType == 0x1)
            {
                pEventHandler->m_pFEventHandlers(pEventHandler->m_bySrc);
            }
        }
        catch(...)
        {
            // Display the error information in the Trace window
            gbSendStrToTrace(_(defSTR_ERROR_IN_EVENT_PROG));
        }
        // Reinitialise the Thread handle before terminating it.
        pEventHandler->m_pCExecuteFunc->
        m_asUtilThread[defEVENT_HANDLER_THREAD].m_hThread = nullptr;
        // Set the event to indicate termination of this thread.
        pEventHandler->m_pCExecuteFunc->
        m_aomState[defEVENT_HANDLER_THREAD].SetEvent();
    }
    return 0;
}

/******************************************************************************
    Function Name    :  unDLLUnloadHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined DLL handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        user-defined DLL unload handler function.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Amitesh Bharti
    Date Created     :  22.07.2004
    Modifications    :
******************************************************************************/
UINT unDLLUnloadHandlerProc(PSEXECUTE_LOAD_HANDLER psExecuteUnLoadHandler)
{
    if (psExecuteUnLoadHandler != nullptr)
    {
        // PSEXECUTE_LOAD_HANDLER psExecuteUnLoadHandler=(PSEXECUTE_LOAD_HANDLER) pParam;
        // Reset the event signaled
        /*psExecuteUnLoadHandler->m_pCExecuteFunc->
        m_aomState[defDLL_UNLOAD_HANDLER_THREAD].ResetEvent();*/
        if(psExecuteUnLoadHandler->m_pCExecuteFunc!=nullptr)
        {
            try
            {
                psExecuteUnLoadHandler->pFDllHandler();
            }
            catch(...)
            {
                // Display the error information in the Trace window
                gbSendStrToTrace(_(defSTR_ERROR_IN_DLL_UNLOAD));
            }
            // There is no memory allocation is this thread. So initialise it to nullptr
            psExecuteUnLoadHandler->m_pCExecuteFunc->
            m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_pvThread = nullptr;
            // Reinitialise the Thread handle before terminating it.
            psExecuteUnLoadHandler->m_pCExecuteFunc->
            m_asUtilThread[defDLL_UNLOAD_HANDLER_THREAD].m_hThread = nullptr;
            // Set the event to indicate termination of this thread.
            /* psExecuteUnLoadHandler->m_pCExecuteFunc->
             m_aomState[defDLL_UNLOAD_HANDLER_THREAD].SetEvent();*/
            delete psExecuteUnLoadHandler;
            psExecuteUnLoadHandler=nullptr;
        }
    }

    return 0;
}

/******************************************************************************
    Function Name    :  unDLLloadHandlerProc
    Input(s)         :  pParam - Typecasted address of user-defined DLL handler
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        user-defined DLL load handler function.
                        - User will be notified status of transmission.
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Ravikumar Patil
    Date Created     :  10.03.2003
    Modifications    :  Amitesh Bharti on 22.07.2004
                        Renamed the function as unDLLloadHandlerProc as the DLL
                        unload functionality got seprated
******************************************************************************/


UINT unDLLloadHandlerProc(LPVOID pParam)
{
    if (pParam != nullptr)
    {
        PSEXECUTE_LOAD_HANDLER psExecuteLoadHandler=(PSEXECUTE_LOAD_HANDLER) pParam;
        // Reset the event signaled
        if(psExecuteLoadHandler->m_pCExecuteFunc!=nullptr)
        {
            psExecuteLoadHandler->m_pCExecuteFunc->
            m_aomState[defDLL_LOAD_HANDLER_THREAD].ResetEvent();
            try
            {
                psExecuteLoadHandler->pFDllHandler();
            }
            catch(...)
            {
                // Display the error information in the Trace window
                gbSendStrToTrace(_(defSTR_ERROR_IN_DLL_LOAD));
            }
            // There is no memory allocation is this thread. So initialise it to nullptr
            psExecuteLoadHandler->m_pCExecuteFunc->
            m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_pvThread = nullptr;
            // Reinitialise the Thread handle before terminating it.
            psExecuteLoadHandler->m_pCExecuteFunc->
            m_asUtilThread[defDLL_LOAD_HANDLER_THREAD].m_hThread = nullptr;
            // Set the event to indicate termination of this thread.
            psExecuteLoadHandler->m_pCExecuteFunc->
            m_aomState[defDLL_LOAD_HANDLER_THREAD].SetEvent();
            delete psExecuteLoadHandler;
            psExecuteLoadHandler=nullptr;
        }
    }
    return 0;
}

UINT unBusPreConnectHandlerProc(LPVOID pParam)
{
    //USES_CONVERSION;
    if (pParam != nullptr)
    {
        PSEXECUTE_BUSEVENT_HANDLER psExecuteBusEventHandler=(PSEXECUTE_BUSEVENT_HANDLER) pParam;
        // Reset the event signaled
        if(psExecuteBusEventHandler->m_pCExecuteFunc!=nullptr)
        {
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_aomState[defBUSEVENT_HANDLER_THREAD].ResetEvent();
            try
            {
                psExecuteBusEventHandler->pFBusEventHandler();
            }
            catch(...)
            {
                // Display the error information in the Trace window
                gbSendStrToTrace(_(defSTR_ERROR_IN_BUS_PRE_CONNECT));
            }
            // There is no memory allocation is this thread. So initialise it to nullptr
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_asUtilThread[defBUSEVENT_HANDLER_THREAD].m_pvThread = nullptr;
            // Reinitialise the Thread handle before terminating it.
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_asUtilThread[defBUSEVENT_HANDLER_THREAD].m_hThread = nullptr;
            // Set the event to indicate termination of this thread.
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_aomState[defBUSEVENT_HANDLER_THREAD].SetEvent();
            delete psExecuteBusEventHandler;
            psExecuteBusEventHandler=nullptr;
        }
    }

    return 0;
}

UINT unBusConnectHandlerProc(LPVOID pParam)
{
    //USES_CONVERSION;
    if (pParam != nullptr)
    {
        PSEXECUTE_BUSEVENT_HANDLER psExecuteBusEventHandler=(PSEXECUTE_BUSEVENT_HANDLER) pParam;
        // Reset the event signaled
        if(psExecuteBusEventHandler->m_pCExecuteFunc!=nullptr)
        {
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_aomState[defBUSEVENT_HANDLER_THREAD].ResetEvent();
            try
            {
                psExecuteBusEventHandler->pFBusEventHandler();
            }
            catch(...)
            {
                // Display the error information in the Trace window
                gbSendStrToTrace(_(defSTR_ERROR_IN_BUS_DISCONNECT));
            }
            // There is no memory allocation is this thread. So initialise it to nullptr
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_asUtilThread[defBUSEVENT_HANDLER_THREAD].m_pvThread = nullptr;
            // Reinitialise the Thread handle before terminating it.
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_asUtilThread[defBUSEVENT_HANDLER_THREAD].m_hThread = nullptr;
            // Set the event to indicate termination of this thread.
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_aomState[defBUSEVENT_HANDLER_THREAD].SetEvent();
            delete psExecuteBusEventHandler;
            psExecuteBusEventHandler=nullptr;
        }
    }

    return 0;
}
UINT unBusDisConnectHandlerProc(LPVOID pParam)
{
    //USES_CONVERSION;
    if (pParam != nullptr)
    {
        PSEXECUTE_BUSEVENT_HANDLER psExecuteBusEventHandler=(PSEXECUTE_BUSEVENT_HANDLER) pParam;
        // Reset the event signaled
        if(psExecuteBusEventHandler->m_pCExecuteFunc!=nullptr)
        {
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_aomState[defBUSEVENT_HANDLER_THREAD].ResetEvent();
            try
            {
                psExecuteBusEventHandler->pFBusEventHandler();
            }
            catch(...)
            {
                // Display the error information in the Trace window
                gbSendStrToTrace(_(defSTR_ERROR_IN_BUS_CONNECT));
            }
            // There is no memory allocation is this thread. So initialise it to nullptr
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_asUtilThread[defBUSEVENT_HANDLER_THREAD].m_pvThread = nullptr;
            // Reinitialise the Thread handle before terminating it.
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_asUtilThread[defBUSEVENT_HANDLER_THREAD].m_hThread = nullptr;
            // Set the event to indicate termination of this thread.
            psExecuteBusEventHandler->m_pCExecuteFunc->
            m_aomState[defBUSEVENT_HANDLER_THREAD].SetEvent();
            delete psExecuteBusEventHandler;
            psExecuteBusEventHandler=nullptr;
        }
    }

    return 0;
}
/******************************************************************************
    Function Name    :  unReadNodeMsgHandlerBuffer
    Input(s)         :  pParam - Typecasted address CExecuteFunc object
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        reading of CExecutFunc's buffer
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  22.12.2005
    Modification By  :  Anish Kumar
    Modification on  :  29.01.07, Removed memory leak due to msg handler thread
                        termination
******************************************************************************/
UINT unReadNodeMsgHandlerBuffer(LPVOID pParam)
{
    if(pParam != nullptr)
    {
        CExecuteFunc* pCExecuteFunc=(CExecuteFunc*)pParam;
        pCExecuteFunc->m_aomState[defMSG_HANDLER_THREAD].ResetEvent();
        //if dll is unloaded exit loop and end thread
        while(pCExecuteFunc->bIsDllLoaded())
        {
            WaitForSingleObject( pCExecuteFunc->m_omReadFromQEvent,INFINITE);
            //wait for event set by write thread
            pCExecuteFunc->m_omReadFromQEvent.ResetEvent();
            UINT unMsgCnt=pCExecuteFunc->unGetBufferMsgCnt();
            //if buffer is empty wait for read event
            while( unMsgCnt>0 && pCExecuteFunc->bIsDllLoaded() &&
                    !(pCExecuteFunc->m_bStopMsgHandlers) )
            {
                STCAN_TIME_MSG sCanMsg=pCExecuteFunc->sReadFromQMsg();
                pCExecuteFunc->vExecuteOnMessageHandlerCAN(sCanMsg);
                unMsgCnt=pCExecuteFunc->unGetBufferMsgCnt();
            }
        }
        pCExecuteFunc->m_aomState[defMSG_HANDLER_THREAD].SetEvent( );
    }
    return 0;
}

/******************************************************************************
    Function Name    :  unReadNodeMsgHandlerBuffer
    Input(s)         :  pParam - Typecasted address CExecuteFunc object
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        reading of CExecutFunc's buffer
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  22.12.2005
    Modification By  :  Anish Kumar
    Modification on  :  29.01.07, Removed memory leak due to msg handler thread
                        termination
******************************************************************************/
UINT unReadNodeMsgHandlerBufferLIN(LPVOID pParam)
{
    if(pParam != nullptr)
    {
        CExecuteFunc* pCExecuteFunc=(CExecuteFunc*)pParam;

        if(nullptr != pCExecuteFunc)
        {
            pCExecuteFunc->m_aomState[defMSG_HANDLER_THREAD].ResetEvent();
            //if dll is unloaded exit loop and end thread
            while(pCExecuteFunc->bIsDllLoaded())
            {
                WaitForSingleObject( pCExecuteFunc->m_omReadFromQEventLIN,INFINITE);
                //wait for event set by write thread
                pCExecuteFunc->m_omReadFromQEventLIN.ResetEvent();
                UINT unMsgCnt=pCExecuteFunc->unGetBufferMsgCntLIN();
                //if buffer is empty wait for read event
                while( unMsgCnt>0 && pCExecuteFunc->bIsDllLoaded() &&
                        !(pCExecuteFunc->m_bStopMsgHandlers) )
                {
                    STLIN_TIME_MSG sLinMsg=pCExecuteFunc->sReadFromQMsgLIN();
                    pCExecuteFunc->vExecuteOnMessageHandlerLIN(sLinMsg);
                    unMsgCnt=pCExecuteFunc->unGetBufferMsgCntLIN();
                }
            }
            pCExecuteFunc->m_aomState[defMSG_HANDLER_THREAD].SetEvent( );
        }
    }
    return 0;
}

/******************************************************************************
    Function Name    :  unReadDllMsgBuffer
    Input(s)         :  pParam - Typecasted address CExecuteFunc object
    Output           :  Zero
    Functionality    :  This is a thread control function to process
                        reading of CExecutemanger's buffer
    Member of        :  Global Thread Function
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  22.12.2005
    Modifications    :  Anish kumar
                     :  29.01.07,changes the code to avoid mem leak
******************************************************************************/
const int SIZE_OF_SDLL_MSG=sizeof(SDLL_MSG);
UINT unReadDllMsgBuffer(LPVOID pParam)
{
    CExecuteManager* pExecMngr = (CExecuteManager*)pParam;
    while(g_bReadDllMsg)
    {
        pExecMngr->m_omDllMsgEvent.ResetEvent();
        if(g_unDllMsgCnt>0)
        {
            SDLL_MSG sDllMessages;
            EnterCriticalSection(&g_omDllMsgCritiSec);
            memcpy(&sDllMessages,&(g_asDllMessages[g_unReadDllMsg]),SIZE_OF_SDLL_MSG);
            g_unDllMsgCnt--;
            g_unReadDllMsg=(g_unReadDllMsg+1)%defMAX_HMODULE_MSG;
            LeaveCriticalSection(&g_omDllMsgCritiSec);
            CExecuteManager::ouGetExecuteManager(pExecMngr->m_eBus, &CGlobalObj::ouGetObj(pExecMngr->m_eBus)).vManageDllMessageHandler(sDllMessages);
            Sleep(0);
        }
        else
        {
            Sleep(50);
        }
    }
    pExecMngr->m_omDllMsgEvent.SetEvent();
    return 0;
}



/*************************************************************************
    Function Name    : unTimerHandlerProc
    Input(s)         : PSTIMERHANDLER
    Output           :
    Functionality    : Execute the task assigned to each timer
    Member of        :  Global timer handler
    Author(s)        :  Anish kumar
    Date Created     :  16.12.05
**************************************************************************/

UINT unTimerHandlerProc(LPVOID pParam)
{
    PSTIMERHANDLER psTimerStruct=(PSTIMERHANDLER)pParam;
    if(psTimerStruct!=nullptr)
        while(psTimerStruct->hDllHandle)
        {
            WaitForSingleObject(psTimerStruct->omWaitEvent,INFINITE);
            if(psTimerStruct->bTimerSelected)
            {
                psTimerStruct->omWaitEvent.ResetEvent();

                if( psTimerStruct->pFTimerHandler!=nullptr &&
                        psTimerStruct->bTimerSelected&&
                        psTimerStruct->hDllHandle )
                {
                    try
                    {
                        psTimerStruct->bFromTimer = TRUE;
                        psTimerStruct->pFTimerHandler();
                    }
                    catch(...)
                    {
                        CHAR acError[256];
                        sprintf( acError,
                                 _(defSTR_ERROR_IN_TIMER_PROG),
                                 psTimerStruct->omStrTimerHandlerName );
                        // Display the error information in the Trace window
                        gbSendStrToTrace(acError);

                    }
                    psTimerStruct->unCurrTime=0;
                    if( (psTimerStruct->bFromTimer == TRUE) && (psTimerStruct->bTimerType==TRUE))
                    {
                        psTimerStruct->bTimerSelected=FALSE;
                    }
                }

            }

        }
    if(psTimerStruct != nullptr)
    {
        psTimerStruct->pomThreadHandle=nullptr;
    }
    return 0;
}
