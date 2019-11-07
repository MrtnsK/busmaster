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
 * \file      LogToExcel.cpp
 * \brief     Definition of CLogToExcel class
 * \authors   Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the CLogToExcel class.
 */

/* Project includes */
#include "Defines.h"
#include "LogToExcel.h"

/**
 * \brief Initializing the Column fields map table
 *
 * Initializing the Column fields map table
 */
void CLogToExcel::fnSetFields()
{
    for(UINT unIndex = 0; unIndex < m_unNumOfFields; unIndex++)
    {
        pFieldMapTbl[m_pacFields[unIndex]] = unIndex;
    }
}

/**
 * \brief set the fields which are selected by the user
 *
 * set the fields which are selected by the user
 */
void CLogToExcel::fnSetSelectedFields(CListBox* omSelectedList)
{
    DWORD unMaxIndex = omSelectedList->GetCount();
    //initialising the SelectedField array to -1
    UINT unIndex;   //unIndex declared outside for loop

    for( unIndex = 0; unIndex < m_unNumOfFields; unIndex++)
    {
        unSelectedField[unIndex] = (UINT)-1;
    }

    //setting the appropriate value for each field
    for(unIndex = 0; unIndex < unMaxIndex; unIndex++)
    {
        string strFieldName;
        CString omFieldName;
        omSelectedList->GetText( unIndex,omFieldName);
        strFieldName = omFieldName.GetBuffer(MAX_PATH);
        pMapValue = pFieldMapTbl.find(strFieldName);
        unSelectedField[pMapValue->second] = pMapValue->second;
    }
}

/**
 * \brief Sets the bus type currently selected
 *
 * Sets the bus type currently selected
 */
void CLogToExcel::vSetBUSType(ETYPE_BUS eBus)
{
    m_eBus = eBus;

    if ( m_eBus == CAN )
    {
        m_strBUSNameInLog = "***PROTOCOL CAN***";
    }
    else if ( m_eBus == J1939 )
    {
        m_strBUSNameInLog = "***PROTOCOL J1939***";
    }
    else
    {
        m_strBUSNameInLog = "***PROTOCOL DEFAULT***";
    }
}

/**
 * \brief opens the log file for reading and excel file for writing
 *
 * opens the log file for reading and excel file for writing
 */
CLogToExcel::CLogToExcel(string strLogFileName, string strExcelFileName, UINT unNoOfFields, CHAR_ARRAY_20* pacFields)
{
    m_pacFields = pacFields;
    m_strLogFileName = strLogFileName;
    m_strExcelFileName = strExcelFileName;
    m_unNumOfFields = unNoOfFields;
    m_bFilesOpened = FALSE;
    fnSetFields();
}

/**
 * \brief Destructor
 *
 * Destructor of CLogToExcel
 */
CLogToExcel::~CLogToExcel()
{
}

/**
 * \brief Retrieves each line from the file and stores it in m_strBuffer
 *
 * Retrieves each line from the file and stores it in m_strBuffer
 */
UINT CLogToExcel::unGetLine()
{
    UINT unBufferIndex = 0;  //index to traverse m_strBuffer string
    char ch;               // stores each character read from the file

    //reads log file till end of line is not reached
    while ( (ch = (char)m_pLogFile.get()) != '\n' )
    {
        //m_strBuffer contains each line read from log file
        m_strBuffer.append(1,ch);

        if( ch == EOF )
        {
            m_strBuffer.append(1,'\0');
            return 1;
        }

        unBufferIndex++;
    }

    m_strBuffer.append(1,'\0');
    return 0;
}

/**
 * \brief Retrieves each field from m_strBuffer and writes it into Excel file.
 *
 * Retrieves each field from m_strBuffer and writes it into Excel file.
 */
void CLogToExcel::vPrintFields()
{
    // ignore the headers in log file
    if( (m_strBuffer.empty() ) ||(m_strBuffer[0] == '*') ||(m_strBuffer[0] == ' ')  )
    {
        return;
    }

    /*if(m_strBuffer[0] != '<')
       return;*/
    UINT unBufferIndex = 0;     //index to traverse m_strBuffer string
    UINT unFieldBufferIndex=0;  //index to traverse m_strFieldBuffer string
    UINT unFieldIndex = 0;      // field index
    bool bFieldIndexExists  = false;
    m_pExcelFile << "<TR>";

    while( unFieldIndex != m_unNumOfFields && m_strBuffer[unBufferIndex] != '\0' )
    {
        unFieldBufferIndex = 0;
        m_strFieldBuffer.clear();

        if( unFieldIndex != m_unNumOfFields-1 )
        {
            //reading each field from m_strBuffer
            while( m_strBuffer[unBufferIndex] != ' ' && m_strBuffer[unBufferIndex] != '\0')
            {
                m_strFieldBuffer.append(1,m_strBuffer[unBufferIndex]);
                unFieldBufferIndex++;
                unBufferIndex++;
            }// while - m_strBuffer[unBufferIndex] != " "

            if( m_strBuffer.length() != unBufferIndex && m_strBuffer[unBufferIndex] == ' ')
            {
                unBufferIndex++;
            }

            if( m_strBuffer.length() != unBufferIndex && m_strBuffer[unBufferIndex] == ' ')
            {
                unBufferIndex++;
            }
        }// unFieldIndex != m_unNumOfFields-1
        else
        {
            // reading the data bytes field from m_strBuffer
            unFieldBufferIndex = 0;

            while((m_strBuffer[unBufferIndex] != '\0') && (m_strBuffer[unBufferIndex] != '\n'))
            {
                m_strFieldBuffer.append(1,m_strBuffer[unBufferIndex]);
                unFieldBufferIndex++;
                unBufferIndex++;
            }
        } // else

        m_strFieldBuffer.append(1,'\0');

        //writing the field into excel file
        for(UINT nIndex = 0; nIndex < m_unNumOfFields; nIndex++)
        {
            if ( unSelectedField[nIndex] == unFieldIndex )
            {
                bFieldIndexExists = true;
                break;
            } // if unSelectedField[nIndex] == unFieldIndex
        }// for loop - unSelectedField

        if ( bFieldIndexExists )
        {
            m_pExcelFile << "<TD>" << m_strFieldBuffer.c_str() << "</TD>";
            bFieldIndexExists = false;
        }
        else
        {
            m_strFieldBuffer="";
        }

        unFieldIndex++;
    }// While - field navigation

    m_strFieldBuffer.clear();
    m_pExcelFile << "</TR>";
}

