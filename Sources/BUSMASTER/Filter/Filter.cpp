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
 * \file      Filter.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "Filter_stdafx.h"
#include <afxdllx.h>
#include "Filter_resource.h"
#include "FilterConfigDlg.h"
#include "MainSubListDlg.h"
//#include "../Application/GettextBusmaster.h"
#include "../Application/MultiLanguage.h"
#include "Utility\MultiLanguageSupport.h"
#define USAGE_EXPORT
#include "Filter_extern.h"


static AFX_EXTENSION_MODULE FilterDLL = { false, nullptr };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{

    static HINSTANCE shLangInst=nullptr;


    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("Filter.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(FilterDLL, hInstance))
        {
            return 0;
        }

        // Insert this DLL into the resource chain
        // NOTE: If this Extension DLL is being implicitly linked to by
        //  an MFC Regular DLL (such as an ActiveX Control)
        //  instead of an MFC application, then you will want to
        //  remove this line from DllMain and put it in a separate
        //  function exported from this Extension DLL.  The Regular DLL
        //  that uses this Extension DLL should then explicitly call that
        //  function to initialize this Extension DLL.  Otherwise,
        //  the CDynLinkLibrary object will not be attached to the
        //  Regular DLL's resource chain, and serious problems will
        //  result.


        // Begin of Multiple Language support
        if ( CMultiLanguage::m_nLocales <= 0 )  // Not detected yet
        {
            CMultiLanguage::DetectLangID();     // Detect language as user locale
            CMultiLanguage::DetectUILanguage(); // Detect language in MUI OS
        }
        TCHAR szModuleFileName[MAX_PATH];       // Get Module File Name and path
        int ret = ::GetModuleFileName(hInstance, szModuleFileName, MAX_PATH);
        if ( ret == 0 || ret == MAX_PATH )
        {
            ASSERT(false);
        }
        // Load resource-only language DLL. It will use the languages
        // detected above, take first available language,
        // or you can specify another language as second parameter to
        // LoadLangResourceDLL. And try that first.
        shLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
        if (shLangInst)
        {
            FilterDLL.hResource = shLangInst;
        }
        // End of Multiple Language support


        new CDynLinkLibrary(FilterDLL);

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if (shLangInst)
        {
            FreeLibrary(shLangInst);
        }

        TRACE0("Filter.DLL Terminating!\n");

        // Terminate the library before destructors are called
        AfxTermExtensionModule(FilterDLL);
    }
    return 1;   // ok
}

USAGEMODE HRESULT Filter_ShowConfigDlg(void* pExistingFilter, const void* psMsgEntry,
                                       ETYPE_BUS eType, UINT nHardware, CWnd* pParent)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(FilterDLL.hResource);

    HRESULT hResult = S_FALSE;
    switch (eType)
    {
        case CAN:
        {
            if (pExistingFilter != nullptr)
            {
                SFILTERAPPLIED_CAN* pAppliedFilterCan = (SFILTERAPPLIED_CAN*)pExistingFilter;
                SFILTERAPPLIED_CAN sTempObj;
                sTempObj.bClone(*pAppliedFilterCan);

                CFilterConfigDlg omDlg( &sTempObj, (IBMNetWorkGetService*)psMsgEntry, nHardware, pParent );
                if (omDlg.DoModal() == IDOK)
                {
                    pAppliedFilterCan->bClone(sTempObj);
                    hResult = S_OK;
                }
                //delete omDlg;
            }
        }
        break;
        case LIN:
            if (pExistingFilter != nullptr)
            {
                SFILTERAPPLIED_LIN* pAppliedFilterLin = (SFILTERAPPLIED_LIN*)pExistingFilter;
                SFILTERAPPLIED_LIN sTempObj;
                sTempObj.bClone(*pAppliedFilterLin);

                CFilterConfigDlg omDlg(&sTempObj,(IBMNetWorkGetService*) psMsgEntry, nHardware, pParent);
                if (omDlg.DoModal() == IDOK)
                {
                    pAppliedFilterLin->bClone(sTempObj);
                    hResult = S_OK;
                }
                //delete omDlg;
            }
            break;

        default:
        {
        }
        break;
    }
    //Place this at the end of the export function.
    //switch back to previous resource handle.
    AfxSetResourceHandle(hInst);
    return hResult;
}

