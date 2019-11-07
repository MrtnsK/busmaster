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
 * \file      NodeDetailsDlg.h
 * \brief     Interface file for CNodeDetailsDlg class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CNodeDetailsDlg class
 */

#pragma once

#include "SimSysNodeInfo.h"     // For CSimSysNodeInfo class decl
#include "NodeSimEx_resource.h"
#include "Utility\radixedit.h"
#include "GlobalObj.h"
#include "SimSysManager.h"

class CNodeDetailsDlg : public CDialog
{
    // Construction
public:
    CNodeDetailsDlg(ETYPE_BUS eBus, CGlobalObj* pGlobalObj = nullptr, CSimSysManager* pSimSysManager = nullptr, PSNODEINFO pNode = nullptr, CWnd* pParent = nullptr);
    BOOL m_bIsNodeModified;  //Track the change

    // Dialog Data
    //{{AFX_DATA(CNodeDetailsDlg)
    enum { IDD = IDD_NODE_DEATILS };
    CString m_omStrDllPath;
    CString m_omStrNodeName;
    CString m_omStrNodeFilePath;
    CString m_omStrCFile;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CNodeDetailsDlg)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CNodeDetailsDlg)
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnBrowse();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    ETYPE_BUS m_eBus;
    PSNODEINFO m_psNodeStuctPtr;
    eNODE_FILE_TYPE m_eNodeFileType;
    void bUpdateNodeInfoFile();
    virtual bool bOpenfile();
    bool bCheckIfOldCppFile(CString& omText);
    CGlobalObj* m_pGlobalObj;
    CSimSysManager* m_pSimSysMgr;
public:
    bool        m_bEdit;
    CRadixEdit m_omPreferedAddress;
    CRadixEdit m_omEcuName;
};
