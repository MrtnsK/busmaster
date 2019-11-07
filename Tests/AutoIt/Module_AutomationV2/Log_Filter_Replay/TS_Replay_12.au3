;=================================================================== Test Cases/Test Data =========================================================================
; Critical (C)		:		Y
; TestCase No.		:		TS_Replay_12
; TestCases			:		Using the Filter Attributes
; Test Data			:
; Test Setup		:		1. Open 'Configure Filters' dialog by menu option 'Configure -> App Filters'.
;~ 							2. Make a filter with the following attributes:
;~ 							3. Choose 'ID' radio box in 'Filter Attributes' group box.
;~ 							4. Select an 'ID' from the combobox or manually enter it.
;~ 							5. Choose 'ID Type' as 'Std'.
;~ 							6. Choose 'Frame' as 'Non RTR'.
;~ 							7. Choose 'Direction' as 'Rx'.
;~ 							8. Choose 'Channel' as '1'.
;~ 							9. Add the filter by clicking on Add button and close the 'Configure Filters' dialog by clicking on 'OK'.
;~ 							10. Open 'Configure Replay dialog' and add a new replay file.
;~ 							11. Choose the confiured filter by opening 'Filter Selection dialog'.
;~ 							12. Now click on connect to get the 'Replay Window' and observe the message entries in it.

; Expected Results  : 		1. The chosen ID in step 4 along with filter attributes choosen should not be present in 'Replay Window'.
;==========================================================================Test Procedure =========================================================================

ConsoleWrite(@CRLF)
ConsoleWrite(@CRLF)
ConsoleWrite("****Start : TS_Replay_12.au3****"&@CRLF)


_launchApp()

Dim $FirstMsg[10],$SecondMsg[10],$ThirdMsg[10],$FourthMsg[10]

if winexists($WIN_BUSMASTER) then

	_loadConfig("cfxReplay_12")																	; Load Configuration

	_CANReplayMenu()																			; Open configure replay dialog

	_ADDReplayFile("Replay_12")																	; Add a replay file
	sleep(1000)

	ControlCommand($WIN_CANReplayConfig,"",$CHKB_Interactive_CANReplayConfig,"Check")			; Select Interactive replay

	_AddReplayFilters()																			; Add filter for replaying

	ControlClick($WIN_CANReplayConfig,"",$BTN_OK_CANReplayConfig)								; Click on 'OK' button

	_ReplayFilterMenu()																			; Select CAN->Replay->Enable Filters

	_DisableOverwriteMode()																		; Disable overwrite mode

;~ 	_EnableHex()																				; Enable Hex mode

	;_ConnectDisconnect()																		; Connect the tool
	_Connect_CAN_Menu()
	Sleep(1000)

	_CANReplayOptionsMenu($CANReplayGoMenu)														; Select "Go" from menu
	sleep(5000)

	_ConnectDisconnect()																		; Disconnect the tool
	sleep(1000)

	$MsgWinCount=_GetCANMsgWinItemCount()														; Fetch the Replay Msg window count
	consolewrite("$MsgWinCount :"&$MsgWinCount&@CRLF)

	$FirstMsg[4]=0
	$SecondMsg[4]=0
	$ThirdMsg[4]=0
	$FourthMsg[4]=0

	$MsgIDs=0
	$MsgDir=0

	$FirstMsg[2]=0
	$SecondMsg[2]=0
	$ThirdMsg[2]=0
	$FourthMsg[2]=0

	if $MsgWinCount=4 Then

		$FirstMsg=_GetMsgWinCANInfo(0)															; Fetch the msg win first row info
		$SecondMsg=_GetMsgWinCANInfo(1)															; Fetch the msg win second row info
		$ThirdMsg=_GetMsgWinCANInfo(2)															; Fetch the msg win third row info
		$FourthMsg=_GetMsgWinCANInfo(3)															; Fetch the msg win fourth row info

	EndIf

	consolewrite("$FirstMsg[4] :"&$FirstMsg[4]&@CRLF)
	consolewrite("$FirstMsg[2] :"&$FirstMsg[2]&@CRLF)
	consolewrite("$SecondMsg[4] :"&$SecondMsg[4]&@CRLF)
	consolewrite("$SecondMsg[2] :"&$SecondMsg[2]&@CRLF)
	consolewrite("$ThirdMsg[4] :"&$ThirdMsg[4]&@CRLF)
	consolewrite("$ThirdMsg[2] :"&$ThirdMsg[2]&@CRLF)
	consolewrite("$FourthMsg[4] :"&$FourthMsg[4]&@CRLF)
	consolewrite("$FourthMsg[2] :"&$FourthMsg[2]&@CRLF)




	if $FirstMsg[4]="0x100" and $SecondMsg[4]="0x100" and $ThirdMsg[4]="0x200" and $FourthMsg[4]="0x200" Then
		$MsgIDs=1
	EndIf

	;if $FirstMsg[2]=2 and $SecondMsg[2]=1 and $ThirdMsg[2]=1 and $FourthMsg[2]=2 Then
	if $FirstMsg[2]=2 and $SecondMsg[2]=1 and $ThirdMsg[2]=1 and $FourthMsg[2]=2 and $FirstMsg[1]="Tx" Then
			$MsgDir=1
		ElseIf $FirstMsg[2]=1 and $SecondMsg[2]=2 and $ThirdMsg[2]=2 and $FourthMsg[2]=1 and $FirstMsg[1]="Rx" Then
			$MsgDir=1
	EndIf



	consolewrite("$MsgIDs :"&$MsgIDs&@CRLF)
	consolewrite("$MsgDir :"&$MsgDir&@CRLF)

	if $MsgIDs=1 and $MsgDir=1 Then
		_WriteResult("Pass","TS_Replay_12")
	Else
		_WriteResult("Fail","TS_Replay_12")
	EndIf

EndIf

$isAppNotRes=_CloseApp()																			; Close the app

if $isAppNotRes=1 Then
	_WriteResult("Warning","TS_Replay_12")
EndIf


ConsoleWrite("****End : TS_Replay_12.au3****"&@CRLF)
ConsoleWrite(@CRLF)
