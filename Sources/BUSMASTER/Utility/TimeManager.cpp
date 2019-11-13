/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  TimeManager.cpp
  Description   :  Implementation file for CTimeManager class
  $Log:   X:/Archive/Sources/SigGrphWnd/TimeManager.cpv  $

      Rev 1.2   06 Jun 2011 11:51:38   CANMNTTM


      Rev 1.1   15 Apr 2011 19:43:52   rac2kor
   Inserted RBEI Copyright information text into the file header.

      Rev 1.0   13 Dec 2010 22:00:52   CANMNTTM


      Rev 1.1   03 Sep 2010 20:36:18   rac2kor
   Removed commented codes and objects that are unreferenced.

      Rev 1.0   16 Aug 2010 21:20:46   rac2kor


  Author(s)     :  Raja N
  Date Created  :  23.06.2004
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/

// Standard include header
#include "Utils_stdafx.h"
// For Time calculation related functions
#include "TimeManager.h"
//#include "SigGrphWnd_Defines.h"

// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/MsgBufAll_Datatypes.h"
//#include "DIL_Datatypes.h"
#include "BaseDIL_CAN.h"
#include "Application\HashDefines.h"
// Initialise the static members

// Initialise absolute Time
int CTimeManager::m_nAbsoluteTime = 0;
// Init System time and reference tick
static LARGE_INTEGER s_Temp;
BOOL bDummy = QueryPerformanceCounter(&s_Temp);
const LARGE_INTEGER CTimeManager::m_sSysRefTickCount = s_Temp;
// Init the frequency
const int CTimeManager::m_nSysRefTime = CTimeManager::nCalculateCurrTimeStamp(FALSE);
BOOL bDummy0001 = QueryPerformanceFrequency((LARGE_INTEGER*) &s_Temp);
const __int64 CTimeManager::m_n64Frequency = s_Temp.QuadPart;

// **** Start of USB related Code **** //
int CTimeManager::m_nOffsetTimeValue =
    CTimeManager::nCalculateOffsetTime();

// **** End of USB Related Code **** //

/*******************************************************************************
 Function Name  : CTimeManager
 Description    : Standard default constructor for dialog box
 Member of      : CTimeManager
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 23.06.2004
*******************************************************************************/
CTimeManager::CTimeManager()
{

}

/*******************************************************************************
 Function Name  : CTimeManager
 Description    : Standard Destructor
 Member of      : CTimeManager
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 23.06.2004
*******************************************************************************/
CTimeManager::~CTimeManager()
{

}

/*******************************************************************************
 Function Name  : vFormatTimeStamp
 Input(s)       : nTimeStamp        - Current time stamp
                  bOverWrite        - Mode of Time Calculation
                  wDisplayTimeMode  - Type of Time Calculation
                  nIndex            - Index of the element
                  nOverWriteMapKey  - Unique key to CMap to get previous time
                                      stamp
                  omStrTime         - Object Reference to store the formatted
                                      time value
 Output         : -
 Functionality  : This function will compute the time and will format it and
                  will store it in the omStrTime string.
 Member of      : CTimeManager
 Author(s)      : Raja N
 Date Created   : 23.06.2004
 Modifications  : Raja N on 22.07.2004
                  Modified the code to refer vFormatTimeStamp overloaded
                  function. This will split the functionality logically
 Modifications  : Raja N on 22.07.2004
                  Changed map index variable type from int to __int64
*******************************************************************************/
void CTimeManager::vFormatTimeStamp(int nTimeStamp, BOOL bOverWrite,
                                    WORD wDisplayTimeMode,
                                    int nIndex,
                                    __int64 /*n64OverWriteMapKey*/,
                                    CString& omStrTime)
{
    int nRefTime = 0;

    // Find reference time stamp for relative mode
    if( wDisplayTimeMode == eRELATIVE_MODE)
    {
        if(bOverWrite == FALSE )
        {
            // To get the previous time stamp
            // If index is not zero then get it from the buffer
            if( nIndex != 0 )
            {
                // ???
            }
            // If index is zero then refer the backup copy of time stamp
            else
            {
                // ???
            }
        }
        else
        {
            // ???
        }
    }
    // Format the time stamp
    vFormatTimeStamp(nTimeStamp, nRefTime, wDisplayTimeMode, omStrTime);
}

