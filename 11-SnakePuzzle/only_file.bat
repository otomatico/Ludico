@echo off
setlocal enabledelayedexpansion
:: Nombre de los archivos de salida
set "ROOT_DIR=%~dp0"
set "OUTPUT_C=%ROOT_DIR%output/main.c"
:: Borra el archivo de salida si ya existe para evitar añadirle contenido
:: Limpiar los archivos de salida si ya existen
if exist "%OUTPUT_C%" del "%OUTPUT_C%"

echo Concatenando archivos...

:: Lee la lista de archivos línea por línea
for /f "tokens=*" %%a in (project.dir) do (
    echo Agregando %%a...
    type "%%a" >> "!OUTPUT_C!"
    echo. >> "!OUTPUT_C!"
)

echo Proceso completado. Archivo de salida: "!OUTPUT_C!"
pause