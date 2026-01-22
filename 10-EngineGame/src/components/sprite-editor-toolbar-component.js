import { UIComponent } from './ui-component.js';

export class SpriteEditorToolbar extends UIComponent {
  constructor(selector, opts = {}) {
    super(selector);
    this.bus = opts.bus; 
    this.eventsHandler();
  }

  eventsHandler() {
    /*
    // 🎨 Color
    this.$('#color-picker')?.addEventListener('input', (e) => {
      this.bus.emit("paint:color", e.target.value);
    });
    */

    // 🖌️ Herramientas
    this.$all('input[name="toolPaint"]').forEach(radio => {
      radio.addEventListener("change", () => {
        this.bus.emit("paint:tool", {tool:radio.value});
      });
    });

    // 🧱 Grosor
    const thick = this.$('#toolSize');
    thick?.addEventListener("input", (e) => {
      this.bus.emit("paint:thickness", {size:(+e.target.value)});
    });
  }
}
