// Obtén el lienzo y su contexto 2D.
const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');

// --- Jugador ---
const player = {
    // El jugador ahora está en el carril central por defecto.
    // Usaremos un índice de carril: 0 (izquierda), 1 (centro), 2 (derecha).
    currentLane: 1, 
    x: 0, // Su posición X se calculará dinámicamente.
    y: 350,
    width: 30,
    height: 50,
    draw() {
        // Calcula la posición X en función del carril actual.
        // Asignaremos una posición X para cada carril.
        const laneWidth = canvas.width / 3;
        this.x = (this.currentLane * laneWidth) + (laneWidth / 2) - (this.width / 2);
        
        ctx.fillStyle = 'red';
        ctx.fillRect(this.x, this.y, this.width, this.height);
    },
    move(direction) {
        // Mueve el jugador de un carril a otro.
        // direction: -1 para izquierda, 1 para derecha.
        this.currentLane += direction;

        // Limita el movimiento para que no salga de los carriles.
        if (this.currentLane < 0) this.currentLane = 0;
        if (this.currentLane > 2) this.currentLane = 2;
    }
};

// --- Carretera y franjas ---
const laneStripes = [];
const baseSpeed = 1;

// Define el punto de fuga (el horizonte).
const horizonY = 100;
const horizonX = canvas.width / 2;
// Define la posición X de cada carril en la parte inferior de la pantalla.
const laneEnds = [133, 400, 667]; // Aproximadamente 1/6, 3/6 y 5/6 de la pantalla.
const finalLaneWidth = 100; // Ancho final de un carril en la parte inferior.

// Función para dibujar la carretera con perspectiva.
function drawRoad() {
    ctx.fillStyle = '#444';
    ctx.beginPath();
    // Dibuja el polígono de la carretera desde el horizonte.
    ctx.moveTo(horizonX, horizonY); // Punto de fuga central
    ctx.lineTo(0, canvas.height);    // Esquina inferior izquierda
    ctx.lineTo(canvas.width, canvas.height); // Esquina inferior derecha
    ctx.lineTo(horizonX, horizonY); // De vuelta al punto de fuga
    ctx.fill();
    ctx.closePath();
}

// Función para crear una nueva franja en el horizonte.
function createStripe() {
    const startSize = 2;
    
    // Elige un carril al azar para la franja.
    const lane = Math.floor(Math.random() * 3);
    
    laneStripes.push({
        lane: lane,
        x: horizonX - (startSize / 2), // Empieza en el punto de fuga.
        y: horizonY,
        width: startSize,
        height: 10,
        draw() {
            ctx.fillStyle = 'white';
            ctx.fillRect(this.x, this.y, this.width, this.height);
        },
        update() {
            // Calcula la "distancia" del objeto a la cámara.
            const distanceFactor = (this.y - horizonY) / (canvas.height - horizonY);
            
            // Acelera y agranda el objeto a medida que baja.
            this.y += baseSpeed + (distanceFactor * 10);
            this.width = startSize + (distanceFactor * 50);
            this.height = 10 + (distanceFactor * 20);

            // --- CORRECCIÓN DE LA POSICIÓN X ---
            // Interpola la posición X desde el horizonte hasta la posición final del carril.
            const targetX = laneEnds[this.lane] - (this.width / 2);
            this.x = horizonX + (targetX - horizonX) * distanceFactor;
        }
    });
}

// --- Obstáculos ---
const obstacles = [];
const obstacleSpeedMultiplier = 10;

// Función para crear un nuevo obstáculo en un carril aleatorio.
function createObstacle() {
    const startSize = 5;
    
    // Elige un carril aleatorio.
    const lane = Math.floor(Math.random() * 3);

    obstacles.push({
        lane: lane,
        x: horizonX - (startSize / 2), // Empieza en el punto de fuga.
        y: horizonY,
        width: startSize,
        height: startSize,
        draw() {
            ctx.fillStyle = 'brown';
            ctx.fillRect(this.x, this.y, this.width, this.height);
        },
        update() {
            const distanceFactor = (this.y - horizonY) / (canvas.height - horizonY);
            this.y += baseSpeed + (distanceFactor * obstacleSpeedMultiplier);
            this.width = startSize + (distanceFactor * 80);
            this.height = startSize + (distanceFactor * 80);

            // --- CORRECCIÓN DE LA POSICIÓN X ---
            // Interpola la posición X desde el horizonte hasta la posición final del carril.
            const targetX = laneEnds[this.lane] - (this.width / 2);
            this.x = horizonX + (targetX - horizonX) * distanceFactor;
        }
    });
}

// --- Detección de colisiones ---
function checkCollision() {
    for (const obstacle of obstacles) {
        // Solo comprueba colisiones si el obstáculo está cerca del jugador en el eje Y.
        if (obstacle.y + obstacle.height > player.y && obstacle.y < player.y + player.height) {
             if (
                // Comprueba si el jugador está en el mismo carril que el obstáculo.
                player.currentLane === obstacle.lane
            ) {
                // ¡Hay colisión!
                alert('Game Over! Tu puntuación: ' + Math.floor(score));
                document.location.reload();
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
    // 1. Limpia el canvas.
    ctx.clearRect(0, 0, canvas.width, canvas.height);

    // 2. Dibuja la carretera y sus franjas.
    drawRoad();
    // Genera franjas de carretera.
    if (frameCount % 20 === 0) {
        createStripe();
    }
    // Actualiza y dibuja las franjas.
    for (let i = laneStripes.length - 1; i >= 0; i--) {
        const stripe = laneStripes[i];
        stripe.update();
        stripe.draw();
        if (stripe.y > canvas.height) {
            laneStripes.splice(i, 1);
        }
    }

    // 3. Dibuja y actualiza al jugador.
    player.draw();
    
    // 4. Genera nuevos obstáculos.
    frameCount++;
    if (frameCount % 60 === 0) {
        createObstacle();
    }
    
    // 5. Actualiza y dibuja los obstáculos.
    for (let i = obstacles.length - 1; i >= 0; i--) {
        const obstacle = obstacles[i];
        obstacle.update();
        obstacle.draw();
        if (obstacle.y > canvas.height) {
            obstacles.splice(i, 1);
        }
    }

    // 6. Comprueba colisiones.
    checkCollision();
    
    // 7. Actualiza la puntuación.
    updateScore();

    // 8. Vuelve a llamar a gameLoop.
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