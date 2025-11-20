import { UIComponent } from './ui-component.js';

// 1. Definición de Paletas: Puedes añadir más paletas aquí
const PALETTES = {
  // Paleta Default (Basada en los 16 colores CGA extendidos, por ejemplo)
  'DEFAULT': [
    '#000000', '#1D2B53', '#7E2553', '#008751',
    '#AB5236', '#5F574F', '#C2C3C7', '#FFF1E8',
    '#FF0000', '#FF77A8', '#FFCCAA', '#FFEF0F',
    '#00FF00', '#00CCFF', '#4C8EEA', '#9B61E5'
  ],
  // Paleta CGA (Clásica de 16 colores)
  'CGA': [
    '#000000', '#0000AA', '#00AA00', '#00AAAA',
    '#AA0000', '#AA00AA', '#AA5500', '#AAAAAA',
    '#555555', '#5555FF', '#55FF55', '#55FFFF',
    '#FF5555', '#FF55FF', '#FFFF55', '#FFFFFF'
  ],
  // Paleta PICO-8 
  'PICO-8': [
    '#000000', '#1D2B53', '#7E2553', '#008751',
    '#AB5236', '#5F574F', '#C2C3C7', '#FFF1E8',
    '#FF004D', '#FFA300', '#FFEC27', '#00E436',
    '#29ADFF', '#83769C', '#FF77A8', '#FFCCAA'
  ],
  // Paleta GB-POCKET
  'GB-POCKET': [
    '#000000', '#808080', '#c0c0c0', '#ffffff'
  ]
};

export class ColorPickerComponent extends UIComponent {
  constructor(selector = '#color-picker') {
    super(selector);

    // Almacenamos todas las paletas
    this.palettes = PALETTES;
    this.currentPaletteName = 'DEFAULT';

    // Inicializar con la paleta por defecto
    this.colors = this.palettes['DEFAULT'];
  }


  setPalette(name) {
    const newColors = this.palettes[name.toUpperCase()];
    if (!newColors) {
      console.warn(`Paleta ${name} no encontrada.`);
      return false;
    }

    this.colors = newColors.map(c => c.toUpperCase());
    this.currentPaletteName = name;

    // Actualizar el <datalist> en el DOM para que el selector de color refleje la nueva paleta
    const dataList = this.dom.list;
    if (dataList) {
      dataList.innerHTML = '';
      this.colors.forEach((color, index) => {
        const option = document.createElement('option');
        // Añadir el color 0 (negro/transparente) como opción por defecto si no existe
        option.value = color;
        option.textContent = `Color ${index}`;
        dataList.appendChild(option);
      });
    }

    return true;
  }
  getPalette() {
    return this.colors;
  }
  setCustomPalette(customColors) {
    if (!Array.isArray(customColors) || customColors.length < 2) {
      console.warn('La paleta personalizada debe ser un array con al menos 2 colores.');
      return false;
    }
    this.colors = customColors.map(c => c.toUpperCase());
    this.currentPaletteName = 'CUSTOM';
    this.updateDataList(this.colors);
    return true;
  }

  //Utilidad para actualizar el datalist
  updateDataList(colors) {
    const dataList = this.dom.list;
    if (dataList) {
      dataList.innerHTML = '';
      colors.forEach((color, index) => {
        const option = document.createElement('option');
        option.value = color;
        option.textContent = `Color ${index}`;
        dataList.appendChild(option);
      });
    }
  }

  getColor() { return this.dom.value; }
  toId(color) { return this.colors.indexOf(color.toUpperCase()); }
  //Devuelve el color de la paleta activa
  toColor(id) { return this.colors[id]; }
}