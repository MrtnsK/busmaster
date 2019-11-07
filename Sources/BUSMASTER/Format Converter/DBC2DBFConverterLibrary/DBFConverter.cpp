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
 * \file      Converter.cpp
 * \brief     Implementation of converter class
 * \author    Amitesh Bharti, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the converter class.
 */
#include "DBC2DBFConverterLibrary_stdafx.h"
#include "DBFConverter.h"
#include "DBFSignal.h"
#include "DBFTagDefinitions.h"
#include "Utility.h"
using namespace std;

bool CDBFConverter::valid_msg = true;
unsigned char CDBFConverter::ucMsg_DLC = 8;
extern "C" int nBus ;
extern "C" int nProtocal ;
extern "C" int FindProtoCol(char*);
extern FILE* yyin;
/**
 * \brief Constructor
 */
CDBFConverter::CDBFConverter(ETYPE_BUS eBus)
{
    m_eBus = eBus;
    m_uiResultCode = CON_RC_NOERROR;
    bHeaderSection = FALSE;
}


/**
 * \brief      Destructor
 */
CDBFConverter::~CDBFConverter()
{
    strDBCFileName = "";
    m_listMessages.clear();
}
HRESULT CDBFConverter::GetMessageNameList(CStringArray& meassageList)
{
    list<CMessage>::iterator rMsg;
    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
    {
        meassageList.Add((*rMsg).m_acName.c_str());
    }
    return S_OK;
}
HRESULT CDBFConverter::ClearMsgList()
{
    m_listMessages.clear();
    m_unsupList.clear();
    m_listSignal.clear();
    m_vTab.clear();
    m_notProcessed.clear();
    m_listNode.clear();
    defList.clear();
    m_cmMsg.clear();
    m_cmNet.clear();
    m_cmNode.clear();
    m_cmSig.clear();
    m_vTab.clear();
    m_listParameters.clear();
    for(int i=0; i< 6; i++)
    {
        m_listParameterArray[i].clear();
        m_listParamValues[i].clear();
    }
    return S_OK;
}

HRESULT CDBFConverter::LoadDBCFile(CString strDBCFile)
{
    strDBCFileName = strDBCFile;

    fstream fileInput;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    fileInput.open(strDBCFileName.c_str(), fstream::in);
    if(!fileInput.is_open())
    {
        return SetResultCode(CON_RC_FILEOPEN_ERROR_INFILE);
    }

    // first line of input file starts with keyword "VERSION", else file format error
    if (!fileInput.getline(acLine, defCON_MAX_LINE_LEN))
    {
        // eof file reached without reading anything
        fileInput.close();
        return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
    }

    // Generate the list of messages

    GenerateMessageList(fileInput);


    // All information gathered, validate and update if necessary
    // Make appropriate changes in the contents of the list
    if(m_listMessages.size() > 0)
    {
        ValidateMessageList();
    }
    if(!fileInput.is_open())
    {
        fileInput.close();
        return S_OK;
    }

    return S_OK;
}
HRESULT CDBFConverter::GenerateImportList(/* sMESSAGE*& */)
{
    return S_OK;
}
HRESULT CDBFConverter::ConvertFile(CString strDBFFile)
{
    /* Reset to default value*/
    SetResultCode(CON_RC_NOERROR);

    fstream fileOutput;
    fileOutput.open(strDBFFile, fstream::out);
    if(!fileOutput.is_open())
    {
        // if output file cannot be opened the close the input file
        // and return the error code
        return SetResultCode(CON_RC_FILEOPEN_ERROR_OUTFILE);
    }

    EncryptData(m_notProcessed);
    bool bRes = WriteToOutputFile(fileOutput);

    fileOutput.close();

    if(!bRes)
    {
        //SSH + Issue# 403 - Log file is not created for DBCtoDBF conversion
        strDBCFileName = strDBFFile;
        //SSH -
        string sLogFile = strDBCFileName.substr(0, strDBCFileName.length()-4);
        sLogFile += ".log";
        m_omLogFilePath = sLogFile;
        fstream fileLog;
        fileLog.open(sLogFile.c_str(), fstream::out);
        if(!fileLog.is_open())
        {
            // if log file cannot be opened return the error code
            return SetResultCode(CON_RC_FILEOPEN_ERROR_LOGFILE);
        }
        else
        {
            CreateLogFile(fileLog);
            fileLog.close();
            return SetResultCode(CON_RC_COMPLETED_WITH_ERROR);
        }
    }

    return m_uiResultCode;
}
HRESULT CDBFConverter::FindMessage(CString omStrMsgName, CMessage* omTemp)
{
    if ( nullptr == omTemp )
    {
        return S_FALSE;
    }
    list<CMessage>::iterator rMsg;

    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
    {
        // compare with message name
        if( omStrMsgName == rMsg->m_acName.c_str())
        {
            *omTemp = *rMsg;
            // send the result
            return S_OK;
        }
    }
    // if not found then
    omTemp->m_uiMsgID = 0xffffffff;
    return FALSE;
}

