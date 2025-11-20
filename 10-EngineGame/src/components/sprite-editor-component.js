import { UIComponent } from './ui-component.js';
import { Renderer } from './renderer.js';

export class SpriteEditorComponent extends UIComponent {
  constructor(selector = '#spriteEditor', opts = {}) {
    super(selector);
    this.opts = opts; // { store, bus }
    Renderer.createGrid(this.dom, +this.dom.dataset.gridW || 8, +this.dom.dataset.gridH || 8, +this.dom.dataset.gridPixel || 16);

    this.on('click', (e) => this.onClick(e.target));
  }

  onClick(cell) {
    if (!cell.classList.contains('cell')) return;
    const colorHex = this.opts.colorPicker.getColor();
    const indexColor = this.opts.colorPicker.toId(colorHex);
    cell.dataset.color = indexColor;
    const y = +cell.dataset.y; const x = +cell.dataset.x;
    const state = this.opts.store.get();
    state.gfx[state.spriteId][y][x] = indexColor;
    cell.style.backgroundColor = colorHex;
    
    // Notificar al store que 'gfx' ha cambiado, disparando la suscripción.
    this.opts.store.set({ gfx: state.gfx }); 
    
    // 2. Notificar al bus (opcional, si hay otra lógica que lo necesite)
    //this.opts.bus.emit('sprite:modified', { id: state.spriteId});
  }

  // cargar sprite actual desde datos (pinta el editor)
  loadSprite(spriteId, gfx, colorLookup) {
    const cells = Array.from(this.dom.querySelectorAll('.cell'));
    const sprite = gfx[spriteId] || Array(8).fill(0).map(()=>Array(8).fill(0));
    sprite.forEach((row, y) => row.forEach((color, x) => {
      const idx = y * (+this.dom.dataset.gridW) + x;
      const cell = cells[idx];
      const hex = colorLookup(color);
      cell.style.backgroundColor = hex;
      cell.dataset.color = color;
    }));
  }
}