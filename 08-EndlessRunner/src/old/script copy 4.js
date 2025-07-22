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

// --- Carretera y entorno ---
const baseSpeed = 1;

// Define el punto de fuga (el horizonte). Ahora es dinámico.
let horizonY = 100;
let horizonX = canvas.width / 2;
// Define la posición X de cada carril en la parte inferior de la pantalla.
const laneEnds = [133, 400, 667]; 

// --- LÓGICA DE LAS COLINAS Y CURVAS ---
let hillAmplitude = 50;
let hillFrequency = 0.01;
let trackPosition = 0;
let curveAmplitude = 100;
let curveFrequency = 0.01;
let curveOffset = 0;

// --- LÓGICA DE LOS OBJETOS EN EL FONDO (PARALAJE) ---
const mountains = [];
const trees = [];
const clouds = [];
const mountainSpeed = 0.5;
const treeSpeed = 10;
const cloudSpeed = 0.2;

function createMountain() {
    const mountainHeight = 100 + Math.random() * 80;
    const mountainWidth = 150 + Math.random() * 100;
    mountains.push({
        // La montaña ahora tiene una posición de inicio aleatoria para aparecer en cualquier lado.
        x: Math.random() * canvas.width,
        y: horizonY,
        width: mountainWidth,
        height: mountainHeight,
        draw() {
            ctx.fillStyle = 'darkgreen';
            ctx.beginPath();
            ctx.moveTo(this.x, this.y);
            ctx.lineTo(this.x + this.width / 2, this.y - this.height);
            ctx.lineTo(this.x + this.width, this.y);
            ctx.fill();
        },
        update() {
            // MOVER LA MONTAÑA EN SENTIDO CONTRARIO A LA CURVA.
            // Si la curva va a la derecha (sin positivo), la montaña va a la izquierda (negativo).
            const curveDirection = Math.sin(curveOffset);
            this.x -= mountainSpeed * curveDirection; // Movimiento lateral opuesto a la curva.
            this.x -= baseSpeed * 0.1; // Sigue moviéndose lentamente hacia la izquierda para simular el paso del tiempo.
        }
    });
}

function createTree() {
    const treeHeight = 50 + Math.random() * 100;
    const treeWidth = 20 + Math.random() * 30;
    
    // Decide si el árbol aparece a la izquierda o a la derecha.
    const side = Math.random() < 0.5 ? 'left' : 'right';
    
    trees.push({
        side: side,
        x: side === 'left' ? 0 : canvas.width - treeWidth, // Posición inicial.
        y: horizonY,
        width: treeWidth,
        height: treeHeight,
        draw() {
            // Dibujamos el tronco y la copa.
            ctx.fillStyle = 'saddlebrown';
            ctx.fillRect(this.x + this.width / 2 - 5, this.y - this.height, 10, this.height);
            ctx.fillStyle = 'forestgreen';
            ctx.beginPath();
            ctx.arc(this.x + this.width / 2, this.y - this.height, this.width / 2, 0, Math.PI * 2);
            ctx.fill();
        },
        update() {
            // Un factor de distancia para que el árbol crezca a medida que se acerca.
            const distanceFactor = (this.y - horizonY) / (canvas.height - horizonY);
            
            // Mueve el árbol a la velocidad de la carretera.
            this.y += baseSpeed + (distanceFactor * treeSpeed);
            
            // Ajusta el tamaño y la posición lateral para el efecto de perspectiva.
            this.width = treeWidth + (distanceFactor * 100);
            this.height = treeHeight + (distanceFactor * 200);

            // Mueve los árboles hacia los lados para seguir la curva de la carretera.
            const currentHorizonX = horizonX + Math.sin(curveOffset) * curveAmplitude;
            if (this.side === 'left') {
                this.x = currentHorizonX - (distanceFactor * canvas.width / 2);
            } else {
                this.x = currentHorizonX + (distanceFactor * canvas.width / 2) - this.width;
            }
        }
    });
}

function createCloud() {
    const cloudWidth = 50 + Math.random() * 100;
    const cloudHeight = 20 + Math.random() * 40;
    clouds.push({
        x: canvas.width,
        y: Math.random() * (horizonY - 50), // Nace en la parte superior del cielo.
        width: cloudWidth,
        height: cloudHeight,
        draw() {
            ctx.fillStyle = 'rgba(255, 255, 255, 0.8)'; // Nubes semitransparentes.
            ctx.beginPath();
            ctx.ellipse(this.x, this.y, this.width, this.height, 0, 0, Math.PI * 2);
            ctx.fill();
        },
        update() {
            this.x -= cloudSpeed;
        }
    });
}

