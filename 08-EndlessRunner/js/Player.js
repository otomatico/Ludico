// js/Player.js
//export class Player {
class Player {
    constructor(canvas) {
        this.canvas = canvas;
        this.currentLane = 1; // 0 (izquierda), 1 (centro), 2 (derecha)
        this.y = 350;
        this.width = 30;
        this.height = 50;
        this.x = 0; // Se calcula dinámicamente en draw()
    }

    draw(ctx) {
        // Calcula la posición X en función del carril actual.
        const laneWidth = this.canvas.width / 3;
        this.x = (this.currentLane * laneWidth) + (laneWidth / 2) - (this.width / 2);
        
        ctx.fillStyle = 'red';
        ctx.fillRect(this.x, this.y, this.width, this.height);
    }

    move(direction) {
        // Mueve el jugador de un carril a otro.
        this.currentLane += direction;

        // Limita el movimiento para que no salga de los carriles.
        if (this.currentLane < 0) this.currentLane = 0;
        if (this.currentLane > 2) this.currentLane = 2;
    }
}