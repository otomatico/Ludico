#include <stdio.h>
#include "../graphics.h"

int main()
{
    clear();
    /* Initialize/Setup */
    gfx bf;
    initGFX(&bf,10,10);
    putPixelGFX(&bf,4,0,RED);
    drawGFX(&bf,1,1);
    destroy_gfx(&bf);
    puts("\r\n");
    return 0;
}