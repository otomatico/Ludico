class Engine {
    #ctx = null
    #size = null
    #img = null
    constructor(ctx, size = 4) {
        this.#ctx = ctx;
        this.#size = size
    }

    async LoadTiles(fileName, spriteMap) {
        this.Map = spriteMap;
        this.#img = await new Promise((resolve, reject) => {
            const img = new Image();
            img.onload = () => resolve(img);
            img.onerror = (err) => reject(err);
            img.src = fileName;
        });
    }

    DrawSprite(sprite) {
        let clip = this.Map[sprite.id]
        //let { x, y, width, height } = sprite
        if (sprite.flipH) {

        }
        if (sprite.flipV) {

        }
        console.table([clip, sprite])
        this.#ctx.drawImage(this.#img,
            clip.x, clip.y, clip.width, clip.height,
            sprite.x, sprite.y, sprite.width, sprite.height
        )
    }
};

class Entity {
    constructor(x, y, width, height) {
        this.x = x
        this.y = y
        this.width = width
        this.height = height
    }
}

class Sprite extends Entity {

    constructor(id, { x, y, width, height }) {
        super(x, y, width, height)
        this.id = id
        this.flipH = false
        this.flipV = false
    }
    Collide(entity) {
        return this.x < entity.x + entity.w &&
            this.x + this.w > entity.x &&
            this.y < entity.y + entity.h &&
            this.h + this.y > entity.y
    }
}
/*
const enumShape = { Line: 0, Square: 1, SquareFill: 2 }
let shapeForm = [
    [enumShape.Line, color, x0, y0, x1, y1],
    [enumShape.Square, color, x, y, w, h],
    [enumShape.SquareFill, color, x, y, w, h]
]*/
export { Engine, Sprite, Entity };