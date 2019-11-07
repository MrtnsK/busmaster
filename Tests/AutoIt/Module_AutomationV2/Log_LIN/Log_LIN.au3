#cs ----------------------------------------------------------------------------

 AutoIt Version: 3.3.6.1
 Author:         Srinibas

 Script Function:
	Parent File containing 'LIN -Log' module test scripts.

#ce ----------------------------------------------------------------------------

#include <IncludeFiles.au3>

_RunMe()

; Create the HTML table to save the result
_StartoHTMLTable("LIN_Log  Test Report")

;Run all the LIN - Log module scripts

; --------------------------------------------------
; LIN DB P1 Case's
;If(@OSVersion = "WIN_7") Then



#include "TS_LIN_Log_01.au3"      		; Pass-Done
;#include "TS_LIN_Log_02.au3"      	; FAIL
#include "TS_LIN_Log_03.au3"      		; Pass-Done

;#include "TS_LIN_Log_04.au3"      	; FAIL (Rx)
;#include "TS_LIN_Log_05.au3"      	; FAIL (Rx)

#include "TS_LIN_Log_06.au3"      		; Pass-Done
#include "TS_LIN_Log_07.au3"      		; Pass-Done
#include "TS_LIN_Log_08.au3"      		; Pass-Done
#include "TS_LIN_Log_09.au3"      		; Pass-Done

;~ ;#include "TS_LIN_Log_10.au3"      	; FAIL (Rx)
;~ ;#include "TS_LIN_Log_11.au3"      	; FAIL (Rx)
#include "TS_LIN_Log_12.au3"      		; Pass-Done
#include "TS_LIN_Log_13.au3"      		; Pass
#include "TS_LIN_Log_14.au3"      		; Pass
#include "TS_LIN_Log_15.au3"      		; Pass
;#include "TS_LIN_Log_16.au3"      		; Pass- CKECK CHECKSUM
;#include "TS_LIN_Log_17.au3"      		; Cannot simulate this usecase
;#include "TS_LIN_Log_18.au3"      		; pass (check for filtering), slave not working
;#include "TS_LIN_Log_19.au3"      		; Pass
;#include "TS_LIN_Log_20.au3"      		; FAIL (Rx)- check
;#include "TS_LIN_Log_21.au3"      		; FAIL (Rx)
#include "TS_LIN_Log_22.au3"      		; Pass
#include "TS_LIN_Log_24.au3"      		; Pass
#include "TS_LIN_Log_25.au3"      		; Pass
;#include "TS_LIN_Log_26.au3"      		; FAIL (Rx)
;#include "TS_LIN_Log_27.au3"      		; FAIL (Rx)
#include "TS_LIN_Log_28.au3"      		; Pass
#include "TS_LIN_Log_29.au3"      		; Pass
#include "TS_LIN_Log_30.au3"      		; Pass
#include "TS_LIN_Log_31.au3"      		; Pass
;#include "TS_LIN_Log_32.au3"      		; Pass- CKECK CHECKSUM
;#include "TS_LIN_Log_33.au3"      		; Cannot simulate this usecase
#include "TS_LIN_Log_34.au3"      		; Pass- not closing
;#include "TS_LIN_Log_35.au3"      		; not closing


; LIN DB P2 Case's
#include "TS_LIN_Log_40.au3"      		; Pass
;#include "TS_LIN_Log_41.au3"      		; Covered in TS_LIN_Log_40
#include "TS_LIN_Log_42.au3"            ; Pass
;#include "TS_LIN_Log_43.au3"      		; Not Automatable
#include "TS_LIN_Log_44.au3"            ; Pass
#include "TS_LIN_Log_45.au3"      		; Pass
;#include "TS_LIN_Log_46.au3"           ; NA
;#include "TS_LIN_Log_47.au3"           ; NA
;#include "TS_LIN_Log_48.au3"           ; Not Automatable
#include "TS_LIN_Log_49.au3"      		; Pass
;#include "TS_LIN_Log_50.au3"           ; Covered in TS_LIN_Log_49
;#include "TS_LIN_Log_51.au3"           ; Covered in TS_LIN_Log_49
;#include "TS_LIN_Log_52.au3"           ; Covered in TS_LIN_Log_49
#include "TS_LIN_Log_53.au3"  		    ; Pass
#include "TS_LIN_Log_54.au3"      		; Pass
;#include "TS_LIN_Log_55.au3"           ; Not automatable


;EndIf












_EndoHTMLTable()

_SaveHTMLReport()

_IEQuit($oIE)

_ExcelBookClose($oExcel, 1, 0)