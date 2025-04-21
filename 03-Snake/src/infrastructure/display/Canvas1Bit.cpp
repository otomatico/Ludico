#include "ICanvas.h"
#ifndef _CANVAS_MONOCROME_H_
#define _CANVAS_MONOCROME_H_
/*
Primero existia una version optimizada que usaba bit a bit de un Byte para almacenar el color 0|1,
 tal que lo optimo era que la dimesion de la pantalla fuera multiple de 8 por ejemplo 128x32 que son (16*8)x(4*8)
Esta optimizacion se perdió por algun USB asi que esta el una implementacion menos optimizda y
similar a la ideia es usará en 4bit. La parte alta del byte(4 primeros bits) armazena la parte superior y
 la parte baja del byte el la fila inferor tal que la altura debe ser par.
*/
// Colores
#define BLACK 0x0
#define WHITE 0xf

class Canvas1bit : public ICanvas
{
public:
    Canvas1bit()
    {
        width = 48;
        height = 48;
        _Initialize();
    }
    Canvas1bit(int width, int height)
    {
        this->width = width;
        this->height = height;
        _Initialize();
        if (buffer == NULL)
        {
            fprintf(stderr, "Error: No se pudo asignar memoria para el buffer.\n");
            exit(1);
        }
    }
    ~Canvas1bit()
    {
        free(buffer);
    }

    void Clear(BYTE color = BLACK)
    {
        if (color == WHITE)
        {
            color = 0xff;
        }
        memset(buffer, color, lenght);
    }

    void SetPixel(int x, int y, BYTE color = BLACK)
    {
        if (0 <= y && y < height && 0 <= x && x < width)
        {
            int index = y / 2 * width + x;
            bool isUpper = (y % 2 == 0);
            if (isUpper)
            {
                buffer[index] = (buffer[index] & 0x0f) | (color << 4);
            }
            else
            {
                buffer[index] = (buffer[index] & 0xf0) | color;
            }
        }
    }

    void Draw(int startX, int startY)
    {
        int logicalHeight = (height / 2);
        int rowFocus;
        for (int row = 0; row < logicalHeight; row++)
        {
            gotoXY(startX, startY + row);
            rowFocus = row * width;
            for (int col = 0; col < width; col++)
            {
                putchar(GetChar(buffer[rowFocus + col]));
            }
        }
    }

protected:
    int width;
    int height;
    int lenght;
    BYTE *buffer;

private:
    void _Initialize()
    {
        lenght = width * (height / 2);
        buffer = allocateArray(BYTE, lenght);
    }
    char GetChar(BYTE value)
    {
        if (value == 0xf0)
        {
            return CHAR_UPPER;
        }
        if (value == 0x0f)
        {
            return CHAR_LOWER;
        }
        if (value == 0xff)
        {
            return CHAR_SOLID;
        }
        return CHAR_EMPTY;
    }
};

#endif
/****
 * esto es codigo para Sprite .. ahora no voy usar .. proximamente hago refactoring
 *
 *
    // Sprite MONOCROME 16x16
    void SetSprite(int x, int y, WORD *sprite)
    {
        const int H = 16, W = 16;
        for (int row = 0; row < H; row++)
        {
            for (int col = 0; col < W; col++)
            {
                SetPixel(x + (W - col), y + row, sprite[row] >> col & 0x0001);
            }
        }
    }
    // Sprite MONOCROME 8x8
    void SetSprite(int x, int y, BYTE *sprite)
    {
        const int H = 8, W = 8;
        for (int row = 0; row < H; row++)
        {
            for (int col = 0; col < W; col++)
            {
                SetPixel(x + (W - col), y + row, sprite[row] >> col & 0x01);
            }
        }
    }
    // Sprite MONOCROME ?x?
    void SetSprite(int x, int y, bool *sprite, int width, int height)
    {
        int row_offset;
        for (int row = 0; row < height; row++)
        {
            row_offset = row * width;
            for (int col = 0; col < width; col++)
            {
                SetPixel(x + col, y + row, sprite[row_offset + col] ? 1 : 0);
            }
        }
    }

 *
 */