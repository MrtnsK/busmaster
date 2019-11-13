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
 * \file      Application/TraceWnd/NotificWnd.h
 * \brief     This file contain the definition CNotificWnd class. The user can
 * \author    Ravikumar Patil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition CNotificWnd class. The user can
 */


#if !defined(AFX_NOTIFICWND_H__E81A5585_2DDD_11D7_BFC8_00096B9231A9__INCLUDED_)
#define AFX_NOTIFICWND_H__E81A5585_2DDD_11D7_BFC8_00096B9231A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotificWnd.h : header file
//

//Definition of CNotificListbox class
#include "NotificListbox.h"
#include "Application\MDIChildBase.h"   // For Common MDI base class
//#include "Structs.h"
/////////////////////////////////////////////////////////////////////////////
// CNotificWnd
typedef struct tagNOTIFICWNDPARAMS
{
    //LOGPARAMS           m_sLogParams;
    WINDOWPLACEMENT     m_sWndPlacement;
    BYTE                m_bSetFlag_Disp;
    BYTE                m_bSetFlag_Log;
} NOTIFICWNDPARAMS, *PNOTIFICWNDPARAMS;

class CNotificWnd : public CMDIChildBase
{
    DECLARE_DYNCREATE(CNotificWnd)
public:
    CNotificWnd();           // public constructor used by dynamic creation
    virtual ~CNotificWnd(); //destructor
    // Attributes
public:
    //CStringArray m_omDispStrArray;

    // Operations
public:
    // Set the font of window
    BOOL bCreateNotificWindow(CMDIFrameWnd* pomParentWnd);
    // To display a string into trace window
    void vDisplayString(const CString& omStr);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNotificWnd)
protected:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CNotificWnd)
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnClose();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    //}}AFX_MSG
    //afx_msg void vAddString(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
private:
    CCriticalSection m_omCriticalSection;
    UINT_PTR m_unTimer;

    // Add string items to the list box of output window
    void vAddString(CString omStr);
    // Set the content of list box.
    void vSaveWinStatus(WINDOWPLACEMENT sWinCurrStatus);
    void vGetWinStatus(WINDOWPLACEMENT& sWinCurrStatus);
    VOID vSetWindowFont();
    CFont m_omNewFont;
    CSize m_omSizeMaxTxtExtent;
    CNotificListbox m_omListBox;
    NOTIFICWNDPARAMS m_sNotificWndParams;
    LRESULT DoConfigOperation(WPARAM WParam, LPARAM LParam);
    void vSaveNotificWndConfig();
    UINT unGetStoreNFBufferSize();
    void vLoadNotificWndConfig();
    void SaveNFDataIntoBuffer(BYTE* DesBuffer);
    void CopyNFDataFromBuffer(BYTE* SrcBuffer);
    BOOL bIsConfigChanged();

public:
    afx_msg void OnDestroy();
    afx_msg void OnTimer(UINT nIDEvent);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTIFICWND_H__051FC126_3176_11D6_BFEB_0010B599CE39__INCLUDED_)
