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
 * \file    CAN_i_VIEW/CAN_i_VIEW.h
 * \author  D Southworth
 * \copyright Copyright (c) 2013, Robert Bosch Automotive Service Solutions. All rights reserved.
 */

#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <array>

#include "VCiViewIF.h"
//#include "include/Error.h"
//#include "Include/Struct_CAN.h"
//#include "Include/BaseDefs.h"
//#include "Include/DIL_CommonDefs.h"
//#include "Include/CAN_Error_Defs.h"
//#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/MsgBufAll_DataTypes.h"
//#include "DataTypes/DIL_DataTypes.h"
#include "Utility/Utility_Thread.h"
#include "Utility/Utility.h"
#include "DIL_Interface\HardwareListingCAN.h"
#include "ChangeRegisters.h"
//#include "DIL_Interface/BaseDIL_CAN_Controller.h"
#include "BaseDIL_CAN_Controller.h"
#include "DILPluginHelperDefs.h"
/**
 * Starts code for the state machine
 */
enum State_e
{
    STATE_DRIVER_UNLOADED   = 0x0,
    STATE_DRIVER_LOADED,
    STATE_DRIVER_SELECTED,
    STATE_HW_INTERFACE_LISTED,
    STATE_HW_INTERFACE_SELECTED,
    STATE_CONNECTED
};

/**
 * \brief Buffer to read msg from the PIPE
 *
 * This buffer include CAN Msg + 1 Byte to indicate TX/RX + UINT64 to indicate timestamp
 */
struct tagPIPE_CANMSG
{
    BYTE m_byTxRxFlag;
    UINT64 m_unTimeStamp;
    STCAN_MSG m_sCanMsg;
};

typedef tagPIPE_CANMSG SPIPE_CANMSG;

const int SIZE_PIPE_CANMSG       = sizeof(BYTE) + sizeof(UINT64) + sizeof(STCAN_MSG);
const BYTE SIZE_TIMESTAMP = sizeof(UINT64);

#define MAX_CLIENT_ALLOWED  16
#define MAX_BUFF_ALLOWED    16

/**
 * See CAN_STUB.cpp for the implementation of this class
 */
class CCAN_i_VIEW_DLL : public CWinApp
{
public:
    CCAN_i_VIEW_DLL();

public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};

/**
 * String Utility Templates
 */

/**
 * fromString
 * \brief Read and convert the given type from a string.
 */
template<class T>
T FromString(const std::string& s, bool Hex=false)
{
    std::istringstream stream (s);
    T t;
    stream >> (Hex ? std::hex : std::dec) >> t;
    return t;
}

/**
 * toString
 * \brief Convert and write the given type to a string.
 */

template<class T>
static std::string ToString(const T& t, bool Hex=false)
{
    std::ostringstream stream;
    stream << (Hex ? std::hex : std::dec) << t;
    return stream.str();
}

/**
 * SetVal
 * \brief Set a value from a string
 * \detail
 *  Set the value of the template input based on the supplied
 *      source string.  Will sanity check the validity of the data
 *  where applicable (through specialisation) and indicate whether it was
 *  successful
 * \param Tgt       reference to the variable we wish to set
 * \param SourceStr Source string we are converting
 */
template<class T>
static bool SetVal(T& Tgt, std::string SourceStr)
{
    LOG(LogNOTICE) << " UNSUPPORTED CONVERSION";
    return false;
}
/* Specialisation of template for UNUM32s */
template<>
bool SetVal<UNUM32>(UNUM32& Tgt, std::string SourceStr)
{
    std::string::iterator Itr;
    for (Itr = SourceStr.begin(); Itr != SourceStr.end(); Itr++)
    {
        if (isdigit(*Itr) == 0)
        {
            return false;
        }
    }
    Tgt = FromString<UNUM32>(SourceStr);
    return true;
}

/* Specialisation of template for strings (simple copy) */
template<>
bool SetVal<std::string>(std::string& Tgt, std::string SourceStr)
{
    Tgt = SourceStr;
    return true;
}

/**
 * FromStringInMap
 * \brief Find a given string in a map
 * \detail
 *  Find the requested string in a map and if it's there, copy out to the
 *  supplied parameter, otherwise, populate with a default value.
 * \param Sought    The name of the value we want to extract
 * \param List      The map we are looking in
 * \param Tgt       The target variable for the data
 */
template<class T>
bool FromStringInMap(
    std::string Sought,
    const std::map< std::string, std::string >& Map,
    T&      Tgt)
{
    bool Found = false;

    std::map< std::string, std::string >::const_iterator Itr;
    if ( (Itr = Map.find(Sought)) != Map.end() )
    {
        SetVal(Tgt, (*Itr).second);
        Found = true;
    }
    return Found;
}

/**
 * VCI Filters
 */
