import { UIComponent } from './ui-component.js';
export class TabComponent extends UIComponent {
  constructor(selector = '.tabs-container') {
    super(selector);
    this.on('click', (e) => {
      if (e.target.classList.contains('tab')) this.activate(e.target);
    });
    // show first
    this.$('.tab')?.click();
  }

  activate(tabElem) {
    this.$('.active')?.classList.remove('active');
    this.$('.tab-panel .show')?.classList.remove('show');
    tabElem.classList.add('active');
    const panel = this.$(tabElem.dataset.tag);
    panel?.classList.add('show');
  }
}