HRESULT CDBFConverter::FindSignalAlias(CString& strMsgName, CString& strSignalName, CString& strSignalAlias)
{
    list<CMessage>::iterator rMsg;
    bool bFound = false;
    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
    {
        // compare with message name
        if( strMsgName == (*rMsg).m_acName.c_str())
        {
            if( true == bGetSignalAlias(*rMsg, strSignalName, strSignalAlias) )
            {
                bFound = true;
            }
            else
            {
                bFound = false;
            }
        }
    }
    if(bFound == false)
    {
        strSignalAlias = strSignalName;
        return S_FALSE;
    }
    else
    {
        return S_OK;
    }
}
bool CDBFConverter::bGetSignalAlias(CMessage& ouMsg, CString& strSignalName, CString& strSignalAlias)
{
    bool bFound = false;
    int nStartBit, nWhichByte;
    list<CSignal>::iterator rSignal;
    for(rSignal = ouMsg.m_listSignals.begin(); rSignal != ouMsg.m_listSignals.end(); ++rSignal)
    {
        if ( rSignal->m_acName.c_str() == strSignalName )
        {
            //if( rSignal->m_acMultiplex.length() > 0 )
            {
                nStartBit = rSignal->m_ucStartBit;
                nWhichByte = rSignal->m_ucWhichByte;
                bFound = true;
                break;
            }
        }
    }
    if ( bFound == true )
    {
        bFound = false;
        for(rSignal = ouMsg.m_listSignals.begin(); rSignal != ouMsg.m_listSignals.end(); rSignal++)
        {
            //--rSignal;
            if ( rSignal->m_ucStartBit == nStartBit && rSignal->m_ucWhichByte == nWhichByte )
            {
                strSignalAlias = rSignal->m_acName.c_str();
                bFound = true;
                break;
            }
        }
    }
    if( bFound == false )
    {
        strSignalAlias = strSignalName;
    }
    return bFound;
}
HRESULT CDBFConverter::FindMessage(UINT unMsgId, CMessage* omTemp)
{
    if ( nullptr == omTemp )
    {
        return S_FALSE;
    }
    list<CMessage>::iterator rMsg;

    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
    {
        // store the data
        *omTemp = *rMsg;
        // compare with message name
        if( unMsgId == omTemp->m_uiMsgID)
        {
            // send the result
            return S_OK;
        }

    }
    // if not found
    omTemp->m_uiMsgID = 0xffffffff;
    return S_FALSE;
}


/**
 * This is the basic function which is to be called
 * to convert any given CANoe file to a CANMon file.
 */
unsigned int CDBFConverter::Convert(string sCanoeFile, string sCanMonFile)
{
    /*
    fstream fileInput;
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    fileInput.open(sCanoeFile.c_str(), fstream::in);
    if(!fileInput.is_open())
    {
        return SetResultCode(CON_RC_FILEOPEN_ERROR_INFILE);
    }

    // first line of input file starts with keyword "VERSION", else file format error
    if(fileInput.getline(acLine, defCON_MAX_LINE_LEN) == NULL)
    {
        // eof file reached without reading anything
        fileInput.close();
        return SetResultCode(CON_RC_FORMAT_ERROR_INFILE);
    }

    // Generate the list of messages

    GenerateMessageList(fileInput);


    // All information gathered, validate and update if necessary
    // Make appropriate changes in the contents of the list

    ValidateMessageList();
    */

    // the format is OK then open the output file
    /*fstream fileOutput;
    fileOutput.open(sCanMonFile.c_str(), fstream::out);
    if(!fileOutput.is_open())
    {
        // if output file cannot be opened the close the input file
        // and return the error code
        //fileInput.close();
        return SetResultCode(CON_RC_FILEOPEN_ERROR_OUTFILE);
    }

    EncryptData(m_notProcessed);
    bool bRes = WriteToOutputFile(fileOutput);


    fileOutput.close();

    if(!bRes)
    {
        string sLogFile = sCanoeFile.substr(0, sCanoeFile.length()-4);
        sLogFile += ".log";
        m_omLogFilePath = sLogFile;
        fstream fileLog;
        fileLog.open(sLogFile.c_str(), fstream::out);
        if(!fileLog.is_open())
        {
            // if log file cannot be opened return the error code
            return SetResultCode(CON_RC_FILEOPEN_ERROR_LOGFILE);
        }
        else
        {
            CreateLogFile(fileLog);
            fileLog.close();
            return SetResultCode(CON_RC_COMPLETED_WITH_ERROR);
        }
    }*/

    return m_uiResultCode;
}


const char* CDBFConverter::m_pacResultStrings[] =
{
    "Conversion completed Successfully",
    "Conversion completed with warnings.",
    "Conversion aborted. Error opening input file.",
    "Conversion aborted. Error creating output file.",
    "Conversion aborted. Error with input file format.",
    "Conversion aborted. Error creating log file."
};


/**
 * \brief Returns the error string.
 */
HRESULT CDBFConverter::GetResultString(char* pchResult)
{
    if( NULL != pchResult )
    {
        strcpy(pchResult, _((char*)m_pacResultStrings[m_uiResultCode]));
        return S_OK;
    }
    return S_FALSE;
}


/**
 * \brief Set result code.
 */
unsigned int CDBFConverter::SetResultCode(unsigned int uiCode)
{
    return (m_uiResultCode = uiCode);
}


/**
 * \brief Validates the message list
 *
 * Validates the message list and set the error in each signal if present
 */
