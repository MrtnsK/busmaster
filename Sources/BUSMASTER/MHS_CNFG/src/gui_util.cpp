/***************************************************************************
                          gui_util.c  -  description
                             -------------------
    begin             : 30.10.2011
    copyright         : Copyright (c) 2011, Robert Bosch Engineering and 
						Business Solutions. All rights reserved.
    author            : Klaus Demlehner, klaus@mhs-elektronik.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software, you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation.             *
 *                                                                         *
 ***************************************************************************/

/**
    Library to talk to Tiny-CAN devices. You find the latest versions at
       http://www.tiny-can.com/
**/

#include <windows.h>
#include <basetyps.h>
#include <commctrl.h>
#include <windowsx.h>
#include <stdio.h>
#include <stddef.h>
#include <tchar.h>
#include "global.h"
#include "util.h"
#include "gui_util.h"



#define HexFormatTabSize 8
static const char *HexFormatTab[] = {"%02X", "%03X", "%04X", "%08X",
                                     "0x%02X", "0x%03X", "0x%04X", "0x%08X"};

void __cdecl ShowErrorMessage(const char *title, const char *text, ...)
{
va_list argptr;
char out[512];

va_start(argptr, text);
_vstprintf(out, text, argptr);
va_end(argptr);
MessageBox(0, out, title, MB_ICONEXCLAMATION | MB_OK);
}


char *GetWidgetTextDup(HWND ctrl_wnd)
{
char *str;
DWORD len;

str = 0;
if ((len = SendMessage(ctrl_wnd, WM_GETTEXTLENGTH, 0, 0)) <= 0)
  return(0);
len++;
str = (char *)mhs_malloc(len);
if (!str)
  return(0);
if (SendMessage(ctrl_wnd, WM_GETTEXT, (WPARAM)len, (LPARAM)str) <= 0)
  {
  mhs_free(str);
  return(0);
  }
return(str);
}


void GetDlgItemTextCpy(char *str, HWND hdlg, WORD id, int max)
{
HWND ctrl_wnd;
DWORD len;

if (!str)
  return;
ctrl_wnd = GetDlgItem(hdlg, id);
str[0] = '\0';
if ((len = SendMessage(ctrl_wnd, WM_GETTEXTLENGTH, 0, 0)) <= 0)
  return;
len++;
if (len > (DWORD)max)
  return;
if (SendMessage(ctrl_wnd, WM_GETTEXT, (WPARAM)len, (LPARAM)str) <= 0)
  str[0] = '\0';
}



char *GetDlgItemTextDup(HWND hdlg, WORD id)
{
return(GetWidgetTextDup(GetDlgItem(hdlg, id)));
}


uint32_t GetTextWidgetAsLong(HWND ctrl_wnd, int base)
{
uint32_t res;
char *str, *endptr;

if (!(str = GetWidgetTextDup(ctrl_wnd)))
  return(0);
res = strtoul(str, (char**)&endptr, base);
save_free(str);
return(res);
}


double GetTextWidgetAsDouble(HWND ctrl_wnd)
{
double res;
char *str;

if (!(str = GetWidgetTextDup(ctrl_wnd)))
  return(0);
res = atof(str);
save_free(str);
return(res);
}


void SetDlgItemHex(HWND hdlg, WORD id, uint32_t format_id, uint32_t value)
{
char puf[10];

if (format_id > HexFormatTabSize)
  format_id = HexFormatTabSize-1;
wsprintf(puf, HexFormatTab[format_id], value);
SetDlgItemText(hdlg, id, puf);
}


uint32_t GetDlgItemHex(HWND hdlg, WORD id)
{
uint32_t value;
char text[12];

if (0 == GetDlgItemText(hdlg, id, text, 12))
  return(0);

sscanf(text, "%x", &value);
return(value);
}


/*-----------------------------------------------------------------------------

FUNCTION: FillComboBox(HWND, char **, DWORD *, WORD, DWORD)

PURPOSE: Populates dialog controls with proper strings

PARAMETERS:
    hCtrlWnd         - window handle of control being filled
    szString         - string table contains strings to fill control with
    npTable          - table of values corresponding to strings
    wTableLen        - length of the string table
    dwCurrentSetting - initialz combo box selection

COMMENTS: This function originally found in the Win32 COMM sample
	  Written by BryanW.  Modified for Win32 MTTTY Sample.

HISTORY:   Date:      Author:     Comment:
	   10/27/95   AllenD      Modified for MTTTY

-----------------------------------------------------------------------------*/
int FillComboBox(HWND hdlg, WORD id, const char **str_tab,
         const DWORD *data_tab, WORD tab_len, DWORD value)
{
HWND ctrl_wnd;
WORD cnt, pos;
int flag = 0;

ctrl_wnd = GetDlgItem(hdlg, id);
for (cnt = 0; cnt < tab_len; cnt++)
  {
  pos = LOWORD(SendMessage(ctrl_wnd, CB_ADDSTRING, 0, (LPARAM)(LPSTR)str_tab[cnt]));
  // use item data to store the actual table value
  SendMessage(ctrl_wnd, CB_SETITEMDATA, (WPARAM)pos,
        (LPARAM) *(data_tab + cnt));
  // if this is our current setting, select it
  if (*(data_tab + cnt) == value)
  {
	  SendMessage(ctrl_wnd, CB_SETCURSEL, (WPARAM)pos, 0L);
	 flag = 1;
  }
}
return flag;
}


/*-----------------------------------------------------------------------------

FUNCTION: SetComboBox(HWND, WORD, DWORD)

PURPOSE: Selects an entry from a dialog combobox

PARAMETERS:
    hCtrlWnd     - windows handle of control
    dwNewSetting - new item to base selection on

HISTORY:   Date:      Author:     Comment:
	   11/20/95   AllenD      Wrote it

-----------------------------------------------------------------------------*/
void SetComboBoxWnd(HWND ctrl_wnd, DWORD value)
{
DWORD cnt, size, item_data;

size = SendMessage(ctrl_wnd, CB_GETCOUNT, 0, 0);
for (cnt = 0; cnt < size; cnt++)
  {
  item_data = SendMessage(ctrl_wnd, CB_GETITEMDATA, (WPARAM)cnt, 0L);
  if (item_data == value)
    {
    SendMessage(ctrl_wnd, CB_SETCURSEL, (WPARAM)cnt, 0L);
    break;
    }
  }
}


void SetComboBox(HWND hdlg, WORD id, DWORD value)
{
HWND ctrl_wnd;

ctrl_wnd = GetDlgItem(hdlg, id);
SetComboBoxWnd(ctrl_wnd, value);
}


DWORD GetComboBoxWnd(HWND ctrl_wnd)
{
DWORD idx;

idx = SendMessage(ctrl_wnd, CB_GETCURSEL, 0, 0);
return(SendMessage(ctrl_wnd, CB_GETITEMDATA, (WPARAM)idx, 0L));
}


DWORD GetComboBox(HWND hdlg, WORD id)
{
HWND ctrl_wnd;
DWORD idx;

ctrl_wnd = GetDlgItem(hdlg, id);
idx = SendMessage(ctrl_wnd, CB_GETCURSEL, 0, 0);
return(SendMessage(ctrl_wnd, CB_GETITEMDATA, (WPARAM)idx, 0L));
}


void SetCheckButton(HWND ctrl_wnd, WORD check)
{
(void)SendMessage(ctrl_wnd, BM_SETCHECK, check, 0L);
}


WORD GetCheckButton(HWND ctrl_wnd)
{
return((WORD)SendMessage(ctrl_wnd, BM_GETCHECK, 0, 0L));
}
