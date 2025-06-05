#include "editor.h"
#include "platform.h"
#include "screen.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>  // Uso de bool

// Key codes
#define KEY_ESC        27
#define KEY_ENTER      13
#define KEY_BACKSPACE   8
#define KEY_CTRL_G      7
#define KEY_ARROW_UP   72
#define KEY_ARROW_DOWN 80
#define KEY_ARROW_LEFT 75
#define KEY_ARROW_RIGHT 77
#define KEY_DELETE     83
#define KEY_HOME       71
#define KEY_END        79

void load_file(TextBuffer *tb, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;

    strncpy(tb->filename, filename, sizeof(tb->filename));
    tb->filename[sizeof(tb->filename) - 1] = '\0';
    tb->modified = false;
    tb->num_lines = 0;

    while (fgets(tb->lines[tb->num_lines], MAX_COLS, f) && tb->num_lines < MAX_LINES) {
        size_t len = strlen(tb->lines[tb->num_lines]);
        if (len > 0 && tb->lines[tb->num_lines][len - 1] == '\n')
            tb->lines[tb->num_lines][len - 1] = '\0';
        tb->num_lines++;
    }
    fclose(f);
}

void save_file(TextBuffer *tb) {
    FILE *f = fopen(tb->filename, "w");
    if (!f) return;

    for (int i = 0; i < tb->num_lines; ++i)
        fprintf(f, "%s\n", tb->lines[i]);

    fclose(f);
    tb->modified = false;
}

void edit(TextBuffer *tb) {
    int x = 0, y = 0;
    first_print_screen(tb, x, y);

    while (true) {
        if (_kbhit()) {
            int key = _getch();

            if (key == KEY_ESC)
                break;
            else if (key == KEY_CTRL_G) {
                save_file(tb);
            }
            else if (key == KEY_BACKSPACE) {
                if (x > 0) {
                    memmove(&tb->lines[y][x - 1], &tb->lines[y][x], strlen(&tb->lines[y][x]) + 1);
                    x--;
                    tb->modified = true;
                } else if (y > 0) {
                    int len_prev = strlen(tb->lines[y - 1]);
                    if (len_prev + strlen(tb->lines[y]) < MAX_COLS) {
                        strcat(tb->lines[y - 1], tb->lines[y]);
                        for (int i = y; i < tb->num_lines - 1; i++)
                            strcpy(tb->lines[i], tb->lines[i + 1]);
                        tb->num_lines--;
                        y--;
                        x = len_prev;
                        tb->modified = true;
                    }
                }
            }
            else if (key == KEY_ENTER) {
                if (tb->num_lines < MAX_LINES) {
                    for (int i = tb->num_lines; i > y + 1; i--)
                        strcpy(tb->lines[i], tb->lines[i - 1]);

                    strcpy(tb->lines[y + 1], &tb->lines[y][x]);
                    tb->lines[y][x] = '\0';
                    y++; x = 0;
                    tb->num_lines++;
                    tb->modified = true;
                }
            }
            else if (key >= 32 && key <= 126) {
                int len = strlen(tb->lines[y]);
                if (len < MAX_COLS - 1) {
                    for (int i = len; i >= x; i--)
                        tb->lines[y][i + 1] = tb->lines[y][i];
                    tb->lines[y][x] = (char)key;
                    x++;
                    tb->modified = true;
                }
            }
            else if (key == 224 || key == 0) {
                key = _getch();
                switch (key) {
                    case KEY_ARROW_UP:
                        if (y > 0) y--;
                        if (x > strlen(tb->lines[y])) x = strlen(tb->lines[y]);
                        break;
                    case KEY_ARROW_DOWN:
                        if (y < tb->num_lines - 1) y++;
                        if (x > strlen(tb->lines[y])) x = strlen(tb->lines[y]);
                        break;
                    case KEY_ARROW_LEFT:
                        if (x > 0) x--;
                        else if (y > 0) {
                            y--;
                            x = strlen(tb->lines[y]);
                        }
                        break;
                    case KEY_ARROW_RIGHT:
                        if (x < strlen(tb->lines[y])) x++;
                        else if (y < tb->num_lines - 1) {
                            y++;
                            x = 0;
                        }
                        break;
                    case KEY_DELETE: {
                        int len = strlen(tb->lines[y]);
                        if (x < len) {
                            memmove(&tb->lines[y][x], &tb->lines[y][x + 1], len - x);
                            tb->modified = true;
                        } else if (y < tb->num_lines - 1) {
                            if (strlen(tb->lines[y]) + strlen(tb->lines[y + 1]) < MAX_COLS) {
                                strcat(tb->lines[y], tb->lines[y + 1]);
                                for (int i = y + 1; i < tb->num_lines - 1; i++)
                                    strcpy(tb->lines[i], tb->lines[i + 1]);
                                tb->num_lines--;
                                tb->modified = true;
                            }
                        }
                        break;
                    }
                    case KEY_HOME:
                        x = 0;
                        break;
                    case KEY_END:
                        x = strlen(tb->lines[y]);
                        break;
                }
            }
            print_screen(tb, x, y);
        }
    }
}
