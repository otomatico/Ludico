// js/Background.js
//export class Background {
class Background {
    constructor(canvas, road) {
        this.canvas = canvas;
        this.road = road; // Referencia a la instancia de la carretera
        this.mountains = [];
        this.trees = [];
        this.clouds = [];
        this.mountainSpeed = 0.5;
        this.treeSpeed = 10;
        this.cloudSpeed = 0.2;

        // Inicializa las montañas al crear el objeto.
        this.initializeMountains();
    }

    initializeMountains() {
        for (let i = 0; i < 5; i++) {
            const mountainHeight = 100 + Math.random() * 80;
            const mountainWidth = 150 + Math.random() * 100;
            this.mountains.push({
                x: Math.random() * this.canvas.width,
                y: this.road.horizonY,
                width: mountainWidth,
                height: mountainHeight,
                draw(ctx) {
                    ctx.fillStyle = 'darkgreen';
                    ctx.beginPath();
                    ctx.moveTo(this.x, this.y);
                    ctx.lineTo(this.x + this.width / 2, this.y - this.height);
                    ctx.lineTo(this.x + this.width, this.y);
                    ctx.fill();
                },
                update(curveDirection) {
                    // Sincroniza el movimiento con la curva de la carretera.
                    this.x -= this.mountainSpeed * curveDirection;
                }
            });
        }
    }

    createTree() {
        const treeHeight = 50 + Math.random() * 100;
        const treeWidth = 20 + Math.random() * 30;
        const side = Math.random() < 0.5 ? 'left' : 'right';
        
        this.trees.push({
            side: side,
            x: this.road.horizonX, 
            y: this.road.horizonY,
            width: treeWidth,
            height: treeHeight,
            draw(ctx) {
                ctx.fillStyle = 'saddlebrown';
                ctx.fillRect(this.x + this.width / 2 - 5, this.y - this.height, 10, this.height);
                ctx.fillStyle = 'forestgreen';
                ctx.beginPath();
                ctx.arc(this.x + this.width / 2, this.y - this.height, this.width / 2, 0, Math.PI * 2);
                ctx.fill();
            },
            update(road) {
                const currentHorizonY = road.horizonY + Math.sin(road.trackPosition) * road.hillAmplitude;
                const distanceFactor = (this.y - currentHorizonY) / (road.canvas.height - currentHorizonY);
                
                this.y += road.baseSpeed + (distanceFactor * this.treeSpeed);
                
                this.width = treeWidth + (distanceFactor * 100);
                this.height = treeHeight + (distanceFactor * 200);

                const currentHorizonX = road.horizonX + Math.sin(road.curveOffset) * road.curveAmplitude;
                if (this.side === 'left') {
                    this.x = currentHorizonX - (distanceFactor * (road.canvas.width / 2));
                } else {
                    this.x = currentHorizonX + (distanceFactor * (road.canvas.width / 2));
                }
            }
        });
    }

    createCloud() {
        const cloudWidth = 50 + Math.random() * 100;
        const cloudHeight = 20 + Math.random() * 40;
        this.clouds.push({
            x: this.canvas.width,
            y: Math.random() * (this.road.horizonY - 50),
            width: cloudWidth,
            height: cloudHeight,
            draw(ctx) {
                ctx.fillStyle = 'rgba(255, 255, 255, 0.8)';
                ctx.beginPath();
                ctx.ellipse(this.x, this.y, this.width, this.height, 0, 0, Math.PI * 2);
                ctx.fill();
            },
            update() {
                this.x -= this.cloudSpeed;
            }
        });
    }

    update() {
        const curveDirection = Math.sin(this.road.curveOffset);
        for (const mountain of this.mountains) {
            mountain.update(curveDirection);
        }
        
        for (let i = this.trees.length - 1; i >= 0; i--) {
            const tree = this.trees[i];
            tree.update(this.road);
            if (tree.y > this.canvas.height || (tree.side === 'left' && tree.x + tree.width < 0) || (tree.side === 'right' && tree.x > this.canvas.width)) {
                this.trees.splice(i, 1);
            }
        }
        
        for (let i = this.clouds.length - 1; i >= 0; i--) {
            const cloud = this.clouds[i];
            cloud.update();
            if (cloud.x + cloud.width < 0) {
                this.clouds.splice(i, 1);
            }
        }
    }

    draw(ctx) {
        for (const mountain of this.mountains) {
            mountain.draw(ctx);
        }
        
        for (const tree of this.trees) {
            tree.draw(ctx);
        }

        for (const cloud of this.clouds) {
            cloud.draw(ctx);
        }
    }
}