void CDBFConverter::ValidateMessageList()
{
    list<CMessage>::iterator rMsg;
    unsigned int uiResult;
    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
    {
        unsigned char ucDataFormat = 0;

        // set the id and frame format
        // canoe puts MSbit = 1 for extended ID
        if(rMsg->m_uiMsgID < 0x80000000UL)
        {
            rMsg->m_cFrameFormat = CMessage::MSG_FF_STANDARD;
        }
        else
        {
            rMsg->m_cFrameFormat = CMessage::MSG_FF_EXTENDED;
            rMsg->m_uiMsgID &= 0x7FFFFFFF;
        }

        rMsg->m_ucNumOfSignals = 0; // reset number of signals to 0
        // this should be updated to number of
        // valid signals as we parse the Signal list

        //pems - Start
        //Scan the message list and make the message format same as the
        //one that has maximum number of signals.
        int iCntMotorolaSignals = 0;
        int iCntIntelSignals = 0;
        list<CSignal>::iterator rSig;
        for(rSig=rMsg->m_listSignals.begin(); rSig!=rMsg->m_listSignals.end(); ++rSig)
        {
            switch (rSig->m_ucDataFormat)
            {
                case CSignal::SIG_DF_INTEL:
                    iCntIntelSignals++;
                    break;

                case CSignal::SIG_DF_MOTOROLA:
                    iCntMotorolaSignals++;
                    break;
            }
        }
        // Update the message data format
        if(iCntIntelSignals >= iCntMotorolaSignals)
        {
            ucDataFormat = CSignal::SIG_DF_INTEL;
            rMsg->m_cDataFormat = ucDataFormat; // set message data format to this i.e format of first valid signal
        }
        else
        {
            ucDataFormat = CSignal::SIG_DF_MOTOROLA;
            rMsg->m_cDataFormat = ucDataFormat;
        }
        //pems - end

        for(rSig=rMsg->m_listSignals.begin(); rSig!=rMsg->m_listSignals.end(); ++rSig)
        {
            uiResult = rSig->Validate(ucDataFormat);

            // if the signal is valid
            if((uiResult == CSignal::SIG_EC_NO_ERR) || (uiResult == CSignal::SIG_EC_OVERFLOW))
            {
                /*Pems Start
                if(ucDataFormat == 0)
                {
                ucDataFormat = rSig.m_ucDataFormat; // now this is updated to the first valid signal's format
                rMsg.m_cDataFormat = ucDataFormat; // set message data format to this i.e format of first valid signal
                }
                Pems End*/
                rMsg->m_ucNumOfSignals++; // increment the signal count for this message
            }
            else
            {
                int flag = 0;
                list<CMessage>::iterator usMsg;
                for(usMsg=m_unsupList.begin(); usMsg!=m_unsupList.end(); ++usMsg)
                {
                    // find matching message from list
                    if((usMsg->m_uiMsgID == rMsg->m_uiMsgID) && (usMsg->m_cFrameFormat == rMsg->m_cFrameFormat))
                    {
                        usMsg->m_listSignals.push_back(*rSig);
                        flag = 1;
                    }

                }
                if(flag == 0)
                {
                    m_unsupList.push_back(*rMsg);
                    CMessage& msg = m_unsupList.back();
                    msg.m_listSignals.clear();
                    msg.m_listSignals.push_back(*rSig);
                }
            }
        }
    }
    //vaildate signals not associated with any messages.
    list<CSignal>::iterator rSig;
    for(rSig=m_listSignal.begin(); rSig!=m_listSignal.end(); ++rSig)
    {
        uiResult = rSig->Validate(rSig->m_ucDataFormat);
        // if the signal is valid
        if((uiResult == CSignal::SIG_EC_NO_ERR) || (uiResult == CSignal::SIG_EC_OVERFLOW))
        {
            /*Pems Start
            if(ucDataFormat == 0)
            {
            ucDataFormat = rSig.m_ucDataFormat; // now this is updated to the first valid signal's format
            rMsg.m_cDataFormat = ucDataFormat; // set message data format to this i.e format of first valid signal
            }
            Pems End*/
        }
        else
        {
            int flag = 0;
            list<CMessage>::iterator usMsg;
            for(usMsg=m_unsupList.begin(); usMsg!=m_unsupList.end(); ++usMsg)
            {
                if((usMsg->m_uiMsgID == 1073741824) && (usMsg->m_cFrameFormat == 'X'))
                {
                    usMsg->m_listSignals.push_back(*rSig);
                    flag = 1;
                }

            }
            if(flag == 0)
            {
                CMessage def_msg;
                def_msg.m_acName = "VECTOR__INDEPENDENT_SIG_MSG";
                def_msg.m_uiMsgID = 1073741824 ;
                def_msg.m_cFrameFormat = 'X';
                m_unsupList.push_back(def_msg);
                def_msg.m_listSignals.push_back(*rSig);
            }
        }
    }
    list<CParameter>::iterator rParam;
    for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
    {
        if(rParam->m_ObjectId == "BU_")
        {
            m_listParameterArray[1].push_back(*rParam);
        }
        else if(rParam->m_ObjectId == "BO_")
        {
            m_listParameterArray[2].push_back(*rParam);
        }
        else if(rParam->m_ObjectId == "SG_")
        {
            m_listParameterArray[3].push_back(*rParam);
        }
        else if(rParam->m_ObjectId == "")
        {
            m_listParameterArray[0].push_back(*rParam);
        }
        else if(rParam->m_ObjectId == "BU_SG_REL_")
        {
            m_listParameterArray[4].push_back(*rParam);
        }
        else if(rParam->m_ObjectId == "BU_BO_REL_")
        {
            m_listParameterArray[5].push_back(*rParam);
        }
    }
}

/**
 * \brief Generates the message list
 *
 * This function will parse the input file and line by line
 * and generates  a list of message,signal,value table,comments,etc
 */
