const opts = {
  suggestedName: name,
  types: [{
    description: 'Archivo de Proyecto',
    accept: { 'application/json': ['.json'] },
  }],
};

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
  async openFile() {
    let [handler] = await window.showOpenFilePicker(opts)
    let fileData = await handler.getFile();
    let payload = await fileData.text();
    payload = JSON.parse(payload)
    this.store.set(payload);
    this.bus.emit('project:loaded', payload);
  }

  async saveAs() {
    let handle = await window.showSaveFilePicker(opts);

    let fileData = await handle.getFile();
    let writable = await handle.createWritable();
    this.store.set({ filename: fileData.name });

    await writable.write(this._payload_text());
    await writable.close();
  }

  saveFile() {
    const s = this.store.get();
    this.download(s.filename, this._payload_text());
  }

  download(filename, content) {
    const blob = new Blob([content], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url; a.download = filename; a.click();
    URL.revokeObjectURL(url);
  }

  _payload() {
    const s = this.store.get();
    return {
      filename: s.filename,
      paletteName: s.paletteName || 'DEFAULT',
      palette: s.palette || [],
      gfx: s.gfx,
      code: s.code,
      tiles: s.tiles,
    };
  }
  _payload_text() {
    const s = this.store.get();
    return `{
      "filename": "${s.filename}",
      "paletteName": "${s.paletteName || 'DEFAULT'}",
      "palette": ${JSON.stringify(s.palette || [])},
      "gfx": [
        ${s.gfx.map(m=>JSON.stringify(m)).join(',\n\t\t\t\t')}
      ],
      "code": ${JSON.stringify(s.code)},
      "tiles": [
        ${s.tiles.map(t=>JSON.stringify(t)).join(',\n\t\t\t\t')}
      ]
}`;
  }
}
/*
  uploadFile(file) {
    const reader = new FileReader();
    reader.onload = (evt) => {
      try {
        const obj = JSON.parse(evt.target.result);
        const state = { ...this.store.get(), ...obj};
        state.filename = file.name;
        this.store.set(state);
        this.bus.emit('project:loaded', state);
      } catch (e) {
        alert('Archivo JSON no válido.');
      }
    };
    reader.readAsText(file);
  }
  */

