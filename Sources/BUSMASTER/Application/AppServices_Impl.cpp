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
 * \file      AppServices_Impl.cpp
 * \brief     Concrete class source for the application service layer
 * \authors   Ratnadip Choudhury, Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Concrete class source for the application service layer
 */

#include "StdAfx.h"
#include "Error.h"

#include "TraceWnd/UIThread.h"
#include "AppServices_Impl.h"


/**
 * \brief Constructor
 */
CAppServices_Impl::CAppServices_Impl()
{
    m_podUIThread = nullptr;
}


/**
 * \brief Destructor
 */
CAppServices_Impl::~CAppServices_Impl()
{
    m_podUIThread = nullptr;
}


/**
 * \brief Write the string on trace window
 * \param pcTraceStr Character buffer to be written on trace
 *
 * Character buffer to be written on trace
 */
BOOL CAppServices_Impl::bWriteToTrace(char* pcTraceStr)
{
    BOOL Result = FALSE;

    if (nullptr != m_podUIThread)
    {
        CString omStr(pcTraceStr);
        m_podUIThread->vAddString(omStr);
        Result = TRUE;
    }
    return Result;
}
