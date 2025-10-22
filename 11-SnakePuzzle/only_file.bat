@echo off
setlocal enabledelayedexpansion

:: Borra el archivo de salida si ya existe para evitar añadirle contenido
if exist /output/main.c del output/main.c

echo Concatenando archivos...

:: Lee la lista de archivos línea por línea
for /f "tokens=*" %%a in (project.dir) do (
    echo Agregando %%a...
    type "%%a" >> output/main.c
)

echo Proceso completado. Archivo de salida: ./output/main.c
pause