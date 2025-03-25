
# Game Design 📚
>Clon de Flappy Bird

## 🏁 Introdución
El objetivo de este proyecto es crear un clon funcional del popular juego *Flappy Bird*, con el fin de aprender y practicar conceptos de desarrollo de videojuegos, como la física básica, la gestión de colisiones, la generación procedural de obstáculos y la implementación de una interfaz de usuario sencilla.

## 💸 Monetización

No se espera ganar dinero con este proyecto, se trata de una prueba de conceptos y un MVP (Producto Minimo viable).

## ⚙️Mecánicas y Elementos de Juego

### Mecanica del entorno

- Se trata de un entorno en 2d donde el personaje (pajaro) tiene que esquivar el mayor numero de obstaculos (tuberia) y evitar caerse en el suelo.
- Los obstaculos se generan de forma aleatoria a una distancia fija unas de la otras y con un hueco fijo para que pueda pasar el personaje.
- Existe menus para gestionar el inicio del juego, salir del juego y para pausarlo.

#### Menú Principal
- **Iniciar Juego**: El jugador puede comenzar una nueva partida.
- **Salir**: Cierra la aplicación.

#### Menú de Pausa
- **Pausa**: El jugador puede parar el juego presionando una tecla (ESC) y presionando la misma tecla para continuar la partida.

### Mecánica del Juego
- El personaje estará en todo momento cayendose (gravedad) por lo que habrá que presionar una tecla para que vuele.
- El personaje se mantiene en una posicion "X" fija en el lado izquierdo de la pantalla encuanto los obstaculos se mueve de derecha a izquierda hacia el personaje.
- Cada vez que el personaje pasa con exito un obstaculo se incrementa los puntos.

### Elementos

De las mecanicas se intuye que tendremos un numero fijo y constante de obstaculos que al desaparecer por la izquierda de la zona de dibujado seran redibujado a la derecha con medidas distintas. ```Obstáculos```

El personaje subira una cantidade de pixeles cada vez que se presione la tecla de vuelo (Espacio) y caerá caso contrario. ```Control del Personaje```

Si el personaje choca con un obstaculo, o toca el suelo, el juego termina. ```Colisión```

Los puntos se incrementa cada vez que el personaje pasa con exito un obstaculo. ```Puntuación```

#### Diagrama de Flujo
Se incluí un diagrama de flujo simple, que muestra el flujo del juego, desde el menú principal hasta el final de la partida.

 ```mermaid
graph TD;
    A((Inicio))
    A --> B[/Menú Principal\]
    B -.-> |Salir| S((Fin))
    B -.-> |Jugar| C[Iniciar Juego]
    C --> D[[Mecánica del Juego]]
    D --> E{"¿Colisión?"}
    E --> |No| D
    E --> |Sí| F[Fin del Juego]
    F --> B
```


## 💻 Tecnologías Utilizadas
- **Lenguaje de Programación**: C/C++
- **Plataforma**: PC.
- **Resolución**: 192x192, Estilo pixel art similar al juego original.


## ⚙️ Configuración del Proyecto

Para configurar y ejecutar este proyecto en tu entorno de desarrollo:
Se recomienda Visual Studio CODE o Notepad++ con el plugin NPPExec

Encontrareis un archivo de lote ```build.bat``` que tiene configurado la ubicacion del compilador gcc y las instrucciones de compilacion tanto en debug como en release.

En la carpeta ```lib``` encontraras los elemntos que permite a gestion de teclado y los grafico de la terminal.

### 🧶Organizacion de carpetas

```
📂 Soluction
┣📁 output   -> Ubicacion del compilado
┣📂 src      -> Codigo fuente del proyecto
┃  ┣📂 lib   -> libreria para la gestion de pantalla y teclado
┃  ┃ ┣📄 display
┃  ┃ ┣📄 keyboard
┃  ┃ ┖📄 Utils
┃  ┣📄 Bird  -> Implenetacion de personaje
┃  ┣📄 Pipe  -> Implenetacion de obstaculo
┃  ┣📄 Game  -> Implenetacion de ciclo de vida del Juego
┃  ┖📰 main  -> Declaracion y ejecucion
┖📅 build    -> Proceso de compilacion

```


## 📝 Licencia

Este proyecto está licenciado bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

## 🤝 Contribuir

Si deseas contribuir a este proyecto, te invitamos a realizar un pull request o enviar un correo a [otavioferreir@gmail.com](mailto:otavioferreir@gmail.com).

---

Desarrollado por [Otavio Ferreira Rosa](https://otomaticoscript.github.io/)