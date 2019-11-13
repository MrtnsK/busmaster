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
 * \file      Filter_extern.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#if defined USAGEMODE
#undef USAGEMODE
#endif

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#include "DataTypes/MsgSignal_Datatypes.h"
#include "DataTypes/MainSubEntry.h"

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

    USAGEMODE HRESULT Filter_ShowConfigDlg(void* pvExistingFilter,
                                           const void* psMsgEntry, ETYPE_BUS eType, UINT nHardware, CWnd* pParent);
    USAGEMODE HRESULT Filter_ShowSelDlg(CWnd* pParent, CMainEntryList* podMainSubList);
    USAGEMODE HRESULT Filter_DoFiltering(const void* pvFilterApplied,
                                         const void* pvFrame, ETYPE_BUS eType);
    USAGEMODE HRESULT Filter_ReUpdateAppliedFilter(void* pvFilterApplied,
            const void* pvFilterConfigured, ETYPE_BUS eType);

    USAGEMODE HRESULT SetECUFilterDetails(void* pExistingFilter, const void* psMsgEntry);

#ifdef __cplusplus
}
#endif
