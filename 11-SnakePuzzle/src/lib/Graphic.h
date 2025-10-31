#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "canvas.h"

#define colorGraphic(front, back) printf("\e[%d;%d;%dm", (front & 0x8 ? 1 : 0), ((back & 0x7) + (back & 0x8 ? 100 : 40)), ((front & 0x7) + 30))
#define resetColor() printf("\e[0m")
#define gotoXY(X, Y) printf("\e[%d;%dH", (Y), (X))
#define hidecursor() printf("\e[?25l")
#define showcursor() printf("\e[?25h")
#define cleaner() printf("\e[1;1H\e[2J\e[0m")

#define CHAR_UPPER 0xdf

// COLORES
#define BLACK 0x0
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define BLUE 0x4
#define MAGENTA 0x5
#define CYAN 0x6
#define LIGHTGREY 0x7
#define DARKGREY 0x8
#define LIGHTRED 0x9
#define LIGHTGREEN 0xa
#define LIGHTYELLOW 0xb
#define LIGHTBLUE 0xc
#define LIGHTMAGENTA 0xd
#define LIGHTCYAN 0xe
#define WHITE 0xf

static inline void Graphic_UpdatePixelColor(Canvas *c, int x, int logicalY, BYTE newColor, int isUpper)
{
    int index = logicalY * c->width + x;
    if (isUpper)
        c->buffer[index].color = (newColor << 4) | (c->buffer[index].color & 0x0F);
    else
        c->buffer[index].color = (c->buffer[index].color & 0xF0) | (newColor & 0x0F);
}

static inline void Graphic_Clear(Canvas *c, BYTE color)
{
    int length = c->width * c->logicalHeight;
    BYTE combined = (color << 4) | color;
    for (int i = 0; i < length; i++)
        c->buffer[i].color = combined;
}

static inline void Graphic_SetPixel(Canvas *c, int x, int y, BYTE color)
{
    if (x >= 0 && x < c->width && y >= 0 && y < c->height)
    {
        int logicalY = y / 2;
        int isUpper = (y % 2 == 0);
        Graphic_UpdatePixelColor(c, x, logicalY, color, isUpper);
    }
}

static inline void Graphic_SetLine(Canvas *c, int x0, int y0, int x1, int y1, BYTE color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        Graphic_SetPixel(c, x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

static inline void Graphic_SetLineDot(Canvas *c, int x0, int y0, int x1, int y1, BYTE color)
{
    int drawable = 1;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        if (drawable)
            Graphic_SetPixel(c, x0, y0, color);

        drawable = !drawable;

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

static inline void Graphic_Draw(Canvas *c, int startX, int startY)
{
    for (int row = 0; row < c->logicalHeight; row++)
    {
        gotoXY(startX, startY + row);
        for (int col = 0; col < c->width; col++)
        {
            Pixel p = c->buffer[row * c->width + col];
            colorGraphic((p.color >> 4) & 0x0F, p.color & 0x0F);
            putchar(CHAR_UPPER);
        }
    }
    resetColor();
}

static inline void Graphic_SetRectangle(Canvas *c, int x0, int y0, int width, int height, BYTE color)
{
    for (int y = y0; y < y0 + height; y++)
    {
        Graphic_SetLine(c, x0, y, x0 + width, y, color);
    }
}

typedef struct
{
    void (*Clear)(Canvas *, BYTE);
    void (*SetPixel)(Canvas *, int, int, BYTE);
    void (*SetLine)(Canvas *, int, int, int, int, BYTE);
    void (*SetLineDot)(Canvas *, int, int, int, int, BYTE);
    void (*SetRectangle)(Canvas *, int, int, int, int, BYTE);
    void (*Draw)(Canvas *, int, int);
} Graphic;

// Inicializador
static Graphic* Graphic_Init(void)
{
    Graphic *g= allocate(Graphic);
    g->Clear = Graphic_Clear;
    g->SetPixel = Graphic_SetPixel;
    g->SetLine = Graphic_SetLine;
    g->SetLineDot = Graphic_SetLineDot;
    g->SetRectangle = Graphic_SetRectangle;
    g->Draw = Graphic_Draw;
    return g;
}

#endif
