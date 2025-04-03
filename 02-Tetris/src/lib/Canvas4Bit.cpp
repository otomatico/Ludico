#ifndef _CANVAS_H_
#define _CANVAS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BYTE unsigned char
#define WORD unsigned int

#define colorGraphic(front, back) printf("\e[%d;%d;%dm", front & 0x8 ? 1 : 0, (back & 0x7) + (back & 0x8 ? 100 : 40), (front & 0x7) + 30)
#define resetColor() printf("\e[0m")
#define gotoXY(X,Y) printf("\e[%d;%dH", (Y), (X))

#define hidecursor() printf("\e[?25l")
#define showcursor() printf("\e[?25h")
#define clear() printf("\e[1;1H\e[2J")
#define DrawChar(x, y, c) printf("\e[%d;%dH%c", y, x, c)
#define DrawInt(x, y, i) printf("\e[%d;%dH%d", y, x, i)
#define DrawString(x, y, s) printf("\e[%d;%dH%s", y, x, s)

#define allocateArray(A, B) (A *)malloc(sizeof(A) * ((B > 0) ? (B) : 1))

#define CHAR_UPPER 0xdf

//COLORES
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


struct Pixel {
    BYTE color;  // foreground (4 bits bajos) | background (4 bits altos)
};
class CanvasColor {
    protected:
        int width;
        int height;  // Altura real (en píxeles físicos)
        int logicalHeight;  // Altura lógica (en caracteres, mitad de height)
        Pixel* buffer;
    
    private:
        void _Initialize() {
            logicalHeight = height / 2;
            int length = width * logicalHeight;
            buffer = allocateArray(Pixel, length);
            Clear();
        }
    
        void _UpdatePixelColor(int x, int logicalY, BYTE newColor, bool isUpper) {
            int index = logicalY * width + x;
            if (isUpper) {  // Mitad superior (foreground)
                buffer[index].color = (newColor << 4) | (buffer[index].color & 0x0F);
            } else {        // Mitad inferior (background)
                buffer[index].color = (buffer[index].color & 0xF0) | (newColor & 0x0F);
            }
        }
    
    public:
        CanvasColor(int width, int height) {
            this->width = width;
            this->height = height;  // Altura física (ej: 6 filas = 3 caracteres)
            _Initialize();
        }
    
        ~CanvasColor() { free(buffer); }
    
        void Clear(BYTE color=BLACK) {
            int length = width * logicalHeight;
            color =  color<<4|color;
            memset(buffer, color, length * sizeof(Pixel));
        }
    
        // SetPixel: 'y' es la coordenada física (0 a height-1)
        void SetPixel(int x, int y, BYTE color) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                int logicalY = y / 2;
                bool isUpper = (y % 2 == 0);
                _UpdatePixelColor(x, logicalY, color, isUpper);
            }
        }
    
        void Draw(int startX, int startY) {
            for (int row = 0; row < logicalHeight; row++) {
                gotoXY(startX, startY + row);
                for (int col = 0; col < width; col++) {
                    Pixel p = buffer[row * width + col];
                    colorGraphic((p.color >> 4) & 0x0F,p.color & 0x0F);
                    putchar(CHAR_UPPER);
                }
            }
            resetColor();
        }
    };
#endif