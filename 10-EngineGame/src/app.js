import { EventBus } from './utils/event-bus.js';
import { Store } from './utils/store.js';
import { ColorPickerComponent } from './components/color-picker-component.js';
import { TabComponent } from './components/tabs.js';
import { NavBarComponent } from './components/navbar-component.js';
import { TilesComponent } from './components/tile-component.js';
import { SpriteEditorComponent } from './components/sprite-editor-component.js';
import { SpriteListComponent } from './components/sprite-list-component.js';
import { CodeMirrorComponentFactory } from './utils/codemirror-wrapper.js';
import { FileHandler } from './utils/file-handler.js';


export async function bootstrap() {
  //Utils
  const bus = new EventBus();
  const store = new Store({});
  const fileHandler = new FileHandler(store, bus);

  // components
  const ColorPicker = new ColorPickerComponent('#color-picker');
  const Tabs = new TabComponent('.tabs-container');
  const Navbar = new NavBarComponent('.navbar', bus);
  const Tile = new TilesComponent('#tile', { store, bus });
  const SpriteEditor = new SpriteEditorComponent('#spriteEditor', { colorPicker: ColorPicker, store, bus });
  const SpriteList = new SpriteListComponent('#spriteList', { store, bus });

  // CodeMirror
  const textarea = document.querySelector('textarea[name="code"]');
  let editor = null;
  if (textarea && window.CodeMirror) {
    editor = CodeMirrorComponentFactory(textarea, (code) => store.set({ code }));
  }
  // bind store Suscripción
  // Suscripción a 'gfx' para la actualizacion de sprite
  store.subscribe('gfx', (gfx, state) => {
    const colorLookup = (i) => ColorPicker.toColor(i);
    const spriteId = state.spriteId || 0;
    Tile.attachData({ gfx, colorLookup });
    Tile.drawMap(state.tiles, state.gfx, colorLookup);
    SpriteList.drawCell(spriteId, gfx, colorLookup);
    SpriteEditor.loadSprite(spriteId, gfx, colorLookup);
  });

  // Suscripción a 'tiles' para que el mapa se actualice
  store.subscribe('tiles', (tiles, state) => {
    Tile.drawMap(tiles, state.gfx, (i) => ColorPicker.toColor(i));
  });

  store.subscribe('spriteId', (spriteId, state) => {
    SpriteEditor.loadSprite(spriteId, state.gfx, (i) => ColorPicker.toColor(i));
  });

  bus.on('sprite:selected', ({ spriteId }) => {
    store.set({ spriteId: spriteId });
  });
  /*
    bus.on('sprite:modified', ({ id }) => {
      const state = store.get();
      // Redibujar la celda de la lista de sprites
      SpriteList.drawCell(id, state.gfx, (i) => ColorPicker.toColor(i));
    });*/

  bus.on('project:loaded', (state) => {
    const colorLookup = (i) => ColorPicker.toColor(i);
    // Cargar Editor MirrorCode
    if (editor) editor.setValue(state.code || '');
    //Carga la Palleta de Colores que se va a ussar
    if (state.paletteName === 'CUSTOM' && state.palette.length > 0) {
      // Cargar la paleta personalizada guardada en el estado
      ColorPicker.setCustomPalette(state.palette);
    } else {
      // Cargar paletas predefinidas por nombre (DEFAULT, CGA, etc.)
      ColorPicker.setPalette(state.paletteName || 'DEFAULT');
    }
    // Cargar los datos de la librería de sprites y redibujarla
    Tile.attachData({ gfx: state.gfx, colorLookup });
    // Redibujar el mapa de tiles
    Tile.drawMap(state.tiles || [], state.gfx, colorLookup);
    // Redibujar 
    SpriteList.drawAll(state.gfx, colorLookup);
    SpriteEditor.loadSprite(state.spriteList || 0, state.gfx, colorLookup);
  });
  bus.on('navbar:click', ({ id, label, action }) => {
    alert(`⚠️ Funcionalidad de ${label} no fue implementada aún.`);
    console.log(`Error :[${ id}, ${ label}, ${action }]`)
  });

  const fileMenu = {
    label: 'Archivo',
    items: [
      // Asignar funciones de fileHandler directamente a la clave 'action'
      { id: 'newBtn', label: 'Nuevo', action: () => fileHandler.newFile() },
      { id: 'openBtn', label: 'Abrir', action: async() => await fileHandler.openFile() },
      { id: 'saveBtn', label: 'Guardar', action: () => fileHandler.saveFile() },
      { id: 'saveAsBtn', label: 'Guardar Como', action: () => fileHandler.saveAs() },
    ]
  };
  const projectMenu = {
    label: 'Proyecto',
    items: [
      { id: 'runBtn', label: 'Ejecutar', },
      { id: 'exportGameBtn', label: 'Exportar Game',},
    ]
  }
  const paletteMenu = {
    label: 'Paleta',
    items: [
      ...Object.keys(ColorPicker.palettes).map(name => ({
        id: `palette-${name.toLowerCase()}`,
        label: name,
        action: () => {
          const success = ColorPicker.setPalette(name);
          if (success) {
            store.set({ gfx: store.get().gfx, paletteName: name });
          }
        }
      })),
      {
        id: 'palette-custom-define',
        label: '🎨 Personalizar Paleta...',
        action: () => {
          // Pedir al usuario que introduzca los colores HEX separados por comas.
          const input = prompt(
            'Introduce los códigos HEX de tu paleta, separados por comas (ej: #FF0000, #00FF00, #0000FF, #FFFFFF):'
          );

          if (!input) return; // Cancelado

          const customColors = input.split(',')
            .map(c => c.trim().toUpperCase()) // Limpiar y estandarizar
            .filter(c => /^#[0-9A-F]{6}$/i.test(c)); // Filtrar y validar HEX

          if (customColors.length < 2) {
            alert('Debes introducir al menos 2 códigos HEX válidos.');
            return;
          }

          const success = ColorPicker.setCustomPalette(customColors);

          if (success) {
            // Si es CUSTOM, guardar el nombre "CUSTOM" y el array de colores.
            store.set({
              gfx: store.get().gfx,
              paletteName: 'CUSTOM',
              palette: customColors // <-- El array de colores se guarda aquí
            });
          }
        }
      }
    ]
  };
  const aboutMenu = {
    label: 'Sobre',
    align: 'right',
    items: [
      { id: 'helpBtn', label: 'Ayuda', action: () => window.open('https://example.com/help', '_blank') },
      { id: 'aboutBtn', label: 'Crédito', action: () => alert("Game Dev 🕹️ - Inspirado en PICO-8 y TIC-80") },
    ]
  };

  //Llamar al método para construir el menú
  Navbar.buildMenu([fileMenu, projectMenu, paletteMenu, aboutMenu]);

  // iniciar proyecto por defecto
  fileHandler.newFile();

  // expose for debugging
  window.App = { bus, store, fileHandler, ColorPicker, Tabs, Navbar, Tile, SpriteEditor, SpriteList };
}

// Auto bootstrap cuando cargue el DOM (si se usa como único bundle)
if (document.readyState !== 'loading') {
  bootstrap();
} else {
  window.addEventListener('DOMContentLoaded', bootstrap);
}