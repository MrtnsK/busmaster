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
 * @file CAN_ETAS_BOA/CAN_ETAS_BOA_stdafx.h
 * @author Pradeep Kadoor
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * include file for standard system include files,
 * or project specific include files that are used frequently, but
 * are changed infrequently
 */

#pragma once

#define WIN32_LEAN_AND_MEAN    // Exclude rarely-used stuff from Windows headers

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS  // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>          // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>         // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include "afxtempl.h"
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wtypes.h>
#include <atlconv.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
#include <locale.h>

#include <string>

// TODO: reference additional headers your program requires here
const int ERR_INVALID_PROCDIL       = -1;
const int ERR_INVALID_PARENTHANDLE  = -2;
const int INFO_RETAINED_CONFDATA    = 0;
const int INFO_CHANGED_CONFDATA     = 1;

const int WARNING_NOTCONFIRMED      = -3;
const int ERR_CONFIRMED_CONFIGURED  = -4;
const int INFO_INIT_DATA_CONFIRMED  = 2;
const int INFO_CONFIRMED_CONFIGURED = 3;


/* Definitions for BOA versions */
#define BOA_VERSION_1_4 0x010400
#define BOA_VERSION_1_5 0x010500
#define BOA_VERSION_2   0x020000
#define BOA_VERSION_2_0 0x020000
#define BOA_VERSION_2_1 0x020100
#define BOA_VERSION_2_2 0x020200

/* Selection which BOA version is used for the compilation. */
//#define BOA_VERSION BOA_VERSION_1_4
