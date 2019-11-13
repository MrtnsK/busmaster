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
 * \file      TSEditorGUI_ChildFrame.h
 * \author    Venkatanarayana Makam
 * \author    GT-Derka
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TSEditorGUI_PropertyView.h"
#include "TestSetupEditorLib/TestSetupEntity.h"
#include "TestSetupEditorLib/DataBaseMsgList.h"
#include "TreeViewEx.h"
//#include "FrameProcessor/LogObjectCAN.h" /* derka */
#include "ListCtrlEx.h"

// CTSEditorChildFrame frame with splitter
#define szFilter    "TestSetup Files (*.xml)|*.xml||"
class CTSEditorChildFrame : public CFrameWndEx
{
    CBaseEntityTA* m_pCurrentEntity;
    DWORD m_dwCurrentEntityID;
    HTREEITEM m_hCurrentTreeItem;
    CTestSetupEntity m_ouTSEntity;
    CString m_omCurrentTSFile;
    BOOL    m_bFileSaved;
    //Cut-copy-paste
    CBaseEntityTA* m_podCopyEntity;
    //Backup
    CSendEntity m_omSendEntity;
    CVerifyEntity m_ouVerifyEntity;
    BOOL m_bQueryConfirm;
    HMENU m_pMainMenu;
    WINDOWPLACEMENT m_sTSDefPlacement;
	CMFCMenuBar mMenuBar;
public:
    BOOL m_bModified;
    CTreeViewEx* m_odTreeView;
    CPropertyView* m_odPropertyView;
    HTREEITEM m_hParentTreeItem;
    BOOL m_bPasted;
    CMenu m_omMenu;
    BOOL m_bInit;
public: // create from serialization only
    CTSEditorChildFrame();
	virtual ~CTSEditorChildFrame();
	DECLARE_DYNCREATE(CTSEditorChildFrame)

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CSplitterWnd m_wndSplitter;

    CSplitterWnd m_omSplitterWnd;
    CImageList*  m_pomImageList;
    INT m_iNumberOfActiveChannels;
protected:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

    DECLARE_MESSAGE_MAP()
public:
    void vLoadTestSetupFile(CString omFilePath, BOOL bEmptyFile = FALSE);
    void vLoadTestSetupFileTemp(CString omFilePath, BOOL bEmptyFile = FALSE);
    void vSetModifiedFlag(BOOL isModified);
    void vSetFileSavedFlag(BOOL isModified);
    void vListCtrlItemChanged(LPNMLISTVIEW pNMLV);
    void vSetNumberOfActiveChannels(INT number);
    UINT unRepisitonEntry(DWORD dwRepositionItemID, DWORD dwInsertAfterItemID, DWORD dwParentID);
    INT nCancelCurrentChanges();
    INT nConfirmCurrentChanges();
    eTYPE_ENTITY eGetCurrentEntityType();
    INT nAddNewEntity(DWORD dwId, eTYPE_ENTITY eEntityType);
    INT nChangeEntityTitle(CBaseEntityTA* pEntity, CString& omstrName);
    INT nDeleteItem(DWORD dwId, DWORD dwParentId);
    HRESULT GetConfigurationData(BYTE*& pDesBuffer, UINT& nBuffSize);
    bool GetConfigurationData(xmlNodePtr& pxmlNodePtr);
    HRESULT SetConfigurationData(BYTE* pSrcBuffer, UINT unBuffSize);
    HRESULT SetConfigurationData(xmlNodePtr pXmlNode);
private:
    INT nPromptForSaveFile();
    void vSetCurrentFile(CString& omNewFilePath);
    INT nUpdateTreeView();
    INT nDisplayEntity(DWORD dwEntityID);
    INT nDisplayEntity(CBaseEntityTA* pEntity);
    void vInitialise();
    void vUpdateMessages();
    void vShowHelpInfo(eTYPE_ENTITY eEntityType);

