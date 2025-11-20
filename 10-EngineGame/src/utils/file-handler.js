export class FileHandler {
  constructor(store, bus) {
    this.store = store; this.bus = bus;
  }

  newFile() {
    const fecha = new Date().toISOString().split('T')[0];
    const state = {
      spriteId: 0,
      filename: `undefined-${fecha}.json`,
      paletteName: 'DEFAULT',
      gfx: Array(128).fill(null).map(() => Array(8).fill(null).map(() => Array(8).fill(0))),
      tiles: Array(8).fill(null).map(() => Array(32).fill(0)),
      code: `\nfunction update() {\n\n}\n\nfunction render() {\n\n}\n`,
      palette: [],
    };
    this.store.set(state);
    this.bus.emit('project:loaded', state);
  }

  saveAs(promptFn = prompt) {
    const name = promptFn('Nombre del Proyecto');
    if (!name) return;
    const s = this.store.get();
    this.store.set({ filename: name });
    this.saveFile();
  }

  saveFile() {
    const s = this.store.get();
    const filename = s.filename;
    const payload = {
      filename: s.filename,
      paletteName: s.paletteName || 'DEFAULT',
      palette: s.palette || [],
      gfx: s.gfx,
      code: s.code,
      tiles: s.tiles,
    };
    this.download(filename, JSON.stringify(payload));
  }

  download(filename, content) {
    const blob = new Blob([content], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url; a.download = filename; a.click();
    URL.revokeObjectURL(url);
  }

  uploadFile(file) {
    const reader = new FileReader();
    reader.onload = (evt) => {
      try {
        const obj = JSON.parse(evt.target.result);
        const state = { ...this.store.get(), ...obj/* gfx: obj.gfx, tiles: obj.tiles, code: obj.code  */ };
        state.filename = file.name;
        this.store.set(state);
        this.bus.emit('project:loaded', state);
      } catch (e) {
        alert('Archivo JSON no válido.');
      }
    };
    reader.readAsText(file);
  }
}

