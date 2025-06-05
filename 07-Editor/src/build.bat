@echo off
if not exist ".\output" (
    mkdir "output"
)
gcc main.c editor.c platform.c screen.c -o .\output\editor.exe