class CFilter
{
public:
    CFilter( UNUM32 Type,
             UNUM32  Flags,
             UNUM32  Size,
             UNUM8*  Pattern,
             UNUM8*  Mask ) :
        m_Type(Type),
        m_Flags(Flags),
        m_Pattern(Pattern,Pattern+Size),
        m_Mask(Mask,Mask+Size),
        m_Id(0) {}
    UNUM32 Type()
    {
        return m_Type;
    }
    UNUM32 Flags()
    {
        return m_Flags;
    }
    UNUM32 Size()
    {
        return m_Pattern.size();
    }
    UNUM8* Pattern()
    {
        if (m_Pattern.size() > 0)
        {
            return &m_Pattern[0];
        }
        else
        {
            return nullptr;
        }
    }
    UNUM8* Mask()
    {
        if (m_Pattern.size() > 0)
        {
            return &m_Mask[0];
        }
        else
        {
            return nullptr;
        }
    }
    void Id( UNUM32 Id)
    {
        m_Id = Id;
    }
    UNUM32 Id()
    {
        return m_Id;
    }
private:
    UNUM32      m_Type;
    UNUM32      m_Flags;
    std::vector<UNUM8>   m_Pattern;
    std::vector<UNUM8>   m_Mask;
    UNUM32      m_Id;
};

typedef CFilter*    pFilter_t;

/**
 * VCI HW (Channel)
 */
class VCI
{
public:
    VCI(    const std::string&   Name,
            UNUM32      TypeId,
            UNUM32      CAN );
    virtual ~VCI();

    UNUM32 Id()
    {
        return m_Id;
    }
    std::string& Name()
    {
        return m_Name;
    }
    void VCiIF( CVCiViewIF* VCiIF )
    {
        m_VCiIF = VCiIF;
    }
    CVCiViewIF* VCiIF()
    {
        return m_VCiIF;
    }
    UNUM32 TypeId()
    {
        return m_TypeId;
    }
    UNUM32 CAN()
    {
        return m_CAN;
    }
    std::string& Firmware()
    {
        return m_Firmware;
    }
    UNUM32 Baudrate()
    {
        return m_Baudrate;
    }
    void Baudrate( UNUM32 Baudrate )
    {
        m_Baudrate = Baudrate;
    }
    void AddFilter( CFilter* Filter )
    {
        m_Filters.push_back(Filter);
    }
    void ClearFilters()
    {
        std::vector<pFilter_t>::iterator i=m_Filters.begin();
        while (i!=m_Filters.end())
        {
            delete *i++;
        }
        m_Filters.clear();
    }
    /** Connect
     * \brief Connect the CAN controller to the bus
     * \return T_PDU_ERROR
     * \retval PDU_STATUS_NOERROR       Function call successful.
     * \retval PDU_ERR_FCT_FAILED       Function call failed.
     */
    virtual T_IVIEW_STATUS Connect();
    /** Disconnect
     * \brief Disconnect the CAN controller from the bus
     * \return T_PDU_ERROR
     * \retval PDU_STATUS_NOERROR       Function call successful.
     * \retval PDU_ERR_FCT_FAILED       Function call failed.
     */
    virtual T_IVIEW_STATUS Disconnect();
private:
    static UNUM32       m_NextId;
    static UNUM32       m_ConnRef;
    static LARGE_INTEGER    m_TickBase;
    UNUM32          m_Id;
    CVCiViewIF*     m_VCiIF;
    std::string     m_Name;
    UNUM32          m_TypeId;
    UNUM32          m_CAN;
    std::string     m_Firmware;
    UNUM32          m_ControllerState;
    /*
     * Configuration
     */
    UNUM32          m_Baudrate;
    std::vector<pFilter_t>   m_Filters;
    /*
     * Stats
     */
    UNUM32          m_TxErrorCounter;
    UNUM32          m_RxErrorCounter;
    UNUM32          m_PeakRxErrorCounter;
    UNUM32          m_PeakTxErrorCounter;
};

typedef VCI* pVCI_t;

/**
 * Client and Client Buffer set
 */
class Client
{
    typedef std::set<CBaseCANBufFSE*> Buffers_t;
public:
    Client() :
        m_ClientID(m_NextClientID++) {};
    Client(     const std::string&   Name ) :
        m_ClientID( m_NextClientID++ ),
        m_ClientName( Name ) {};

    BOOL BufferExists(
        CBaseCANBufFSE* pBuf);

    BOOL AddClientBuffer(
        CBaseCANBufFSE* Buffer);

    void WriteClientBuffers(
        STCANDATA& CanData);

    BOOL RemoveClientBuffer(
        CBaseCANBufFSE* Buffer);

    void RemoveClientBuffers();

    static DWORD        m_NextClientID;
    DWORD           m_ClientID;
    Buffers_t       m_ClientBuf;
    std::string          m_ClientName;
};

