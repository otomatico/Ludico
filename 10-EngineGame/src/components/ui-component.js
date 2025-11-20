export class UIComponent {
  constructor(selector) {
    this.dom = typeof selector === 'string' ? document.querySelector(selector) : selector;
    if (!this.dom) throw new Error(`UIComponent: selector no encontrado -> ${selector}`);
  }

  on(event, handler, options) {
    this.dom.addEventListener(event, handler, options);
    return () => this.dom.removeEventListener(event, handler, options);
  }

  $(sel) { return this.dom.querySelector(sel); }
  $all(sel) { return [...this.dom.querySelectorAll(sel)]; }

  addClass(c) { this.dom.classList.add(c); }
  removeClass(c) { this.dom.classList.remove(c); }
}