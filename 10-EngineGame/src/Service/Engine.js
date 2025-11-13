class Engine {
  #canvas = null;
  #ctx = null;
  //gfx=>128x64=> (16x8)x8
  #gfx = [];
  #mapTile = [];
  #palette = [];
  
  constructor(width = 320, height = 240, color = "#fafafa") {
    //40x30x8
    this.#canvas = document.createElement("CANVAS");
    this.#canvas.width = width;
    this.#canvas.height = height;
    this.#canvas.style.backgroundColor = color;
    document.body.appendChild(this.#canvas);
    this.#ctx = this.#canvas.getContext("2d");
    this.sizePixel = 1;
    this.#palette = [
      paletteDefault,
      paletteDefault,
      paletteDefault,
      paletteDefault,
    ];
  }

  async Wait(hertz=20) 
  {
    //const hertz = 1000 / 50;
    await new Promise((resolver) => setTimeout(resolver, hertz));
  }

  Clear() {
    let { width, height } = this.#canvas;
    this.#ctx.clearRect(0, 0, width, height);
  }

  loadGFX(map) {
    this.#gfx = [...map];
  }

  loadTileMap(map) {
    this.#mapTile = [...map];
  }

  setPalettes(palette) {
    this.#palette = [palette[0], palette[1], palette[2], palette[3]];
  }

  setPalette(palette, id=0) {
    this.#palette[id % 4] = [...palette];
  }

  Draw(id, x, y, w, h, pal = 0) {
    const startX = (id % 16) * 8;
    const startY = parseInt(id / 16) * 8;
    for (let row = 0; row < 8 * h; row++) {
      for (let col = 0; col < 8 * w; col++) {
        let color = this.#gfx[row + startY][col + startX];
        this.#ctx.fillStyle = this.#palette[pal % 4][color % 16];
        this.#ctx.fillRect(
          (x + col) * this.sizePixel,
          (y + row) * this.sizePixel,
          this.sizePixel,
          this.sizePixel
        );
      }
    }
  }
  TileDraw(x, y) {}
}

const paletteDefault = [
  "#F5F5F5",
  "#DCDCDC",
  "#D3D3D3",
  "#C0C0C0",
  "#778899",
  "#BEBEBE",
  "#A9A9A9",
  "#9B9887",
  "#808080",
  "#707070",
  "#696969",
  "#5F5A5A",
  "#4F4F4F",
  "#333333",
  "#191919",
  "#000000",
];
/*
Blanco Humo (White Smoke)	#F5F5F5
Gris Gema (Gainsboro)	#DCDCDC
Gris Claro (Light Gray)	#D3D3D3
Plata (Silver)	#C0C0C0
Gris Ceniza (Light Slate Gray)	#778899
Gris Medio (Medium Gray)	#BEBEBE
Gris Pálido (Dark Gray)	#A9A9A9
Gris Topo (Taupe Gray)	#8B8887
Gris (Gray)	#808080
Gris Cadete (Cadet Gray)	#5F9EA0
Gris Pizarra (Slate Gray)	#708090
Gris Oscuro (Dim Gray)	#696969
Gris Plomo (Dark Slate Gray)	#2F4F4F
Gris Carbón (Charcoal Gray)	#333333
Gris Medianoche (Midnight Gray)	#191970
Negro (Black)	#000000
*/

/*
  collide(other) {
    const x1 = this.position.x;
    const y1 = this.position.y;
    const w1 = this.size.w;
    const h1 = this.size.h;
    const x2 = other.position.x;
    const y2 = other.position.y;
    const w2 = other.size.w;
    const h2 = other.size.h;
    return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && h1 + y1 > y2;
  }
*/