// Función para dibujar la carretera con perspectiva y colinas.
function drawRoad() {
    ctx.fillStyle = '#444';
    ctx.beginPath();
    
    // El punto de fuga ahora es dinámico y sigue la curva.
    const currentHorizonY = horizonY + Math.sin(trackPosition) * hillAmplitude;
    const currentHorizonX = horizonX + Math.sin(curveOffset) * curveAmplitude;
    
    // --- CAMBIO CLAVE: DIBUJAR UN TRAPECIO EN LUGAR DE UN TRIÁNGULO ---
    const roadWidthAtBottom = canvas.width * 0.8; // El 80% del ancho del canvas.
    const roadStartOffset = (canvas.width - roadWidthAtBottom) / 3;

    ctx.moveTo(currentHorizonX, currentHorizonY); // 1. Punto de fuga en el horizonte.
    ctx.lineTo(roadStartOffset, canvas.height); // 2. Esquina inferior izquierda (dentro del canvas).
    ctx.lineTo(canvas.width - roadStartOffset, canvas.height); // 3. Esquina inferior derecha (dentro del canvas).
    ctx.lineTo(currentHorizonX, currentHorizonY); // 4. De vuelta al punto de fuga.
    
    ctx.fill();
    ctx.closePath();
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
        x: horizonX - (startSize / 2),
        y: horizonY + Math.sin(trackPosition) * hillAmplitude, // Nace en el horizonte dinámico.
        width: startSize,
        height: startSize,
        draw() {
            ctx.fillStyle = 'brown';
            ctx.fillRect(this.x, this.y, this.width, this.height);
        },
        update() {
            const currentHorizonY = horizonY + Math.sin(trackPosition) * hillAmplitude;
            const distanceFactor = (this.y - currentHorizonY) / (canvas.height - currentHorizonY);
            this.y += baseSpeed + (distanceFactor * obstacleSpeedMultiplier);
            this.width = startSize + (distanceFactor * 80);
            this.height = startSize + (distanceFactor * 80);

            // Interpola la posición X para el efecto de perspectiva, igual que las franjas.
            const currentHorizonX = horizonX + Math.sin(curveOffset) * curveAmplitude;
            const targetX = laneEnds[this.lane] - (this.width / 2);
            this.x = currentHorizonX + (targetX - currentHorizonX) * distanceFactor;
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
    
    // 2. Dibuja el degradado del cielo (fondo dinámico).
    const gradient = ctx.createLinearGradient(0, 0, 0, canvas.height);
    gradient.addColorStop(0, '#00bfff'); // Azul oscuro en la parte superior.
    gradient.addColorStop(1, '#87ceeb'); // Azul claro en la parte inferior.
    ctx.fillStyle = gradient;
    ctx.fillRect(0, 0, canvas.width, canvas.height);

    // 3. ACTUALIZA LA POSICIÓN DE LA PISTA para las colinas y las curvas.
    trackPosition += hillFrequency;
    curveOffset += curveFrequency;
    
    // 4. Dibuja y actualiza las nubes.
    if (frameCount % 100 === 0) {
        createCloud();
    }
    for (let i = clouds.length - 1; i >= 0; i--) {
        const cloud = clouds[i];
        cloud.update();
        if (cloud.x + cloud.width < 0) {
            clouds.splice(i, 1);
        } else {
            cloud.draw();
        }
    }
    
    // 5. Dibuja y actualiza las montañas (paralaje).
    if (frameCount % 150 === 0) {
        createMountain();
    }
    for (let i = mountains.length - 1; i >= 0; i--) {
        const mountain = mountains[i];
        mountain.update();
        if (mountain.x + mountain.width > 0 && mountain.x < canvas.width) {
            mountain.draw();
        } else {
            mountains.splice(i, 1);
        }
    }
    
    // 6. Dibuja y actualiza los árboles.
    if (frameCount % 30 === 0) {
        createTree();
    }
    for (let i = trees.length - 1; i >= 0; i--) {
        const tree = trees[i];
        tree.update();
        if (tree.y > canvas.height || (tree.side === 'left' && tree.x > canvas.width) || (tree.side === 'right' && tree.x + tree.width < 0)) {
            trees.splice(i, 1);
        } else {
            tree.draw();
        }
    }

    // 7. Dibuja la carretera.
    drawRoad();
    
    // 8. Dibuja y actualiza al jugador.
    player.draw();
    
    // 9. Genera nuevos obstáculos.
    frameCount++;
    if (frameCount % 60 === 0) {
        createObstacle();
    }
    
    // 10. Actualiza y dibuja los obstáculos.
    for (let i = obstacles.length - 1; i >= 0; i--) {
        const obstacle = obstacles[i];
        obstacle.update();
        obstacle.draw();
        if (obstacle.y > canvas.height) {
            obstacles.splice(i, 1);
        }
    }

    // 11. Comprueba colisiones.
    checkCollision();
    
    // 12. Actualiza la puntuación.
    updateScore();

    // 13. Vuelve a llamar a gameLoop.
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