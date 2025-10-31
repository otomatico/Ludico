@echo off
setlocal enabledelayedexpansion
:: Nombre de los archivos de salida
set "ROOT_DIR=%~dp0"
set "OUTPUT_C=%ROOT_DIR%output/main.c"
:: Borra el archivo de salida si ya existe para evitar añadirle contenido
:: Limpiar los archivos de salida si ya existen
if exist "%OUTPUT_C%" (del "%OUTPUT_C%")

:: Define y escribe los patrones de exclusion al archivo. 
set "PATTERN_FILE=findstr_patterns.txt"
echo Generando fichero de patrones: "%PATTERN_FILE%"
> "%PATTERN_FILE%" (
    echo #include "
    echo // ===
    echo //#include
)

echo Concatenando archivos...

:: Lee la lista de archivos línea por línea
for /f "tokens=*" %%a in (project.dir) do (
    echo Agregando %%a ...
    echo /*"%%a"*/ >> "!OUTPUT_C!"
    echo. >> "!OUTPUT_C!"
    type %%a | findstr /V /g:%PATTERN_FILE% >> "!OUTPUT_C!"
    echo. >> "!OUTPUT_C!"
)
:: Borra fichero de patrones
del %PATTERN_FILE%
echo Proceso completado. Archivo de salida: "!OUTPUT_C!"
pause