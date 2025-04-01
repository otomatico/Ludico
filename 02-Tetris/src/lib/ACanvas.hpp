#ifndef _CANVAS_H_
#define _CANVAS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BYTE unsigned char
#define WORD unsigned int

#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))
#define allocate(A) (A *)malloc(sizeof(A))

//COLORES
#define BLACK 0x0
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define BLUE 0x4
#define MAGENTA 0x5
#define CYAN 0x6
#define LIGHTGREY 0x7
#define DARKGREY 0x10
#define LIGHTRED 0x11
#define LIGHTGREEN 0x12
#define LIGHTYELLOW 0x13
#define LIGHTBLUE 0x14
#define LIGHTMAGENTA 0x15
#define LIGHTCYAN 0x16
#define WHITE 0x17

//CARACTERES
#define CHAR_EMPTY 0x20
#define CHAR_SOLID 0xdb
#define CHAR_LOWER 0xdc
#define CHAR_UPPER 0xdf

#define CHAR_SOLID_25 0xb0
#define CHAR_SOLID_50 0xb1
#define CHAR_SOLID_75 0xb2

#define CHAR_UPPER_LEFT  0x2598
#define CHAR_UPPER_RIGHT 0x259d
#define CHAR_LOWER_LEFT  0x2596
#define CHAR_LOWER_RIGHT 0x2597

#define CHAR_UPPER_LEFT_LOWER_RIGHT 0x259a
#define CHAR_UPPER_RIGHT_LOWER_LEFT 0x259e

#define CHAR_UPPER_LEFT_LOWER  0x2599
#define CHAR_UPPER_RIGHT_LOWER 0x259f

#define CHAR_UPPER_LOWER_LEFT  0x259b
#define CHAR_UPPER_LOWER_RIGHT 0x259c

#define CHAR_HALF_LEFT  0x258c
#define CHAR_HALF_RIGHT 0x2590

#define CHAR_UPPER_HALF 0x2580
#define CHAR_LOWER_HALF 0x2584


//#define BIT bool
//Demasiado Grande NO USAR
//#define DWORD unsigned long
//#define QWORD unsigned double

class ACanvas
{
protected:
    int width;
    int height;
    int lenght;
    BYTE *buffer;

public:
    virtual void Clear() = 0;
    virtual void SetPixel(int x, int y, int pixel = 1) = 0;
    virtual void Draw(int startX, int startY) = 0;
};

// Hack -> Coloca o CURSOR na posição (X,Y) do Monitor e escrever uma letra ou cadena
#define DrawChar(x, y, c) printf("\e[%d;%dH%c", y, x, c)
#define DrawString(x, y, s) printf("\e[%d;%dH%s", y, x, s)
// Hack -> Somente mudar a cor de letra e fundo
#define colorGraphic(front, back) printf("\e[%d;%d;%dm", front & 0x10 ? 1 : 0, (back & 0xF) + (back & 0x10 ? 100 : 40), (front & 0xF) + 30)
// Hack -> devolver a cor por defeito do terminal
#define resetColor() printf("\e[0m")

#define hidecursor() printf("\e[?25l")
#define showcursor() printf("\e[?25h")
#define clearsrc()      printf("\e[1;1H\e[2J")
//#define clear() system("cls")

#define gotoxy(x, y) printf("\e[%d;%dH", (int)y, (int)x)

void DrawBarVertical(short x, short y, short size, char c)
{
    for (short index = 0; index < size; index++)
    {
        DrawChar(x, y + index, c);
    }
}

void DrawBarHorizontal(short x, short y, short size, char c)
{
    char buffer[size + 1];
    memset(buffer, c, size);
    buffer[size] = '\0';
    DrawString(x,y,buffer);
    /*
    gotoxy(x, y);
    for (short index = 0; index < size; index++)
    {
        printf("%c", c);
    }
    */
}
void DrawFrame(int x,int y,int w,int h)
{
    DrawBarHorizontal(x, y, w, CHAR_EMPTY);
    for(int i = 1; i<h-1;i++)
    {
        DrawBarHorizontal(x, y+i, w, CHAR_EMPTY);
    }
    DrawBarHorizontal(x, y+h-1, w, CHAR_EMPTY);
}
#endif