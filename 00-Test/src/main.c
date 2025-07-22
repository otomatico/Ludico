#include <nesdoug.h> // Usaremos la librería de nesdoug, que es similar a neslib.
#include <neslib.h>
#include <bank_helpers.h> // Para manejar la memoria del juego.
#include <peekpoke.h> // Para leer y escribir en la memoria.

// La tabla de paleta de colores. Cada color es un valor hexadecimal.
const unsigned char palette[] = {
    0x0F, // Color de fondo.
    0x17, 0x27, 0x37, // Paleta de colores para los sprites.
    0x0F, 0x18, 0x28, 0x38, // Paleta de colores para los tiles (objetos del fondo).
    0x0F, 0x1A, 0x2A, 0x3A,
    0x0F, 0x1B, 0x2B, 0x3B
};

// La posición inicial de nuestro personaje (el sprite).
unsigned char sprite_x = 120;
unsigned char sprite_y = 100;

// Aquí va el código principal del juego.
void main(void) {
    // Apaga el PPU (Picture Processing Unit) para preparar la pantalla.
    ppu_off();

    // Carga la paleta de colores.
    pal_spr(palette);
    pal_bg(palette);

    // Carga los gráficos del personaje (sprite).
    // Aquí cargaríamos los datos de los gráficos, pero para este ejemplo,
    // vamos a usar el gráfico por defecto que tiene nesdoug.
    // normal_sprite_tiles();

    // Enciende el PPU para mostrar la pantalla.
    ppu_on_all();
    
    // Este es el bucle principal del juego.
    // Se ejecuta infinitamente.
    while (1) {
        // Obtenemos la entrada del mando (el "joystick").
        pad_poll(0); // El 0 es el primer mando.
        
        // Si el botón de "derecha" está presionado y no nos salimos de la pantalla.
        if (pad_poll(0) & PAD_RIGHT) {
            sprite_x++; // Incrementa la posición x.
        }
        
        // Si el botón de "izquierda" está presionado.
        if (pad_poll(0) & PAD_LEFT) {
            sprite_x--;
        }
        
        // Si el botón de "arriba" está presionado.
        if (pad_poll(0) & PAD_UP) {
            sprite_y--;
        }
        
        // Si el botón de "abajo" está presionado.
        if (pad_poll(0) & PAD_DOWN) {
            sprite_y++;
        }
        
        // Actualiza el sprite en la pantalla con la nueva posición.
        oam_meta_spr(sprite_x, sprite_y, 0, 0); // La última variable es el índice del gráfico del sprite.
        
        // Espera al V-Blank (el momento en que la pantalla se actualiza).
        // Esto es crucial para que el juego funcione sin "parpadeos".
        ppu_wait_nmi();
    }
}