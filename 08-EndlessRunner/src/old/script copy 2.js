// Obtén el lienzo y su contexto 2D.
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// --- Jugador ---
const player = {
    x: canvas.width / 2 - 15, // Posición central en X
    y: 350, // Posición fija cerca de la parte inferior de la pantalla
    width: 30,
    height: 50,
    speed: 5, // Velocidad de movimiento lateral
    draw() {
        ctx.fillStyle = 'red';
        ctx.fillRect(this.x, this.y, this.width, this.height);
    },
    move(direction) {
        // Mueve el jugador a izquierda o derecha
        this.x += this.speed * direction;

        // Limita el movimiento dentro de los bordes del canvas
        if (this.x < 0) this.x = 0;
        if (this.x + this.width > canvas.width) this.x = canvas.width - this.width;
    }
};

// --- Obstáculos ---
const obstacles = [];
const baseSpeed = 1; // Velocidad base de los obstáculos

// Función para crear un nuevo obstáculo en el horizonte
function createObstacle() {
    // Empieza en el centro del canvas, con un tamaño muy pequeño
    const startSize = 5;
    const startY = 100; // Un poco más abajo del centro para el horizonte
    const startX = canvas.width / 2 - startSize / 2;

    obstacles.push({
        x: startX,
        y: startY,
        width: startSize,
        height: startSize,
        draw() {
            ctx.fillStyle = 'brown';
            ctx.fillRect(this.x, this.y, this.width, this.height);
        },
        update() {
            // Calcula la "distancia" del obstáculo a la cámara (parte inferior de la pantalla)
            const distanceFactor = (this.y - startY) / (canvas.height - startY);

            // Acelera y agranda el obstáculo a medida que baja
            this.y += baseSpeed + (distanceFactor * 10);
            this.width = startSize + (distanceFactor * 80);
            this.height = startSize + (distanceFactor * 80);
            
            // Mantiene el obstáculo centrado horizontalmente a medida que se agranda
            this.x = canvas.width / 2 - this.width / 2;
        }
    });
}

// --- Detección de colisiones ---
function checkCollision() {
    for (const obstacle of obstacles) {
        // Solo comprueba colisiones si el obstáculo está cerca de la posición Y del jugador
        if (obstacle.y + obstacle.height > player.y && obstacle.y < player.y + player.height) {
             if (
                player.x < obstacle.x + obstacle.width &&
                player.x + player.width > obstacle.x
            ) {
                // ¡Hay colisión!
                alert('Game Over! Tu puntuación: ' + Math.floor(score));
                document.location.reload(); // Recarga la página para reiniciar.
            }
        }
    }
}

// --- Puntuación ---
let score = 0;
function updateScore() {
    score += 0.1;
    ctx.fillStyle = 'black';
    ctx.font = '20px Arial';
    ctx.fillText('Score: ' + Math.floor(score), 10, 30);
}

// --- Bucle principal del juego ---
let frameCount = 0;
function gameLoop() {
    // 1. Limpia el canvas
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // 2. Dibuja y actualiza elementos
    player.draw();
    
    // 3. Genera nuevos obstáculos
    frameCount++;
    if (frameCount % 60 === 0) { // Genera un obstáculo cada 60 fotogramas
        createObstacle();
    }
    
    // 4. Actualiza y dibuja los obstáculos
    for (let i = obstacles.length - 1; i >= 0; i--) {
        const obstacle = obstacles[i];
        obstacle.update();
        obstacle.draw();

        // Elimina los obstáculos que salieron de la pantalla
        if (obstacle.y > canvas.height) {
            obstacles.splice(i, 1);
        }
    }

    // 5. Comprueba colisiones
    checkCollision();
    
    // 6. Actualiza la puntuación
    updateScore();

    // 7. Vuelve a llamar a gameLoop
    requestAnimationFrame(gameLoop);
}

// --- Manejo de la entrada del usuario ---
document.addEventListener('keydown', (event) => {
    if (event.code === 'ArrowLeft') {
        player.move(-1); // Mueve a la izquierda
    }
    if (event.code === 'ArrowRight') {
        player.move(1); // Mueve a la derecha
    }
});

// ¡Inicia el juego!
gameLoop();