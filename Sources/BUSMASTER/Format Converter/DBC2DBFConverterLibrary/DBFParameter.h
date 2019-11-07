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
 * \file      Parameter.h
 * \brief     Definition of parameter class
 * \authors   Padmaja A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the parameter class.
 */

#pragma once

/* C++ includes */
#include <fstream>
#include <list>
#include <string>

/* Project includes */
#include "DBFParameterValues.h"
#include "SignalValue.h"

using namespace std;

class CParameter
{
public:
    string          m_ObjectId;
    string          m_ParamType;
    CSignalValue    m_MaxVal;
    CSignalValue    m_MinVal;
    string          m_ValRange;
    CSignalValue    m_InitVal;
    string          m_ParamName;
    bool            m_RangeError;
    bool            m_defError;
    list<CParameterValues> m_listParamValues[6];
    CParameter();
    int Format(char* pcLine);
    CParameter& operator=(const CParameter& param);
    int FormatParamValue(char* pcLine);
    int ReadDefaultVal(char* pcToken);
    friend bool WriteParametersToFile(fstream& fileOutput, list<CParameter>& m_listParameter);
    virtual ~CParameter();
};
