
# Game Design 📚
>Clon de SnakePuzzle 

## 🏁 Introdución
El objetivo de este proyecto es crear un clon funcional del juego *Snake Puzzle*, con el fin de aprender y practicar conceptos de desarrollo de videojuegos, como la física básica, la gestión de colisiones, la generación procedural de obstáculos y la implementación de una interfaz de usuario sencilla.

## 💸 Monetización

No se espera ganar dinero con este proyecto, se trata de una prueba de conceptos y un MVP (Producto Minimo viable).

## ⚙️Mecánicas y Elementos de Juego

### Mecanica del entorno

- Se trata de un entorno en 2d donde el personaje (la serpiente) tiene que comer la fruta y evitar chocarse contra si misma y las trampas del escenario.
- Se trata de mapas que para superar hay que comer ciertas manzanas.
- Existe menus para gestionar el inicio del juego, salir del juego y para pausarlo.

#### Menú Principal
- **Iniciar Juego**: El jugador puede comenzar una nueva partida.
- **Salir**: Cierra la aplicación.

#### Menú de Pausa
- **Pausa**: El jugador puede parar el juego presionando una tecla (ESC) y presionando la misma tecla para continuar la partida.

### Mecánica del Juego
- El personaje se no puede volver sobre sus pasos.
- Una vez que el personaje coma una fruta el personaje crecerá una unidad.
- 
<!--Resto de la documentacion en desarrollo -->

### Elementos

De las mecanicas se intuye que tendremos una unica fruta que se moverá de ubicacion de forma aleatoria cada vez que el personaje la coma.```Comer```

El personaje se mueve simpre una misma direcion a menos que se le diga lo contrario. ```Control del Personaje```

Pasar sobre si mismo se considera colisionar, ademas debe evitar chocar contra las paredes. ```Colisión```

Si el personaje choca con una fruta, esta se reubicará en otra posicion y el personaje come una fruta aumenta de tamaño. ```Puntuación```

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
┃  ┣📂 etc   -> configuracion del proyecto
┃  ┃ ┣📄 env -> constantes del proyeto como dimesion de pantalla 
┃  ┣📂 lib   -> libreria para la gestion de pantalla y teclado
┃  ┃ ┣📄 keyboard
┃  ┃ ┣📄 Canvas4Bit
┃  ┃ ┖📄 Entities
┃  ┣📄 Player    -> Implenetacion de personaje
┃  ┣📄 Obstacle  -> Implenetacion de obstaculo
┃  ┣📄 Game      -> Implenetacion de ciclo de vida del Juego
┃  ┖📰 main      -> Declaracion y ejecucion
┖📅 build        -> Proceso de compilacion

```


## 📝 Licencia

Este proyecto está licenciado bajo la Licencia MIT. Consulta el archivo [LICENSE](LICENSE) para más detalles.

## 🤝 Contribuir

Si deseas contribuir a este proyecto, te invitamos a realizar un pull request o enviar un correo a [otavioferreir@gmail.com](mailto:otavioferreir@gmail.com).

---

Desarrollado por [Otavio Ferreira Rosa](https://otomaticoscript.github.io/)