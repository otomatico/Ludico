import { UIComponent } from './ui-component.js';
import { Renderer } from './renderer.js';

export class SpriteListComponent extends UIComponent {
  constructor(selector = '#spriteList', opts = {}) {
    super(selector);
    this.opts = opts; // { bus, store }
    Renderer.createGrid(this.dom, +this.dom.dataset.gridW || 16, +this.dom.dataset.gridH || 4, +this.dom.dataset.gridPixel || 8);

    this.on('click', (e) => {
      const target = e.target;
      if (target.tagName === 'CANVAS' || target.classList.contains('cell')) {
        const ctx = target.getContext('2d');
        const imageData = ctx.getImageData(0, 0, ctx.canvas.width, ctx.canvas.height);
        const spriteId = +target.dataset.y * (+this.dom.dataset.gridW || 16) + (+target.dataset.x || 0);
        this.opts.bus.emit('sprite:selected', { spriteId, imageData });
      }
    });
  }

  // dibuja una celda individual a partir de gfx
  drawCell(id, gfx, colorLookup) {
    const cell = this.dom.querySelectorAll('.cell')[id];
    if (!cell) return;
    const ctx = cell.getContext('2d');
    ctx.clearRect(0, 0, ctx.canvas.width, ctx.canvas.height);
    Renderer.drawSprite(cell.getContext('2d'), gfx[id], colorLookup, 8);
  }

  // dibuja todo el listado
  drawAll(gfx, colorLookup) {
    this.$all('.cell').forEach((c, id) => this.drawCell(id, gfx, colorLookup));
  }
}
