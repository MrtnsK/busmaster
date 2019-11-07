;=================================================================== Test Cases/Test Data ========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_TX_LIN_02
; TestCases			:		Add Non DB frames to configure transmission messages - Master and Slave
; Test Data			:
; Test Setup		:		1. Import LDF file
;							2. Select LIN->Transmit->Configure.
;							3. Select frames for transmission from dropdown Box provided in Tx frame List.(Add Message).
;							4. Connect

; Expected Results  : 		1. Select LIN->Transmit->Configure.
;							2. Add Master and slave frames with id 0x10 from Tx frame lIst using Add message drop down.
;							3. Close Tx window
;							4. Connect

;							After step2, frames should get added in the Tx window.
;							After step4, Master frame should be transmitted and Slave should received.

;==========================================================================Test Procedure =========================================================================
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_TX_LIN_02****"&@CRLF)

Local $Timedelay=0,$VerifyMsg=1,$a=0,$FirstMsg=0

_launchApp()

WinActivate($WIN_BUSMASTER)
Local $Time_cyclic=0,$a=0
if winexists($WIN_BUSMASTER) then


	_createConfig("TS_TX_LIN_02")
	;_SetViewToolBarLIN()																	 ; Select LIN Tool Bar to view.
	sleep(1000)
	;_linDriverSelectionXml()																 ; Select LIN driver selection from xml configuration File.
	;_linDriverSelection()
	_linDriverSelection()																 ; Select LIN driver selection from xml configuration File.
	sleep(1000)
	sleep(1000)
	_openLINClusterConfig()
	WinWaitActive($WIN_ClustrConfigLIN)
	if winexists($WIN_ClustrConfigLIN) then
		sleep(1000)
		_importLDF("TestLIN2_1.ldf")
		sleep(1000)
		ControlCommand($WIN_ClustrConfigLIN, "",$chckBoxEnableMaster, "Check", "") 					; Select Check box for enable Master mode.
		Sleep(1000)
		ControlClick($WIN_ClusterConfiguration,"",$BTN_OK_ClusterConfig)
		sleep(1000)
		_TxMsgMenuLIN()
		_ConfigTXWinDetails(0,"ndb",10,1,"","Master","",1000,"")									; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetition,Key
		_ConfigTXWinDetails(1,"ndb",10,1,"","Slave","",1000,"")										; $Rownum,$HSCaseNo("db","ndb"),$messageid/Index(For dbMessage),chnl,datalength,Messageype,RTR(Yes/No),Repetit
		_CloseTxWindowArg("LIN")																	; Close Tx window
		_ConnectDisconnect_LIN()
		sleep(1000)
		_ConnectDisconnect_LIN()
		_CloseTxWindowArg("LIN")																	; Close Tx window
	EndIf


	$rCount=_GetLINMsgWinItemCount()																;count no. of items in msgwindow
	$CountCol=_GetLINMsgWinColCount()																;count no. of columns in msgwindow
	ConsoleWrite("$rCount="&$rCount&@CRLF)
	ConsoleWrite("$Count1="&$CountCol&@CRLF)


	Local $Msg0 = ObjCreate("System.Collections.ArrayList") 										; create object
	Local $Msg1 = ObjCreate("System.Collections.ArrayList") 										; create object

	if $rCount = 1 Then
		$Msg0=_GetMsgWinInfo($LVC_CID_LINMsgWin,0,10)												; Fetch the first row data in the msg window
		sleep(2000)
		if $Msg0.Item(0) <>" " and $Msg0.Item(1)="0x10" and $Msg0.Item(2)="LIN Message" and $Msg0.Item(3)="Tx" and $Msg0.Item(4)=1 and $Msg0.Item(5)=8 and $Msg0.Item(6)= 0x010 and $Msg0.Item(7)="FF FF FF FF FF FF FF FF" and $Msg0.Item(8)="0xAF (""Enhanced"")" Then			; Compare the Direction, Channel and Msg ID of the first row
			$FirstMsg=1

		Endif
	EndIf


EndIf

	ConsoleWrite("$FirstMsg = "& $FirstMsg &@CRLF)

	If($FirstMsg=1 ) Then
		_WriteResult("Pass","TS_TX_LIN_02")
	Else
		_WriteResult("Fail","TS_TX_LIN_02")
	EndIf


	$isAppNotRes=_CloseApp()																		; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_TX_LIN_02")
EndIf

ConsoleWrite("****End : TS_TX_LIN_02.au3****"&@CRLF)
ConsoleWrite(@CRLF)