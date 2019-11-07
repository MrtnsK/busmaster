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
 * \file      TSEditorGUI_PropertyView.cpp
 * \author    Venkatanarayana makam
 * \author    GT-Derka
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "stdafx.h"
#include "TSEditorGUI_ChildFrame.h"
#include "TSEditorGUI_PropertyView.h"
#include "TSEditorGUI_Definitions.h"
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

#define def_HEIGHT_PROPERTYVIEW     0.65
#define def_WINDOW_GAP              18          //Looks Good
#define def_WINDOW_SEMIGAP          9
#define def_WIDTH_COL_CATEGORY      0.2
#define def_WIDTH_COL_VALUE         0.62    // new dimensions, to get a nicer look of channel-column
#define def_WIDTH_COL_CHANNEL       0.13    // solves issue #711, 1st bullet point
// CPropertyView

IMPLEMENT_DYNCREATE(CPropertyView, CFormView)

/******************************************************************************
Function Name  :  CPropertyView
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
CPropertyView::CPropertyView() : CFormView(CPropertyView::IDD)
    , m_omstrHelpString("")
{
    m_bSizeAvailable = FALSE;
}

/******************************************************************************
Function Name  :  ~CPropertyView
Input(s)       :
Output         :  -
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
CPropertyView::~CPropertyView()
{
}

/******************************************************************************
Function Name  :  DoDataExchange
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
void CPropertyView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROPERTYLIST, m_omPropertyList);
    DDX_Text(pDX, IDC_EDITHELP, m_omstrHelpString);
}
HRESULT CPropertyView::onMsg(LPARAM,WPARAM)
{
    MessageBox("hI");
    return S_OK;
}
BEGIN_MESSAGE_MAP(CPropertyView, CFormView)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDC_CONFIRM, OnBnClickedConfirm)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_PROPERTYLIST, OnLvnItemchanged)
END_MESSAGE_MAP()


// CPropertyView diagnostics

#ifdef _DEBUG
void CPropertyView::AssertValid() const
{
    CFormView::AssertValid();
}

void CPropertyView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG


// CPropertyView message handlers

/******************************************************************************
Function Name  :  OnInitialUpdate
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
void CPropertyView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    InitCommonControls();

    //Window is Constructed Now and Size its size is available
    m_bSizeAvailable=TRUE;
    m_omPropertyList.SetExtendedStyle(m_omPropertyList.GetExtendedStyle() | LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
    m_omPropertyList.InsertColumn(def_COLUMN_CATEGORY, _(defFIRSTCOLUMN_NAME));
    m_omPropertyList.InsertColumn(def_COLUMN_VALUE, _(defSECONDCOLUMN_NAME));
    //Just To increase the List Item Height
    m_omImageList.Create(def_WIDTH_LISTITEM, def_HEIGHT_LISTITEM, ILC_COLOR24, 4, 1);
    m_omPropertyList.SetImageList(&m_omImageList, LVSIL_SMALL);
	CRect PropertyRect;
	this->GetClientRect(&PropertyRect);                                         // Get Window-Size to...
	ResizeWindow(0, PropertyRect.Width(), PropertyRect.Height());
}

/******************************************************************************
Function Name  :  vSetChannelColumn
Input(s)       :  enum Visibility visibility -  SHOW to show channel-column
                                                HIDE to hide channel-column
Output         :  void
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  GT-Derka
Date Created   :  06/05/2014
Modifications  :
******************************************************************************/
void CPropertyView::vSetChannelColumn(Visibility visibility)
{
    int iNumberOfColumns = m_omPropertyList.GetHeaderCtrl()->GetItemCount();    // Get Number of existing columns
    if((visibility == SHOW) && (iNumberOfColumns == def_COLUMN_CHANNEL))        // "... == def_COLUMN_CHANNEL" => "Channel" does not exist yet
    {
        CRect PropertyRect;
        m_omPropertyList.InsertColumn(def_COLUMN_CHANNEL, _(defTHIRDCOLUMN_NAME));  // Create "Channel"-column
        this->GetClientRect(&PropertyRect);                                         // Get Window-Size to...
        this->OnSize(0, PropertyRect.Width(), PropertyRect.Height());               // ...call OnSize to fit column-widths
    }
    else if((visibility == HIDE) && (iNumberOfColumns == def_COLUMN_CHANNEL+1))     // "Channel" does exist -> allowed to delete
    {
        m_omPropertyList.DeleteColumn(def_COLUMN_CHANNEL);                          // Delete "Channel"
    }
}


