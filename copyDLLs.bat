if exist bin\Debug\ (
	echo "Updating DLLs for Debug"
	del bin\Debug\*.dll
	ROBOCOPY ThirdParty\SDL2\lib\x64\   bin\Debug SDL2.dll 
	ROBOCOPY ThirdParty\SDL2_image\lib\x64\ bin\Debug SDL2_image.dll
)

if exist bin\Release\ (
	echo "Updating DLLs for Release"
	del bin\Release\*.dll
	ROBOCOPY ThirdParty\SDL2\lib\x64\   bin\Release SDL2.dll 
	ROBOCOPY ThirdParty\SDL2_image\lib\x64\ bin\Release SDL2_image.dll
) 

exit 0