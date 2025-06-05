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
