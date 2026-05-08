// ======================
// OBJETO PARA MANEJAR PESTAÑAS
// ======================
const TabManager = {
    tabCounter: 0,

    // Añade una nueva pestaña
    addNewTab() {
        this.tabCounter++;
        const tabId = `tab-${this.tabCounter}`;
        const paneId = `pane-${this.tabCounter}`;

        const tabsHeader = document.querySelector(".tabs-header");
        const tabsContent = document.querySelector(".tabs-content");

        // Crear botón de pestaña
        const tabButton = document.createElement("div");
        tabButton.id = tabId;
        tabButton.className = "tab-button";
        tabButton.innerHTML = `Petición ${this.tabCounter}<span class="tab-close" onclick="event.stopPropagation(); TabManager.closeTab('${tabId}', '${paneId}')">x</span>`;
        tabButton.addEventListener('click', () => this.switchTab(tabId, paneId));

        // Crear panel de pestaña
        const tabPane = document.createElement("div");
        tabPane.id = paneId;
        tabPane.className = "tab-pane";

        // Contenido del panel
        tabPane.innerHTML = `
            <div class="main-wrapper">
                <div class="container">
                    <div class="row-table">
                        <div class="cell" style="width:100px;flex:none">
                            <label>Método:</label>
                            <select id="method-${tabId}">
                                <option value="POST">POST</option>
                                <option value="GET">GET</option>
                                <option value="PUT">PUT</option>
                                <option value="DELETE">DELETE</option>
                            </select>
                        </div>
                        <div class="cell" style="width:100%">
                            <label>Endpoint URL:</label>
                            <input type="text" id="url-${tabId}" value="http://localhost:8080/api/graphql" placeholder="URL de la API">
                        </div>
                        <div class="cell" style="flex:none">
                            <button class="btn btn-execute" onclick="event.stopPropagation(); RequestManager.executeRequest('${tabId}')">Enviar</button>
                        </div>
                    </div>
                    <label>Headers de la Petición:</label>
                    <div id="headers-container-${tabId}"></div>
                    <button class="btn btn-add" onclick="event.stopPropagation(); TabManager.addHeaderRow('${tabId}', '', '')">+ Añadir Cabecera</button>
                    <label for="body-${tabId}">Cuerpo de la Petición (Body/Query):</label>
                    <textarea id="body-${tabId}" placeholder='{"query": "{ ... }"}'></textarea>
                </div>
                <div id="output-${tabId}" class="output-panel">
                    Los resultados de la API aparecerán en este panel...
                </div>
            </div>
        `;

        // Añadir al DOM
        tabsHeader.insertBefore(tabButton, tabsHeader.lastChild);
        tabsContent.appendChild(tabPane);

        // Activar la nueva pestaña
        this.switchTab(tabId, paneId);

        // Añadir headers por defecto
        this.addHeaderRow(tabId, "Content-Type", "application/json");
    },

    // Cierra una pestaña
    closeTab(tabId, paneId) {
        if (document.querySelectorAll(".tab-button").length <= 1) {
            alert("No puedes cerrar la última pestaña.");
            return;
        }

        const tabButton = document.getElementById(tabId);
        if (tabButton) tabButton.remove();

        const tabPane = document.getElementById(paneId);
        if (tabPane) tabPane.remove();

        // Activar la primera pestaña restante
        const firstTabButton = document.querySelector(".tab-button");
        if (firstTabButton) {
            const tabId = firstTabButton.id;
            const paneId = `pane-${tabId.replace("tab-", "")}`;
            this.switchTab(tabId, paneId);
        }
    },

    // Cambia entre pestañas
    switchTab(tabId, paneId) {
        document.querySelectorAll(".tab-button").forEach(tab => {
            tab.classList.remove("active");
        });
        document.querySelectorAll(".tab-pane").forEach(pane => {
            pane.classList.remove("active");
        });

        document.getElementById(tabId).classList.add("active");
        document.getElementById(paneId).classList.add("active");
    },

    // Añade una fila de header
    addHeaderRow(tabId, key, value) {
        const container = document.getElementById(`headers-container-${tabId}`);
        const div = document.createElement("div");
        div.className = "header-row";
        div.innerHTML = `
            <div class="cell">
                <input type="text" class="h-key" placeholder="Key" value="${key || ""}">
            </div>
            <div class="cell">
                <input type="text" class="h-val" placeholder="Value" value="${value || ""}">
            </div>
            <div class="cell" style="width:35px">
                <button class="btn btn-del" onclick="event.stopPropagation(); TabManager.removeHeader(this)">x</button>
            </div>
        `;
        container.appendChild(div);
        // Aplicar placeholders a los nuevos inputs
        this.applyPlaceholdersToTab(tabId);
    },

    // Elimina una fila de header
    removeHeader(btn) {
        const row = btn.closest(".header-row");
        if (row) row.remove();
    },

    // Obtiene la configuración de una pestaña (URL, método, body, headers)
    getTabConfig(tabId) {
        const getVal = (id) => {
            const el = typeof id === "string" ? document.getElementById(id) : id;
            if (!el) return "";
            const placeholder = el.getAttribute("placeholder");
            return el.value === placeholder ? "" : el.value;
        };

        const config = {
            url: getVal(`url-${tabId}`),
            method: getVal(`method-${tabId}`),
            body: getVal(`body-${tabId}`),
            headers: [],
        };

        // Recoger todos los headers dinámicos
        const headerRows = document.querySelectorAll(`#headers-container-${tabId} .header-row`);
        headerRows.forEach(row => {
            const keyInput = row.querySelector('.h-key');
            const valInput = row.querySelector('.h-val');
            const key = getVal(keyInput);
            const val = getVal(valInput);
            if (key !== "") {
                config.headers.push({ key, val });
            }
        });

        return config;
    },

    // Carga la configuración en una pestaña
    loadConfigToTab(tabId, data) {
        document.getElementById(`url-${tabId}`).value = data.url || "";
        document.getElementById(`method-${tabId}`).value = data.method || "POST";
        document.getElementById(`body-${tabId}`).value = data.body || "";

        // Limpiar headers existentes
        document.getElementById(`headers-container-${tabId}`).innerHTML = "";

        // Añadir headers desde el archivo
        if (data.headers && data.headers.length > 0) {
            data.headers.forEach(header => {
                this.addHeaderRow(tabId, header.key, header.val);
            });
        } else {
            // Headers por defecto
            this.addHeaderRow(tabId, "Content-Type", "application/json");
        }
    },

    // Aplica placeholders a los inputs de una pestaña
    applyPlaceholdersToTab(tabId) {
        const paneId = `pane-${tabId.replace("tab-", "")}`;
        const tabPane = document.getElementById(paneId);
        if (!tabPane) return;
        const elementos = tabPane.querySelectorAll("[placeholder]");
        elementos.forEach(el => setupElements(el));
    },
};

// Funciones de placeholder (auxiliares)
function setupElements(el) {
    const placeholder = el.getAttribute("placeholder");
    if (placeholder && el.value === "") {
        el.value = placeholder;
        el.classList.add("placeholder-style");
    }

    el.addEventListener('focus', function() {
        if (this.value === this.getAttribute("placeholder")) {
            this.value = "";
            this.classList.remove("placeholder-style");
        }
    });

    el.addEventListener('blur', function() {
        if (this.value === "") {
            this.value = this.getAttribute("placeholder");
            if (!this.classList.contains("placeholder-style")) {
                this.classList.add("placeholder-style");
            }
        }
    });
}