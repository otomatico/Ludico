#ifndef _CANVAS_MONOCROME_H_
#define _CANVAS_MONOCROME_H_
#include "ICanvas.hpp"

class CanvasMonocrome : public ICanvas
{
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

    int enumChar(int item)
    {
        if (item == CHAR_EMPTY)
            return 0;
        if (item == CHAR_LOWER)
            return 1;
        if (item == CHAR_UPPER)
            return 2;
        return 3;
    }

public:
    CanvasMonocrome()
    {
        width = 48;
        height = 48;
        _Initialize();
    }
    CanvasMonocrome(int width, int height)
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
    ~CanvasMonocrome()
    {
        free(buffer);
    }

    void Clear()
    {
        memset(buffer, CHAR_EMPTY, lenght);
    }

    // Pixel MONOCROME
    void SetPixel(int x, int y, int pixel = 1)
    {
        const unsigned char pixelTransitionMap[16] = {
            CHAR_EMPTY, CHAR_LOWER, CHAR_EMPTY, CHAR_LOWER,
            CHAR_UPPER, CHAR_SOLID, CHAR_UPPER, CHAR_SOLID,
            CHAR_EMPTY, CHAR_EMPTY, CHAR_UPPER, CHAR_UPPER,
            CHAR_LOWER, CHAR_LOWER, CHAR_SOLID, CHAR_SOLID};

        if (0 <= y && y < height && 0 <= x && x < width)
        {
            int index = (y / 2) * width + x;
            BYTE charOld = buffer[index];
            int charOldIndex = enumChar(charOld);
            buffer[index] = pixelTransitionMap[(y % 2) * 8 + pixel * 4 + charOldIndex];
        }
    }
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

    void Draw(int startX, int startY)
    {
        for (int row = 0; row < height / 2; row++)
        {
            printf("\e[%d;%dH%.*s", startY + row, startX, width, buffer + (row * width));
        }
    }
};
#endif