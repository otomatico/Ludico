#include <stdio.h>
#include <wchar.h>

int main()
{
   wprintf (L"%c \n", L"\u2590");
   printf ("\e[0m");
   return 0;
}
/*
#include <stdio.h>
int main()
{
    /*
    ESC[=18h	640 x 480 color (16-color graphics)
    ESC[=19h	320 x 200 color (256-color graphics)


 ESC Code Sequence	Description
 ESC[38;5;{ID}m	Set foreground color.
 ESC[48;5;{ID}m	Set background color.

    printf("\e[=18h");
    //printf("\e(0");
    //printf("\e[?12h");
    printf("\e[16;500]");

    for (int c = 1; c < 256; c++)
    {
        printf("\e[38;5;%dm%03d ", c, c);//foreground
        }

       //printf("\e(b");
       //printf("\e[?12l");
       printf("\e[0m");
    return 0;
}
*/