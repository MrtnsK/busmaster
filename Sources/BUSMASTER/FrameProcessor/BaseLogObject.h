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
 * \file      BaseLogObject.h
 * \brief     Definition file for CBaseLogObject class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CBaseLogObject class
 */

#pragma once

#include "DataTypes/Log_Datatypes.h"
#include "IBMNetWorkGetService.h"
#include "../CommonClass/RefTimeKeeper.h"


//#include "../Application/ConfigMsgLogDlg.h"
//#include "../Application/resource.h"
class CBaseLogObject
{
private:

    CRITICAL_SECTION m_CritSection; // For thread synchronization
    int m_nCurrFileCnt;             // Keep the number of log file created

    DWORD m_dTotalBytes;            // To keep the current file size

    bool m_bNewSession;         // For file overwriting in new session

    // Find the name and size of the file which will be used for logging.
    // i.e., file name which contains max file count
    DWORD dwGetFileSize(CString omFileName); // Get size of the file

    // Reset certain data member values
    void vResetValues(void);

    void vUpdateLogTimeForTimeTrigger(UINT64& qwRefSysTime);

    int m_nMeasurement,m_nSize,m_nTime,m_nDefault;

    void vSetNextFileName(eFILENAMESUFFIX eFileNameSuffix);


protected:
    //All log info
    CString m_omVersion;            // Application suite version information

    //File pointer
    FILE* m_pLogFile;

    //Current trigger type
    ELOGTRIGGERSTATE m_CurrTriggerType;

    // To format the header
    virtual void vFormatHeader(CString& omHeader, ETYPE_BUS);

    // To format the footer
    virtual void vFormatFooter(CString& omFooter);

    void vWriteTextToFile(CString& om_LogText, ETYPE_BUS);

    // To copy specific data pertaining to the conrete class.
    virtual void Der_CopySpecificData(const CBaseLogObject* pouLogObjSrc) = 0;

    // Set config. data - additional tasks to be done by the concrete class
    virtual BYTE* Der_SetConfigData(BYTE* pvDataStream) = 0;
    virtual void Der_GetConfigData(xmlNodePtr pNodePtr) const = 0;
    virtual int Der_SetConfigData(xmlNodePtr) = 0;
    // Get config. data - additional tasks to be done by the concrete class
    virtual BYTE* Der_GetConfigData(BYTE* pvDataStream) const = 0;
    // Buffer size calculation - additional tasks to be done by the concrete class
    virtual UINT Der_unGetBufSize(void) const = 0;

    virtual CString getNetworkFormatData(ETYPE_BUS);
public:
    SLOGINFO m_sLogInfo;
    CBaseLogObject(CString omVersion);
    virtual ~CBaseLogObject();

    // Equal operator. THERE SHOULDN'T BE ANY EQUAL OPERATOR OVERLOADING FOR
    // THE DERIVED CLASSES
    CBaseLogObject& operator=(const CBaseLogObject& RefObj);

    /** To do actions before logging starts */
    BOOL bStartLogging(ETYPE_BUS);

    void vCloseLogFile();

    /** To do actions before logging stop */
    bool bStopLogging(void);

    bool bStopOnlyLogging(void);

    /** To log a string */
    bool bLogString(CString& omString);

    /** Enable / disable filter */
    virtual void EnableFilter(bool bEnable) = 0;

    /** Enable / disable logging */
    void EnableLogging(bool bEnable);

    /** Query - if logging is enable */
    bool IsLoggingEnable(void);

    /** Get the log info structure */
    void GetLogInfo(SLOGINFO& sLoginfo) const;

    /**Set Log info structure */
    void SetLogInfo(const SLOGINFO& sLoginfo);

    /* Set configuration data */
    BYTE* SetConfigData(BYTE* pvDataStream, BYTE bytLogVersion);
    INT nSetConfigData(xmlNodePtr pNode);
    /* Get configuration data */
    BYTE* GetConfigData(BYTE* pvDataStream) const;
    bool GetConfigData(xmlNodePtr pxmlNodePtr) const;
    // To get the total buffer size
    UINT unGetBufSize(void) const;

    // To get the ID
    UINT GetID(void);

    // To get the list of associated database files
    virtual void Der_GetDatabaseFiles(CStringArray& omList) = 0;
    // To set the list of database files associated
    virtual void Der_SetDatabaseFiles(const CStringArray& omList) = 0;

    // To get the list of associated database files
    void GetDatabaseFiles(CStringArray& omList);

    // To set the list of database files associated
    void SetDatabaseFiles(const CStringArray& omList);

    // To update the channel baud rate info to logger
    virtual void Der_SetChannelBaudRateDetails(void* controllerDetails,
            int nNumChannels) ;

    // To update the channel baud rate info to logger
    void SetChannelBaudRateDetails(void* controllerDetails,
                                   int nNumChannels);
    // To update the channel baud rate info to logger
    virtual void Der_GetChannelBaudRateDetails(void* controllerDetails,
            int& nNumChannels) =0;
    // To get the channel baud rate
    void GetChannelBaudRateDetails(void* controllerDetails,
                                   int& nNumChannels);
    void vSetMeasurementFileName();
};
