if exist bin\x86\Debug\ (
	echo "Updating DLLs for x86 Debug"
	del bin\x86\Debug\*.dll
	ROBOCOPY ThirdParty\SDL2\lib\x86\   bin\x86\Debug SDL2.dll 
	ROBOCOPY ThirdParty\SDL2_image\lib\x86\ bin\x86\Debug SDL2_image.dll
) 

if exist bin\x86\Release\ (
	echo "Updating DLLs for x86 Release"
	del bin\x86\Release\*.dll
	ROBOCOPY ThirdParty\SDL2\lib\x86\   bin\x86\Release SDL2.dll 
	ROBOCOPY ThirdParty\SDL2_image\lib\x86\ bin\x86\Release SDL2_image.dll
) 

if exist bin\x64\Debug\ (
	echo "Updating DLLs for x64 Debug"
	del bin\x64\Debug\*.dll
	ROBOCOPY ThirdParty\SDL2\lib\x64\   bin\x64\Debug SDL2.dll 
	ROBOCOPY ThirdParty\SDL2_image\lib\x64\ bin\x64\Debug SDL2_image.dll
) 

if exist bin\x64\Release\ (
	echo "Updating DLLs for x64 Release"
	del bin\x64\Release\*.dll
	ROBOCOPY ThirdParty\SDL2\lib\x64\   bin\x64\Release SDL2.dll 
	ROBOCOPY ThirdParty\SDL2_image\lib\x64\ bin\x64\Release SDL2_image.dll
) 

exit 0