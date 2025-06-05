#include "screen.h"
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
    printf(GRAY_BG WHITE "%c" DARK_FG, CHAR_TL);
    draw_char(CHAR_H, MAX_COLS);
    printf("%c" RESET, CHAR_TR);

    int title_pos = (MAX_COLS - (int)strlen(tb->filename)) / 2 + 2;
    gotoxy(title_pos, 1);
    printf(GRAY_BG DARK_FG " %s " RESET, tb->filename);

    // Cuerpo
    for (int i = 2; i < MAX_ROWS; ++i)
    {
        gotoxy(1, i);
        printf(GRAY_BG WHITE "%c" DARK_FG "%*s%c" RESET, CHAR_V, MAX_COLS, "", CHAR_V);
    }

    // Pie
    gotoxy(1, MAX_ROWS);
    printf(GRAY_BG WHITE "%c", CHAR_BL);
    draw_char(CHAR_H, MAX_COLS);
    printf(DARK_FG "%c" RESET, CHAR_BR);
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
        state.scroll_offset = cursorY;
    else if (cursorY >= state.scroll_offset + MAX_ROWS - 2)
        state.scroll_offset = cursorY - 20;
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