/**
 * \brief Converts the .log file to .xls file
 *
 * Converts the .log file to .xls file
 */
HRESULT CLogToExcel::bConvert()
{
    HRESULT hRetVal = S_OK;
    //open log file in read mode
    m_pLogFile.open(m_strLogFileName.c_str(), fstream::in);

    if(!m_pLogFile.is_open())
    {
        //log file could not be opened
        hRetVal = ERR_EXPORTTOEXCEL_LOGFILEOPENERROR;
        //MessageBox(NULL,EXPORTTOEXCEL_LOGFILEOPENERROR,APPLICATION_NAME,MB_OK);
        m_bFilesOpened = FALSE;
    }
    else
    {
        //open Excel file in write
        m_pExcelFile.open(m_strExcelFileName.c_str(), fstream::out);

        if(!m_pExcelFile.is_open())
        {
            //Excel file could not be opened
            hRetVal = ERR_EXPORTTOEXCEL_CSVFILEOPENERROR;
            //MessageBox(NULL,EXPORTTOEXCEL_CSVFILEOPENERROR,APPLICATION_NAME,MB_OK);
            m_bFilesOpened = FALSE;
        }
        else
        {
            m_bFilesOpened = TRUE;

        }
    }

    if(m_bFilesOpened == TRUE)
    {
        m_pExcelFile << "<HTML><HEAD></HEAD><BODY>" << endl;
        m_pExcelFile << _("<FONT COLOR =\"GREEN\"><CENTER><B>BUSMASTER - Exported Log File Report</B></CENTER>");
        m_pExcelFile << "<BR><BR>";
        m_pExcelFile << "<TABLE BORDER COLOR=\"BLACK\"><TR>";

        // write column headers
        for( UINT unIndex = 0; unIndex < m_unNumOfFields; unIndex++)
            if(unSelectedField[unIndex]!=-1)
            {
                m_pExcelFile << "<TD><FONT COLOR=\"Green\"><B> " << m_pacFields[unIndex] << " </B></TD>";
            }

        m_pExcelFile << "</TR>";

        size_t found;

        // write each column
        while(!unGetLine())
        {
            found = 0;
            if ( m_strBuffer.find("***PROTOCOL") !=  string::npos )
            {
                found = m_strBuffer.find( m_strBUSNameInLog );
            }

            /* If the current BUS type and log file selected matches, then perform conversion */
            if (found!=string::npos)
            {
                vPrintFields();
            }
            else
            {
                MessageBox(NULL,_(EXPORTTOEXCEL_FAILUREMSG),_(APPLICATION_NAME),MB_OK);
                //close file
                m_pExcelFile.close();
                m_pLogFile.close();
                ::DeleteFile(m_strExcelFileName.c_str());
                return S_FALSE;
            }
            m_strBuffer.clear();
        }

        m_pExcelFile << "</TABLE></BODY></HTML>" << endl;
        //close file
        m_pExcelFile.close();
        m_pLogFile.close();
        hRetVal =S_OK;
    }
    else
    {
        if(m_pLogFile.is_open())
        {
            m_pLogFile.close();
        }
        if(m_pExcelFile.is_open())
        {
            m_pExcelFile.close();
        }
    }

    return hRetVal;
}
void CLogToExcel::GetErrorString(HRESULT hResult, string& omStrErrorString)
{
    switch(hResult)
    {
        case ERR_EXPORTTOEXCEL_SYNTAXERROR:
            omStrErrorString = _(EXPORTTOEXCEL_SYNTAXERROR);
            break;
        case ERR_EXPORTTOEXCEL_FILESNOTOPEN:
            omStrErrorString = EXPORTTOEXCEL_FILESNOTOPEN;
            break;
        case ERR_EXPORTTOEXCEL_FIELDSELECTIONERROR:
            omStrErrorString = _(EXPORTTOEXCEL_FIELDSELECTIONERROR);
            break;
        case ERR_EXPORTTOEXCEL_LOGFILEERROR:
            omStrErrorString = _(EXPORTTOEXCEL_LOGFILEERROR);
            break;
        case ERR_EXPORTTOEXCEL_CSVFILEERROR:
            omStrErrorString = _(EXPORTTOEXCEL_CSVFILEERROR);
            break;
        case ERR_EXPORTTOEXCEL_FILEERROR:
            omStrErrorString = _(EXPORTTOEXCEL_FILEERROR);
            break;
        case ERR_EXPORTTOEXCEL_LOGFILEOPENERROR:
            omStrErrorString = _(EXPORTTOEXCEL_LOGFILEOPENERROR);
            break;
        case ERR_EXPORTTOEXCEL_CSVFILEOPENERROR:
            omStrErrorString = _(EXPORTTOEXCEL_CSVFILEOPENERROR);
            break;
        default:
            omStrErrorString = _("Invalid Error");
            break;
    }
}