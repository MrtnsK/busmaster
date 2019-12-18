 @echo off

	SET actualpath=%~dp0
echo [35mInstalling Busmaster requirement[0m
	START /WAIT %actualpath%install.exe

echo [33mCleaning and copying custom file[0m
	del "C:\Program Files (x86)\BUSMASTER_v3.2.2/." /Q /F >nul
	copy "%actualpath%BUSMASTER_v3.2.2" "C:\Program Files (x86)\BUSMASTER_v3.2.2" >nul

echo [32mDone[0m
pause