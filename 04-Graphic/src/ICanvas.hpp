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

class ICanvas
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
#endif