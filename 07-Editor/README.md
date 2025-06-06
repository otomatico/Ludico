
## 📁 Estructura Proyecto
```
📁src
 ┣📄 editor.h        → definición de la estructura y funciones
 ┣📄 editor.c        → implementación principal del editor
 ┣📄 platform.c/h    → entrada por teclado multiplataforma (_kbhit, _getch)
 ┣📄 screen.c/h      → funciones para imprimir y posicionar
 ┖📄 main.c          → función principal
```

## 🛠 Compilar
Espero que uses gcc, si tienes pereza de escribir todo el comando teneis un ```build.bat```

```batch
gcc main.c editor.c platform.c screen.c -o editor.exe
```

## 💡 Mejoras
- Agregar mas comando como [RePag] y [AvPag] para **Retroceder** o **Avanzar** Pagina
- Mejorar los graficos en la barra de scroll vertical agregado ▲▼
- Agregar scroll horizontal y agregar mejora grafica(◄ ►)