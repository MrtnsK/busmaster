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
 * \file      LogToExcel.h
 * \brief     Definition of CLogToExcel class
 * \authors   Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the CLogToExcel class.
 */

#pragma once

#define VC_EXTRALEAN        /* Exclude rarely-used stuff from Windows headers */

/* MFC includes */
#include <afxwin.h>         /* MFC core and standard components */
#include <afxext.h>         /* MFC extensions */

/* C++ includes */
#include <fstream>
#include <map>
#include <string>

using namespace std;

#define defNO_OF_FIELDS_CAN   7
#define defNO_OF_FIELDS_J1939 11
// Fileld names

typedef char CHAR_ARRAY_20[20];

__declspec( selectany ) char acFields_CAN[defNO_OF_FIELDS_CAN][20] = {  "Time",
        "Direction",
        "Channel",
        "CAN ID",
        "Type",
        "DLC",
        "Data Bytes"
                                                                     };
__declspec( selectany ) char acFields_J1939[defNO_OF_FIELDS_J1939][20] = {  "Time",
        "Channel",
        "CAN ID",
        "PGN",
        "TYPE",
        "Source",
        "Destination",
        "Priority",
        "Direction",
        "DLC",
        "Data Bytes"
                                                                         };

#define defSTR_CSV_DIALOG_CAPTION    _("Select export file")
#define defSTR_CSV_FORMAT_SPECIFIER  _("Comma Separated Values file(s) (*.csv)|*.csv||")
#define defSTR_CSV_FORMAT            "csv"

#define defSTR_XLS_FORMAT_SPECIFIER  _("Excel file(s) (*.xls)|*.xls||")
#define defSTR_XLS_FORMAT            "xls"

#define defSTR_LOG_DIALOG_CAPTION    _("Select log file")
#define defSTR_LOG_FORMAT_SPECIFIER  "*.log|*.log||"
#define defSTR_LOG_FORMAT            "log"
#define defSTR_FILE_NAME_EMPTY       _("Please enter log file and export file names")
//#define defSTR_FILE_OPEN_ERROR       _("Unable to open the file %s")

#define defSTR_CSV_HEADER            _("BUSMASTER generated report\n\n")

class CLogToExcel
{
public:
    CLogToExcel(string strLogFileName, string strExcelFileName, UINT unNoOfFields, CHAR_ARRAY_20* pacFields) ; // Open spreadsheet for reading and writing
    ~CLogToExcel();
    HRESULT bConvert();
    void fnSetSelectedFields(CListBox*);
    void GetErrorString(HRESULT hResult, string& omStrErrorString);
    void fnSetFields();
    void vSetBUSType(ETYPE_BUS eBus);

private:
    ETYPE_BUS m_eBus;
    string m_strBUSNameInLog;
    CHAR_ARRAY_20* m_pacFields;
    map<string, DWORD> pFieldMapTbl;
    map<string, DWORD> :: const_iterator pMapValue;
    string m_strLogFileName;
    string m_strExcelFileName;
    fstream m_pLogFile;
    fstream m_pExcelFile;
    string m_strBuffer;
    string m_strFieldBuffer;
    UINT unSelectedField[defNO_OF_FIELDS_J1939];
    UINT m_unNumOfFields;
    UINT unGetLine();
    void vPrintFields();
    BOOL m_bFilesOpened;
};
