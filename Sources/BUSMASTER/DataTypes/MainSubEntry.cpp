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
 * \file      MainSubEntry.cpp
 * \brief     Implementation file for main sub entry entities.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for main sub entry entities.
 */

#include "DataTypes_stdafx.h"
#include "MainSubEntry.h"
#include "Utility/MultiLanguageSupport.h"

SGUIPARAMS::SGUIPARAMS()
{
    m_acTitleName[MAX_PATH - 1] = '\0';
    m_acMainListName[MAX_PATH - 1] = '\0';
    m_acUnSelListName[MAX_PATH - 1] = '\0';
    m_acSelListName[MAX_PATH - 1] = '\0';

    _tcsset(m_acTitleName, '\0');
    _tcsset(m_acMainListName, '\0');
    _tcsset(m_acUnSelListName, '\0');
    _tcsset(m_acSelListName, '\0');

    m_pomImageList = nullptr;
    m_unUnSelIconIndex = 0;
    m_unSelIconIndex = 0;
    m_bCombine = TRUE;
}

tagSubEntry::tagSubEntry()
{
    mIsSelected = false;
}

tagSubEntry::~tagSubEntry()
{
}

BOOL tagSubEntry::operator ==(const tagSubEntry& RefObj) const
{
    BOOL bResult = FALSE;
    if (/*(m_unSubEntryID == RefObj.m_unSubEntryID) || */
        (m_omSubEntryName == RefObj.m_omSubEntryName))
    {
        bResult = TRUE;
    }
    return bResult;
}

tagMainEntry::tagMainEntry()
{
    vResetAll();
}

tagMainEntry::~tagMainEntry()
{
    vResetAll();
}

void tagMainEntry::vResetAll(void)
{
    m_unMainEntryID = 0;
    m_omMainEntryName = "";
    m_odUnSelEntryList.RemoveAll();
    m_odSelEntryList.RemoveAll();
}

tagMainEntry& tagMainEntry::operator=(tagMainEntry& RefObj)
{
    vResetAll();

    m_unMainEntryID     = RefObj.m_unMainEntryID;
    m_omMainEntryName   = RefObj.m_omMainEntryName;
    m_odUnSelEntryList.AddTail(&(RefObj.m_odUnSelEntryList));
    m_odSelEntryList.AddTail(&(RefObj.m_odSelEntryList));

    return *this;
}

BOOL tagMainEntry::operator==(const tagMainEntry& RefObj) const
{
    BOOL bResult = FALSE;
    if ((m_unMainEntryID == RefObj.m_unMainEntryID) ||
            (m_omMainEntryName == RefObj.m_omMainEntryName))
    {
        bResult = TRUE;
    }
    return bResult;
}
