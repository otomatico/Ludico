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

  Draw(id, x, y, pal = 0) {
    const sprite = this.#gfx[id];
    const palette =this.#palette[pal % 4]
    const paletteMax = palette.length
    for (let row = 0; row < 8; row++) {
      for (let col = 0; col < 8; col++) {
        let color = sprite[row ][col];
        this.#ctx.fillStyle = palette[color % paletteMax];
        this.#ctx.fillRect(
          x + (col * this.sizePixel),
          y + (row * this.sizePixel),
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
