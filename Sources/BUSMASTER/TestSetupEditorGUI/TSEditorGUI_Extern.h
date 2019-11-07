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
 * \file      TSEditorGUI_Extern.h
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "Utility/XMLUtils.h"
#pragma once

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif
    static HINSTANCE m_hDllInstance;
    USAGEMODE HRESULT TS_vShowTSEditorWindow(void* pParentWnd);
    USAGEMODE HRESULT TS_vSetDILInterfacePtr(void* ptrDILIntrf);
    USAGEMODE HRESULT TS_vPostMessageToTSWnd(UINT msg, WPARAM wParam, LPARAM lParam);
    USAGEMODE HRESULT TS_hTSEditorWindowShown();
    USAGEMODE HRESULT TS_hLoadTestSetupFile(CString omFilePath);
    USAGEMODE HWND hGetHwnd();
    //USAGEMODE HRESULT TSE_hGetConfigurationData(BYTE*& pDesBuffer, UINT& nBuffSize);
    USAGEMODE HRESULT TSE_hGetConfigurationData(xmlNodePtr* pxmlNodePtr);
    USAGEMODE HRESULT TSE_hSetConfigurationData(BYTE* pSrcBuffer, UINT nBuffSize);
    USAGEMODE HRESULT TSE_hSetXmlConfigurationData(xmlDocPtr pXmlDoc);
#ifdef __cplusplus
}
#endif
