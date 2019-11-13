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
 * \file      ValueDescriptor.cpp
 * \brief     Implementation of value descriptor class
 * \authors   Ratnadip Choudhury, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the value descriptor class.
 */

/* Project includes */
#include "DBFSignal.h"
#include "DBFTagDefinitions.h"
#include "DBFValueDescriptor.h"

using namespace std;

/**
 * \brief Constructor
 *
 * Constructor of CValueDescriptor
 */
CValueDescriptor::CValueDescriptor()
{
    m_value.iValue = 1;
    m_value.uiValue = 1;
    m_value.fValue = 1;
    m_value.dValue = 1;
    m_value.cValue = "";
    m_value.i64Value = 1;
    m_value.ui64Value = 1;
    m_acDescriptor = "";
}


/**
 * \brief Destructor
 *
 * Destructor of CValueDescriptor
 */
CValueDescriptor::~CValueDescriptor()
{
}

/**
 * \brief     writes the value descriptors in the given list to the output file
 * \param[in] fileOutput Pointer to the Output file
 * \param[in] m_ucType data type of the value
 * \param[in] m_listValueDescriptor List of Value descriptors
 *
 * Writes the value descriptors in the given list to the output file.
 */
void CValueDescriptor::writeValueDescToFile(fstream& fileOutput, char m_ucType, list<CValueDescriptor>& m_listValueDescriptor)
{
    list<CValueDescriptor>::iterator desc;

    for (desc=m_listValueDescriptor.begin(); desc!=m_listValueDescriptor.end(); ++desc)
    {
        fileOutput << T_VALUE_DESC " \"";
        fileOutput << desc->m_acDescriptor.c_str();
        fileOutput << "\",";

        switch(m_ucType)
        {
            case CSignal::SIG_TYPE_BOOL:
            case CSignal::SIG_TYPE_UINT:
                fileOutput << dec << desc->m_value.uiValue;
                break;

            case CSignal::SIG_TYPE_INT:
                fileOutput << dec << desc->m_value.iValue;
                break;

            case CSignal::SIG_TYPE_FLOAT:
                fileOutput << dec << desc->m_value.fValue;
                break;

            case CSignal::SIG_TYPE_DOUBLE:
                fileOutput << dec << desc->m_value.dValue;
                break;

            case CSignal::SIG_TYPE_INT64:
                fileOutput << dec << desc->m_value.i64Value;
                break;

            case CSignal::SIG_TYPE_UINT64:
                fileOutput << dec << desc->m_value.ui64Value;
                break;

            default:
                break;
        }

        fileOutput << endl;
    }

    return;
}
