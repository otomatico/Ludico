#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER (48 * 48) // 2304
#define SPRITE_W 16
#define SPRITE_H 16
#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))
#define allocate(A) (A *)malloc(sizeof(A))

#define clear() printf("\e[1;1H\e[2J")
#define resetColor() printf("\e[0m")

#define CHAR_EMPTY 0x20
#define CHAR_SOLID 0xdb
#define CHAR_LOWER 0xdc
#define CHAR_UPPER 0xdf
//#define BIT bool
#define BYTE unsigned char
#define WORD unsigned int
//Demasiado Grande NO USAR
//#define DWORD unsigned long
//#define QWORD unsigned double

typedef struct
{
    int width;
    int height;
    unsigned char *buffer;
} GraphicsBuffer;

GraphicsBuffer gfxBuffer;

void CreateBuffer(int w, int h)
{
    if (w <= 0 || h <= 0)
    {
        fprintf(stderr, "Error: Dimensiones del buffer no válidas.\n");
        exit(1);
    }
    gfxBuffer.width = w;
    gfxBuffer.height = h;
    gfxBuffer.buffer = allocateArray(unsigned char, w *(h / 2));
    if (gfxBuffer.buffer == NULL)
    {
        fprintf(stderr, "Error: No se pudo asignar memoria para el buffer.\n");
        exit(1);
    }
}

void ClearBuffer()
{
    int max = gfxBuffer.width * (gfxBuffer.height / 2);
    memset(gfxBuffer.buffer, 0x20, max);
}

void SetPixelInBuffer(int x, int y, int pixel)
{
    /*
    const unsigned char pixelTransitionMap[2][2][4] = {
        {{CHAR_EMPTY, CHAR_LOWER, CHAR_EMPTY, CHAR_LOWER},
        {CHAR_UPPER, CHAR_SOLID, CHAR_UPPER, CHAR_SOLID}},
        {{CHAR_EMPTY, CHAR_EMPTY, CHAR_UPPER, CHAR_UPPER},
        {CHAR_LOWER, CHAR_LOWER, CHAR_SOLID, CHAR_SOLID}}};
    */
    const unsigned char pixelTransitionMap[16] = {
        CHAR_EMPTY, CHAR_LOWER, CHAR_EMPTY, CHAR_LOWER,
        CHAR_UPPER, CHAR_SOLID, CHAR_UPPER, CHAR_SOLID,
        CHAR_EMPTY, CHAR_EMPTY, CHAR_UPPER, CHAR_UPPER,
        CHAR_LOWER, CHAR_LOWER, CHAR_SOLID, CHAR_SOLID};

    if (0 <= y && y < gfxBuffer.height && 0 <= x && x < gfxBuffer.width)
    {
        int index = (y / 2) * gfxBuffer.width + x;
        unsigned char charOld = gfxBuffer.buffer[index];
        int charOldIndex = charOld == CHAR_EMPTY ? 0 : charOld == CHAR_LOWER ? 1
                                                   : charOld == CHAR_UPPER   ? 2
                                                                             : 3;
        // gfxBuffer.buffer[index] = pixelTransitionMap[y % 2][pixel][charOldIndex];
        gfxBuffer.buffer[index] = pixelTransitionMap[(y % 2) * 8 + pixel * 4 + charOldIndex];
    }
}

void SetSpriteInBuffer(int x, int y, unsigned char *sprite)
{
    for (int row = 0; row < SPRITE_H; row++)
    {
        for (int col = 0; col < SPRITE_W; col++)
        {
            SetPixelInBuffer(x + col, y + row, sprite[row * SPRITE_W + col]);
        }
    }
}

void SetSpriteCompressInBuffer(int x, int y, unsigned int *sprite)
{
    for (int row = 0; row < SPRITE_H; row++)
    {
        for (int col = 0; col < SPRITE_W; col++)
        {
            SetPixelInBuffer(x + (SPRITE_W - col), y + row, sprite[row] >> col & 0x0001);
        }
    }
}

void FlipHorizontalSprite(unsigned char *sprite)
{
    int aux;
    for (int row = 0; row < SPRITE_H; row++)
    {
        for (int col = 0; col < SPRITE_W / 2; col++)
        {
            aux = sprite[row * SPRITE_W + col];
            sprite[row * SPRITE_W + col] = sprite[row * SPRITE_W + (SPRITE_W - col - 1)];
            sprite[row * SPRITE_W + (SPRITE_W - col - 1)] = aux;
        }
    }
}

void FlipVerticalSprite(unsigned char *sprite)
{
    int aux;
    for (int row = 0; row < SPRITE_H / 2; row++)
    {
        for (int col = 0; col < SPRITE_W; col++)
        {
            aux = sprite[row * SPRITE_W + col];
            sprite[row * SPRITE_W + col] = sprite[(SPRITE_H - row - 1) * SPRITE_W + col];
            sprite[(SPRITE_H - row - 1) * SPRITE_W + col] = aux;
        }
    }
}

void CloneSprite(unsigned char *target, unsigned char *source)
{
    int max = SPRITE_H * SPRITE_W;
    memcpy(target, source, max);
}

void DrawBuffer(int startX, int startY)
{
    for (int row = 0; row < gfxBuffer.height; row++)
    {
        printf("\e[%d;%dH%.*s", startY + row, startX, gfxBuffer.width, gfxBuffer.buffer + (row * gfxBuffer.width));
    }
}

void DestroyBuffer()
{
    free(gfxBuffer.buffer);
}

int main()
{
    clear();
    CreateBuffer(48, 48);
    ClearBuffer();
    // Ejemplo de uso
    unsigned char sprite[SPRITE_W * SPRITE_H] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0,
        0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0,
        1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0,
        0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
        0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    WORD spriteCompress[SPRITE_H] = {
        0b0000000000000000,
        0b0000000000000000,
        0b0000001111100000,
        0b0000110001010000,
        0b0001000010001000,
        0b0111100010010100,
        0b1000010010010100,
        0b1000001001000100,
        0b1000011000111110,
        0b0100010001000001,
        0b0011100010111110,
        0b0010000001000010,
        0b0001100000111100,
        0b0000011111000000,
        0b0000000000000000,
        0b0000000000000000};

    SetSpriteInBuffer(0, 0, sprite);

    SetSpriteCompressInBuffer(20, 0, spriteCompress);

    DrawBuffer(1, 1);
    DestroyBuffer();
    return 0;
}
