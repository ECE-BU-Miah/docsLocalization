@echo off

echo sory this script is not operational yet.

::set origDir=%CD%
::set /P BBBDir="Beagle Bone Directory:"
::
:::: Switch around directorys to run putty sftp for file transfer
::set PCDir=%CD%
::C:
::cd C:\Program Files (x86)\PuTTY
::
:::: Open and copy files from remote code directory
::echo Copying files from BBB directory to BBBlue directory on pc...
::
::set script=%origDir%\psftp_script.txt
::>%script% echo cd %BBBDir%
::>>%script% echo lcd %PCDir%
::>>%script% echo mget -r *
::
::psftp 192.168.7.2 -b %script%
::
::echo Done copying files

pause