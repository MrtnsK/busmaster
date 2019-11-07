/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphConfiguration.h
  Description   :  Interface file for CGraphConfiguration class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphConfiguration.h_v  $
 *
 *    Rev 1.1   15 Apr 2011 19:43:30   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 *
 *    Rev 1.0   13 Dec 2010 22:00:34   CANMNTTM
 *
 *
 *    Rev 1.0   16 Aug 2010 21:20:32   rac2kor
 *

  Author(s)     :  Raja N
  Date Created  :  01/12/2004
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#pragma once

// For Graph Parameter Class definition
#include "GraphParameters.h"
#include "GraphList.h"

class CGraphConfiguration : public CDialog
{
    // Construction
public:
    // Local instance of View Style parameter
    CGraphParameters m_omGraphParams;
    // standard constructor
    CGraphConfiguration(CWnd* pParent = nullptr);

    CGraphList* m_pGraphList;
    // To have reference of Graph Control interface
    IDMGraphCtrl* m_pDMGraphCtrl;

    // Dialog Data
    //{{AFX_DATA(CGraphConfiguration)
    enum { IDD = IDD_DLG_GRAPH_WINDOW_CONF };
    CColorSelector   m_omAxisColor;
    CColorSelector   m_omFrameColor;
    CColorSelector   m_omGridColor;
    CColorSelector   m_omPlotColor;
    int     m_nYGridCount;
    int     m_nXGridCount;
    int     m_nUpdateRate;
    int     m_nBufferSize;
    int     m_omFrameType;
    //}}AFX_DATA
    // To set the values of the object in to UI control
    void vSetValues(CGraphParameters omParam);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGraphConfiguration)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CGraphConfiguration)
    afx_msg void OnSetDefault();
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    int m_nLineDisplay;
};
