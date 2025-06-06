// editor.h
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
