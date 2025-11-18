//ENUM 🏗️
const Flag = {
    None: 0,      //Reset
    Visibled: 1,  //This is visible
    Updated: 2,   //This move
    Collided: 4,  //This can hitbox
    Horizontal: 8, //Move axis X
    Vertical: 16   //Move axis Y
}

class Entity {
    constructor(id = null, x = 0, y = 0, height = 0, width = 0) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.height = height;
        this.width = width;
    }
}
class Sprite extends Entity {
    constructor(entity, flag) {
        const { id, x, y, width, height } = entity;
        super(id, x, y, width, height);
        this.flag = flag;
    }
}

class Player extends Sprite {
    constructor(x, y, width, height) {
        let entity = { id: 0, x: x, y: y, width: width, height: height };
        let flag = Flag.Visibled | Flag.Updated | Flag.Collided;
        super(entity, flag);
        this.lastX = x;
        this.lastY = y;
    }

    Collide(otherEntity) {
        return this.x < otherEntity.x + otherEntity.w &&
            this.x + this.w > otherEntity.x &&
            this.y < otherEntity.y + otherEntity.h &&
            this.h + this.y > otherEntity.y;
    }

    Move(x, y) {
        this.lastX = this.x
        this.lastY = this.Y
        if (this.x + x < 0) {
            this.flag = this.flag | Flag.Horizontal;
        } else {
            this.flag = this.flag & !Flag.Horizontal;
        }
        if (this.y + y < 0) {
            this.flag = this.flag | Flag.Vertical;
        } else {
            this.flag = this.flag & !Flag.Vertical;
        }
        this.x += (this.x + x >= 0) ? x : 0;
        this.y += (this.y + y >= 0) ? y : 0;
    }
}