void CDBFConverter::GenerateMessageList(fstream& fileInput)
{
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    string local_copy;
    char* pcTok;
    int flag=0;
    int _flag_BS_= 0; //possible values 0,1,2// this flag is used to check if parsing is done in between NS_: and BS_:  #git issue 174

    // parsing the input file
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN))
    {
        char* pcToken=0, *pcLine=0;

        for (;;)
        {
            pcLine = acLine;
            pcLine += strlen(pcLine); // go to last position
            pcLine --;

            if (*pcLine == '\r')
            {
                fileInput.getline(pcLine, defCON_MAX_LINE_LEN);
            }
            else
            {
                break;
            }
        }

        // avoid leading <spaces> before tokenising, so passing the
        // starting point will be correct in each case, when calling
        // msg.Format, sig.Format etc.
        local_copy = acLine;
        pcLine = acLine;

        while(*pcLine && *pcLine == ' ')
        {
            *pcLine++;
        }

        pcToken = strtok_s(pcLine, " :", &pcTok);

        if(pcToken)
        {
            //compare token to known types to interpret the line further
            if(strstr(pcToken, "BS_") != NULL)  // git issue #174
            {
                if (_flag_BS_ == 1 )
                {
                    _flag_BS_ = 2;    // this means just NS_ and BS_ are done.
                }
            }
            if(strstr(pcToken, "NS_") != NULL)
            {
                if (_flag_BS_ == 0 )
                {
                    _flag_BS_ = 1;    // this means just NS_ is hit and BS_ is still due.
                }
            }
            // new line - skip
            else if(strcmp(pcToken, "\n") == 0)
            {
                continue;
            }
            // message
            else if(strcmp(pcToken, "BO_") == 0)
            {
                CMessage msg;
                msg.Format(pcLine + strlen(pcToken)+1);

                // add the new message to the list
                if((msg.m_acName != "VECTOR__INDEPENDENT_SIG_MSG") && !(msg.m_uiMsgID == 3221225472) )
                {
                    if( CAN == m_eBus )
                    {
                        if( msg.m_ucLength <= 8 )
                        {
                            CDBFConverter::valid_msg = true;
                            m_listMessages.push_back(msg);
                        }
                        else
                        {
                            CDBFConverter::valid_msg = false;
                            m_unsupList.push_back(msg);
                        }
                    }
                    else
                    {
                        CDBFConverter::valid_msg = true;
                        m_listMessages.push_back(msg);
                    }
                }
                else
                {
                    CDBFConverter::valid_msg = false;
                }
            }
            // signal
            else if(strcmp(pcToken, "SG_") == 0)
            {
                CSignal sig;
                sig.Format(pcLine + strlen(pcToken) + 1);

                //if signal associated with a msg add it to that perticular list
                //elses add it to msg independent list
                if(CDBFConverter::valid_msg == true)
                {
                    //insert signals in sorted order
                    int flag = 0;
                    CMessage& msg = m_listMessages.back();

                    /* if (msg.m_listSignals.empty())
                     {
                         msg.m_listSignals.push_front(sig);
                         flag = 1;
                         continue;
                     }

                     int count = 0;
                     list<CSignal>::iterator sig1 = msg.m_listSignals.end();

                     while(sig1!=msg.m_listSignals.begin())
                     {
                         --sig1;
                         count++;

                         if(((sig1->m_ucWhichByte * 8) + sig1->m_ucStartBit) > ((sig.m_ucWhichByte * 8) + sig.m_ucStartBit))
                         {
                             ++sig1;
                             msg.m_listSignals.insert(sig1, sig);
                             flag = 1;
                             break;
                         }
                     }

                     if (flag == 0)*/
                    {
                        msg.m_listSignals.push_back(sig);
                    }

                    // this signal should belong to the last message
                    msg.m_ucNumOfSignals++; // increment the signal count
                }
                else
                {
                    sig.m_ucWhichByte = 1;
                    sig.m_ucStartBit = 0;
                    m_listSignal.push_back(sig);
                }
            }
            // value descriptor
            else if(strcmp(pcToken, "VAL_") == 0)
            {
                // <msgid><sp><signalName><sp><value1><sp><"desc1"><sp><value2><sp><"desc2"> ...;
                // get MsgId, find the message from the messagelist.
                // find the signal from the message, then add the value descritors
                // to the respective signals
                pcLine = pcLine + strlen(pcToken) + 1; // to get next token
                pcToken = strtok_s(pcLine, " ", &pcTok); // msgid
                unsigned int id = (unsigned int) strtoul(pcToken, NULL, 10);

                if(id != 3221225472)
                {
                    list<CMessage>::iterator rMsg;

                    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
                    {
                        // find matching message from list
                        if(rMsg->m_uiMsgID == id)
                        {
                            pcLine = pcLine + strlen(pcToken) + 1; // to get next token
                            pcToken = strtok_s(pcLine, " ", &pcTok); // Signal name
                            list<CSignal>::iterator rSig;

                            // find matching signal
                            for(rSig=rMsg->m_listSignals.begin(); rSig!=rMsg->m_listSignals.end(); ++rSig)
                            {
                                if(rSig->m_acName == pcToken)
                                {
                                    rSig->AddValueDescriptors(pcLine + strlen(pcToken) + 1, fileInput);
                                    break; // if we got the signal we wanted
                                }
                            }

                            break; // we got the message we wanted
                        }
                    }
                }
                else
                {
                    pcLine = pcLine + strlen(pcToken) + 1; // to get next token
                    pcToken = strtok_s(pcLine, " ", &pcTok); // Signal name
                    list<CSignal>::iterator rSig;

                    // find matching signal
                    for(rSig=m_listSignal.begin(); rSig!=m_listSignal.end(); ++rSig)
                    {
                        if(rSig->m_acName == pcToken)
                        {
                            rSig->AddValueDescriptors(pcLine + strlen(pcToken) + 1, fileInput);
                            break; // if we got the signal we wanted
                        }
                    }
                }
            }
            // signal value qualifier
            else if(strcmp(pcToken, "SIG_VALTYPE_") == 0)
            {
                // <msgID> <signal name> : 1 -- float
                // <msgID> <signal name> : 2 -- double
                // get MsgId, find the message from the messagelist.
                // find the signal from the message, then update the
                // signal type appropriately of the respective signal
                pcToken = strtok_s(NULL, " :;", &pcTok); // msgid
                unsigned int id = (unsigned int)strtoul(pcToken, NULL, 10);

                if(id != 3221225472)
                {
                    list<CMessage>::iterator rMsg;

                    for(rMsg=m_listMessages.begin(); rMsg!=m_listMessages.end(); ++rMsg)
                    {
                        // find matching message from list
                        if(rMsg->m_uiMsgID == id)
                        {
                            pcToken = strtok_s(NULL, " :;", &pcTok); // Signal name
                            list<CSignal>::iterator rSig;

                            // find matching signal
                            for(rSig=rMsg->m_listSignals.begin(); rSig!=rMsg->m_listSignals.end(); ++rSig)
                            {
                                if(rSig->m_acName == pcToken)
                                {
                                    if(pcToken = strtok_s(NULL, " :;", &pcTok)) // qualifier (1 or 2)
                                    {
                                        // update signal type based on qualifier
                                        switch(*pcToken)
                                        {
                                            case '1':
                                                rSig->m_ucType = CSignal::SIG_TYPE_FLOAT;
                                                break;

                                            case '2':
                                                rSig->m_ucType = CSignal::SIG_TYPE_DOUBLE;
                                                break;

                                            default:
                                                break;
                                        }
                                    }

                                    break; // we got the signal we wanted
                                }
                            }

                            break; // we got the message we wanted
                        }
                    }
                }
                else
                {
                    pcToken = strtok_s(NULL, " :;", &pcTok); // Signal name
                    // find matching signal
                    list<CSignal>::iterator rSig;

                    for(rSig=m_listSignal.begin(); rSig!=m_listSignal.end(); ++rSig)
                    {
                        if(rSig->m_acName == pcToken)
                        {
                            if(pcToken = strtok_s(NULL, " :;", &pcTok)) // qualifier (1 or 2)
                            {
                                // update signal type based on qualifier
                                switch(*pcToken)
                                {
                                    case '1':
                                        rSig->m_ucType = CSignal::SIG_TYPE_FLOAT;
                                        break;

                                    case '2':
                                        rSig->m_ucType = CSignal::SIG_TYPE_DOUBLE;
                                        break;

                                    default:
                                        break;
                                }
                            }

                            break; // we got the signal we wanted
                        }
                    }
                }
            }
            //value table
            else if(strcmp(pcToken, "VAL_TABLE_") == 0)
            {
                CValueTable tab;
                pcToken = strtok_s(pcLine, " ", &pcTok);
                tab.Format(pcLine + strlen(pcToken) + 1, fileInput);
                m_vTab.push_back(tab);
            }
            //comments
            else if(strcmp(pcToken, "CM_") == 0)
            {
                string comment = pcTok; // for network node - venkat
                pcLine = pcLine + strlen(pcToken) + 1;
                pcToken = strtok_s(pcLine, " ", &pcTok);
                CComment cm;

                //comments related to node
                if(strcmp(pcToken, "BU_") == 0)
                {
                    pcToken = strtok_s(NULL, " ", &pcTok);
                    cm.m_elementName= pcToken;
                    pcToken = strtok_s(NULL, "", &pcTok);
                    comment = pcToken;

                    while(strstr(pcToken, "\";") == NULL)
                    {
                        fileInput.getline(acLine, defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }

                    cm.m_comment= comment;
                    m_cmNode.push_back(cm);
                }
                //comments related to messages
                else if(strcmp(pcToken,"BO_") == 0)
                {
                    pcToken = strtok_s(NULL, " ", &pcTok);
                    cm.m_msgID = strtoul(pcToken, NULL, 10);

                    // set the id and frame format
                    // canoe puts MSbit = 1 for extended ID
                    if(cm.m_msgID < 0x80000000UL)
                    {
                        cm.m_msgType= 'S';
                    }
                    else
                    {
                        cm.m_msgType= 'X';
                        cm.m_msgID  &= 0x7FFFFFFF;
                    }

                    pcToken = strtok_s(NULL, "", &pcTok);
                    comment = pcToken;

                    while(strstr(pcToken, "\";") == NULL)
                    {
                        fileInput.getline(acLine, defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }

                    cm.m_comment= comment;
                    m_cmMsg.push_back(cm);
                }
                //comments related to signals
                else if(strcmp(pcToken, "SG_") == 0)
                {
                    pcToken = strtok_s(NULL, " ", &pcTok);
                    cm.m_msgID = strtoul(pcToken, NULL, 10);

                    if(cm.m_msgID < 0x80000000UL)
                    {
                        cm.m_msgType = 'S';
                    }
                    else
                    {
                        cm.m_msgType = 'X';
                        cm.m_msgID &= 0x7FFFFFFF;
                    }

                    pcToken = strtok_s(NULL, " ", &pcTok);
                    cm.m_elementName = pcToken;
                    pcToken = strtok_s(NULL, "", &pcTok);
                    comment = pcToken;

                    while(strstr(pcToken, "\";") == NULL)
                    {
                        fileInput.getline(acLine, defCON_MAX_LINE_LEN);
                        pcToken = acLine;
                        comment = comment + pcToken;
                    }

                    cm.m_comment= comment;
                    m_cmSig.push_back(cm);
                }
                //comments related to network
                else
                {
                    //comment = pcToken;
                    int nRetVal = comment.find(";");
                    if( nRetVal < 0)
                    {
                        while(strstr(pcToken, "\";") == NULL)
                        {
                            fileInput.getline(acLine, defCON_MAX_LINE_LEN);
                            pcToken = acLine;
                            comment = comment + pcToken;
                        }
                    }

                    cm.m_comment= comment;
                    m_cmNet.push_back(cm);
                }
            }
            // node
            else if(strcmp(pcToken, "BU_") == 0)
            {
                create_Node_List(pcLine + strlen(pcToken)+1);
            }
            else if ( ( (strcmp(pcToken, "BA_DEF_")==0) || (strcmp(pcToken, "BA_DEF_REL_")==0)) && _flag_BS_ > 1)
            {
                CParameter pObj;
                pObj.Format(pcLine + strlen(pcToken) + 1); // to get next token
                m_listParameters.push_back(pObj);
            }
            //Param Initial Values
            else if(strcmp(pcToken, "BA_DEF_DEF_")==0 && _flag_BS_ > 1 )
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;
                pcToken = strtok_s(NULL, "\"", &pcTok);
                pcToken = strtok_s(NULL, "\"", &pcTok);
                flag=0;

                while(*pcToken && *pcToken != '"')
                {
                    *pcTemp++ = *pcToken++;
                }

                *pcTemp = '\0';
                list<CParameter>::iterator rParam;

                for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
                {
                    // find matching Parameter from list
                    if(rParam->m_ParamName == acTemp)
                    {
                        pcTemp=acTemp;
                        pcToken = strtok_s(NULL, ";", &pcTok); // default val
                        rParam->ReadDefaultVal(pcToken);
                        flag=1;
                        break;
                    }
                }

                if(flag==0)
                {
                    string errString = "BA_DEF_DEF_ \"";
                    errString += acTemp;
                    errString += "\" ";
                    errString += pcToken;
                    errString += _(" : Match not Found in Param List\n");
                    defList.push_back(errString);
                }
            }
            //RX,Tx Parameter Definition
            else if(strcmp(pcToken,"BA_DEF_DEF_REL_")==0 && _flag_BS_ > 1)
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;
                flag = 0;
                pcToken = strtok_s(NULL, "\"", &pcTok);

                while(*pcToken && (*pcToken != '"'))
                {
                    *pcTemp++ = *pcToken++; // copy SIG_NAME only, i.e. till first 'space'
                }

                *pcTemp = '\0';
                list<CParameter>::iterator rParam;

                for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
                {
                    // find matching Parameter from list
                    if(rParam->m_ParamName == acTemp)
                    {
                        pcTemp = acTemp;
                        pcToken = strtok_s(NULL, ";", &pcTok); // default val
                        rParam->ReadDefaultVal(pcToken);
                        flag=1;
                        break;
                    }
                }

                if(flag==0)
                {
                    string errString = "BA_DEF_DEF_REL \"";
                    errString += acTemp;
                    errString += "\" ";
                    errString += pcToken;
                    errString += _(" : Match not Found in Param List\n");
                    defList.push_back(errString);
                }
            }
            // Parameter Other values //
            else if(strcmp(pcToken, "BA_")==0)
            {
                char acTemp[defCON_TEMP_LEN],*pcTemp;
                pcTemp = acTemp;

                while(*pcLine && (*pcLine == ' '))
                {
                    *pcLine++;
                }

                //get Param name
                pcLine = pcLine + strlen(pcToken) + 1;
                pcToken = strtok_s(pcLine, "\"", &pcTok);

                while(*pcToken && (*pcToken != '"'))
                {
                    *pcTemp++ = *pcToken++;
                }

                *pcTemp = '\0';
                list<CParameter>::iterator rParam;

                for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
                {
                    // find matching Parameter from list
                    if(rParam->m_ParamName == acTemp)
                    {
                        rParam->FormatParamValue(pcLine + strlen(acTemp) + 3); // to get next token
                        pcTemp=acTemp;
                        if(rParam->m_ParamName == SIGNAL_LONG_NAME)
                        {
                            CParameterValues uParamVal = rParam->m_listParamValues[3].back();
                            vUpdateSignalNameFromParam(uParamVal);
                            int i = 0;
                        }
                        else if(rParam->m_ParamName == MESSAGE_LONG_NAME)
                        {
                            CParameterValues uParamVal = rParam->m_listParamValues[2].back();
                            vUpdateMessageNameFromParam(uParamVal);
                            int i = 0;
                        }

                        break;
                    }
                }
            }
            //maintain a list of lines not processed
            else
            {
                string str = local_copy;
                m_notProcessed.push_back(str);
                continue;
            }
        }
    }
}

