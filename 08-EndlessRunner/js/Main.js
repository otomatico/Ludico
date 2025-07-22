// js/Main.js
/*import { Player } from './Player.js';
import { Road } from './Road.js';
import { Background } from './Background.js';
import { ObstacleManager } from './Obstacle.js';
*/

// Obtén el lienzo y su contexto 2D.
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// --- Estado del juego ---
let score = 0;
let frameCount = 0;

// --- Instancia de las clases del juego ---
const road = new Road(canvas);
const player = new Player(canvas);
const background = new Background(canvas, road);
//const obstacleManager = new ObstacleManager(canvas, road);

// --- Bucle principal del juego ---
function gameLoop() {
    // 1. Limpia el canvas.
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    // 2. Dibuja el degradado del cielo.
    const gradient = ctx.createLinearGradient(0, 0, 0, canvas.height);
    gradient.addColorStop(0, '#00bfff');
    gradient.addColorStop(1, '#87ceeb');
    ctx.fillStyle = gradient;
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    // 3. Actualiza el estado de los componentes del juego.
    road.update();
    background.update();
    //obstacleManager.update();
    
    // 4. Genera nuevos objetos periódicamente.
    frameCount++;
    if (frameCount % 30 === 0) {
        background.createTree();
    }
    if (frameCount % 100 === 0) {
        background.createCloud();
    }
    if (frameCount % 60 === 0) {
        //obstacleManager.createObstacle();
    }

    // 5. Dibuja los componentes en el lienzo.
    road.draw(ctx);
    background.draw(ctx);
    player.draw(ctx);
    //obstacleManager.draw(ctx);
    
    // 6. Comprueba colisiones.
    //if (obstacleManager.checkCollision(player)) {
    //    alert('Game Over! Tu puntuación: ' + Math.floor(score));
    //    document.location.reload();
    //}

    // 7. Actualiza y dibuja la puntuación.
    score += 0.1;
    ctx.fillStyle = 'black';
    ctx.font = '20px Arial';
    ctx.fillText('Score: ' + Math.floor(score), 10, 30);

    // 8. Vuelve a llamar al bucle para el siguiente fotograma.
    requestAnimationFrame(gameLoop);
}

// --- Manejo de la entrada del usuario ---
document.addEventListener('keydown', (event) => {
    if (event.code === 'ArrowLeft') {
        player.move(-1);
    }
    if (event.code === 'ArrowRight') {
        player.move(1);
    }
});

// ¡Inicia el juego!
gameLoop();