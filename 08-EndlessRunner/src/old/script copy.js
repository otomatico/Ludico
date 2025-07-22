// Obtén el lienzo y su contexto 2D.
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// --- Jugador ---
const player = {
    x: 50,
    y: 300,
    width: 30,
    height: 50,
    dy: 0, // Velocidad vertical (para la gravedad)
    gravity: 0.8,
    isJumping: false,
    draw() {
        ctx.fillStyle = 'red';
        ctx.fillRect(this.x, this.y, this.width, this.height);
    },
    update() {
        // Aplica gravedad si no está en el suelo.
        if (this.y + this.height < canvas.height) {
            this.dy += this.gravity;
            this.y += this.dy;
        }

        // Si toca el suelo, detén la caída.
        if (this.y + this.height >= canvas.height - 10) { // Ajuste para el suelo
            this.y = canvas.height - this.height - 10; // Ajuste para el suelo
            this.dy = 0;
            this.isJumping = false;
        }
    },
    jump() {
        if (!this.isJumping) {
            this.dy = -15; // Velocidad de salto
            this.isJumping = true;
        }
    }
};

// --- Suelo ---
function drawGround() {
    ctx.fillStyle = 'green';
    ctx.fillRect(0, canvas.height - 10, canvas.width, 10);
}

// --- Obstáculos ---
const obstacles = [];
const gameSpeed = 5; // Velocidad base del juego.

// Función para crear un nuevo obstáculo.
function createObstacle() {
    // Genera un tamaño y una posición vertical aleatoria para el efecto 2.5D.
    const size = 20 + Math.random() * 40; // Tamaño entre 20 y 60
    const yPos = canvas.height - 10 - size; // Dibuja el obstáculo sobre el suelo.
    
    // Calcula la velocidad del obstáculo en función de su tamaño.
    // Los más grandes van más rápido.
    const speed = gameSpeed * (size / 40);

    obstacles.push({
        x: canvas.width, // Empieza fuera de la pantalla.
        y: yPos,
        width: size,
        height: size,
        speed: speed,
        draw() {
            ctx.fillStyle = 'brown';
            ctx.fillRect(this.x, this.y, this.width, this.height);
        },
        update() {
            this.x -= this.speed; // Mueve el obstáculo hacia la izquierda.
        }
    });
}

// --- Detección de colisiones ---
function checkCollision() {
    for (const obstacle of obstacles) {
        // Comprueba si el jugador y el obstáculo se solapan en los ejes X e Y.
        if (
            player.x < obstacle.x + obstacle.width &&
            player.x + player.width > obstacle.x &&
            player.y < obstacle.y + obstacle.height &&
            player.y + player.height > obstacle.y
        ) {
            // ¡Hay colisión! El juego termina.
            alert('Game Over! Tu puntuación: ' + Math.floor(score));
            document.location.reload(); // Recarga la página para reiniciar.
        }
    }
}

// --- Puntuación ---
let score = 0;
function updateScore() {
    score += 0.1; // Incrementa la puntuación constantemente.
    ctx.fillStyle = 'black';
    ctx.font = '20px Arial';
    ctx.fillText('Score: ' + Math.floor(score), 10, 30);
}

// --- Bucle principal del juego ---
let frameCount = 0;
function gameLoop() {
    // 1. Limpia el canvas en cada fotograma.
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // 2. Dibuja y actualiza los elementos.
    drawGround();
    player.update();
    player.draw();

    // 3. Genera nuevos obstáculos cada cierto tiempo.
    frameCount++;
    if (frameCount % 100 === 0) { // Genera un obstáculo cada 100 fotogramas.
        createObstacle();
    }

    // 4. Actualiza y dibuja los obstáculos.
    for (let i = obstacles.length - 1; i >= 0; i--) {
        const obstacle = obstacles[i];
        obstacle.update();
        obstacle.draw();

        // Elimina los obstáculos que salieron de la pantalla para no acumularlos en memoria.
        if (obstacle.x + obstacle.width < 0) {
            obstacles.splice(i, 1);
        }
    }

    // 5. Comprueba si hay colisiones.
    checkCollision();
    
    // 6. Actualiza la puntuación.
    updateScore();

    // 7. Vuelve a llamar a gameLoop en el siguiente fotograma.
    requestAnimationFrame(gameLoop);
}

// --- Manejo de la entrada del usuario ---
document.addEventListener('keydown', (event) => {
    if (event.code === 'Space') {
        player.jump();
    }
});

// ¡Inicia el juego!
gameLoop();