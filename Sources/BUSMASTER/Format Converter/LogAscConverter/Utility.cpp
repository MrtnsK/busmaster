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
 * \file      Utility.cpp
 * \brief     Definiton of the Utility Functions.
 * \authors   Venkatanarayana Makam, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of utility functions.
 */

/* C includes */
#include <atltime.h>
#include <string.h>

#include "../../Utility/MultiLanguageSupport.h"
extern "C" int nGetDayFromDate(char* pchDate, char* pchMonth, char* chWeekDay)
{
    if(pchDate == NULL || chWeekDay == NULL || pchMonth == NULL)
    {
        return -1;
    }

    int nDate, nMonth, nYear;
    char chDelim[] = ":. ";
    char* pchTemp = strtok(pchDate, chDelim);
    nDate = nMonth = nYear = 0;
    if (NULL != pchTemp)
    {
        nDate = atoi(pchTemp);
    }

    pchTemp = strtok(NULL, chDelim);
    if (NULL != pchTemp)
    {
        nMonth = atoi(pchTemp);
    }

    pchTemp = strtok(NULL, chDelim);
    if (NULL != pchTemp)
    {
        nYear = atoi(pchTemp);
    }
    /*sscanf(pchDate, "%d%c%d%c%d", &nDate, &chTemp, &nMonth,
        &chTemp, &nYear);*/
    CTime omTime(nYear, nMonth, nDate, 0, 0, 0, 0);
    //Day
    int nDay = omTime.GetDayOfWeek();

    switch(nDay)
    {
        case 1:
            strcpy(chWeekDay, _T("Sun"));
            break;

        case 2:
            strcpy(chWeekDay, _T("Mon"));
            break;

        case 3:
            strcpy(chWeekDay, _T("Tue"));
            break;

        case 4:
            strcpy(chWeekDay, _T("Wed"));
            break;

        case 5:
            strcpy(chWeekDay, _T("Thu"));
            break;

        case 6:
            strcpy(chWeekDay, _T("Fri"));
            break;

        case 7:
            strcpy(chWeekDay, _T("Sat"));
            break;
    }

    //Month
    switch(nMonth)
    {
        case 1:
            strcpy(pchMonth, "Jan");
            break;

        case 2:
            strcpy(pchMonth, "Feb");
            break;

        case 3:
            strcpy(pchMonth, "Mar");
            break;

        case 4:
            strcpy(pchMonth, "Apr");
            break;

        case 5:
            strcpy(pchMonth, "May");
            break;

        case 6:
            strcpy(pchMonth, "Jun");
            break;

        case 7:
            strcpy(pchMonth, "Jul");
            break;

        case 8:
            strcpy(pchMonth, "Aug");
            break;

        case 9:
            strcpy(pchMonth, "Sep");
            break;

        case 10:
            strcpy(pchMonth, "Oct");
            break;

        case 11:
            strcpy(pchMonth, "Nov");
            break;

        case 12:
            strcpy(pchMonth, "Dec");
            break;
    }

    return 0;
}