/*******************************************************************************
 Function Name  : vFormatTimeStamp
 Input(s)       : nTimeStamp        - Current time stamp
                  nRefTimeStamp     - Reference time stamp for relative mode
                  wDisplayTimeMode  - Type of Time Calculation
                  omStrTime         - Object Reference to store the formatted
                                      time value
 Output         : -
 Functionality  : This function will compute the time and will format it and
                  will store it in the omStrTime string.
 Member of      : CTimeManager
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CTimeManager::vFormatTimeStamp( int nTimeStamp,
                                     int nRefTimeStamp,
                                     WORD wDisplayTimeMode,
                                     CString& omStrTime)
{
    // Static variables to reduce the creation time
    static int nTemp, nMicSec, nSec, nMinute, nHour;

    if(wDisplayTimeMode == eABSOLUTE_MODE )
    {
        // Subract the value of absolute time reference.
        nTimeStamp -= m_nAbsoluteTime;
    }
    else if( wDisplayTimeMode == eRELATIVE_MODE )
    {
        nTimeStamp -= nRefTimeStamp;
    }

    // Make sure that there wont be any negative time value
    if( nTimeStamp < 0)
    {
        nTimeStamp = 0;
    }

    nMicSec = nTimeStamp % 10000;// hundreds of microseconds left
    nTemp = nTimeStamp / 10000;  // expressed in seconds
    nSec = nTemp % 60;           // seconds left
    nTemp = nTemp / 60;          // expressed in minutes
    nMinute = nTemp % 60;        // minutes left
    nHour = nTemp / 60;          // expressed in hours

    // Check for system mode and wrap up with in 24 Hours
    if( wDisplayTimeMode == eSYSTEM_MODE )
    {
        nHour %= 24;
    }
    // Format the time value in the output string
    omStrTime.Format(defTIME_STAMP_FORMAT, nHour, nMinute,nSec, nMicSec);
}

/*******************************************************************************
 Function Name  : vSetAbsoluteTime
 Input(s)       : nAbsoluteTime     - Absolute Time Reference
 Output         : -
 Functionality  : This function will init the absolute reference variable with
                  given time value
 Member of      : CTimeManager
 Author(s)      : Raja N
 Date Created   : 23.06.2004
*******************************************************************************/
void CTimeManager::vSetAbsoluteTime(int nAbsoluteTime)
{
    m_nAbsoluteTime = nAbsoluteTime;
}

/*******************************************************************************
 Function Name  : nGetAbsoluteTime
 Input(s)       : -
 Output         : int     - Absolute Time Reference
 Functionality  : This function will return the absolute reference value
 Member of      : CTimeManager
 Author(s)      : Raja N
 Date Created   : 23.06.2004
*******************************************************************************/
int CTimeManager::nGetAbsoluteTime()
{
    return m_nAbsoluteTime;
}

/*******************************************************************************
 Function Name  : nCalculateCurrTimeStamp
 Input(s)       : -
 Output         : int     - Current system time in 10's of Milli Seconds
 Functionality  : This function will return the current system time in 10's of
                  milliseconds. This will be called to init reference variables
 Member of      : CTimeManager
 Author(s)      : Raja N
 Date Created   : 23.06.2004
*******************************************************************************/
int CTimeManager::nCalculateCurrTimeStamp(BOOL bFromDIL)
{
    SYSTEMTIME CurrSysTime;
    //UINT64 TimeStamp;

    int nResult = 0;
    if (bFromDIL == FALSE)
    {
        GetLocalTime(&CurrSysTime);
        nResult = (CurrSysTime.wHour * 3600 + CurrSysTime.wMinute * 60
                   + CurrSysTime.wSecond) * 10000 + CurrSysTime.wMilliseconds * 10;
    }
    else
    {
        //g_pouDIL_CAN_Interface->DILC_GetTimeModeMapping(CurrSysTime, TimeStamp);
    }
    return nResult;
}


/******************************************************************************
 Function Name  : nGetAbsoluteTime
 Input(s)       : -
 Output         : int     - Absolute Time Reference
 Functionality  : This function will return the absolute reference value
 Member of      : CTimeManager
 Author(s)      : Raja N
 Date Created   : 23.06.2004
******************************************************************************/
void CTimeManager::vInitAbsoluteTime()
{
    m_nAbsoluteTime = nCalculateCurrTimeStamp();

}

/******************************************************************************
 Function Name  : nGetAbsoluteTime
 Input(s)       : n64CurrTick   - Time Tick Value
 Output         : int           - System Time in 10s of millisecond
 Functionality  : This function will calculate the saystem time value of the
                  clock tick. This will refer the reference time time and clock
                  tick frequency to compute the system time
 Member of      : CTimeManager
 Author(s)      : Raja N
 Date Created   : 23.06.2004
 Modifications  : Raja N on 31.08.2004
                  Added code for USB Implementation
******************************************************************************/
int CTimeManager::nCalculateElapsedTimeFromTick(const __int64& /*n64CurrTick*/)
{
    return 0;
}
void CTimeManager::bReinitOffsetTimeValForES581(void)
{
    CTimeManager::m_nOffsetTimeValue = nCalculateCurrTimeStamp(TRUE);
}

/******************************************************************************
 Function Name  : nCalculateOffsetTime
 Input(s)       : -
 Output         : int           - Offset Time in 10s of millisecond
 Functionality  : This function will calculate the time offset between windows
                  start-up time and system time. This will be added to time
                  stamping in the CAN message from driver.
 Member of      : CTimeManager
 Author(s)      : Raja N
 Date Created   : 31.08.2004
 Modifiecations : Raja N on 13.09.2004
                  Included overflow member value while calculating the offset
                  value
******************************************************************************/
int CTimeManager::nCalculateOffsetTime()
{
    return 0;
}

// **** End of USB Related Code **** //