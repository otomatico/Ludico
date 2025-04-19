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
//Por alguna razon clear coloziona con alguna funcion de las librerias de C
// #define cleaner() system("cls")
#define cleaner() printf("\e[1;1H\e[2J\e[0m")

//Pos si hace falta texto en la terminal
#define DrawChar(x, y, c) printf("\e[%d;%dH%c", y, x, c)
#define DrawInt(x, y, i) printf("\e[%d;%dH%d", y, x, i)
#define DrawString(x, y, s) printf("\e[%d;%dH%s", y, x, s)

#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))

//#define BIT bool
//Demasiado Grande NO USAR
//#define DWORD unsigned long
//#define QWORD unsigned double
namespace Canvas
{
    //enum Draweble {Horizontal, Vertical, Flip_Horizontal, Flip_Vertical, Dot};
    class ICanvas
    {
        protected:
        int width;
        int height;
        int lenght;
        BYTE *buffer;
        
        public:
        virtual void Clear(BYTE color=0) = 0;
        virtual void SetPixel(int x, int y, BYTE color = 1) = 0;
        virtual void Draw(int startX, int startY) = 0;
        virtual void SetLine(int sx, int sy, int ex, int ey, BYTE color)=0;
        virtual void SetLineDot(int sx, int sy, int ex, int ey, BYTE color)=0;
    };
}
#endif

/*

void setLine(int x0, int y0, int x1, int y1, BYTE color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        setPixel(x0, y0, color);  // Dibuja el píxel actual

        if (x0 == x1 && y0 == y1) break;  // Fin de la línea

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
*/