USAGEMODE HRESULT Filter_ShowSelDlg(CWnd* pParent, CMainEntryList* podMainSubList)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(FilterDLL.hResource);

    SGUIPARAMS sGuiParams;
    /* Update GUI related information */
    /* 1. Title Name, Main entry combo box name,
            Name of the list controls */
    strcpy_s(sGuiParams.m_acTitleName, MAX_PATH, _("Filter Selection Dialog"));
    strcpy_s(sGuiParams.m_acMainListName, MAX_PATH, _("Bus"));
    strcpy_s(sGuiParams.m_acUnSelListName, MAX_PATH, _("Configured Filters"));
    strcpy_s(sGuiParams.m_acSelListName, MAX_PATH, _("Selected Filters"));
    /* Whether to combine main entry Id with sub entry name or not*/
    sGuiParams.m_bCombine = FALSE;
    /* What image to be loaded */
    sGuiParams.m_pomImageList = new CImageList;
    sGuiParams.m_pomImageList->Create(IDB_BMP_FILTER,
                                      16,
                                      1,
                                      defCOLOR_WHITE);
    //Icon index to unselected list
    sGuiParams.m_unUnSelIconIndex = 2;
    //Icon index to selected list
    sGuiParams.m_unSelIconIndex = 2;

    CMainSubListDlg omDlg(pParent, podMainSubList, sGuiParams);
    INT_PTR nReturn = omDlg.DoModal();
    //delete the created list
    sGuiParams.m_pomImageList->DeleteImageList();
    delete sGuiParams.m_pomImageList;

    //Place this at the end of the export function.
    //switch back to previous resource handle.
    AfxSetResourceHandle(hInst);
    return (HRESULT)nReturn;
}

static INT nGetFilterIndexInSrc(SFILTERSET& DestSet, const SFILTERAPPLIED_CAN& sSrcFilter)
{
    INT nIndex = -1;
    UINT SrcCount = sSrcFilter.m_ushTotal;
    for (UINT i = 0; i < SrcCount; i++)
    {
        if (DestSet.m_sFilterName.m_acFilterName == sSrcFilter.m_psFilters[i].m_sFilterName.m_acFilterName)
        {
            nIndex = i;
            break;
        }
    }
    return nIndex;
}

static INT nGetFilterIndexInSrc(SFILTERSET& DestSet, const SFILTERAPPLIED_LIN& sSrcFilter)
{
    INT nIndex = -1;
    UINT SrcCount = sSrcFilter.m_ushTotal;
    for (UINT i = 0; i < SrcCount; i++)
    {
        if (DestSet.m_sFilterName.m_acFilterName == sSrcFilter.m_psFilters[i].m_sFilterName.m_acFilterName)
        {
            nIndex = i;
            break;
        }
    }
    return nIndex;
}



static void vReUpdateAppliedFilterCAN(SFILTERAPPLIED_CAN& DestFilter,
                                      SFILTERAPPLIED_CAN& SrcFilter)
{
    UINT DestCount = DestFilter.m_ushTotal;
    UINT DelCount = 0;
    for (UINT i = 0; i < DestCount; i++)
    {
        INT nIndex = nGetFilterIndexInSrc(DestFilter.m_psFilters[i],
                                          SrcFilter);
        if (nIndex != -1) //update the applied filter
        {
            DestFilter.m_psFilters[i].bClone(SrcFilter.m_psFilters[nIndex]);
        }
        else //Remove the applied filter
        {
            DestFilter.m_psFilters[i].bClone(DestFilter.m_psFilters[DestCount - 1]);
            DelCount++;
        }
    }
    //DestFilter.m_ushTotal -= DelCount;
    DestFilter.m_ushTotal = DestFilter.m_ushTotal - (USHORT) DelCount;
}


static void vReUpdateAppliedFilterLIN(SFILTERAPPLIED_LIN& DestFilter,
                                      SFILTERAPPLIED_LIN& SrcFilter)
{
    UINT DestCount = DestFilter.m_ushTotal;
    UINT DelCount = 0;
    for (UINT i = 0; i < DestCount; i++)
    {
        INT nIndex = nGetFilterIndexInSrc(DestFilter.m_psFilters[i],
                                          SrcFilter);
        if (nIndex != -1) //update the applied filter
        {
            DestFilter.m_psFilters[i].bClone(SrcFilter.m_psFilters[nIndex]);
        }
        else //Remove the applied filter
        {
            DestFilter.m_psFilters[i].bClone(DestFilter.m_psFilters[DestCount - 1]);
            DelCount++;
        }
    }
    //DestFilter.m_ushTotal -= DelCount;
    DestFilter.m_ushTotal = DestFilter.m_ushTotal - (USHORT) DelCount;
}



USAGEMODE HRESULT Filter_ReUpdateAppliedFilter(void* pvFilterApplied,
        const void* pvFilterConfigured, ETYPE_BUS eType)
{
    HRESULT hResult = S_OK;
    if ((pvFilterApplied != nullptr) && (pvFilterConfigured != nullptr))
    {
        switch (eType)
        {
            case CAN:
            {
                SFILTERAPPLIED_CAN* psFilterApplied = (SFILTERAPPLIED_CAN*)pvFilterApplied;
                SFILTERAPPLIED_CAN* psFilterConfigured = (SFILTERAPPLIED_CAN*)pvFilterConfigured;
                vReUpdateAppliedFilterCAN(*psFilterApplied, *psFilterConfigured);
            }
            break;
            case LIN:
            {
                SFILTERAPPLIED_LIN* psFilterApplied = (SFILTERAPPLIED_LIN*)pvFilterApplied;
                SFILTERAPPLIED_LIN* psFilterConfigured = (SFILTERAPPLIED_LIN*)pvFilterConfigured;
                vReUpdateAppliedFilterLIN(*psFilterApplied, *psFilterConfigured);
            }
            break;

            default:
            {
                ASSERT(false);
            }
            break;
        }
    }
    return hResult;
}




