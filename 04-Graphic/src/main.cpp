#include "CanvasMonocrome.cpp"

#define BOARD_WIDTH  48
#define BOARD_HEIGHT 48
#define SPRITE_W 16
#define SPRITE_H 16


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

int main()
{
    CanvasMonocrome *canvas = new CanvasMonocrome(BOARD_WIDTH,BOARD_HEIGHT);
    
    canvas->Clear();
    // Ejemplo de uso
    bool sprite[SPRITE_W * SPRITE_H] = {
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

    canvas->SetSprite(0, 0, sprite,SPRITE_W,SPRITE_H);
    canvas->Draw(1,1);
    
    canvas->SetSprite(20, 0, spriteCompress);
    canvas->Draw(1,1);
    
    return 0;
}