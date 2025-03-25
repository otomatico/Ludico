/*
 * Version 1.0 => plasmar la idea de forma imperativa
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>   // srand, rand
#include <time.h>     // time
#include "lib/display.h"  // gotoxy, textcolor, textbackground
//#define LINUX       // Si vc qué compilar para linux, por defecto compila para windows
#include "lib/keyboard.h" // _kbhit, _getch, Sleep
// CONSTANTES
/*
    Existe elementos do codigo que se repite muitos.
    Assim que certas expressões uma vez que tudo funciona,
    convem reunirla a modo de "#define" o const.

    Para reduzir o maximo o tamanho do binario,
    coloquei funções e operações como constante.
    Como resultado reduci mais de 1Kb de codigo(dependendo de como compile).
*/
// Dimesão do monitor
#define SCREENWIDTH 80
#define SCREENHEIGHT 24

// Dimesão da Zona de Jogo
#define BOARDWIDTH 60
#define BOARDHEIGHT 10

#define pipeMAX 4
#define pipeGap 3
#define wide 15
// Hack -> Coloca o CURSOR na posição (X,Y) do Monitor e escrever uma letra ou cadena
#define DrawChar(x, y, c) printf("\e[%d;%dH%c", y, x, c)
#define DrawString(x, y, s) printf("\e[%d;%dH%s", y, x, s)
// Hack "Ultra Instinct" -> Somente mudar a cor de letra e fundo
#define colorGraphic(front, back) printf("\e[%d;%d;%dm", front & 0x10 ? 1 : 0, (back & 0xF) + (back & 0x10 ? 100 : 40), (front & 0xF) + 30)
// Hack "Poder maior de 8.000" -> devolver a cor por defeito do terminal
#define resetColor() printf("\e[0m")
// Hack "Eu ja escuto o teu sinal" -> repoduzir um pito/pitido
#define bell() printf("%c", 0x7)
// como o proprio nome indica
#define stepAnimation(step) ++animateCount > step

// Isso é mania minha eu gosto que short seja de 8bits
#define short signed char
#define bool unsigned char
#define false 0
#define true !false
/*
 O jogo ou o bucle de jogo funciona como uma MAQUINA DE ESTADOS.
 - [OPENING] com os creditos
 - [MENU] com o nome do jogo e a espera de [START] para dar começo
 - [START] o bucle principal do jogo se vc gaña Jogo [ENDING] senão [OVER]
 - [PAUSE] Para o "bucle principal" para dar un respiro
 - [OVER] Credito de Game Over e "Score"
 - [ENDING] Credito de Parabens vc Venceu
 - [EXIT] Fin do jogo, apaga o programa
*/
typedef enum 
{
    OPENING,
    MENU,
    START,
    PAUSE,
    OVER,
    ENDING,
    EXIT
}StateGame;
typedef struct
{
    short x;
    short y;
}Point;
// VARIABLE
Point bird = {0, 0};
Point pipe[pipeMAX];

short FPS = 3;
int score = 0;
short animateCount = 0;
bool drawable = false;
StateGame stateGame;

// INITIALIZE -> Todas as função de SETTER de variavel
// Gera posição do buraco
short generateGapePipe()
{
    short seed = rand() % (BOARDHEIGHT - pipeGap);
    return seed > 2 ? seed : 2;
}

// Inicializa elementos do jogo
void InitializeGame()
{
    for (short index = 0; index < pipeMAX; index++)
    {
        pipe[index].x = wide * (index + 1);
        pipe[index].y = generateGapePipe();
    }
    bird.x = 1;
    bird.y = BOARDHEIGHT / 2;
    score = 0;
}

