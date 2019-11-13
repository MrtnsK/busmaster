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
 * @author Venkatanarayana Makam
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "afxwin.h"
#include "EditItem.h"

class CButtonItem;

class CBrowseEditItem : public CEditItem
{
public:
    CBrowseEditItem(int nItem, int nSubItem, CString& sContent, CButtonItem* pomButtonItem);
    ~CBrowseEditItem();

    CButtonItem* m_pomButton;
    BOOL m_bIsButtonValid;
    BOOL m_bKillFocus;

protected:
    afx_msg void OnKillFocus(CWnd* pNewWnd);

private:
    DECLARE_MESSAGE_MAP()
};

class CButtonItem : public CButton
{
public:
    CButtonItem(CString, CString);
    virtual ~CButtonItem(void);

    BOOL m_bKillFocus;
    CBrowseEditItem* m_pomEditItem;
    BOOL m_bIsEditValid;
    BOOL m_bButtonclicked;

    void vSetEditItem(CBrowseEditItem* pomEditItem);

    DECLARE_MESSAGE_MAP()

    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnBnClicked();

private:
    CString m_omStrDefExt;
    CString m_omStrFilter;
};
