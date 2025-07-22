// js/Obstacle.js
//export class ObstacleManager {
class ObstacleManager {
    constructor(canvas, road) {
        this.canvas = canvas;
        this.road = road; // Referencia a la instancia de la carretera
        this.obstacles = [];
        this.obstacleSpeedMultiplier = 10;
        this.laneEnds = [133, 400, 667];
    }

    createObstacle() {
        const startSize = 5;
        const lane = Math.floor(Math.random() * 3);
        
        this.obstacles.push({
            lane: lane,
            x: this.road.horizonX - (startSize / 2),
            y: this.road.horizonY + Math.sin(this.road.trackPosition) * this.road.hillAmplitude,
            width: startSize,
            height: startSize,
            draw(ctx) {
                ctx.fillStyle = 'brown';
                ctx.fillRect(this.x, this.y, this.width, this.height);
            },
            update(road) {
                const currentHorizonY = road.horizonY + Math.sin(road.trackPosition) * road.hillAmplitude;
                const distanceFactor = (this.y - currentHorizonY) / (road.canvas.height - currentHorizonY);
                this.y += road.baseSpeed + (distanceFactor * this.obstacleSpeedMultiplier);
                this.width = startSize + (distanceFactor * 80);
                this.height = startSize + (distanceFactor * 80);

                const currentHorizonX = road.horizonX + Math.sin(road.curveOffset) * road.curveAmplitude;
                const targetX = this.laneEnds[this.lane] - (this.width / 2);
                this.x = currentHorizonX + (targetX - currentHorizonX) * distanceFactor;
            }
        });
    }

    update() {
        for (let i = this.obstacles.length - 1; i >= 0; i--) {
            const obstacle = this.obstacles[i];
            obstacle.update(this.road);
            if (obstacle.y > this.canvas.height) {
                this.obstacles.splice(i, 1);
            }
        }
    }

    draw(ctx) {
        for (const obstacle of this.obstacles) {
            obstacle.draw(ctx);
        }
    }

    checkCollision(player) {
        for (const obstacle of this.obstacles) {
            // Solo comprueba colisiones si el obstáculo está cerca del jugador en el eje Y.
            if (obstacle.y + obstacle.height > player.y && obstacle.y < player.y + player.height) {
                if (player.currentLane === obstacle.lane) {
                    return true;
                }
            }
        }
        return false;
    }
}