void Initialize()
{
    srand(time(NULL));
    InitializeGame();
    stateGame = OPENING;
    animateCount = 0;
    drawable = true;
    hidecursor();
}
// DRAW -> Agora todas as função de desenho
void DrawBarVertical(short x, short y, short size, char c)
{
    for (short index = 0; index < size; index++)
    {
        DrawChar(x, y + index, c);
    }
}
void DrawBarHorizontal(short x, short y, short size, char c)
{
    gotoxy(x, y);
    for (short index = 0; index < size; index++)
    {
        printf("%c", c);
    }
}
void DrawWindow(short x, short y, short width, short height)
{
    height--;
    DrawChar(x, y, 0xc9);
    DrawBarHorizontal(x + 1, y, width - 1, 0xcd);
    DrawChar(x + width, y, 0xbb);
    DrawBarVertical(x + width, y + 1, height - 1, 0xba);
    DrawChar(x + width, y + height, 0xbc);
    DrawBarHorizontal(x + 1, y + height, width - 1, 0xcd);
    DrawChar(x, y + height, 0xc8);
    DrawBarVertical(x, y + 1, height - 1, 0xba);
}
void DrawPipe()
{
    for (short index = 0; index < pipeMAX; index++)
    {
        int Y = pipe[index].y + pipeGap;
        textcolor(GREEN);
        DrawBarVertical(pipe[index].x, 1, pipe[index].y, 0xdb);
        DrawBarVertical(pipe[index].x, Y, BOARDHEIGHT - Y, 0xdb);
    }
}
void DrawBird()
{
    textcolor(YELLOW);
    DrawChar(bird.x, bird.y, 0xfe); // Desenha o pássaro
    resetColor();
}
void DrawScore()
{
    gotoxy(BOARDWIDTH + 8, (BOARDHEIGHT / 2) + 1);
    printf("Score: %03d", score);
    resetColor();
}
void DrawBase()
{
    // Desenha o chao
    colorGraphic(LIGHTGREY, RED);
    DrawBarHorizontal(1, BOARDHEIGHT, BOARDWIDTH, 0xb2);
    resetColor();
    // Marco do Jogo
    textcolor(DARKGREY);
    // DrawWindow(BOARDWIDTH + 1,1,SCREENWIDTH - BOARDWIDTH - 1,BOARDHEIGHT);
    DrawBarHorizontal(BOARDWIDTH + 1, BOARDHEIGHT, SCREENWIDTH - BOARDWIDTH - 2, 0xdc);
    DrawBarVertical(BOARDWIDTH, 1, BOARDHEIGHT, 0xdb);
    DrawBarHorizontal(BOARDWIDTH + 1, 1, SCREENWIDTH - BOARDWIDTH - 2, 0xdf);
    DrawBarVertical(SCREENWIDTH - 1, 1, BOARDHEIGHT, 0xdb);
    // Score
    textcolor(LIGHTGREY);
    DrawScore();
}
void DrawGameOver()
{
    short Y = BOARDHEIGHT / 2;
    textcolor(LIGHTRED);
    DrawString(BOARDWIDTH + 6, Y, "Game Over!");
    textcolor(YELLOW);
    DrawString(BOARDWIDTH + 2, Y + 1, "Final");
    DrawScore();
    bell();
}
// Esta animação é un "plus" totalmente desnecessario, só que é divertido
// Desenha o flappy de Esquerda a Direita
void DrawFlappy(int x, int y)
{
    unsigned char birdChar[] = {
        0x20, 0x20, 0x20, 0x20, 0xdc, 0xdc, 0xdf, 0xdf, 0xdf, 0xdb, 0xdf, 0xdc, 0x20, 0x20, 0x20, 0x20,
        0x20, 0xdc, 0xdc, 0xdb, 0xdc, 0x20, 0x20, 0x20, 0xdb, 0x20, 0x20, 0xdc, 0xdf, 0xdc, 0x20, 0x20,
        0xdb, 0x20, 0x20, 0x20, 0x20, 0xdf, 0xdc, 0x20, 0xdf, 0xdc, 0x20, 0xdf, 0x20, 0xdb, 0x20, 0x20,
        0xdf, 0xdc, 0x20, 0x20, 0x20, 0xdc, 0xdf, 0x20, 0x20, 0xdc, 0xdf, 0xdf, 0xdf, 0xdf, 0xdf, 0xdc,
        0x20, 0x20, 0xdb, 0xdf, 0xdf, 0x20, 0x20, 0x20, 0xdf, 0xdc, 0xdf, 0xdf, 0xdf, 0xdf, 0xdb, 0x20,
        0x20, 0x20, 0x20, 0xdf, 0xdf, 0xdc, 0xdc, 0xdc, 0xdc, 0xdc, 0xdf, 0xdf, 0xdf, 0xdf, 0x20, 0x20};

    y += (x % 3) - 1;
    for (int i = 0; i < 6; i++)
    {
        printf("\e[%d;%dH%.*s", y, x, 16, birdChar + (i * 16));
        y++;
    }
}
void DrawCredits()
{
    short step = animateCount * 2;
    colorGraphic(BLUE, LIGHTBLUE);
    DrawBarHorizontal(1, 3, step, 0xdf);
    colorGraphic(GREEN, LIGHTGREEN);
    DrawBarHorizontal(1, 4, step, 0xb1);
    colorGraphic(WHITE, LIGHTGREEN);
    if (step <= 30)
    {
        resetColor();
    }
    textattr(BRIGHT);
    DrawString(31, 4, "RETRO");
    if (step <= 35)
    {
        resetColor();
    }
    DrawString(37, 4, "GAME");
    colorGraphic(YELLOW, LIGHTRED);
    DrawBarHorizontal(1, 5, step, 0xdf);
    textcolor(RED);
    DrawBarHorizontal(1, 6, step, 0xdf);
    resetColor();
    DrawFlappy(step, 2);
}
void DrawOpening()
{
    int color[] = {BLUE, LIGHTBLUE, GREEN, LIGHTGREEN, YELLOW, LIGHTYELLOW, LIGHTCYAN, CYAN};
    short Y = (BOARDHEIGHT - 2) / 2;
    textcolor(color[animateCount]);
    DrawWindow(10, Y - 1, BOARDWIDTH - 20, 4);
    DrawString((BOARDWIDTH - 19) / 2, Y, "Flappy Bird en C++"); // centra o texto de 19 letras
    DrawString((BOARDWIDTH - 13) / 2, Y + 1, "by Otomatico");   // centra o texto de 13 letras
    DrawString((BOARDWIDTH - 14) / 2, ((BOARDHEIGHT - 2) / 2) + 5, "Press any key");
    resetColor();
    DrawFlappy(animateCount * 5 + 5, SCREENHEIGHT - 12);
}
void DrawPause()
{
    textcolor(WHITE);
    short X = (BOARDWIDTH - 5) / 2;
    short Y = (BOARDHEIGHT - 1) / 2;
    DrawWindow(X - 5, Y - 1, 15, 3);
    DrawString(X, Y, "PAUSE");
}
void Draw()
{
    clear();
    if (stateGame == OPENING)
    {
        DrawCredits();
    }
    if (stateGame == MENU)
    {
        DrawOpening();
    }
    if (stateGame == START || stateGame == PAUSE || stateGame == OVER)
    {
        DrawBase();
        DrawPipe();
        DrawBird();
        if (stateGame == OVER)
        {
            DrawGameOver();
        }
        if (stateGame == PAUSE)
        {
            DrawPause();
        }
    }
    resetColor();
}

