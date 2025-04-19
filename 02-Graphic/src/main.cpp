#include "Canvas1bit.cpp"

#define BOARD_WIDTH 48
#define BOARD_HEIGHT 48
#define SPRITE_W 16
#define SPRITE_H 16

class Sprite1bit : public Canvas1bit
{
private:
    Canvas1bit *canvas = new Canvas1bit(BOARD_WIDTH, BOARD_HEIGHT);

public:
    // Sprite MONOCROME 16x16
    void SetSprite(int x, int y, WORD *sprite)
    {
        const int H = 16, W = 16;
        for (int row = 0; row < H; row++)
        {
            for (int col = 0; col < W; col++)
            {
                canvas->SetPixel(x + (W - col), y + row, (sprite[row] >> col & 0x0001) ? WHITE : BLACK);
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
                canvas->SetPixel(x + (W - col), y + row, (sprite[row] >> col & 0x01 ? WHITE : BLACK));
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
                canvas->SetPixel(x + col, y + row, sprite[row_offset + col] ? WHITE : BLACK);
            }
        }
    }
    void Draw(int x, int y)
    {
        cleaner();
        /*
        canvas->SetLine(0, 0, 47, 0, WHITE);
        canvas->SetLine(0, 0, 24, 47, WHITE);
        canvas->SetLine(47, 0, 24, 47, WHITE);
        */
        canvas->Draw(x, y);
    }

    void Clear()
    {
        canvas->Clear(BLACK);
    }
    void Clone(unsigned char *target, unsigned char *source)
    {
        int max = SPRITE_H * SPRITE_W;
        memcpy(target, source, max);
    }
};

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

int main()
{
    Sprite1bit canvas;

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

    canvas.Clear();
    canvas.SetSprite(0, 0, sprite, SPRITE_W, SPRITE_H);
    // canvas.Draw(1, 1);

    canvas.SetSprite(20, 0, spriteCompress);
    canvas.Draw(2, 3);

    return 0;
}