/******************************************************************************
Function Name  :  OnSize
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam, GT-Derka
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
void CPropertyView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);
    if(m_bSizeAvailable == TRUE)
    {
		ResizeWindow(nType, cx, cy);
    }
}
void CPropertyView::ResizeWindow(UINT nType, int cx, int cy)
    {
        CRect PropertyRect;
        CRect ConfirmButtonRect, CancelButtonRect;
        CRect StaticHelpRect, EditHelpRect;
        GetDlgItem(IDC_PROPERTYLIST)->GetClientRect(&PropertyRect);
        GetDlgItem(IDC_CONFIRM)->GetClientRect(&ConfirmButtonRect);
        GetDlgItem(IDC_CANCEL)->GetClientRect(&CancelButtonRect);
        GetDlgItem(IDC_EDITHELP)->GetClientRect(&EditHelpRect);
        GetDlgItem(IDC_STATICHELP)->GetClientRect(&StaticHelpRect);

        int PropertyListWidth = (int)(def_HEIGHT_PROPERTYVIEW * cy);
        PropertyRect.top = def_WINDOW_GAP;
        PropertyRect.left = def_WINDOW_GAP;
        PropertyRect.bottom = PropertyListWidth - def_WINDOW_GAP;
        PropertyRect.right = cx -def_WINDOW_GAP;
        GetDlgItem(IDC_PROPERTYLIST)->MoveWindow(PropertyRect);

        m_omPropertyList.SetColumnWidth(def_COLUMN_CATEGORY, (INT)(def_WIDTH_COL_CATEGORY*PropertyRect.Width()));
        m_omPropertyList.SetColumnWidth(def_COLUMN_VALUE,  (INT)(def_WIDTH_COL_VALUE*PropertyRect.Width()));
        m_omPropertyList.SetColumnWidth(def_COLUMN_CHANNEL,  (INT)(def_WIDTH_COL_CHANNEL*PropertyRect.Width()));

        int nHeight = ConfirmButtonRect.Height();
        int nWidth = ConfirmButtonRect.Width();
        ConfirmButtonRect.top = PropertyRect.bottom + def_WINDOW_SEMIGAP;
        ConfirmButtonRect.bottom = ConfirmButtonRect.top + nHeight;
        ConfirmButtonRect.left = (int)(def_WIDTH_COL_VALUE*cx);
        ConfirmButtonRect.right = ConfirmButtonRect.left + nWidth;
        GetDlgItem(IDC_CONFIRM)->MoveWindow(ConfirmButtonRect);

        CancelButtonRect.top = ConfirmButtonRect.top;
        CancelButtonRect.bottom = ConfirmButtonRect.bottom;
        CancelButtonRect.left = ConfirmButtonRect.left + nWidth + def_WINDOW_SEMIGAP;
        CancelButtonRect.right = CancelButtonRect.left + nWidth;
        GetDlgItem(IDC_CANCEL)->MoveWindow(CancelButtonRect);

        PropertyRect.top = def_WINDOW_SEMIGAP/2;
        PropertyRect.left = def_WINDOW_SEMIGAP;
        PropertyRect.bottom = PropertyListWidth  + ConfirmButtonRect.Height() ;
        PropertyRect.right = cx - def_WINDOW_SEMIGAP ;
        GetDlgItem(IDC_STATICDETAILS)->MoveWindow(PropertyRect);

        nHeight = cy - PropertyRect.bottom - def_WINDOW_GAP; //StaticHelpRect.Height();
        nWidth = StaticHelpRect.Width();
        StaticHelpRect.top = PropertyRect.bottom + def_WINDOW_SEMIGAP;
        StaticHelpRect.bottom = StaticHelpRect.top + nHeight;
        StaticHelpRect.left = PropertyRect.left;
        StaticHelpRect.right = PropertyRect.right;
        GetDlgItem(IDC_STATICHELP)->MoveWindow(StaticHelpRect);

        EditHelpRect.top = StaticHelpRect.top + def_WINDOW_GAP;
        EditHelpRect.bottom = StaticHelpRect.bottom - def_WINDOW_SEMIGAP;
        EditHelpRect.left = StaticHelpRect.left + def_WINDOW_SEMIGAP;
        EditHelpRect.right = StaticHelpRect.right - def_WINDOW_SEMIGAP;
        GetDlgItem(IDC_EDITHELP)->MoveWindow(EditHelpRect);
}

/******************************************************************************
Function Name  :  OnBnClickedCancel
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
void CPropertyView::OnBnClickedCancel()
{
    CTSEditorChildFrame* pEditor = GetEditorWindow();

    if(nullptr != pEditor)
    {
        pEditor->nCancelCurrentChanges();
    }
}


/******************************************************************************
Function Name  :  OnBnClickedConfirm
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
void CPropertyView::OnBnClickedConfirm()
{
    CTSEditorChildFrame* pEditor = GetEditorWindow();

    if(nullptr != pEditor)
    {
        pEditor->OnFileSave();
    }
    //GetEditorWindow()->nConfirmCurrentChanges();
}

/******************************************************************************
Function Name  :  GetEditorWindow
Input(s)       :
Output         :  CTSEditorChildFrame*
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
CTSEditorChildFrame* CPropertyView::GetEditorWindow()
{
    CWnd* wndParent;
    if((wndParent = GetParent())!= nullptr)
    {
        wndParent = wndParent->GetParent();
        if(wndParent != nullptr)
        {
            return ((CTSEditorChildFrame*)wndParent);
        }
    }
    return nullptr;
}

/******************************************************************************
Function Name  :  OnLvnItemchanged
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
void CPropertyView::OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

    //If Text is Changed...
    if(pNMLV->uChanged&LVIF_TEXT)
    {
        CTSEditorChildFrame* pChildFrame = GetEditorWindow();
        if (pChildFrame != nullptr)
        {
            pChildFrame->vListCtrlItemChanged(pNMLV);
        }
    }
    *pResult = 0;
}

/******************************************************************************
Function Name  :  vShowHelpInfo
Input(s)       :
Output         :  void
Functionality  :
Member of      :  CPropertyView
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :
******************************************************************************/
void CPropertyView::vShowHelpInfo(CString& omHelpString)
{
    m_omstrHelpString = omHelpString;
    UpdateData(FALSE);
}