/**
 * Updates the Short Message Name with Long Name (more than 32 characters)
 */
void CDBFConverter::vUpdateMessageNameFromParam(CParameterValues& uParamVal)
{
    list<CMessage>::iterator msg;

    for(msg=m_listMessages.begin(); msg!=m_listMessages.end(); ++msg)
    {
        if( uParamVal.m_MsgId == msg->m_uiMsgID )
        {
            if( uParamVal.m_ParamVal.cValue.length() > 0 )
            {
                msg->m_acName = uParamVal.m_ParamVal.cValue;
                strTrimLeft(msg->m_acName, "\" ");
                strTrimRight(msg->m_acName, "\" ");
            }
        }
    }
}
/**
 * Updates the Short Signal Name with Long Name (more than 32 characters)
 */
void CDBFConverter::vUpdateSignalNameFromParam(CParameterValues& uParamVal)
{
    list<CMessage>::iterator msg;

    for(msg=m_listMessages.begin(); msg!=m_listMessages.end(); ++msg)
    {
        if( uParamVal.m_MsgId == msg->m_uiMsgID )
        {
            list<CSignal>::iterator signal;
            for(signal = msg->m_listSignals.begin(); signal!= msg->m_listSignals.end(); ++signal)
            {
                if( signal->m_acName == uParamVal.m_SignalName )
                {
                    if( uParamVal.m_ParamVal.cValue.length() > 0 )
                    {
                        signal->m_acName = uParamVal.m_ParamVal.cValue;
                        strTrimLeft(signal->m_acName, "\" ");
                        strTrimRight(signal->m_acName, "\" ");
                    }
                }
            }
        }
    }
}


