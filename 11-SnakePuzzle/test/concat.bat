@echo off
setlocal enabledelayedexpansion
:: dir /S /B>project.dir
:: Directorio principal donde se encuentra el script
set "ROOT_DIR=%~dp0"
:: Crear la carpeta de salida si no existe
if not exist "%ROOT_DIR%output\" mkdir "%ROOT_DIR%output\"
:: Nombre de los archivos de salida
set "OUTPUT_C=%ROOT_DIR%output\main.c"
:: Limpiar los archivos de salida si ya existen
if exist "%OUTPUT_C%" del "%OUTPUT_C%"

echo Iniciando la concatenacion por carpetas...
echo.
echo. >> "!OUTPUT_C!"
:: Recorrer cada subcarpeta a partir del directorio actual
FOR /D /R "%ROOT_DIR%" %%d IN (*) DO (
    cd "%%d"
    echo Procesando carpeta: %%d
    for %%f in (*.c) do (
        echo Anadiendo %%f al inicio...
        set "TEMP_FILE=%TEMP%\tmpfile_0.txt"
        :: Escribir el nuevo archivo primero
        echo "!TEMP_FILE!"
        type "%%f" > "!TEMP_FILE!"
        echo. >> "!TEMP_FILE!"

        :: Añadir el contenido previo
        type "!OUTPUT_C!" >> "!TEMP_FILE!"
        :: Reemplazar el archivo original
        move /Y "!TEMP_FILE!" "!OUTPUT_C!" >nul
    )
    for /R %%f in (*.h) do (
        echo Anadiendo %%f al inicio...
        set "TEMP_FILE=%TEMP%\tmpfile_0.txt"
        :: Escribir el nuevo archivo primero
        type "%%f" > "!TEMP_FILE!"
        echo. >> "!TEMP_FILE!"

        :: Añadir el contenido previo
        type "!OUTPUT_C!" >> "!TEMP_FILE!"
        :: Reemplazar el archivo original
        move /Y "!TEMP_FILE!" "!OUTPUT_C!" >nul
    )
)