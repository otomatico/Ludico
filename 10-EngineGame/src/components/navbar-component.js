import { UIComponent } from './ui-component.js';

export class NavBarComponent extends UIComponent {
    constructor(selector = '.navbar', bus) {
        super(selector);
        this.bus = bus;

        // Lógica para cerrar otros menús
        this.on('click', (e) => {
            const detailOpened = this.$('[open]'); // Encuentra el <details> abierto
            if (e.target.tagName === 'SUMMARY' && e.target.parentNode.hasAttribute("open")) {
                return;
            }
            // cerramos el menú que estaba abierto previamente.
            detailOpened?.removeAttribute('open')
        });
    }

    buildMenu(menuData) {
        this.dom.innerHTML = '';

        menuData.forEach(menu => {
            // Insertar espaciador si la alineación es 'right' Solo delbe de haber uno.. supongo
            if (menu.align === 'right') {
                const spacer = document.createElement('span');
                spacer.className = 'spacer';
                this.dom.appendChild(spacer);
            }

            const details = document.createElement('details');
            details.className = 'dropdown';

            const summary = document.createElement('summary');
            summary.className = 'dropbtn';
            summary.textContent = menu.label;
            details.appendChild(summary);

            const ul = document.createElement('ul');
            ul.className = 'dropdown-content hover';

            // 2. Añadir los elementos del submenú (<li>)
            menu.items.forEach(item => {
                const li = document.createElement('li');
                li.textContent = item.label;
                li.id = item.id;

                // EJECUTAR FUNCIÓN DIRECTA O EMITIR EVENTO
                li.addEventListener('click', () => {
                    if (typeof item.action === 'function') {
                        // Ejecutar la función asignada directamente
                        item.action();
                    } else if (this.bus) {
                        // Comportamiento de fallback: emitir evento genérico si no hay función
                        this.bus.emit('navbar:click', { id: item.id, label: item.label, action: item.action || item.id });
                    }
                    // Cerrar el menú después de la selección
                    details.removeAttribute('open');
                });
                ul.appendChild(li);
            });

            details.appendChild(ul);
            this.dom.appendChild(details);
        });
    }
}