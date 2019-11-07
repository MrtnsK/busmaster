/**
 * @brief Definition of CFormatMsgJ1939 class
 * @author Anish kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CFormatMsgJ1939 class
 */

#pragma once

#include "GUI_FormatMsgCommon.h"
#include "include/BaseDefs.h"
#include "DataTypes/J1939_DataTypes.h"

class CFormatMsgJ1939 : public CFormatMsgCommon
{
private:
    void vFormatTime(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,  __int64 nDeltime,
                     PSFORMATTEDATA_J1939 psJ1939FData);
    void vFormatDataAndId(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                          PSFORMATTEDATA_J1939 psJ1939FData);
    USHORT usProcessCurrErrorEntry(SERROR_INFO& sErrorInfo);
    char* vFormatCurrErrorEntry(USHORT usErrorID);
public:
    CFormatMsgJ1939(void);
    ~CFormatMsgJ1939(void);
    void vFormatJ1939DataMsg(PSTJ1939_MSG psJ1939BData, __int64 nDeltime,
                             SFORMATTEDATA_J1939* CurrDataJ1939,
                             BYTE bExprnFlag_Log);
};
