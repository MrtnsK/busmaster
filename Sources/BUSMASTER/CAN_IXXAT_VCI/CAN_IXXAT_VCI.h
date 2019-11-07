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
 * @file
 *  CAN_IXXAT_VCI.h
 *
 * @brief
 *  The main header file for the DLL. The corresponding
 *  CPP file exports the DLL C-function.
 */

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


/**
 * @class CCAN_IXXAT_VCIApp
 *
 * @brief
 *  The DLL application class.
 *
 */
class CCAN_IXXAT_VCIApp : public CWinApp
{
public:
    CCAN_IXXAT_VCIApp();

    // Overrides
public:
    virtual BOOL InitInstance();
    virtual int  ExitInstance();

    DECLARE_MESSAGE_MAP()
};

