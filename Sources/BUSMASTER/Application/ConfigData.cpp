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
 * \file      ConfigData.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "StdAfx.h"
#include "ProjectConfiguration/ProjectConfiguration_extern.h"
#include "ConfigData.h"
//#include "GettextBusmaster.h"
#include "Utility\MultiLanguageSupport.h"

#define DEFAULT_PROJECT_NAME                         "BUSMASTER"
#define BUSMASTER_UNIQUE_ID                              133146

CConfigData CConfigData::m_ouConfigDetails;
CConfigData::CConfigData(void)
{
    m_strCurrProjName = DEFAULT_PROJECT_NAME;
    m_omStrCurrFileName = "";
}

CConfigData::~CConfigData(void)
{
}

CConfigData& CConfigData::ouGetConfigDetailsObject()
{
    return m_ouConfigDetails;
}
void CConfigData::vReadConfigFile()
{
    if (DoDatastorageOperation(OPEN) == 0)
    {
        PROJECTDATA sTempData;
        GetProjectData(m_strCurrProjName, sTempData);
        if (sTempData.m_dwAppUniqueId != BUSMASTER_UNIQUE_ID)
        {
            AfxMessageBox (_("The Configuration file is not Compatible"),
                           MB_OK | MB_ICONERROR);
            DoDatastorageOperation(CLOSE);
        }
    }
    else
    {
        AfxMessageBox (_("The Configuration file is not Valid"),
                       MB_OK | MB_ICONERROR);
        DoDatastorageOperation(CLOSE);
    }
}

void CConfigData::vSaveConfigFile()
{
    DoDatastorageOperation(SAVE);
    //bSetCheckSum(m_omStrCurrFileName , &pCheckSum);
}

void CConfigData::vCloseConfigFile()
{
    DoDatastorageOperation(CLOSE);
    m_omStrCurrFileName.Empty();
}

int CConfigData::SetConfigDatastorage(DATASTORAGEINFO* StorageInfo)
{
    m_omStrCurrFileName = StorageInfo->FSInfo->m_FilePath;
    return SetDatastorageConfig(StorageInfo);
}

int CConfigData::GetConfigDatastorage(DATASTORAGEINFO* StorageInfo)
{
    return GetDatastorageConfig(StorageInfo);
}

void CConfigData::vSetCurrProjName(std::string strCurrProjName)
{
    m_strCurrProjName = strCurrProjName;
}

std::string CConfigData::GetCurrProjName()
{
    return m_strCurrProjName;
}


BOOL CConfigData::bSetCurrProjInfo(PROJECTDATA* ProjData)
{
    ProjData->m_ProjectName = m_strCurrProjName;
    return AddModifyProjectTable(m_strCurrProjName , ProjData);
}

BOOL CConfigData::bGetCurrProjInfo(PROJECTDATA* ProjData)
{
    return GetProjectData(m_strCurrProjName, *ProjData);
}

BOOL CConfigData::bSetData(LPVOID lpVoid, int nStreamLength, std::string strSectionName)
{
    BOOL bRetVal = TRUE;

    // check input param
    if(lpVoid != nullptr)
    {
        //if(m_bConfigInfoLoaded == TRUE)
        //{

        SECTIONDATA tempSecData;
        tempSecData.m_omSectionName = strSectionName;
        tempSecData.m_nBLOBLen = nStreamLength;
        tempSecData.m_bBLOB = new BYTE[nStreamLength];
        memcpy(tempSecData.m_bBLOB,lpVoid, nStreamLength);
        AddModifySectionData(m_strCurrProjName,
                             tempSecData.m_omSectionName, &tempSecData);
        //delete []tempSecData.m_bBLOB;

        //}
    }
    return bRetVal;
}

BOOL CConfigData::bGetData(void*& lpData, int& nStreamLength, std::string strSectionName)
{
    BOOL bRetVal = FALSE;

    // is the configuration loaded?
    // if((m_bConfigInfoLoaded == TRUE) && (bRetVal == TRUE))
    {
        SECTIONDATA tempSecData;
        tempSecData.m_omSectionName = strSectionName;
        bRetVal = GetSectionData(m_strCurrProjName,
                                 tempSecData.m_omSectionName, tempSecData);
        if (bRetVal != FALSE)
        {
            bRetVal = FALSE;
            BYTE* pbNewDat = new BYTE[tempSecData.m_nBLOBLen];
            if (pbNewDat != nullptr)
            {
                memcpy(pbNewDat, tempSecData.m_bBLOB, tempSecData.m_nBLOBLen);
                nStreamLength = tempSecData.m_nBLOBLen;
                lpData = (void*) pbNewDat;
                bRetVal = TRUE;
            }
            else
            {
            }
        }

    }
    return bRetVal;
}

