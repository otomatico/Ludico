# Game Design Document (GDD)

**Título del Juego**: Urban Dash (Ejemplo)
**Género**: Endless Runner (Carrera Infinita)
**Plataforma**: Móvil (Android/iOS)
**Público Objetivo**: Casual gamers (8-25 años)

## 1. Concepto Básico

Un juego de carrera infinita en 3D donde el jugador controla un personaje que huye de un enemigo (ej: un guardia o monstruo), recorriendo una ciudad dinámica llena de obstáculos, power-ups y coleccionables.

Diferenciación del Clonico:
- **Tema único**: En lugar de trenes, el escenario es una ciudad futurista con vehículos voladores y trampas tecnológicas.
- **Mecánica extra**: El jugador puede _"hackear" obstáculos_ para desactivarlos temporalmente.
- **Modo multijugador asincrónico**: Competir contra récords de amigos en tiempo real.

## 2. Mecánicas Principales

**Controles (Touch Screen):**
- Deslizar ↑ → Saltar
- Deslizar ↓ → Rodar (esquivar obstáculos bajos)
- Deslizar ←/→ → Cambiar de carril
- Toque prolongado → Activar "Hackeo" (desactiva obstáculos por 3 segundos, con cooldown).

**Gameplay Loop:**
- Correr automáticamente en un mundo generado proceduralmente.
- Esquivar obstáculos (vehículos, paredes, huecos).
- Recoger monedas y power-ups (imán, sprint, escudo).
- Superar misiones diarias (ej: "Recoge 50 monedas sin chocar").
- Mejorar personajes y desbloquear skins.

**Sistemas de Progresión:**
- Monedas: Para comprar personajes y mejoras.
- Gemas (Premium): Para acelerar progreso o skins exclusivas.
- Niveles de Experiencia: Desbloquean nuevos fondos y habilidades.

## 3. Arte y Estilo Visual

- Gráficos: 3D low-poly con colores vibrantes (similar a Subway Surfers, pero con un estilo cyberpunk).
- Personajes: Diversos y personalizables (ej: hacker, robot, atleta).
- Escenarios: Ciudades futuristas con cambios temáticos cada temporada (ej: "Neo-Tokyo", "Marte 2150").

## 4. Monetización
- Free-to-Play (F2P) con anuncios opcionales:
    + Anuncios para revivir o duplicar recompensas.

- Microtransacciones:
    + Skins exclusivas.
    + Paquetes de gemas/monedas.

- Battle Pass: Recompensas por jugar diariamente.

## 5. Niveles y Dificultad

**Dificultad progresiva:** La velocidad y densidad de obstáculos aumenta con la distancia.

**Eventos Especiales:**
    + "Modo Infinito" (sin fin hasta perder).
    + "Modo Espejo" (los controles se invierten).

## 6. Inspiraciones y Referencias

- Subway Surfers (base del gameplay).
- Temple Run 2 (obstáculos dinámicos).
- Jetpack Joyride (power-ups y humor).

## 7. Roadmap (Fases de Desarrollo)

1. **Prototipo:** Mecánicas básicas (carrera, saltos, obstáculos).
2. **Alpha:** Primer escenario + sistema de monedas.
3. **Beta:** Integrar anuncios y tienda.
4. **Lanzamiento:** Con 3 personajes y 1 mapa.
5. **Actualizaciones:** Nuevas skins y eventos mensuales.