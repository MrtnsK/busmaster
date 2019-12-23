@echo off

	SET actualpath=%~dp0
if exist "C:\Program Files (x86)\Sontheim\MT_Api" (
echo [35mSontheim Driver already installed[0m
) else (
echo [35mInstalling Sontheim Driver[0m
	START /WAIT %actualpath%MT_API\data\MT_API_fr.msi /qr /norestart
	START "C:\Program Files (x86)\Sontheim\MT_Api\SIECA132.exe" exit
)
echo [35mInstalling Busmaster requirement[0m
	START /WAIT %actualpath%install.exe

echo [33mCleaning and copying custom file[0m
	del "C:\Program Files (x86)\BUSMASTER_v3.2.2/." /Q /F >nul
	copy "%actualpath%BUSMASTER_v3.2.2" "C:\Program Files (x86)\BUSMASTER_v3.2.2" >nul

echo [32mDone[0m
pause