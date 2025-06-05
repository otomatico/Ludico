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

    while (true)
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
}