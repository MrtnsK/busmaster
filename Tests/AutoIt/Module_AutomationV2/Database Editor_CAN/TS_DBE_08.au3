;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_DBE_08
; TestCases			:		Database Usage in Busmaster
; Test Data			:
; Test Setup		:		1. Continue from above test case TS_DBE_07.
;~ 							2. Click 'Yes' in message box.
;~ 							3. Click 'Yes' in message box.

; Expected Results  : 		1. After step2, a message box 'Dissociation will clear graph element list of all buses.
;~ 								Do you want to clear the list?' will be displayed.
;~ 								After step3, the database entry gets cleared from the list in 'Dissociate Database' dialog.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_DBE_08.au3****"&@CRLF)
$assocDBCount=1
_launchApp()

WinActivate($WIN_BUSMASTER,3)
if winexists($WIN_BUSMASTER) then
	_loadConfig("cfxDBE_08")
	_DissociateCANDB(0)																		; Dissociate DB

	;WinWaitActive("",$disDBtxt1)
	WinWaitActive($WIN_Dissociate_CANDB)

	;if winexists("",$disDBtxt1) Then
		;consolewrite("Dissociation will clear Signal Watch List window exists" & @CRLF)
		;WinActivate("",$disDBtxt1)
		;send("{ENTER}",1)				  													; Click on 'yes' button in "Dissociation will clear Signal Watch List." warning message
		;sleep(2000)
		;autoitsetoption("WinTextMatchMode",2)
		;WinWaitActive("",$disDBtxt1)
		;ControlClick("",$disDBtxt1,"[CLASS:Button; INSTANCE:1]")
		;Send("{ENTER}")
		;ControlSend("","Are you sure you want to dissociate the file(s)?",1,"",1)
		;ControlClick($WIN_BUSMASTER,"",1)
		;Send("{ENTER}")
	;EndIf
	$disDBlbHWD=ControlGetHandle($WIN_Dissociate_CANDB,"",$LSTB_DBFile_Dis_CANDB)			; Fetch the handle of dissociate Db list box
	$assocDBCount=_GUICtrlListBox_GetCount($disDBlbHWD)										; Fetch the count of items in dissociate DB window
	ConsoleWrite("$assocDBCount : " & $assocDBCount &@CRLF)
	ControlClick($WIN_Dissociate_CANDB,"",$BTN_Cancel_Dis_CANDB)							; Click on 'Cancel' button in Dissociate database window
EndIf
if $assocDBCount=0 Then
	_WriteResult("Pass","TS_DBE_08")
Else
	_WriteResult("Fail","TS_DBE_08")
EndIf


;$isAppNotRes=_CloseApp()																	; Close the app

;if $isAppNotRes=1 Then
	;_WriteResult("Warning","TS_DBE_08")
;EndIf

ConsoleWrite("****End : TS_DBE_08.au3****"&@CRLF)
ConsoleWrite(@CRLF)