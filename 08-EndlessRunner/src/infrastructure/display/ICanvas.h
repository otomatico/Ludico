#ifndef _CANVAS_H_
#define _CANVAS_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BYTE unsigned char
#define WORD unsigned int

#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))
#define allocate(A) (A *)malloc(sizeof(A))

#define CHAR_EMPTY 0x20
#define CHAR_SOLID 0xdb
#define CHAR_LOWER 0xdc
#define CHAR_UPPER 0xdf

#define resetColor() printf("\e[0m")
#define gotoXY(X, Y) printf("\e[%d;%dH", (Y), (X))

#define hidecursor() printf("\e[?25l")
#define showcursor() printf("\e[?25h")
// Por alguna razon clear coloziona con alguna funcion de las librerias de C
//  #define cleaner() system("cls")
#define cleaner() printf("\e[1;1H\e[2J\e[0m")

// Pos si hace falta texto en la terminal
#define DrawChar(x, y, c) printf("\e[%d;%dH%c", y, x, c)
#define DrawInt(x, y, i) printf("\e[%d;%dH%d", y, x, i)
#define DrawString(x, y, s) printf("\e[%d;%dH%s", y, x, s)

#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))
#endif