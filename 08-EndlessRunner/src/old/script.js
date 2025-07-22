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
let hillAmplitude = 25; // REDUCIDO: Ahora las colinas son más suaves.
let hillFrequency = 0.01;
let trackPosition = 0;
let curveAmplitude = 100;
let curveFrequency = 0.01;
let curveOffset = 0;
// Factor para controlar la curvatura de los bordes del trapecio de la carretera.
// Un valor más alto hace la curva más pronunciada.
const roadCurveStrength = 80; 

// --- LÓGICA DE LOS OBJETOS EN EL FONDO (PARALAJE) ---
const mountains = [];
const trees = [];
const clouds = [];
const mountainSpeed = 0.5;
const treeSpeed = 10;
const cloudSpeed = 0.2;

// --- FUNCIÓN PARA DIBUJAR MONTANAS ESTÁTICAS AL INICIO ---
function initializeMountains() {
    for (let i = 0; i < 5; i++) {
        const mountainHeight = 100 + Math.random() * 80;
        const mountainWidth = 150 + Math.random() * 100;
        mountains.push({
            // Se dibujan en una posición aleatoria para llenar el fondo.
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
                // Sincroniza el movimiento con la curva de la carretera.
                const curveDirection = Math.sin(curveOffset);
                this.x -= mountainSpeed * curveDirection;
            }
        });
    }
}

function createTree() {
    const treeHeight = 50 + Math.random() * 100;
    const treeWidth = 20 + Math.random() * 30;
    
    // Decide si el árbol aparece a la izquierda o a la derecha.
    const side = Math.random() < 0.5 ? 'left' : 'right';
    
    trees.push({
        side: side,
        // Los árboles nacen en el punto de fuga para que sigan la perspectiva.
        x: horizonX, 
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
            const currentHorizonY = horizonY + Math.sin(trackPosition) * hillAmplitude;
            const distanceFactor = (this.y - currentHorizonY) / (canvas.height - currentHorizonY);
            
            // Mueve el árbol a la velocidad de la carretera.
            this.y += baseSpeed + (distanceFactor * treeSpeed);
            
            // Ajusta el tamaño y la posición lateral para el efecto de perspectiva.
            this.width = treeWidth + (distanceFactor * 100);
            this.height = treeHeight + (distanceFactor * 200);

            // MOVIMIENTO CON LA CURVA DE LA CARRETERA
            const currentHorizonX = horizonX + Math.sin(curveOffset) * curveAmplitude;
            if (this.side === 'left') {
                this.x = currentHorizonX - (distanceFactor * (canvas.width / 2));
            } else {
                this.x = currentHorizonX + (distanceFactor * (canvas.width / 2));
            }
        }
    });
}

function createCloud() {
    const cloudWidth = 50 + Math.random() * 100;
    const cloudHeight = 20 + Math.random() * 40;
    clouds.push({
        x: canvas.width,
        y: Math.random() * (horizonY - 50),
        width: cloudWidth,
        height: cloudHeight,
        draw() {
            ctx.fillStyle = 'rgba(255, 255, 255, 0.8)';
            ctx.beginPath();
            ctx.ellipse(this.x, this.y, this.width, this.height, 0, 0, Math.PI * 2);
            ctx.fill();
        },
        update() {
            this.x -= cloudSpeed;
        }
    });
}

// --- FUNCIÓN MEJORADA: DIBUJA LA CARRETERA COMO UN TRAPECIO CURVADO ---
function drawRoad() {
    ctx.fillStyle = '#444';
    ctx.beginPath();
    
    // Puntos del horizonte (arriba del trapecio).
    const currentHorizonY = horizonY + Math.sin(trackPosition) * hillAmplitude;
    const currentHorizonX = horizonX + Math.sin(curveOffset) * curveAmplitude;
    
    // Ancho de la carretera en la parte superior del trapecio (en el horizonte).
    const roadTopWidth = 20; 
    const horizonLeftX = currentHorizonX - (roadTopWidth / 2); 
    const horizonRightX = currentHorizonX + (roadTopWidth / 2);

    // Puntos de la base del trapecio (abajo).
    const roadWidthAtBottom = canvas.width * 0.8; 
    const roadBottomLeftX = (canvas.width - roadWidthAtBottom) / 2;
    const roadBottomRightX = canvas.width - ((canvas.width - roadWidthAtBottom) / 2);

    // Factor de influencia de la curva basado en la dirección de giro.
    const curveFactor = Math.sin(curveOffset);

    // Calcula los puntos de control para las curvas de Bézier.
    // El 'pull' horizontal es positivo para el lado izquierdo y negativo para el derecho.
    const controlPointLeftX = roadBottomLeftX + (horizonLeftX - roadBottomLeftX) * 0.5 + curveFactor * roadCurveStrength;
    const controlPointLeftY = (canvas.height + currentHorizonY) / 2;

    const controlPointRightX = roadBottomRightX + (horizonRightX - roadBottomRightX) * 0.5 + curveFactor * roadCurveStrength; // Se invierte el signo para el lado derecho.
    const controlPointRightY = (canvas.height + currentHorizonY) / 2;

    // 1. Mueve a la esquina inferior izquierda de la carretera.
    ctx.moveTo(roadBottomLeftX, canvas.height); 
    
    // 2. Dibuja la curva del borde izquierdo hasta el punto superior izquierdo del trapecio.
    ctx.quadraticCurveTo(controlPointLeftX, controlPointLeftY, horizonLeftX, currentHorizonY);

    // 3. Dibuja la línea superior del trapecio (horizontal en el horizonte).
    ctx.lineTo(horizonRightX, currentHorizonY);

    // 4. Dibuja la curva del borde derecho desde el punto superior derecho hasta la esquina inferior derecha.
    ctx.quadraticCurveTo(controlPointRightX, controlPointRightY, roadBottomRightX, canvas.height);
    
    // 5. Cierra el camino para dibujar la base inferior del trapecio.
    ctx.closePath();
    ctx.fill();
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
    for (let i = mountains.length - 1; i >= 0; i--) {
        const mountain = mountains[i];
        mountain.update();
        if (mountain.x + mountain.width > 0 && mountain.x < canvas.width) {
            mountain.draw();
        } else {
            // Si la montaña sale de la pantalla, la reposicionamos en el otro lado.
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
        // Borra el árbol cuando sale de la vista por la parte inferior o lateral.
        if (tree.y > canvas.height || (tree.side === 'left' && tree.x + tree.width < 0) || (tree.side === 'right' && tree.x > canvas.width)) {
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

// Llama a esta función al inicio para dibujar las montañas.
initializeMountains();

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