/*
 * Version 1.2 => Traduce la idea de forma imperativa a una orientada por Objeto
 */
//#define LINUX       // Si quieres compilar en linux, por defecto es para windows
#include "Game.hpp"
#define short signed char
/*
 El juego o bucle de juego funciona como una MÁQUINA DE ESTADOS.
 - [OPENING] con créditos
 - [MENU] con el nombre del juego y esperando [START] para iniciarlo
 - [START] el bucle principal del juego si ganas el juego [OVER] de lo contrario [ENDING]
 - [PAUSE] Detener el "bucle principal" para tomar un respiro
 - [OVER] Crédito y "Puntuación" de Game Over
 - [ENDING] Felicitaciones Crédito, Ganaste
 - [EXIT] Fin del juego, eliminar el programa
*/
int main()
{
    Game flappyBird;
    return flappyBird.Run();
}

/* Hoja de referencia de letras
0x20 -> espacio
0xb0 -> bloque denso 25%
0xb1 -> bloque denso 50%
0xb2 -> bloque denso 75%
0xdb -> bloque denso 100%
0xdc -> bloque de abajo
0xdf -> bloque arriba

0Xbf -> esquina superior
0xc0 -> esquina inf izq
0xd9 -> esquina inferior
0xda -> esquina sup izq
0xc4 -> barra de hor
0xb3 -> barra de visualización

0xfe -> cuadrado
*/