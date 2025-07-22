@echo off
rem if not exist ".\output"   mkdir "output"
rem gcc main.c editor.c platform.c screen.c -o .\output\editor.exe -O3
echo //platform.h > editor_minify.c
type platform.h >> editor_minify.c
echo //screen.h >> editor_minify.c
type screen.h >> editor_minify.c
echo //editor.h >> editor_minify.c
type editor.h >> editor_minify.c

echo //platform.c >> editor_minify.c
type platform.c >> editor_minify.c
echo //screen.c >> editor_minify.c
type screen.c >> editor_minify.c
echo //editor.c >> editor_minify.c
type editor.c >> editor_minify.c
echo //main.c >> editor_minify.c
type main.c >> editor_minify.c