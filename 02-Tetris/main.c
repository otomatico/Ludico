#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> // usleep
#include "lib.h"
#include "joypad.h"

// Constantes
#define delay(a) usleep(a*1000);
#define uint8 unsigned char

#define hidecursor() printf("\033[?25l")
#define showcursor() printf("\033[?25h")
#define clear() printf("\e[1;1H\e[2J") // Limpiar pantalla
#define gotoxy(x, y) printf("\033[%d;%dH", y, x)

#define _FPS_ 2
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 16
#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 20

const uint8 SHAPES[7][4][4] =
	{
		{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 1, 1, 1}}, // SHAPE_I
		{{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}}, // SHAPE_J
		{{0, 0, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0}}, // SHAPE_L
		{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 1, 1, 0}}, // SHAPE_O
		{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 1, 1}, {0, 1, 1, 0}}, // SHAPE_S
		{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 1}, {0, 0, 1, 0}}, // SHAPE_T
		{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1}}  // SHAPE_Z
};

// Tipo de Dato
typedef struct pieceBase
{
	Point2D position;
	uint8 shape[4][4];
} Piece;

// Variables Globales
Joypad stick;
int FPS = _FPS_;
int screen[BOARD_HEIGHT][BOARD_WIDTH];
EnumGameState state = OPEN;
Piece currentPiece;
int score = 0;
int level = 0;
int animationCount = 0;
// Initiaze Elements

void initScreen()
{
	int row,col;
	for (row = 0; row < BOARD_HEIGHT; row++)
	{
		for (col = 0; col < BOARD_WIDTH; col++)
		{
			screen[row][col] = 0;
		}
	}
}

void copyPiece(uint8 shape[4][4], const uint8 other[4][4])
{
	memcpy(shape, other, sizeof(uint8) * 16);
}

void generateShape(uint8 shape[4][4])
{
	int index = rand() % 7;
	copyPiece(shape, SHAPES[index]);
}

// Draw Elements
void drawBar(int len, int type)
{
	int baseChar[]={
		196, // ─
		205, // ═
		176, // ░
		32 //" "
		};  
	for (int col = 0; col < len; col++)
	{
		printf("%c",baseChar[type]);
	}
}

void drawFrame(int x, int y, int w, int h)
{
	int baseChar[]={
		214, //╓
		191, //┐
		200, //╚
		190, //╛
		186, //║
		179 //│
	};
	gotoxy(x,y); printf("%c",baseChar[0]); drawBar(w-2, 0);	printf("%c",baseChar[1]);
	for (int row = 1; row < h-1; row++)
	{
		gotoxy(x,y+row); printf("%c",baseChar[4]);drawBar(w-2, 3); printf("%c",baseChar[5]);
	}
	gotoxy(x,y+h-1); printf("%c",baseChar[2]);drawBar(w-2, 1); printf("%c",baseChar[3]);

}

void drawOpenning()
{
	drawFrame(5,3,32,6);
	gotoxy(9, 4);printf("Cetris, Tetris make in C");
	gotoxy(13,7);printf("Press any key ");
}



void drawBackground()
{
	gotoxy(1, 1);
	for (int r = 0; r < SCREEN_HEIGHT; r++)
	{
		drawBar(SCREEN_WIDTH,2);
		printf("\n");
	}
	gotoxy(1, 1);
}

void drawBoard()
{
	uint8 pix = 0;
	drawFrame(1,1,BOARD_WIDTH+2,BOARD_HEIGHT+1);
	for (int row = 0; row < BOARD_HEIGHT; row++)
	{
		gotoxy(2,row+1);
		for (int col = 0; col < BOARD_WIDTH; col++)
		{
			pix = screen[row][col];
			printf("%s", pix ? "■" : "□");
		}
	}	
	gotoxy(SCREEN_WIDTH - 15, 1);
	printf("SCORE: %04d", score);
	gotoxy(SCREEN_WIDTH - 15, 2);
	printf("LEVEL:  %03d", level);
}

void drawPiece()
{
	uint8 pix;
	Point2D board = {2, 1};
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			pix = currentPiece.shape[row][col];
			if (pix)
			{
				int x = currentPiece.position.x + col + board.x;
				int y = currentPiece.position.y + row + board.y;
				gotoxy(x, y);
				printf("■");
			}
		}
	}
}

void drawPause()
{
	drawFrame(5,4,32,7);
	gotoxy(15, 5);	printf("Game Pause");
	gotoxy(13, 7);	printf("Press [ESC]  Exit");
	gotoxy(9, 8);	printf("Press [Intro] Reboot");
	gotoxy(9, 9);	printf("Press [Space] continue");

}

void drawGameOver()
{
	drawFrame(5,4,32,5);
	gotoxy(15, 5); 	printf("Game Over");
	gotoxy(11, 7);	printf("Press any continue");
}

// Logic
int insideBoard(int x, int y)
{
	return (0 <= x && x < BOARD_WIDTH) && (0 <= y && y < BOARD_HEIGHT);
}