    void parseSendEntity(CBaseEntityTA* pEntity, HTREEITEM hTCTreeitem);
    void parseVerifyEntity(CBaseEntityTA* pEntity, HTREEITEM hTCTreeitem);
    void parseWaitEntity(CBaseEntityTA* pEntity, HTREEITEM hTCTreeitem);
    void parseReplayEntity(CBaseEntityTA* pEntity, HTREEITEM hTCTreeitem);
    void parseTestCaseEntiy(CBaseEntityTA* pEntity, HTREEITEM hSubParent);

    void vCreateChannelDropdown(CListCtrlEx& omTempListCtrl, BYTE byChannelnumber /*CSend_MessageData* pSubEntity*/, INT iNumberOfActiveChannels, INT iTestCaseNumber);

    void vDisplayHeaderInfo(INT /*nTestSetupIndex*/);
    void vDisplaySendInfo(CBaseEntityTA* pEntity);
    void vDisplayVerifyInfo(CBaseEntityTA* pEntity, int nVerifyRowIndex = -1);
    void vDisplayWaitInfo(CBaseEntityTA* pEntity);
    void vDisplayReplayInfo(CBaseEntityTA* pEntity);
    void vDisplaySendMessageInfo(CBaseEntityTA* pEntity);
    void vDisplayVerifyMessageInfo(CBaseEntityTA* pEntity);
    void vDisplayTestcaseInfo(CBaseEntityTA* pEntity);
    void vDisplayVerifyResponseInfo(CBaseEntityTA* pEntity);

    void vSaveHeaderInfo(INT /*nTestSetupIndex*/);
    void vSaveTestcaseInfo(CBaseEntityTA* pEntity);
    void vSaveSendInfo(CBaseEntityTA* pEntity);
    void vSaveVerifyInfo(CBaseEntityTA* pEntity);
    void vSaveSendMessageInfo(CBaseEntityTA* pEntity);
    void vSaveVerifyMessageInfo(CBaseEntityTA* pEntity);
    void vSaveWaitInfo(CBaseEntityTA* pEntity);
    void vSaveReplayInfo(CBaseEntityTA* pEntity);
    void vSaveVerfiyReponseInfo(CBaseEntityTA* m_pCurrentEntity);

    void vHandleSendEntity(LPNMLISTVIEW pNMLV);
    void vHandleTestSetup(LPNMLISTVIEW pNMLV);
    void vHandleVerifyResponseEntity(LPNMLISTVIEW pNMLV);
    void vHandleVerifyEntity(LPNMLISTVIEW pNMLV);

    BOOL isParentChild(eTYPE_ENTITY eParent, eTYPE_ENTITY eChild);
    BOOL bEnablePase();
    BOOL bEnableCopy();

    void vCopyTreeItem(CBaseEntityTA** podCopyEntity, CBaseEntityTA* pCurrentEntity);
    void vDisplaySignalInfo(CString& omStrMsg);
    void vSetDefaultWndPlacement(void);

public:
    afx_msg void OnDestroy(void);
    //    afx_msg void OnFileNew(void);
    afx_msg void OnFileOpen(void);
    afx_msg void OnHelpAbouttestsetupeditor(void);
    afx_msg LRESULT OnSelectionChanged(WPARAM pNMTreeView, LPARAM);
    afx_msg LRESULT OnSelectionChanging(WPARAM pNMTreeView, LPARAM);
    afx_msg void OnFileSave(void);
    afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
    afx_msg void OnFileNew(void);
    afx_msg void OnFileSaveas(void);
    afx_msg void OnFileClose(void);
    afx_msg void OnFileExit(void);
    afx_msg void OnUpdateFileSaveas(CCmdUI* pCmdUI);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
public:
    afx_msg void OnEditCopy(void);
    afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
    afx_msg void OnEditPaste(void);
    afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
    afx_msg void OnEditCut(void);
    afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
    afx_msg void OnDisplayReset(void);
    afx_msg void OnDisplaySettings(void);
    afx_msg void OnFileValidate(void);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// Generated message map functions
protected:
	
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
    afx_msg void OnHelpTesteditorhelp();
    afx_msg void OnClose();

    void SetTSEditorMenu();
};
