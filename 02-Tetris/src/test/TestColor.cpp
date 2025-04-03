#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BYTE unsigned char
#define WORD unsigned int

#define colorGraphic(front, back) printf("\e[%d;%d;%dm", front & 0x10 ? 1 : 0, (back & 0xF) + (back & 0x10 ? 100 : 40), (front & 0xF) + 30)
#define resetColor() printf("\e[0m")
#define gotoXY(X,Y) printf("\e[%d;%dH", (Y), (X))
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
#define DARKGREY 0x10
#define LIGHTRED 0x11
#define LIGHTGREEN 0x12
#define LIGHTYELLOW 0x13
#define LIGHTBLUE 0x14
#define LIGHTMAGENTA 0x15
#define LIGHTCYAN 0x16
#define WHITE 0x17


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
        }
    
        void _UpdatePixelColor(int x, int logicalY, BYTE newColor, bool isUpper) {
            int index = logicalY * width + x;
            if (isUpper) {  // Mitad superior (foreground)
                buffer[index].color = (buffer[index].color & 0xF0) | (newColor & 0x0F);
            } else {        // Mitad inferior (background)
                buffer[index].color = (newColor << 4) | (buffer[index].color & 0x0F);
            }
        }
    
    public:
        CanvasColor(int width, int height) {
            this->width = width;
            this->height = height;  // Altura física (ej: 6 filas = 3 caracteres)
            _Initialize();
        }
    
        ~CanvasColor() { free(buffer); }
    
        void Clear() {
            int length = width * logicalHeight;
            memset(buffer, BLACK, length * sizeof(Pixel));
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
                    colorGraphic(p.color & 0x0F, (p.color >> 4) & 0x0F);
                    putchar(CHAR_UPPER);
                }
            }
            resetColor();
        }
    };
    int main() {
        CanvasColor canvas(80, 60);  // 10 columnas x 6 filas (3 caracteres de altura)
        canvas.Clear();
        // Línea horizontal (rojo en mitades superiores, azul en inferiores)
        for (int x = 0; x < 320; x++) {
            canvas.SetPixel(x, 0, RED);    // Y=0 (par) -> foreground
            canvas.SetPixel(x, 1, BLUE);   // Y=1 (impar) -> background
        }
    
        // Pieza de Tetris (verde en Y=2, amarillo en Y=3)
        canvas.SetPixel(4, 2, GREEN);
        canvas.SetPixel(4, 3, YELLOW);
    
        canvas.Draw(1, 1);  // Dibujar en (1,1)
        return 0;
    }