/**
 * \brief Writes all the data to the output file in CANoe format
 *
 * Writes all the data to the output file in CANoe format.
 */
bool CDBFConverter::WriteToOutputFile(fstream& fileOutput)
{
    bool bResult = true;
    // write to the output file
    // write header
    fileOutput << T_HEADER << endl;
    fileOutput << endl;
    fileOutput << T_DB_VER " " T_VER_NO << endl;
    fileOutput << endl;
    if(m_eBus == J1939)
    {
        fileOutput << T_PROTOCOL " " T_PRPTOCOL_J1939;
    }
    else
    {
        //default CAN
        fileOutput << T_PROTOCOL " " T_PROTOCOL_CAN;
    }
    fileOutput << endl;
    fileOutput << endl;

    //For easy replacement of version Info #define is not added
    fileOutput<< "[BUSMASTER_VERSION] [3.2.2]"<<endl;

    // number of messages
    fileOutput << T_NUM_OF_MSG " " << dec << m_listMessages.size() << endl;
    fileOutput << endl;
    //Write Messagess to the Output file
    CMessage msg;
    bResult &= msg.writeMessageToFile(fileOutput, m_listMessages, false, m_eBus);

    // write all messages, signals not associated with any Messages
    if(!m_listSignal.empty())
    {
        fileOutput << T_ST_SIG_LIST << endl;
        CSignal sig;
        bResult &= sig.WriteSignaltofile(fileOutput, m_listSignal, 0, 0, false);
        fileOutput << T_END_SIG_LIST << endl;
        fileOutput << endl;
    }

    //write value table
    fileOutput << T_ST_VAL_TAB << endl;
    CValueTable temp_vtab;
    temp_vtab.writeValueTabToFile (fileOutput,m_vTab);
    fileOutput << T_END_VAL_TAB << endl;
    fileOutput << endl;
    //write list of nodes
    fileOutput << T_NODE " ";
    bool comma = false;
    list<string>::iterator node;

    for(node=m_listNode.begin(); node!=m_listNode.end(); ++node)
    {
        if(comma)
        {
            fileOutput << ",";
        }

        fileOutput << node->c_str();
        comma = true;
    }

    fileOutput << endl;
    fileOutput << endl;
    //Write Comments
    fileOutput << T_ST_COMMENT << endl;
    //network comments
    fileOutput << T_ST_CM_NET << endl;
    list<CComment>::iterator cmt;

    for(cmt=m_cmNet.begin(); cmt!=m_cmNet.end(); ++cmt)
    {
        fileOutput << cmt->m_elementName.c_str();
        fileOutput << " " << cmt->m_comment.c_str() << endl;
    }

    fileOutput << T_END_CM_NET << endl;
    fileOutput << endl;
    //node comments
    fileOutput << T_ST_CM_NODE << endl;

    for(cmt=m_cmNode.begin(); cmt!=m_cmNode.end(); ++cmt)
    {
        fileOutput << cmt->m_elementName.c_str();
        fileOutput << " " << cmt->m_comment.c_str() << endl;
    }

    fileOutput << T_END_CM_NODE << endl;
    fileOutput << endl;
    //message comments
    fileOutput << T_ST_CM_MSG << endl;

    for(cmt=m_cmMsg.begin(); cmt!=m_cmMsg.end(); ++cmt)
    {
        fileOutput << cmt->m_msgID;
        fileOutput << " " << cmt->m_msgType;
        fileOutput << " " << cmt->m_comment.c_str() << endl;
    }

    fileOutput << T_END_CM_MSG << endl;
    fileOutput << endl;
    //signal comments
    fileOutput << T_ST_CM_SIG << endl;

    for(cmt=m_cmSig.begin(); cmt!=m_cmSig.end(); ++cmt)
    {
        fileOutput << cmt->m_msgID;
        fileOutput << " " << cmt->m_msgType;
        fileOutput << " " << cmt->m_elementName.c_str();
        fileOutput << " " << cmt->m_comment.c_str() << endl;
    }

    fileOutput << T_END_CM_SIG << endl;
    fileOutput << T_END_COMMENT << endl;
    fileOutput << endl;
    //Write Parameters to the output file.
    fileOutput << START_PARAM_TAG << endl;
    fileOutput << START_NETPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[0]);
    fileOutput << END_NETPARAM_TAG << endl;
    fileOutput << endl;
    fileOutput << START_NODEPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[1]);
    fileOutput << END_NODEPARAM_TAG << endl;
    fileOutput << endl;
    fileOutput << START_MSGPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[2]);
    fileOutput << END_MSGPARAM_TAG << endl;
    fileOutput << endl;
    fileOutput << START_SIGPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[3]);
    fileOutput << END_SIGPARAM_TAG << endl;
    fileOutput << endl;
    fileOutput << START_RXPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[4]);
    fileOutput << END_RXPARAM_TAG << endl;
    fileOutput << endl;
    fileOutput << START_TXPARAM_TAG << endl;
    bResult=bResult & WriteParametersToFile(fileOutput, m_listParameterArray[5]);
    fileOutput << END_TXPARAM_TAG << endl;
    fileOutput << END_PARAM_TAG << endl;
    fileOutput << endl;
    //Parameter Values
    fileOutput << START_PARAMVAL_TAG << endl;
    fileOutput << START_NETVAL_TAG << endl;
    list<CParameter>::iterator rParam;

    for(rParam=m_listParameterArray[0].begin(); rParam!=m_listParameterArray[0].end(); ++rParam)
    {
        list<CParameterValues>::iterator vParam;

        for(vParam=rParam->m_listParamValues[0].begin(); vParam!=rParam->m_listParamValues[0].end(); ++vParam)
        {
            vParam->WriteNetValuesToFile(fileOutput, rParam->m_ParamType, rParam->m_ParamName);
        }
    }

    fileOutput << END_NETVAL_TAG << endl;
    fileOutput << endl;
    fileOutput << START_NODEVAL_TAG << endl;

    for(rParam=m_listParameterArray[1].begin(); rParam!=m_listParameterArray[1].end(); ++rParam)
    {
        list<CParameterValues>::iterator vParam;

        for(vParam=rParam->m_listParamValues[1].begin(); vParam!=rParam->m_listParamValues[1].end(); ++vParam)
        {
            vParam->WriteNodeValuesToFile(fileOutput, rParam->m_ParamType, rParam->m_ParamName);
        }
    }

    fileOutput << END_NODEVAL_TAG << endl;
    fileOutput << endl;
    fileOutput << START_MSGVAL_TAG << endl;

    for(rParam=m_listParameterArray[2].begin(); rParam!=m_listParameterArray[2].end(); ++rParam)
    {
        list<CParameterValues>::iterator vParam;

        for(vParam=rParam->m_listParamValues[2].begin(); vParam!=rParam->m_listParamValues[2].end(); ++vParam)
        {
            vParam->WriteMesgValuesToFile(fileOutput, rParam->m_ParamType, rParam->m_ParamName);
        }
    }

    fileOutput << END_MSGVAL_TAG << endl;
    fileOutput << endl;
    fileOutput << START_SIGVAL_TAG << endl;

    for(rParam=m_listParameterArray[3].begin(); rParam!=m_listParameterArray[3].end(); ++rParam)
    {
        list<CParameterValues>::iterator vParam;

        for(vParam=rParam->m_listParamValues[3].begin(); vParam!=rParam->m_listParamValues[3].end(); ++vParam)
        {
            vParam->WriteSigValuesToFile(fileOutput, rParam->m_ParamType, rParam->m_ParamName);
        }
    }

    fileOutput << END_SIGVAL_TAG << endl;
    fileOutput << endl;
    fileOutput << END_PARAMVAL_TAG << endl;
    fileOutput << endl;
    fileOutput << endl;
    //list of not supported
    fileOutput << T_ST_NOT_SUP << endl;
    msg.writeMessageToFile(fileOutput, m_unsupList, true, m_eBus);
    fileOutput << T_END_NOT_SUP << endl;
    fileOutput << endl;
    //lines that were not processed
    fileOutput << T_ST_NOT_PRO << endl;
    list<string>::iterator np;

    for(np=m_notProcessed.begin(); np!=m_notProcessed.end(); ++np)
    {
        fileOutput << np->c_str() << endl;
    }

    fileOutput << endl;
    fileOutput << T_END_NOT_PRO << endl;
    return bResult;
}