int checkHit()
{
	uint8 square;
	int hit = 0;
	for (int row = 3; row >= 0 && !hit; row--)
	{
		for (int col = 0; col < 4 && !hit; col++)
		{
			square = currentPiece.shape[row][col];
			if (square)
			{
				int x = currentPiece.position.x + col;
				int y = currentPiece.position.y + row;
				hit = insideBoard(x, y) ? screen[y][x] != 0 : 1;
			}
		}
	}
	return hit;
}

void rotatePiece(int angle)
{
	int row, col;
	uint8 rotated[4][4];
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			if (angle == 1)
			{
				rotated[col][row] = currentPiece.shape[row][3 - col];
			}
			else if (angle == -1)
			{
				rotated[row][col] = currentPiece.shape[3 - col][row];
			}
		}
	}
	copyPiece(currentPiece.shape, rotated);
}

void movePiece()
{
	if (kbhit())
	{
		char key = getchar();
		Point2D oldPos = currentPiece.position;
		switch (key)
		{
		case KEY_PAUSE:
			state = PAUSE;
			break;
		case KEY_LEFT:
			currentPiece.position.x--;
			break;
		case KEY_RIGHT:
			currentPiece.position.x++;
			break;
		case KEY_DOWN:
			currentPiece.position.y++;
			break;
		case KEY_UP:
			rotatePiece(1);
			break;
		}
		if (checkHit())
		{
			currentPiece.position = oldPos;
			if (key == KEY_UP)
				rotatePiece(-1);
		}
	}
	currentPiece.position.y++;
}

void solidifyPiece()
{
	uint8 square;
	int x, y, row, col;

	currentPiece.position.y--;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			square = currentPiece.shape[row][col];
			if (square)
			{
				x = currentPiece.position.x + col;
				y = currentPiece.position.y + row;
				screen[y][x] = square;
			}
		}
	}
}

void dropDown(int row)
{
	while(row > 0)
	{
		memcpy(screen[row], screen[row - 1], sizeof(screen[row]));
		row--;
	}
	memset(screen[0], 0, sizeof(screen[0]));
}

int theyAreAllOne(int column[])
{
	int col, hasFull = 1;
	for (col = 0; col < BOARD_WIDTH && hasFull; col++)
	{
		hasFull = column[col];
	}
	return hasFull;
}

int clearRows()
{
	int cleared = 0, row = BOARD_HEIGHT - 1;
	while (row >= 0)
	{
		if (theyAreAllOne(screen[row]))
		{
			dropDown(row);
			cleared++;
		}else{
			row--;
		}
	}
	return cleared;
}

void generateNewPiece()
{
	currentPiece.position.x = 4;
	currentPiece.position.y = 0;
	generateShape(currentPiece.shape);
}

int isGameOver()
{
	return currentPiece.position.y == 0;
}

// Aproximación de Phi [https://www.masscience.com/una-aproximacion-de-la-razon-aurea-phi-%CF%86-por-medio-del-numero-de-euler-e/]
int fibonachiLevel(int points)
{
	int level = 1;
	if (points >= 100)
	{
		level = (int)((points / 100) * 1.618);
	}
	return level;
}

// Dibujar Pantalla
void draw()
{
	gotoxy(1, 1); // en lugar de clear(), esto evita parpadeos
	drawBackground();
	switch (state)
	{
	case OPEN:
		drawOpenning();
		break;
	case GAME:
	case OVER:
	case PAUSE:
		drawBoard();
		drawPiece();
		if(state == OVER){
			drawGameOver();
		}
		if(state == PAUSE){
			drawPause();
		}
		break;
	default:
		printf("...Error...");
	}
}

// Actualizar
int update()
{
	// Aplicando la de los Johns [Carmack|Romero]
	//  Si no haz modificado nado no hay que re-pintar
	int change = 0;
	switch (state)
	{
	case OPEN:

		if (kbhit() && getchar())
		{
			generateNewPiece();
			initScreen();
			state = GAME;
			change = 1;
			score = 0;
			level = 1;
		}
		break;
	case GAME:

		if (checkHit())
		{
			if (!isGameOver())
			{
				solidifyPiece();
				score += clearRows() * 100;
				level = fibonachiLevel(score);
				generateNewPiece();
			}else{
				state = OVER;
			}
		}else{
			if((animationCount++)%4==0){
				movePiece();
				animationCount=0;
			} 
				
		}
		change = 1;
		break;
	case PAUSE:

		if (kbhit())
		{
			switch (getchar())
			{
			case KEY_ESC:
				state = EXIT;
				break;
			case KEY_ENTER:
				state = OPEN;
				break;
			case KEY_PAUSE:
				state = GAME;
				break;
			}
			change = 1;
		}
		break;
	case OVER:
		if (kbhit())
		{
			state = OPEN;
		}
		break;
	}
	return change;
}

// Inicializar variables del juego
void init()
{
	initJoy(&stick);
	srand(time(NULL));
	state = OPEN;
	clear();
}

int main()
{
	int hasChange = 1;
	hidecursor();
	init();
	while (state != EXIT)
	{
		if (hasChange)
		{
			draw();
		}
		hasChange = update();
		delay(1000 / (FPS + level));
	}
	showcursor();
	return 0;
}
