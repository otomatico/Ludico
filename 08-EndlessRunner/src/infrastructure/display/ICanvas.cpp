#include "ICanvas.h"

class ICanvas
{

public:
    virtual void Clear(BYTE color = 0) = 0;
    virtual void SetPixel(int x, int y, BYTE color = 0) = 0;
    virtual void Draw(int startX, int startY) = 0;
    // virtual void SetLine(int sx, int sy, int ex, int ey, BYTE color)=0;
    // virtual void SetLineDot(int sx, int sy, int ex, int ey, BYTE color)=0;

    void SetLine(int x0, int y0, int x1, int y1, BYTE color)
    {
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
            SetPixel(x0, y0, color); // Dibuja el píxel actual

            if (x0 == x1 && y0 == y1)
                break; // Fin de la línea

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
    void SetLineDot(int x0, int y0, int x1, int y1, BYTE color)
    {
        bool drawable = true;
        int dx = abs(x1 - x0);
        int dy = abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
            if (drawable)
            {
                SetPixel(x0, y0, color); // Dibuja el píxel actual
            }
            drawable = !drawable;

            if (x0 == x1 && y0 == y1)
                break; // Fin de la línea

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
};
