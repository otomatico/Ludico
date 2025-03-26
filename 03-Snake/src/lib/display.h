#ifndef _DISPLAY_H_
#define _DISPLAY_H_
/*************************Como desenhar com cores*****************************/
/* Atributos */
#define RESETATTR 0
#define BRIGHT 1
#define DIM 2
#define ITALIC 3
#define UNDERSCORE 4
#define BLINK 5 /* Puede que não funcione em todos os monitores*/
#define REVERSE 7
#define HIDDEN 8
/* Cores para textos e fundos */
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

#define hidecursor() printf("\e[?25l")
#define showcursor() printf("\e[?25h")
#define clear() printf("\e[1;1H\e[2J")
//#define clear() system("cls")

#define gotoxy(x, y) printf("\e[%d;%dH", (int)y, (int)x)

#define textattr(attr) __set_gm(attr, 0, 0)
#define textcolor(color) __set_gm(RESETATTR, color, 30)
#define textbackground(color) __set_gm(RESETATTR, color, 40)
void __set_gm(int attr, int color, int val)
{
    if (!color)
        printf("\e[%dm", attr);
    else
        printf("\e[%d;%dm", color & 0x10 ? 1 : 0, (color & 0xF) + val);
}

/**************************************************************************/
/*  IMPORTANTE: Os Monitores da Terminal/Consola são de 80x24 caracteres  */
/**************************************************************************/
#endif