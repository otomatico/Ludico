import { UIComponent } from './ui-component.js';
import { Renderer } from './renderer.js';

export class SpriteEditorComponent extends UIComponent {

  constructor(selector = '#spriteEditor', opts = {}) {
    super(selector);
    this.opts = opts; // { store, bus }
    this.tool = "brush";  // "brush" | "bucket"
    this.thickness = 1;   // 1, 2, 3
    Renderer.createGrid(this.dom, +this.dom.dataset.gridW || 8, +this.dom.dataset.gridH || 8, +this.dom.dataset.gridPixel || 16);
    this.on('click', (e) => this.onClick(e.target));
  }

  setTool(name) {
    const tools = { "brush": "brush", "bucket": "bucket", "clean": "clean" };
    this.tool = tools[name] || "brush";
  }
  setThickness(value) {
    this.thickness = value;
  }
  setColor(color, index) {
    this.colorHex = color;
    this.indexColor = index;
  }

  onClick(cell) {
    if (!cell.classList.contains('cell')) return;
    const { indexColor, colorHex } = this;

    const y = +cell.dataset.y;
    const x = +cell.dataset.x;

    const state = this.opts.store.get();
    const sprite = state.gfx[state.spriteId];

    switch (this.tool) {
      case "bucket":
        this.floodFill(sprite, x, y, indexColor);
        break;
      case "brush":
        this.paintBlock(sprite, x, y, indexColor, colorHex);
        break;
      case "clean":
        this.cleanCanvas();
        break;
    }
    this.opts.bus?.emit('sprite:selected', { spriteId: state.spriteId })
  }
  //Tools "Event"
  paintBlock(sprite, x, y, newColor, newHex) {
    const size = this.thickness; // 1,2,3...
    const half = Math.floor(size / 2);

    for (let dy = -half; dy <= half; dy++) {
      for (let dx = -half; dx <= half; dx++) {
        const yy = y + dy;
        const xx = x + dx;

        if (sprite[yy] && sprite[yy][xx] !== undefined) {
          sprite[yy][xx] = newColor;

          const cell = this.$(`.cell[data-x="${xx}"][data-y="${yy}"]`);
          if (cell) {
            cell.dataset.color = newColor;
            cell.style.backgroundColor = newHex;
          }
        }
      }
    }
  }

  floodFill(sprite, startX, startY, newColor) {
    const oldColor = sprite[startY][startX];
    if (oldColor === newColor) return;

    const w = sprite[0].length;
    const h = sprite.length;

    const stack = [[startX, startY]];

    while (stack.length > 0) {
      const [x, y] = stack.pop();

      if (x < 0 || x >= w || y < 0 || y >= h) continue;
      if (sprite[y][x] !== oldColor) continue;

      sprite[y][x] = newColor;
      /*
      const cell = this.$(`.cell[data-x="${x}"][data-y="${y}"]`);
      if (cell) {
        cell.dataset.color = newIndexColor;
        cell.style.backgroundColor = newHexColor;
      }
      */

      stack.push([x + 1, y]);
      stack.push([x - 1, y]);
      stack.push([x, y + 1]);
      stack.push([x, y - 1]);
    }
  }

  cleanCanvas() {
    const state = this.opts.store.get();
    const sprite = state.gfx[state.spriteId];

    for (let y = 0; y < sprite.length; y++) {
      for (let x = 0; x < sprite[y].length; x++) {

        sprite[y][x] = 0;
/* 
        const cell = this.$(`.cell[data-x="${x}"][data-y="${y}"]`);
        if (cell) {
          cell.dataset.color = 0;
          cell.style.backgroundColor = this.colorPicker.toColor(0);
        }
 */      }
    }
    this.opts.store.set({ gfx: state.gfx });
  }

  // cargar sprite actual desde datos (pinta el editor)
  loadSprite(spriteId, gfx, colorLookup) {
    const cells = Array.from(this.dom.querySelectorAll('.cell'));
    const sprite = gfx[spriteId] || Array(8).fill(0).map(() => Array(8).fill(0));
    sprite.forEach((row, y) => row.forEach((color, x) => {
      const idx = y * (+this.dom.dataset.gridW) + x;
      const cell = cells[idx];

      const hex = colorLookup(color);
      cell.style.backgroundColor = hex;
      cell.dataset.color = color;
    }));
  }

}