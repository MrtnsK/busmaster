#pragma once

#include "UDS_Resource.h"


// Cuadro de di�logo de CUDS_SettingsWnd

class CUDS_SettingsWnd : public CDialog
{
    DECLARE_DYNAMIC(CUDS_SettingsWnd)

public:
    CUDS_SettingsWnd(CWnd* pParent = NULL);   // Constructor est�ndar
    virtual ~CUDS_SettingsWnd();

    // Datos del cuadro de di�logo
    enum { IDD = IDM_UDS };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

    DECLARE_MESSAGE_MAP()
};
