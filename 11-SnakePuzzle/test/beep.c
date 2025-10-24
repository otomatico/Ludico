//gcc -o ./test/beep.exe ./test/beep.c 
#include <windows.h>

int main() {
    Beep(440, 500); // 440 Hz por 0.5 segundos
    Beep(880, 500);
    return 0;
}