typedef Client* pClient_t;

/**
 * CDIL_CAN_i_VIEW class definition
 */

class CDIL_CAN_i_VIEW :
    public CBaseDIL_CAN_Controller,
    public CVCiViewBrowserIF,
    public CVCiViewHostIF
{
    typedef std::array<pVCI_t,defNO_OF_CHANNELS> Channels_t;
    typedef std::map<UNUM32, pVCI_t>    pVCIMap_t;
    typedef std::map<DWORD,pClient_t>   pClientMap_t;
public:
    CDIL_CAN_i_VIEW();
    virtual ~CDIL_CAN_i_VIEW();
    HRESULT CAN_PerformInitOperations(void);
    HRESULT CAN_PerformClosureOperations(void);
    HRESULT CAN_GetTimeModeMapping(SYSTEMTIME& CurrSysTime, UINT64& TimeStamp, LARGE_INTEGER& QueryTickCount);
    HRESULT CAN_ListHwInterfaces(INTERFACE_HW_LIST& sSelHwInterface, INT& nCount, PSCONTROLLER_DETAILS InitData);
    HRESULT CAN_SelectHwInterface(const INTERFACE_HW_LIST& sSelHwInterface, INT nCount);
    HRESULT CAN_DeselectHwInterface(void);
    HRESULT CAN_SetConfigData(PSCONTROLLER_DETAILS InitData, int Length);
    HRESULT CAN_StartHardware(void);
    HRESULT CAN_StopHardware(void);
    HRESULT CAN_ResetHardware(void);
    HRESULT CAN_GetCurrStatus(STATUSMSG& StatusData);
    HRESULT CAN_SendMsg(DWORD dwClientID, const STCAN_MSG& sCanTxMsg);
    HRESULT CAN_GetLastErrorString(std::string& acErrorStr);
    HRESULT CAN_GetControllerParams(LONG& lParam, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetControllerParams(int nValue, ECONTR_PARAM eContrparam);
    HRESULT CAN_GetErrorCount(SERROR_CNT& sErrorCnt, UINT nChannel, ECONTR_PARAM eContrParam);
    HRESULT CAN_SetAppParams(HWND hWndOwner);
    HRESULT CAN_ManageMsgBuf(BYTE bAction, DWORD ClientID, CBaseCANBufFSE* pBufObj);
    HRESULT CAN_RegisterClient(BOOL bRegister, DWORD& ClientID, char* pacClientName);
    HRESULT CAN_GetCntrlStatus(const HANDLE& hEvent, UINT& unCntrlStatus);
    HRESULT CAN_LoadDriverLibrary(void);
    HRESULT CAN_UnloadDriverLibrary(void);
    HRESULT CAN_SetHardwareChannel(PSCONTROLLER_DETAILS,DWORD dwDriverId,bool bIsHardwareListed, unsigned int unChannelCount);

private:
    void GetSystemErrorString();
    void iViewResolver( iVIEW_Event_e   State,
                        std::string&    Service,
                        std::string&    Type,
                        std::string&    Hostname,
                        std::string&    Address,
                        UNUM16      Port,
                        std::map< std::string, std::string >& TxtList );
    int ListHardwareInterfaces(
        HWND        hParent,
        DWORD       /*dwDriver*/,
        INTERFACE_HW*   psInterfaces,
        int*        pnSelList,
        int&        nCount,PSCONTROLLER_DETAILS );

    void RxData(
        UNUM32              Id,
        vci_data_record_with_data*  VCIRec);
    void RxEvent(
        UNUM32              Id,
        vci_event_record_s*     VCIEvent );

    pClient_t GetClient(
        std::string      ClientName );

    pClient_t GetClient(
        DWORD       ClientID );

    BOOL AddClient(
        pClient_t   Client );

    BOOL RemoveClient(
        DWORD       ClientId);

    CreateBrowser_t         m_CreateBrowser;
    CreateCCommTCP_t        m_CreateCCommTCP;
    CreateCVCiViewIF_t      m_CreateCVCiViewIF;
    HINSTANCE           m_hDll;
    CRITICAL_SECTION        m_Mutex;
    CVCiViewBrowser*        m_iViewBrowser;
    State_e             m_CurrState;
    std::string         m_Err;
    HWND                m_hOwnerWnd;
    pVCIMap_t           m_VCI;
    UINT                m_nChannels;
    Channels_t          m_Channel;
    pClientMap_t            m_Clients;
    INT             m_SelectedVCI[CHANNEL_ALLOWED];
    /**
     * Timer variables
     */
    SYSTEMTIME          m_CurrSysTime;
    UINT64              m_TimeStamp;
    LARGE_INTEGER           m_QueryTickCount;
    LARGE_INTEGER           m_lnFrequency;
};
