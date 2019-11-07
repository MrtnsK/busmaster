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
 * \file      Converter.h
 * \brief     CConverter class.
 * \authors   Mahesh B S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CConverter class.
 */

#pragma once

#include <afxcoll.h>
/* C++ includes */
#include <fstream>
#include <list>
#include <string>

/* Project includes */
#include "Comment.h"
#include "Message.h"
#include "Parameter.h"
#include "ParameterVal.h"
#include "ValueTable.h"

//#include "../FormatConverterApp/GettextBusmaster.h"
#include "../../Utility/MultiLanguageSupport.h"

using namespace std;

class CConverter
{
public:
    CConverter();
    virtual ~CConverter();

public:
    unsigned int Convert(string sCanoeFile,string sCanMonFile);
    void GetResultString(string& str);
    static fstream fileLog;
    static bool bLOG_ENTERED;
    static unsigned int ucMsg_DLC;
    string m_omLogFilePath;

private:
    enum {CON_RC_NOERROR=0,CON_RC_COMPLETED_WITH_ERROR,CON_RC_FILEOPEN_ERROR_INFILE,
          CON_RC_FILEOPEN_ERROR_OUTFILE,CON_RC_FORMAT_ERROR_INFILE,CON_RC_FILEOPEN_ERROR_LOGFILE
         };

    unsigned int SetResultCode(unsigned int uiCode);
    unsigned int GenerateMessageList(fstream& fileInput);
    void ValidateMessageList();
    bool WriteToOutputFile(fstream& fileOutput);
    void CreateLogFile(fstream& fileLog);
    void create_Node_List(char*);
    void DecryptData(list<string>& m_notProcessed);

    unsigned int m_uiResultCode;
    list<string> m_notProcessed;
    list<string> m_listNode;
    list<CMessage> m_listMessages;
    list<CParameter> m_listParameterArray[6];
    list<CValueTable> m_vTab;
    list<CComment> m_listComments[4];
};