/**
 * \brief Logs the eror info in log file
 *
 * Logs the eror info in log file.
 */
void CDBFConverter::CreateLogFile(fstream& fileLog)
{
    char first_msg = 1;
    // write to the output file
    fileLog << "Conversion Warnings Log" << endl;
    fileLog << endl;
    list<CMessage>::iterator msg;

    for(msg=m_listMessages.begin(); msg!=m_listMessages.end(); ++msg)
    {
        first_msg = 1;
        list<CSignal>::iterator sig;

        for(sig=msg->m_listSignals.begin(); sig!=msg->m_listSignals.end(); ++sig)
        {
            // write signal only if it is not valid
            if(sig->m_uiError != CSignal::SIG_EC_NO_ERR)
            {
                string str;

                // for the first wrong signal, log the message details also
                if(first_msg == 1)
                {
                    fileLog << endl;
                    fileLog << "MSG_ID: " << dec << msg->m_uiMsgID;
                    fileLog << " \tMSG_TYPE: " << msg->m_cFrameFormat;
                    fileLog << " \tMSG_NAME: " << msg->m_acName.c_str() << endl;
                    first_msg = 0;
                }

                fileLog << "\tSIG_NAME: " << sig->m_acName.c_str();
                sig->GetErrorString(str);
                fileLog << ", STATUS: " << str;
                sig->GetErrorAction(str);
                fileLog << ", ACTION: " << str << endl;
            }
        }
    }

    //log errors in the signal list.
    first_msg = 1;
    list<CSignal>::iterator sig;

    for(sig=m_listSignal.begin(); sig!=m_listSignal.end(); ++sig)
    {
        // write signal only if it is not valid
        if(sig->m_uiError != CSignal::SIG_EC_OVERFLOW)
        {
            string str;

            // for the first wrong signal, log the message details also
            if(first_msg == 1)
            {
                fileLog << endl;
                fileLog << "MSG_ID: 1073741824";
                fileLog << " \tMSG_TYPE: X";
                fileLog << " \tMSG_NAME: VECTOR__INDEPENDENT_SIG_MSG" << endl;
                first_msg = 0;
            }

            fileLog << "\tSIG_NAME: " << sig->m_acName.c_str();
            sig->GetErrorString(str);
            fileLog << ", STATUS: " << str.c_str();
            sig->GetErrorAction(str);
            fileLog << ", ACTION: " << str.c_str() << endl;
        }
    }

    list<string>::iterator str;

    for(str=defList.begin(); str!=defList.end(); ++str)
    {
        fileLog << str->c_str();
    }

    list<CParameter>::iterator rParam;

    for(rParam=m_listParameters.begin(); rParam!=m_listParameters.end(); ++rParam)
    {
        if(rParam->m_defError)
        {
            fileLog << "OBJECT ID : " << rParam->m_ObjectId.c_str();
            fileLog << "\tPARAM_NAME :\"" << rParam->m_ParamName.c_str() << "\"" << endl;
            fileLog << _("\tDescription:Default Value tag(BA_DEF_DEF_) doesn;t exist ");
            fileLog << _("\t Action Taken : Reset to default value") << endl;
        }

        if(rParam->m_RangeError)
        {
            fileLog << "OBJECT ID : " << rParam->m_ObjectId.c_str();
            fileLog << "\tPARAM_NAME :\"" << rParam->m_ParamName.c_str() << "\"" << endl;
            fileLog << _("\tDescription: Invalid Data Ranges");
            fileLog << _("\t Action Taken : Reset to default value") << endl;
        }
    }
}


