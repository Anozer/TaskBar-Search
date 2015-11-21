@echo off
echo -------------------------------------------------
echo Don't forget to add "C:\Qt\Tools\mingw492_32\bin"
echo and "C:\Qt\5.5\android_x86\bin"                  
echo to your path environement variable               
echo -------------------------------------------------
echo.
echo.



echo -------------------------------------------------
echo COMPILATION           
echo -------------------------------------------------
echo.

rem // compilation of release version
cd build
qmake.exe ../TaskBarSearch.pro -r -spec win32-g++
mingw32-make.exe
cd ..



echo.
echo.
echo.
echo -------------------------------------------------
echo Deployment           
echo -------------------------------------------------
echo.

rem // deployment with qt for windows
rmdir deploy /S /Q
mkdir deploy
mkdir deploy\TaskBarSearch
copy "build\release\TaskBarSearch.exe" "deploy\TaskBarSearch\TaskBarSearch.exe"
windeployqt --no-translations deploy/TaskBarSearch/

rem // copy some none essential files
copy "default.ini" "deploy\TaskBarSearch\default.ini"



echo.
echo.
echo.
echo -------------------------------------------------
echo ZIP           
echo -------------------------------------------------
echo.

rem // Zip everything
cd deploy
"%ProgramFiles%/7-zip/7z.exe" a TaskBarSearch.zip TaskBarSearch/



echo.
echo ------------------------------------------------
echo.
echo Script over                                     
echo.
echo You can now "/deploy/TaskBarSearch" wherever you want
echo.
echo ------------------------------------------------
echo.
pause

