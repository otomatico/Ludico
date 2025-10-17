/*🧱 canvas.h*/
#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef allocate
#define allocate(A) (A *)malloc(sizeof(A))
#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))
#endif
typedef struct
{
    BYTE color; // foreground (4 bits altos) | background (4 bits bajos)
} Pixel;

typedef struct
{
    int width;
    int height;        // Altura física (en píxeles)
    int logicalHeight; // Altura lógica (en caracteres)
    Pixel *buffer;
} Canvas;

// Funciones base del canvas
static inline Canvas *Canvas_Create(int width, int height)
{
    Canvas *c = allocate(Canvas);
    if (!c)
        return NULL;
    c->width = width;
    c->height = height;
    c->logicalHeight = height / 2;
    int length = width * c->logicalHeight;
    c->buffer = allocateArray(Pixel, length);
    memset(c->buffer, 0, length * sizeof(Pixel));
    return c;
}

static inline void Canvas_Destroy(Canvas *c)
{
    if (c)
    {
        free(c->buffer);
        free(c);
    }
}

#endif
