# Game Design Document: Tetris en C++ (Terminal)

## **1. Información General**
- **Título**: Tetris Terminal  
- **Género**: Puzzle / Estrategia  
- **Plataforma**: Terminal de Windows 11 (Consola de comandos)  
- **Lenguaje**: C++  
- **Gráficos**: Caracteres ASCII/Unicode (Texto)  
- **Controles**: Teclado (Teclas direccionales y opciones personalizadas).  

---

## **2. Descripción del Juego**  
Tetris es un juego de puzzle donde los jugadores deben encajar piezas geométricas (*tetrominós*) que caen desde la parte superior de la pantalla. El objetivo es completar líneas horizontales sin huecos para eliminarlas y ganar puntos.

---

## **3. Mecánicas Principales**  
### **3.1. Sistema de Juego**  
- **Campo de Juego**: Matriz de caracteres (ejemplo: `20 filas x 10 columnas`).  
- **Piezas (Tetrominós)**:  
  - Representadas con caracteres (ejemplo: `[ ]` para bloques, colores opcionales con ANSI).  
  - Tipos: I, O, T, L, J, S, Z.  
- **Movimiento**:  
  - Caída automática (gravedad).  
  - Movimiento lateral (izquierda/derecha).  
  - Rotación (sentido horario/antihorario).  
  - "Hard drop" (caída instantánea).  

### **3.2. Puntuación**  
- Línea completada: +100 puntos.  
- Múltiples líneas: Bonus (ejemplo: 4 líneas = +800).  
- Niveles: La velocidad aumenta cada 10 líneas.  

### **3.3. Fin del Juego**  
- Cuando una pieza nueva colisiona con bloques existentes en la zona superior.  

---

## **4. Diseño Técnico**  
### **4.1. Estructuras de Datos**  
```cpp
// Ejemplo en C++:
struct Tetromino {
    char shape[4][4]; // Matriz de la pieza
    int x, y;         // Posición en el tablero
};

class Game {
    char board[20][10]; // Matriz del tablero
    int score, level;
    Tetromino currentPiece;
    // Métodos: update(), draw(), etc.
};
```
---

### 4.2. Bibliotecas
- <windows.h>: Para manejar la terminal (colores, input).

- <conio.h>: Para entrada de teclado (kbhit(), getch()).

### 4.3. Renderizado en Terminal
- Caracteres: Usar ■ (U+25A0) para bloques y (espacios) para vacío.

- Colores: Secuencias ANSI (ejemplo: \033[31m para rojo).

## 5. Interfaz de Usuario (UI)
### 5.1. Pantalla de Inicio
```
  T E T R I S  
  [1] Jugar  
  [2] Salir  
```

### 5.2. Durante el Juego
```
Puntuación: 1500   Nivel: 2  
┌────────────┐  
│            │  
│    [][]    │  
│   [][][]   │  
│            │  
└────────────┘  
Teclas: ← → ↓ (Rotar: ↑)  
```
---

## 6. Controles

- ← →: Mover pieza.
- ↓: Acelerar caída.
- ↑: Rotar.
- Espacio: Hard drop.
- P: Pausa.
---
## 7. Cronograma de Desarrollo
- Semana 1: Lógica básica (piezas, movimiento).

- Semana 2: Sistema de puntuación y colisiones.

- Semana 3: Interfaz y optimización.
---
## 8. Referencias
[Tetris Guideline](https://tetris.fandom.com/wiki/Tetris_Wiki)

[ANSI Escape Codes](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797)


---

### **Notas adicionales**:  
- Para colores en Windows, activa el soporte de ANSI en la terminal con:  
```cpp
  system("cls"); // Limpiar pantalla
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleMode(hConsole, ENABLE_VIRTUAL_TERMINAL_PROCESSING);
```

  Puedes usar #define para personalizar teclas y tamaños del tablero.