/**
 * \brief     creates a list of nodes inthe network
 * \param[in] pcLine String having all the node names
 *
 * Creates a list of nodes in the network.
 */
void CDBFConverter::create_Node_List(char* pcLine)
{
    char* pcToken, *pcTok;
    // get the MSG ID
    pcToken = strtok_s(pcLine, " ,", &pcTok);
    while(pcToken)
    {
        string str = pcToken;
        m_listNode.push_back(str);
        pcToken = strtok_s(NULL, " ,", &pcTok);
    }
}


/**
 * \brief     encrypts all the strings present in the list
 * \param[in] m_notProcessed List of strings
 *
 * Encrypts all the strings present in the list
 */
void CDBFConverter::EncryptData(list<string>& m_notProcessed)
{
    list<string>::iterator str;

    for(str=m_notProcessed.begin(); str!=m_notProcessed.end(); ++str)
    {
        //read the string at the position
        string::iterator ch;

        for(ch=str->begin() ; ch<str->end(); ++ch)
        {
            if ((*ch >= 'a' && *ch <= 'm') || (*ch >= 'A' && *ch <= 'M'))
            {
                *ch = *ch + 13;
            }
            else if ((*ch >= 'n' && *ch <= 'z') || (*ch >= 'N' && *ch <= 'Z'))
            {
                *ch = *ch - 13;
            }
        }
    }
}
