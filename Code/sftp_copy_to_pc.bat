:: Coppy all fies from Beagle Bone Blue to PC
::   Will delete all old files in BBBlue folder exept the ReadMe.md
::   and ReadMe.txt and then copy over new code files from selected 
::   directory on BBB into the BBBlue directory on the pc

@echo off

:: Warning
color 0E
echo [WARRNING] This batch script will deleate all files in BBBlue before copying files from BBB
echo|set /p="press Ctrl+c to exit or "
pause
color 0F

set origDir=%CD%
set /P BBBDir="Beagle Bone Directory:"

:: Clear out old code files from the local Beagle Bone Blue code directory
echo|set /p="Deleating old local pc files in BBBlue directory..."
cd BBBlue
for /F "delims=" %%i in ('dir /b') do ( 
	if exist %%i\NUL (
		rmdir "%%i" /s/q
	) else if "%%i"=="ReadMe.md" (
		echo Skiped ReadMe
	) else if "%%i"=="ReadMe.txt" (
		echo Skiped ReadMe
	) else (
		1>NUL  del "%%i" /s/q
	)
)
echo Done

:: Switch around directorys to run putty sftp for file transfer
set PCDir=%CD%
C:
cd C:\Program Files (x86)\PuTTY

:: Open and copy files from remote code directory
::   - PSFTP is the Secure File Transfer Protocol (SFTP) client of PuTTY
::   - script is sftp dynamicaly built commands script
echo Copying files from BBB directory to BBBlue directory on pc...

set script=%origDir%\psftp_script.txt
>%script% echo cd %BBBDir%
>>%script% echo lcd %PCDir%
>>%script% echo mget -r *

psftp 192.168.7.2 -b %script%

echo Done copying files

pause