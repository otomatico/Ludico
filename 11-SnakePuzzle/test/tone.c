#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
void beep_tone(int freq, int dur)
{
    Beep(freq, dur); // frecuencia en Hz, duración en milisegundos
}
#else
#include <unistd.h>
void beep_tone(int freq, int dur)
{
    // Requiere el programa 'beep' instalado: sudo apt install beep
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "beep -f %d -l %d", freq, dur);
    system(cmd);
}
#endif

typedef struct
{
    int freq; // frecuencia en Hz
    int dur;  // duración en ms
} Note;

int main()
{
    /*
    Note escala[] = {
        {261, 400}, // DO
        {293, 400}, // RE
        {329, 400}, // MI
        {349, 400}, // FA
        {392, 400}, // SOL
        {440, 400}, // LA
        {493, 400}, // SI
        {523, 600}, // DO (más alto)
    };

    int n = sizeof(escala) / sizeof(escala[0]);
    */
    Note starwars[] = {
        {440, 500}, {440, 500}, {440, 500},
        {349, 350}, {523, 150},
        {440, 500}, {349, 350}, {523, 150}, {440, 1000},
        {659, 500}, {659, 500}, {659, 500},
        {698, 350}, {523, 150},
        {415, 500}, {349, 350}, {523, 150}, {440, 1000}
    };
    int n = sizeof(starwars) / sizeof(starwars[0]);

    Note *escala = starwars;
    wprintf(L"🎵 Reproduciendo musica...\n");
    for (int i = 0; i < n; i++)
    {
        beep_tone(escala[i].freq, escala[i].dur);
    }

    wprintf(L"✅ ¡Melodía terminada!\n");
    return 0;
}
