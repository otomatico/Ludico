import { UIComponent } from './ui-component.js';
import { Renderer } from './renderer.js';

// Constantes para los selectores de las grillas dentro del componente
const TILE_LIST_SELECTOR = '#tileList';
const TILE_DATA_SELECTOR = '#tileData';

export class TilesComponent extends UIComponent {
  constructor(selector = '#tile', opts = {}) {
    super(selector);
    this.opts = opts; // { store, bus }

    // 1. Inicializar grillas y datos
    this.list = this.dom.querySelector(TILE_LIST_SELECTOR); // El "mapa" de tiles
    this.data = this.dom.querySelector(TILE_DATA_SELECTOR); // La "librería" de sprites
    this.selectedSprite = 0; // El ID del sprite seleccionado actualmente

    // Inicializar el mapa de tiles (Tile.list)
    Renderer.createGrid(this.list,
      +this.list.dataset.gridW || 32,
      +this.list.dataset.gridH || 8,
      +this.list.dataset.gridPixel || 8
    );
    // Inicializar la biblioteca de sprites (Tile.data)
    Renderer.createGrid(this.data,
      +this.data.dataset.gridW || 16,
      +this.data.dataset.gridH || 4,
      +this.data.dataset.gridPixel || 8
    );

    // 2. Escuchar los eventos de click
    this.list.addEventListener('click', (e) => this.onMapClick(e));
    this.data.addEventListener('click', (e) => this.onLibraryClick(e));
  }

  // --- Manejo de Clicks ---

  // Click en la librería de sprites (Tile.data)
  onLibraryClick(e) {
    const cell = e.target;
    if (cell.tagName !== 'CANVAS' || !cell.classList.contains('cell')) return;

    // Obtener el ID del sprite (índice en la librería 16x4 = 64 sprites)
    const x = +cell.dataset.x;
    const y = +cell.dataset.y;
    this.selectedSprite = y * (+this.data.dataset.gridW) + x;

    // Puedes agregar alguna lógica visual para resaltar la selección aquí, si quieres:
    this.data.querySelectorAll('.cell').forEach(c => c.classList.remove('selected'));
    cell.classList.add('selected');

    console.log(`Sprite seleccionado: ${this.selectedSprite}`);
  }

  // Click en el mapa de tiles (Tile.list)
  onMapClick(e) {
    const cell = e.target;
    if (cell.tagName !== 'CANVAS' || !cell.classList.contains('cell')) return;

    // Obtener la posición en el mapa
    const x = +cell.dataset.x;
    const y = +cell.dataset.y;

    // 1. Obtener el estado actual (mapa de tiles y librería de sprites)
    const state = this.opts.store.get();
    const mapTiles = state.tiles;

    // 2. Actualizar el mapa de tiles con el sprite ID seleccionado
    if (!mapTiles[y]) mapTiles[y] = [];
    mapTiles[y][x] = this.selectedSprite;
    
    // 3. Notificar al store para que el resto de componentes se actualicen (ej: el editor de código)
    // Es vital llamar a set para que se ejecuten los suscriptores.
    this.opts.store.set({ tiles: mapTiles });

    // 4. Redibujar SOLO la celda modificada
    this.drawCell(x, y, this.selectedSprite, state.gfx, this.colorLookup);
  }

  // --- Funciones de Dibujo ---

  attachData({ gfx, colorLookup }) {
    this.gfx = gfx;
    this.colorLookup = colorLookup;
    this.drawLibrary(gfx, colorLookup);
  }

  // Dibuja la librería de sprites (Tile.data)
  drawLibrary(gfx, colorLookup) {
    const libraryCells = Array.from(this.data.querySelectorAll('.cell'));
    const gridW = +this.data.dataset.gridW;
    const spriteSize = +this.data.dataset.gridW > 8 ? 8 : 8; // Asumimos sprites 8x8

    libraryCells.forEach((cell, idx) => {
      const spriteId = idx;
      const sprite = gfx[spriteId];
      if (sprite) {
        Renderer.drawSprite(cell.getContext('2d'), sprite, colorLookup, spriteSize);
      } else {
        // Limpiar si no hay sprite
        cell.getContext('2d').clearRect(0, 0, cell.width, cell.height);
      }
      cell.dataset.color = spriteId;
    });
  }

  // Dibuja el mapa de tiles (Tile.list)
  drawMap(tiles, gfx, colorLookup) {
    const mapCells = Array.from(this.list.querySelectorAll('.cell'));
    const mapW = +this.list.dataset.gridW;
    const spriteSize = +this.list.dataset.gridW > 32 ? 8 : 8;

    mapCells.forEach((cell, idx) => {
      const x = idx % mapW;
      const y = Math.floor(idx / mapW);
      const spriteId = tiles[y] ? (tiles[y][x] || 0) : 0; // Por defecto sprite 0

      const sprite = gfx[spriteId];
      if (sprite) {
        Renderer.drawSprite(cell.getContext('2d'), sprite, colorLookup, spriteSize);
      } else {
        cell.getContext('2d').clearRect(0, 0, cell.width, cell.height);
      }
      cell.dataset.color = spriteId; // Guarda el ID del sprite asociado a la celda del mapa
    });
  }
  
  // Dibuja una única celda del mapa
  drawCell(x, y, spriteId, gfx, colorLookup) {
    const mapW = +this.list.dataset.gridW;
    const idx = y * mapW + x;
    const cell = this.list.querySelectorAll('.cell')[idx];
    const sprite = gfx[spriteId];
    if (cell && sprite) {
      Renderer.drawSprite(cell.getContext('2d'), sprite, colorLookup, 8);
    }
    cell.dataset.color = spriteId;
  }
}