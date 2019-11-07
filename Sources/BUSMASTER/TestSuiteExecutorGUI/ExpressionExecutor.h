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
 * \file      ExpressionExecutor.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "Afxtempl.h"

const int SIZE_CHAR = sizeof(CHAR);

class CExpressionExecutor
{
    //Member Variables
private:
    CString m_omFormula;

    //Member Functions
    BOOL SubstituteX(CString& omFormula, float fVal);
public:
    CExpressionExecutor() {};
    BOOL bGetExpressionValue(CString omFormula, float fValue);
    ~CExpressionExecutor() {};
};

