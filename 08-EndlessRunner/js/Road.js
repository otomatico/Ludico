// js/Road.js
//export class Road {
class Road {
    constructor(canvas) {
        this.canvas = canvas;
        this.baseSpeed = 1;
        this.horizonY = 100;
        this.horizonX = this.canvas.width / 2;
        this.hillAmplitude = 25;
        this.hillFrequency = 0.01;
        this.trackPosition = 0;
        this.curveAmplitude = 100;
        this.curveFrequency = 0.01;
        this.curveOffset = 0;
        this.roadCurveStrength = 80;
    }

    update() {
        // Actualiza la posición de la pista para las colinas y las curvas.
        this.trackPosition += this.hillFrequency;
        this.curveOffset += this.curveFrequency;
    }

    // La función drawRoad que quieres, convertida en un método de clase.
    draw(ctx) {
        ctx.fillStyle = '#444';
        ctx.beginPath();
        
        // Puntos del horizonte (arriba del trapecio).
        const currentHorizonY = this.horizonY + Math.sin(this.trackPosition) * this.hillAmplitude;
        const currentHorizonX = this.horizonX + Math.sin(this.curveOffset) * this.curveAmplitude;
        
        // Ancho de la carretera en la parte superior del trapecio (en el horizonte).
        const roadTopWidth = 20; 
        const horizonLeftX = currentHorizonX - (roadTopWidth / 2); 
        const horizonRightX = currentHorizonX + (roadTopWidth / 2);

        // Puntos de la base del trapecio (abajo).
        const roadWidthAtBottom = this.canvas.width * 0.8; 
        const roadBottomLeftX = (this.canvas.width - roadWidthAtBottom) / 2;
        const roadBottomRightX = this.canvas.width - ((this.canvas.width - roadWidthAtBottom) / 2);

        // Factor de influencia de la curva basado en la dirección de giro.
        const curveFactor = Math.sin(this.curveOffset);

        // Calcula los puntos de control para las curvas de Bézier.
        // El 'pull' horizontal es positivo para el lado izquierdo y negativo para el derecho.
        const controlPointLeftX = roadBottomLeftX + (horizonLeftX - roadBottomLeftX) * 0.5 + curveFactor * this.roadCurveStrength;
        const controlPointLeftY = (this.canvas.height + currentHorizonY) / 2;

        const controlPointRightX = roadBottomRightX + (horizonRightX - roadBottomRightX) * 0.5 - curveFactor * this.roadCurveStrength; // Se invierte el signo para el lado derecho.
        const controlPointRightY = (this.canvas.height + currentHorizonY) / 2;

        // 1. Mueve a la esquina inferior izquierda de la carretera.
        ctx.moveTo(roadBottomLeftX, this.canvas.height); 
        
        // 2. Dibuja la curva del borde izquierdo hasta el punto superior izquierdo del trapecio.
        ctx.quadraticCurveTo(controlPointLeftX, controlPointLeftY, horizonLeftX, currentHorizonY);

        // 3. Dibuja la línea superior del trapecio (horizontal en el horizonte).
        ctx.lineTo(horizonRightX, currentHorizonY);

        // 4. Dibuja la curva del borde derecho desde el punto superior derecho hasta la esquina inferior derecha.
        ctx.quadraticCurveTo(controlPointRightX, controlPointRightY, roadBottomRightX, this.canvas.height);
        
        // 5. Cierra el camino para dibujar la base inferior del trapecio.
        ctx.closePath();
        ctx.fill();
    }
}