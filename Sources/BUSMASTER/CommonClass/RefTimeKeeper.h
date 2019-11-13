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
 * @author Anish kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

class CRefTimeKeeper
{
public:
    CRefTimeKeeper();

    /**
     * set time parameters
     *
     * @param[in] currentSystemTime current system time (granularity: milliseconds)
     * @param[in] connectTime connect time (granularity: 100 microseconds)
     */
    void vSetTimeParams(SYSTEMTIME& currentSystemTime, UINT64 connectTime);

    /**
     * get time parameters
     *
     * @param[out] referenceSystemTime_ reference system time (granularity: 100 microseconds)
     * @param[out] absoluteBasetime_ absolute base time (granularity: 100 microseconds)
     */
    void vGetTimeParams(UINT64& referenceSystemTime_, UINT64& absoluteBaseTime_);

protected:
    /** reference system time (granularity: 100 microseconds) */
    UINT64 m_qwRefSysTime;

    /** absolute base time (granularity: 100 microseconds) */
    UINT64 m_qwAbsBaseTime;
};
