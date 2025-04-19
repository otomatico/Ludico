#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_
#ifdef LINUX
// Codigo para LINUX
#include <unistd.h> //
#define Sleep(a) usleep(a * 1000)

#include <fcntl.h>
#include <termios.h>
// Captura de teclado
static int _initialized_ = 0;
void init_kbhit()
{
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
    _initialized_ = 1;
}

int _kbhit()
{
    if (!_initialized_)
    {
        init_kbhit();
    }
    int ch = getchar();
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
}
#else
// Codigo para WINDOWS
#include <conio.h>   // _kbhit y _getch
#include <windows.h> //Sleep
#endif

// Constante para GamePAD
//#define KEY_UP 38
//#define KEY_DOWN 39
//#define KEY_RIGHT 40
//#define KEY_LEFT 41

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

#define KEY_ESC 27
#define KEY_SPACE 32
#define KEY_ENTER 10
#define KEY_A 97
#define KEY_B 115
#define KEY_EMPTY 0

int press()
{
    int key = KEY_EMPTY;
    if (_kbhit())
    {
        key = _getch();
        switch (key)
        {
            case KEY_UP:
            case KEY_DOWN:
            case KEY_RIGHT:
            case KEY_LEFT:
            case KEY_ESC:
            case KEY_SPACE:
            case KEY_ENTER:
            case KEY_A:
            case KEY_B:
                break;
            default:
                key = KEY_EMPTY;
                break;
        }
    }
    return key;
}
#endif