void LogicPause()
{
    drawable = false;
    if (_kbhit()) // Detecta si se pressionó uma tecla
    {
        char key = _getch(); // captura a tecla pressionada
        if (key == 0x1b)     // key==ESC
        {
            stateGame = START;
            drawable = true;
        }
    }
}
void LogicGameLoop()
{
    // MoveBird
    if (_kbhit())
    {
        char key = _getch();
        if (key == 0x1b) // key==ESC
        {
            stateGame = PAUSE;
        }
        else
        {
            if (key == 0x20 && bird.y > 1) // key==SPACE
            {
                bird.y -= 2; // Sube o pássaro
            }
        }
    }
    bird.y++; // Gravedade: o pássaro cae

    // CollidePipe
    for (short index = 0; index < pipeMAX; index++)
    {
        pipe[index].x--; // Move o tubo para esquerda
        if (pipe[index].x < 1)
        {
            pipe[index].x = pipeMAX * wide;     // Reinicia la posição d0 tubo
            pipe[index].y = generateGapePipe(); // Gera posição do buraco
            score++;                            // Aumenta os pontos
        }
        // FallenBird || Verifica colisão
        if (bird.y > (BOARDHEIGHT - 1) || (pipe[index].x == bird.x && !(pipe[index].y <= bird.y && bird.y < (pipe[index].y + pipeGap))))
        {
            stateGame = OVER;
        }
    }

    drawable = true;
}
void LogicMenu()
{
    FPS = 2;
    drawable = true;
    animateCount = (animateCount + 1) % 8;
    if (_kbhit())
    {
        InitializeGame();
        stateGame = START;
        animateCount = 0;
        FPS = 3;
    }
}

void Logic()
{
    switch (stateGame)
    {
    case OPENING:
        FPS = 5;
        drawable = true;
        if (stepAnimation(40))
        {
            stateGame = MENU;
            animateCount = 0;
        }
        break;
    case MENU:
        LogicMenu();
        break;
    case START:
        LogicGameLoop();
        break;
    case PAUSE:
        LogicPause();
        break;
    default:
        FPS = 1;
        if (stepAnimation(5))
        {
            stateGame = MENU;
            animateCount = 0;
            FPS = 3;
            drawable = true;
        }
        break;
    }
}

int main()
{
    Initialize();
    while (stateGame != EXIT)
    {
        if (drawable) // evitar parpadeo
        {
            Draw();
            drawable = false;
        }
        Logic();
        Sleep(1000 / FPS); // Controla a velocidade del Jogo
    }
    return 0;
}

/* Folha de Refencia de Letras
0x20 -> espaço
0xb0 -> bloque denso 25%
0xb1 -> bloque denso 50%
0xb2 -> bloque denso 75%
0xdb -> bloque denso 100%
0xdc -> bloque abajo
0xdf -> bloque arriba

0Xbf -> esquina sup der
0xc0 -> esquina inf izq
0xd9 -> esquina inf der
0xda -> esquina sup izq
0xc4 -> barra hor
0xb3 -> barra ver

0xfe -> cuadrado
*/