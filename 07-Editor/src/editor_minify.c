//platform.h 
#ifndef PLATFORM_H
#define PLATFORM_H

int _kbhit();
int _getch();

#endif
//screen.h 
#ifndef SCREEN_H
#define SCREEN_H

#include "editor.h"

void first_print_screen(const TextBuffer *tb, int cursorX, int cursorY);
void print_screen(const TextBuffer *tb, int cursorX, int cursorY);

#endif
//editor.h 
#ifndef EDITOR_H
#define EDITOR_H

#include <stdbool.h>

#define MAX_LINES 1000
#define MAX_COLS 118
#define MAX_ROWS 24
#define MAX_LEN_FILENAME 256

typedef struct {
    char filename[MAX_LEN_FILENAME];
    char lines[MAX_LINES][MAX_COLS+2];
    int num_lines;
    bool modified;
} TextBuffer;

void load_file(TextBuffer *tb, const char *filename);
void save_file(TextBuffer *tb);
void edit(TextBuffer *tb);

#endif
//platform.c 
#include "platform.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

static int _initialized = 0;

void init_term()
{
    struct termios newt;
    tcgetattr(STDIN_FILENO, &newt);
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);
    _initialized = 1;
}

int _kbhit()
{
    if (!_initialized) init_term();
    int ch = getchar();
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int _getch()
{
    return getchar();
}
#endif
//screen.c 
//#include "screen.h"
#include <stdio.h>
#include <string.h>

#define clear() printf("\033[2J")
#define gotoxy(x, y) printf("\033[%d;%dH", y, x)
#define showcursor() printf("\033[?25h")
#define hidecursor() printf("\033[?25l")

#define INVERT "\033[7m"
#define RESET "\033[0m"
#define GRAY_BG "\033[48;5;250m"
#define COUNT_BG "\033[48;5;242m"
#define DARK_FG "\033[38;5;235m"
#define SCROLL_BG "\033[48;5;252m"
#define SCROLL_FG "\033[38;5;238m"
#define WHITE "\033[38;5;15m"

#define CHAR_TL 218
#define CHAR_TR 191
#define CHAR_BL 192
#define CHAR_BR 217
#define CHAR_H 196
#define CHAR_V 179
#define CHAR_HS 0xB2
#define CHAR_MS 0xB1
#define CHAR_SS 0xB0

#define draw_char(character, length) \
    for (int i = 0; i < length; i++) \
    printf("%c", character)

typedef struct
{
    int lastCursorY;
    int lastX;
    int lastY;
    int lastModified;
    int lastLineCount;
    int scroll_offset;
} ScreenState;

static ScreenState state = {-1, -1, -1, -1, -1, 0};

void draw_scrollbar(const TextBuffer *tb)
{
    int visible_lines = MAX_ROWS - 2;
    int bar_top = 2;
    int bar_height = visible_lines;

    for (int i = 0; i < bar_height; ++i)
    {
        gotoxy(MAX_COLS + 2, bar_top + i);
        printf(SCROLL_BG WHITE "%c" RESET, CHAR_HS);
    }

    if (tb->num_lines <= visible_lines)
        return;

    int bar_pos = (state.scroll_offset * bar_height) / tb->num_lines;
    int bar_len = (visible_lines * visible_lines) / tb->num_lines;
    if (bar_len < 1)
        bar_len = 1;

    for (int i = 0; i < bar_len; ++i)
    {
        gotoxy(MAX_COLS + 2, bar_top + bar_pos + i);
        printf(SCROLL_BG SCROLL_FG "%c" RESET, CHAR_MS);
    }
}

void draw_frame(const TextBuffer *tb)
{
    // Cabecera
    gotoxy(1, 1);
    printf(GRAY_BG WHITE "%c", CHAR_TL);
    draw_char(CHAR_H, MAX_COLS + 1);
    printf(DARK_FG "%c" RESET, CHAR_TR);

    int title_pos = (MAX_COLS - (int)strlen(tb->filename)) / 2 + 2;
    gotoxy(title_pos, 1);
    printf(GRAY_BG DARK_FG " %s " RESET, tb->filename);

    // Cuerpo
    for (int i = 2; i < MAX_ROWS; ++i)
    {
        gotoxy(1, i);
        printf(GRAY_BG WHITE "%c" DARK_FG " %*s%c" RESET, CHAR_V, MAX_COLS, "", CHAR_V);
    }

    // Pie
    gotoxy(1, MAX_ROWS);
    printf(GRAY_BG WHITE "%c" DARK_FG, CHAR_BL);
    draw_char(CHAR_H, MAX_COLS + 1);
    printf("%c" RESET, CHAR_BR);
}

void draw_footer(const TextBuffer *tb, int x, int y)
{
    gotoxy(2, MAX_ROWS);
    printf(GRAY_BG DARK_FG "[Ctrl+G: Guardar | ESC: Salir] [ Linea %d, Columna %d ] [ Lineas: %d ] [ %s ]" RESET,
           y + 1, x + 1, tb->num_lines, tb->modified ? "Modificado [*]" : " Guardado [OK]");
}

void draw_line(const TextBuffer *tb, int y_offset, int index, int highlight)
{
    gotoxy(2, index + y_offset);
    if (highlight)
        printf(INVERT);
    printf(COUNT_BG DARK_FG "%3d " GRAY_BG " %-*s%s" RESET,
           state.scroll_offset + index + 1,
           MAX_COLS - 5,
           tb->lines[state.scroll_offset + index],
           highlight ? RESET : "");
}

void draw_visible_lines(const TextBuffer *tb, int y_offset, int cursorY)
{
    for (int i = 0; i < MAX_ROWS - 2; ++i)
    {
        int index = state.scroll_offset + i;
        if (index < tb->num_lines)
            draw_line(tb, y_offset, i, index == cursorY);
        else
        {
            gotoxy(2, i + y_offset);
            printf(GRAY_BG DARK_FG "%*s" RESET, MAX_COLS, "");
        }
    }
    draw_scrollbar(tb);
}

void update_scroll(int cursorY)
{
    if (cursorY < state.scroll_offset)
    {
        state.scroll_offset = cursorY;
    }
    else if (cursorY >= state.scroll_offset + MAX_ROWS - 2)
    {
        state.scroll_offset = cursorY - MAX_ROWS + 3;
    }
}

void update_cursor_and_state(int cursorX, int cursorY, const TextBuffer *tb)
{
    gotoxy(cursorX + 7, (cursorY - state.scroll_offset) + 2);
    state.lastCursorY = cursorY;
    state.lastX = cursorX;
    state.lastY = cursorY;
    state.lastModified = tb->modified;
    state.lastLineCount = tb->num_lines;
}

void first_print_screen(const TextBuffer *tb, int cursorX, int cursorY)
{
    clear();
    draw_frame(tb);
    draw_visible_lines(tb, 2, cursorY);
    draw_footer(tb, cursorX, cursorY);
}
void print_screen(const TextBuffer *tb, int cursorX, int cursorY)
{
    int y_offset = 2;

    update_scroll(cursorY);
    hidecursor();

    bool redraw_all =
        state.lastLineCount != tb->num_lines ||
        state.lastCursorY != cursorY ||
        cursorY < state.scroll_offset ||
        cursorY >= state.scroll_offset + MAX_ROWS - 2;

    if (redraw_all)
    {
        draw_visible_lines(tb, y_offset, cursorY);
    }
    else if (tb->modified && cursorY >= 0 && cursorY < tb->num_lines)
    {
        draw_line(tb, y_offset, cursorY - state.scroll_offset, 1);
    }

    if (cursorX != state.lastX || cursorY != state.lastY ||
        tb->modified != state.lastModified ||
        tb->num_lines != state.lastLineCount)
    {
        draw_footer(tb, cursorX, cursorY);
    }

    showcursor();
    update_cursor_and_state(cursorX, cursorY, tb);
}
//editor.c 
#include "editor.h"
#include "platform.h"
#include "screen.h"
#include <stdio.h>
#include <string.h>

// Key codes - mejor organizados en un enum
typedef enum
{
    KEY_ESC = 27,
    KEY_ENTER = 13,
    KEY_BACKSPACE = 8,
    KEY_CTRL_G = 7,
    KEY_ARROW_UP = 72,
    KEY_ARROW_DOWN = 80,
    KEY_ARROW_LEFT = 75,
    KEY_ARROW_RIGHT = 77,
    KEY_DELETE = 83,
    KEY_HOME = 71,
    KEY_END = 79,
    EXTENDED_KEY_PREFIX1 = 224,
    EXTENDED_KEY_PREFIX2 = 0
} KeyCodes;

// Funciones helper implementación
static void handle_backspace(TextBuffer *tb, int *x, int *y)
{
    if (*x > 0)
    {
        // Borrar caracter en posición actual
        memmove(&tb->lines[*y][*x - 1], &tb->lines[*y][*x], strlen(&tb->lines[*y][*x]) + 1);
        (*x)--;
        tb->modified = true;
    }
    else if (*y > 0)
    {
        // Unir línea actual con la anterior
        int prev_len = strlen(tb->lines[*y - 1]);
        if (prev_len + strlen(tb->lines[*y]) < MAX_COLS)
        {
            strcat(tb->lines[*y - 1], tb->lines[*y]);
            // Mover líneas hacia arriba
            for (int i = *y; i < tb->num_lines - 1; i++)
            {
                strcpy(tb->lines[i], tb->lines[i + 1]);
            }
            tb->num_lines--;
            (*y)--;
            *x = prev_len;
            tb->modified = true;
        }
    }
}

static void handle_enter(TextBuffer *tb, int *x, int *y)
{
    if (tb->num_lines >= MAX_LINES)
        return;

    // Mover líneas hacia abajo para hacer espacio
    for (int i = tb->num_lines; i > *y + 1; i--)
    {
        strcpy(tb->lines[i], tb->lines[i - 1]);
    }

    // Dividir la línea
    strcpy(tb->lines[*y + 1], &tb->lines[*y][*x]);
    tb->lines[*y][*x] = '\0';
    (*y)++;
    *x = 0;
    tb->num_lines++;
    tb->modified = true;
}

static void handle_printable_char(TextBuffer *tb, int key, int *x, int *y)
{
    int len = strlen(tb->lines[*y]);
    if (len >= MAX_COLS - 1)
        return;

    // Hacer espacio para el nuevo carácter
    for (int i = len; i >= *x; i--)
    {
        tb->lines[*y][i + 1] = tb->lines[*y][i];
    }
    tb->lines[*y][*x] = (char)key;
    (*x)++;
    tb->modified = true;
}

static void handle_delete(TextBuffer *tb, int *x, int y)
{
    int len = strlen(tb->lines[y]);
    if (*x < len)
    {
        memmove(&tb->lines[y][*x], &tb->lines[y][*x + 1], len - *x);
        tb->modified = true;
    }
    else if (y < tb->num_lines - 1)
    {
        if (strlen(tb->lines[y]) + strlen(tb->lines[y + 1]) < MAX_COLS)
        {
            strcat(tb->lines[y], tb->lines[y + 1]);
            for (int i = y + 1; i < tb->num_lines - 1; i++)
            {
                strcpy(tb->lines[i], tb->lines[i + 1]);
            }
            tb->num_lines--;
            tb->modified = true;
        }
    }
}

static void adjust_x_position(TextBuffer *tb, int *x, int y)
{
    int line_len = strlen(tb->lines[y]);
    if (*x > line_len)
    {
        *x = line_len;
    }
}

static void handle_arrow_keys(TextBuffer *tb, int key, int *x, int *y)
{
    switch (key)
    {
    case KEY_ARROW_UP:
        if (*y > 0)
            (*y)--;
        adjust_x_position(tb, x, *y);
        break;

    case KEY_ARROW_DOWN:
        if (*y < tb->num_lines - 1)
            (*y)++;
        adjust_x_position(tb, x, *y);
        break;

    case KEY_ARROW_LEFT:
        if (*x > 0)
        {
            (*x)--;
        }
        else if (*y > 0)
        {
            (*y)--;
            *x = strlen(tb->lines[*y]);
        }
        break;

    case KEY_ARROW_RIGHT:
        if (*x < strlen(tb->lines[*y]))
        {
            (*x)++;
        }
        else if (*y < tb->num_lines - 1)
        {
            (*y)++;
            *x = 0;
        }
        break;

    case KEY_DELETE:
        handle_delete(tb, x, *y);
        break;

    case KEY_HOME:
        *x = 0;
        break;

    case KEY_END:
        *x = strlen(tb->lines[*y]);
        break;
    }
}

void load_file(TextBuffer *tb, const char *filename)
{
    if (!filename || !tb)
        return;

    FILE *f = fopen(filename, "r");
    if (!f)
        return;

    // Manejo seguro del nombre de archivo
    strncpy(tb->filename, filename, sizeof(tb->filename) - 1);
    tb->filename[sizeof(tb->filename) - 1] = '\0';

    tb->modified = false;
    tb->num_lines = 0;

    while (fgets(tb->lines[tb->num_lines], MAX_COLS, f) && tb->num_lines < MAX_LINES)
    {
        // Eliminar el salto de línea si existe
        char *newline = strchr(tb->lines[tb->num_lines], '\n');
        if (newline)
            *newline = '\0';
        tb->num_lines++;
    }

    fclose(f);
}

void save_file(TextBuffer *tb)
{
    if (!tb || !tb->filename[0])
        return;

    FILE *f = fopen(tb->filename, "w");
    if (!f)
        return;

    for (int i = 0; i < tb->num_lines; ++i)
    {
        fprintf(f, "%s\n", tb->lines[i]);
    }

    fclose(f);
    ((TextBuffer *)tb)->modified = false;
}

void edit(TextBuffer *tb)
{
    if (!tb)
        return;

    int cursor_x = 0, cursor_y = 0;
    first_print_screen(tb, cursor_x, cursor_y);

    while (1)
    {
        if (_kbhit())
        {
            int key = _getch();

            if (key == KEY_ESC)
            {
                break;
            }
            else if (key == KEY_CTRL_G)
            {
                save_file(tb);
            }
            else if (key == KEY_BACKSPACE)
            {
                handle_backspace(tb, &cursor_x, &cursor_y);
            }
            else if (key == KEY_ENTER)
            {
                handle_enter(tb, &cursor_x, &cursor_y);
            }
            else if (key >= 32 && key <= 126)
            { // Caracteres imprimibles
                handle_printable_char(tb, key, &cursor_x, &cursor_y);
            }
            else if (key == EXTENDED_KEY_PREFIX1 || key == EXTENDED_KEY_PREFIX2)
            {
                int extended_key = _getch();
                handle_arrow_keys(tb, extended_key, &cursor_x, &cursor_y);
            }

            print_screen(tb, cursor_x, cursor_y);
        }
    }
}//main.c 
#include "editor.h"
#include <string.h>

int main(int argc, char *argv[])
{
    TextBuffer tb;
    const char *filename = "archivo_guardado.txt";

    if (argc > 1)
        filename = argv[1];

    load_file(&tb, filename);
    edit(&tb);
    return 0;
}
