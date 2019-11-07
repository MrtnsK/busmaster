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
 * \file      ValueDescriptor.h
 * \brief     CValueDescriptor class.
 * \authors   Mahesh B S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CValueDescriptor class.
 */

#pragma once

/* C++ includes */
#include <fstream>
#include <list>
#include <string>

/* Project includes */
#include "Definitions.h"
#include "SignalValue.h"

using namespace std;

class CValueDescriptor
{
public:
    CValueDescriptor();
    virtual ~CValueDescriptor();
    int Format(char* pcLine);
    //  CValueDescriptor& operator=(CValueDescriptor& valDesc);
    void writeValueDescToFile(fstream& fileOutput, char m_ucType, list<CValueDescriptor>& m_listValueDescriptor);
    string      m_sDescriptor;
    CSignalValue   m_value;
};
