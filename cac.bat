@echo off
set "CURRENT_DIR=%~dp0"
set "ARMCC_CALCELF=%CURRENT_DIR%calcelf.exe"

:: Find the first .axf file in MDK-ARM directory and extract its name
for %%F in ("%CURRENT_DIR%MDK-ARM\*.uvprojx") do (
    set "PROJECT_NAME=%%~nF"
    goto :found
)
echo No .uvprojx file found in MDK-ARM directory!
exit /b 1

:found
%ARMCC_CALCELF% -m "%CURRENT_DIR%MDK-ARM\%PROJECT_NAME%\%PROJECT_NAME%.map" -s "%CURRENT_DIR%MDK-ARM\%PROJECT_NAME%\%PROJECT_NAME%.sct"
%ARMCC_CALCELF% --format=progbar --map="%CURRENT_DIR%MDK-ARM\%PROJECT_NAME%\%PROJECT_NAME%.map" --scatter="%CURRENT_DIR%MDK-ARM\%PROJECT_NAME%\%PROJECT_NAME%.sct"