
#ifndef _UTILS_H_
#define _UTILS_H_
// Hack -> Coloca o CURSOR na posição (X,Y) do Monitor e escrever uma letra ou cadena
#define DrawChar(x, y, c) printf("\e[%d;%dH%c", y, x, c)
#define DrawString(x, y, s) printf("\e[%d;%dH%s", y, x, s)
// Hack -> Somente mudar a cor de letra e fundo
#define colorGraphic(front, back) printf("\e[%d;%d;%dm", front & 0x10 ? 1 : 0, (back & 0xF) + (back & 0x10 ? 100 : 40), (front & 0xF) + 30)
// Hack -> devolver a cor por defeito do terminal
#define resetColor() printf("\e[0m")
// Hack -> repoduzir um pito/pitido
#define bell() printf("%c", 0x7)

#include "Utils.cpp"  // gotoxy, textcolor, textbackground

void DrawBarVertical(short x, short y, short size, char c);
void DrawBarHorizontal(short x, short y, short size, char c);
void DrawWindow(short x, short y, short width, short height);
#endif