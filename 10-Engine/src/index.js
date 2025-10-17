import { Engine, Entity, Sprite } from "./lib/engine.js";

const Main = async (body) => {
    let canvas = document.createElement("CANVAS");
    canvas.width = "320"
    canvas.height = "240"
    canvas.style.backgroundColor = '#fafafa'

    let game = new Engine(canvas.getContext('2d'))
    await game.LoadTiles("./assets/allshd.png", [
        new Entity(40, 200, 80, 80),//ball
        new Entity(1170, 650, 80, 180),//PadA
        new Entity(1330, 650, 80, 180)//Pad
    ])
    body.style.backgroundColor = '#333'
    body.appendChild(canvas)

    console.log("Loader")
    const elements = {
        pad2: new Sprite(2, new Entity(10, 10, 20, 45)),
        pad1: new Sprite(1, new Entity(100, 10, 20, 45)),
        ball: new Sprite(0, new Entity(50, 50, 20, 20))
    }
    elements.ball.flipH = true;
    for (let prop in elements) {
        game.DrawSprite(elements[prop])
    }

}
Main(document.body)