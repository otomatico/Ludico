// ======================
// FUNCIONES DE MENÚ
// ======================
function toggleMenu() {
    var menu = document.getElementById("fileMenu");
    if (menu.className.indexOf("show") == -1) {
        menu.className += " show";
    } else {
        menu.className = menu.className.replace(" show", "");
    }
}

// Cerrar el menú si se hace clic fuera de él
document.onclick = function (event) {
    if (!event.target.className.match('menu-button')) {
        var menu = document.getElementById("fileMenu");
        menu.className = menu.className.replace(" show", "");
    }
};

// ======================
// FUNCIONES DE PLACEHOLDER
// ======================
function applyPlaceholders() {
    var elementos = document.querySelectorAll("[placeholder]");
    for (var i = 0; i < elementos.length; i++) {
        setupElements(elementos[i]);
    }
}

function setupElements(el) {
    var p = el.getAttribute("placeholder");
    if (p && el.value === "") {
        el.value = p;
        el.className += " placeholder-style";
    }
    el.onfocus = function () {
        if (this.value === this.getAttribute("placeholder")) {
            this.value = "";
            this.className = this.className.replace(/\s?placeholder-style/g, "");
        }
    };
    el.onblur = function () {
        if (this.value === "") {
            this.value = this.getAttribute("placeholder");
            if (this.className.indexOf("placeholder-style") === -1) {
                this.className += " placeholder-style";
            }
        }
    };
}

// ======================
// FUNCIONES DE PESTAÑAS
// ======================
var tabCounter = 0;

// Añadir una nueva pestaña
function addNewTab() {
    tabCounter++;

    var tabId = "tab-" + tabCounter;
    var paneId = "pane-" + tabCounter;

    var tabsHeader = document.querySelector(".tabs-header");
    var tabsContent = document.querySelector(".tabs-content");

    // Crear botón de pestaña
    var tabButton = document.createElement("div");
    tabButton.id = tabId;
    tabButton.className = "tab-button";
    tabButton.innerHTML = "Petición " + tabCounter +
        "<span class=\"tab-close\" onclick=\"event.stopPropagation(); closeTab('" + tabId + "', '" + paneId + "')\">x</span>";
    tabButton.onclick = function () {
        switchTab(tabId, paneId);
    };

    // Crear panel de pestaña
    var tabPane = document.createElement("div");
    tabPane.id = paneId;
    tabPane.className = "tab-pane";

    // Contenido del panel
    var content = "<div class=\"main-wrapper\">" +
        "<div class=\"container\">" +
        "<div class=\"row-table\">" +
        "<div class=\"cell\" style=\"width:100px\">" +
        "<label>Método:</label>" +
        "<select id=\"method-" + tabId + "\">" +
        "<option value=\"POST\">POST</option>" +
        "<option value=\"GET\">GET</option>" +
        "<option value=\"PUT\">PUT</option>" +
        "<option value=\"DELETE\">DELETE</option>" +
        "</select>" +
        "</div>" +
        "<div class=\"cell\">" +
        "<label>Endpoint URL:</label>" +
        "<input type=\"text\" id=\"url-" + tabId + "\" value=\"http://localhost:8080/api/graphql\">" +
        "</div>" +
        "<div class=\"cell\" style=\"width:120px\">" +
        "<button class=\"btn btn-execute\" onclick=\"ExecuteRequest('" + tabId + "')\">Enviar</button>" +
        "</div>" +
        "</div>" +
        "<label>Headers de la Petición:</label>" +
        "<div id=\"headers-container-" + tabId + "\"></div>" +
        "<button class=\"btn btn-add\" onclick=\"addHeaderRow('" + tabId + "', '', '')\">+ Añadir Cabecera</button>" +
        "<label for=\"body-" + tabId + "\">Cuerpo de la Petición (Body/Query):</label>" +
        "<textarea id=\"body-" + tabId + "\" placeholder='{\"query\": \"{ ... }\"}'></textarea>" +
        "</div>" +
        "<div id=\"output-" + tabId + "\" class=\"output-panel\">" +
        "Los resultados de la API aparecerán en este panel..." +
        "</div>" +
        "</div>";

    tabPane.innerHTML = content;

    // Añadir al DOM
    tabsHeader.insertBefore(tabButton, tabsHeader.lastChild);
    tabsContent.appendChild(tabPane);

    // Activar la nueva pestaña
    switchTab(tabId, paneId);

    // Añadir header por defecto
    addHeaderRow(tabId, "x-user", "");
    applyPlaceholdersToTab(tabId);
}

// Cerrar una pestaña
function closeTab(tabId, paneId) {
    if (document.querySelectorAll(".tab-button").length <= 1) {
        alert("No puedes cerrar la última pestaña.");
        return;
    }

    var tabButton = document.getElementById(tabId);
    if (tabButton) {
        tabButton.remove();
    }

    var tabPane = document.getElementById(paneId);
    if (tabPane) {
        tabPane.remove();
    }

    // Activar la primera pestaña restante
    var firstTabButton = document.querySelector(".tab-button");
    if (firstTabButton) {
        var onclickCode = firstTabButton.getAttribute("onclick");
        var tabIdMatch = onclickCode.match(/switchTab\('([^']+)'/);
        var paneIdMatch = onclickCode.match(/switchTab\('[^']+', '([^']+)'/);
        if (tabIdMatch && paneIdMatch) {
            switchTab(tabIdMatch[1], paneIdMatch[1]);
        }
    }
}

// Cambiar entre pestañas
function switchTab(tabId, paneId) {
    var tabButtons = document.querySelectorAll(".tab-button");
    for (var i = 0; i < tabButtons.length; i++) {
        tabButtons[i].className = tabButtons[i].className.replace(" active", "");
    }

    var tabPanes = document.querySelectorAll(".tab-pane");
    for (var i = 0; i < tabPanes.length; i++) {
        tabPanes[i].className = tabPanes[i].className.replace(" active", "");
    }

    document.getElementById(tabId).className += " active";
    document.getElementById(paneId).className += " active";
}

// ======================
// FUNCIONES DE HEADERS
// ======================
function addHeaderRow(tabId, key, value) {
    var container = document.getElementById("headers-container-" + tabId);
    var div = document.createElement("div");
    div.className = "header-row";
    div.innerHTML = "<div class=\"cell\"><input type=\"text\" class=\"h-key\" placeholder=\"Key\" value=\"" + (key || "") + "\"></div>" +
        "<div class=\"cell\"><input type=\"text\" class=\"h-val\" placeholder=\"Value\" value=\"" + (value || "") + "\"></div>" +
        "<div class=\"cell\" style=\"width:35px\"><button class=\"btn btn-del\" onclick=\"removeHeader(this)\">x</button></div>";
    container.appendChild(div);

    // Aplicar placeholders a los nuevos inputs
    var newInputs = div.getElementsByTagName("input");
    for (var i = 0; i < newInputs.length; i++) {
        setupElements(newInputs[i]);
    }
}

function removeHeader(btn) {
    var row = btn.parentNode.parentNode;
    row.parentNode.removeChild(row);
}

// ======================
// FUNCIONES DE PETICIÓN
// ======================
function ExecuteRequest(tabId) {
    var outputDiv = document.getElementById("output-" + tabId);
    var getVal = function (id) {
        var el = document.getElementById(id);
        if (!el) return "";
        var p = el.getAttribute("placeholder");
        return (el.value === p) ? "" : el.value;
    };

    // Recoger Headers
    var keys = document.querySelectorAll("#headers-container-" + tabId + " .h-key");
    var vals = document.querySelectorAll("#headers-container-" + tabId + " .h-val");
    var headers = {};

    for (var i = 0; i < keys.length; i++) {
        var k = getVal(keys[i]);
        var v = getVal(vals[i]);
        if (k !== "") {
            headers[k] = v;
        }
    }

    var body = getVal("body-" + tabId);
    var url = getVal("url-" + tabId);
    var method = getVal("method-" + tabId);

    outputDiv.innerText = "Enviando...";
    outputDiv.style.color = "#80ff80";

    try {
        var http = new ActiveXObject("WinHttp.WinHttpRequest.5.1");
        http.Open(method, url, false);
        for (var key in headers) {
            http.SetRequestHeader(key, headers[key]);
        }
        if (method !== "GET" && body !== "") {
            http.Send(body);
        } else {
            http.Send();
        }

        if (http.Status >= 200 && http.Status < 300) {
            var responseText = http.ResponseText;
            outputDiv.innerText = "RESPUESTA:\n" + (responseText || "Éxito.");
            outputDiv.style.color = "#dcdcdc";
        } else {
            outputDiv.innerText = "ERROR:\nHTTP " + http.Status + " - " + http.StatusText;
            outputDiv.style.color = "#ff8080";
        }
    } catch (e) {
        outputDiv.innerText = "Error: " + e.message;
        outputDiv.style.color = "#ff8080";
    }
}

// ======================
// FUNCIONES DE ARCHIVO
// ======================
function SelectFile(mode) {
    try {
        var shell = new ActiveXObject("WScript.Shell");
        // Script de PowerShell para abrir el diálogo nativo
        var title = (mode === "save") ? "Guardar Configuración JSON" : "Abrir Configuración JSON";
        var psCommand = "powershell -NoProfile -Command \"Add-Type -AssemblyName System.Windows.Forms; " +
            "$f = New-Object System.Windows.Forms." + (mode === "save" ? "SaveFileDialog" : "OpenFileDialog") + "; " +
            "$f.Filter = 'JSON Files (*.json)|*.json|All Files (*.*)|*.*'; " +
            "$f.InitialDirectory = '" + shell.CurrentDirectory.replace(/\\/g, "\\\\") + "'; " +
            "$f.Title = '" + title + "'; " +
            "if($f.ShowDialog() -eq 'OK'){ $f.FileName }\"";

        var exec = shell.Exec(psCommand);
        var path = exec.StdOut.ReadAll().replace(/\r\n/g, "");
        return path;
    } catch (e) {
        return "";
    }
}

function SaveConfig() {
    var filePath = SelectFile("save");
    if (!filePath) return;

    try {
        var activePane = document.querySelector(".tab-pane.active");
        if (!activePane) return;
        var paneId = activePane.id;
        var tabId = paneId.replace("pane-", "tab-");

        var getVal = function (id) {
            var el = document.getElementById(id);
            if (!el) return "";
            var p = el.getAttribute("placeholder");
            return (el.value === p) ? "" : el.value;
        };

        var config = {
            url: getVal("url-" + tabId),
            method: getVal("method-" + tabId),
            body: getVal("body-" + tabId),
            headers: []
        };

        var keys = document.querySelectorAll("#headers-container-" + tabId + " .h-key");
        var vals = document.querySelectorAll("#headers-container-" + tabId + " .h-val");
        for (var i = 0; i < keys.length; i++) {
            config.headers.push({
                key: getVal(keys[i]),
                val: getVal(vals[i])
            });
        }

        var jsonStr = JSONStringify(config);

        var fso = new ActiveXObject("Scripting.FileSystemObject");
        var file = fso.CreateTextFile(filePath, true);
        file.Write(jsonStr);
        file.Close();

        alert("Archivo guardado con éxito.");
    } catch (e) {
        alert("Error al guardar el archivo: " + e.message);
    }
}

function LoadConfig() {
    var filePath = SelectFile("open");
    if (!filePath) return;

    try {
        var fso = new ActiveXObject("Scripting.FileSystemObject");
        var file = fso.OpenTextFile(filePath, 1);
        var jsonStr = file.ReadAll();
        file.Close();

        var data = eval("(" + jsonStr + ")");

        var activePane = document.querySelector(".tab-pane.active");
        if (!activePane) return;
        var paneId = activePane.id;
        var tabId = paneId.replace("pane-", "tab-");

        document.getElementById("url-" + tabId).value = data.url;
        document.getElementById("method-" + tabId).value = data.method;
        document.getElementById("body-" + tabId).value = data.body;

        document.getElementById("headers-container-" + tabId).innerHTML = "";
        for (var i = 0; i < data.headers.length; i++) {
            addHeaderRow(tabId, data.headers[i].key, data.headers[i].val);
        }

        applyPlaceholdersToTab(tabId);
    } catch (e) {
        alert("Error al cargar el archivo JSON: " + e.message);
    }
}

// ======================
// FUNCIONES AUXILIARES
// ======================
function JSONStringify(obj) {
    if (obj === null || obj === undefined) {
        return "null";
    }
    if (typeof obj === "string") {
        return escapeJSONString(obj);
    }
    if (typeof obj === "number" || typeof obj === "boolean") {
        return String(obj);
    }
    if (Array.isArray(obj)) {
        var arrStr = "[";
        for (var i = 0; i < obj.length; i++) {
            if (i > 0) arrStr += ",";
            arrStr += JSONStringify(obj[i]);
        }
        arrStr += "]";
        return arrStr;
    }
    if (typeof obj === "object") {
        var objStr = "{";
        var first = true;
        for (var key in obj) {
            if (!first) objStr += ",";
            objStr += escapeJSONString(key) + ":" + JSONStringify(obj[key]);
            first = false;
        }
        objStr += "}";
        return objStr;
    }
    return "null";
}

function escapeJSONString(str) {
    if (str === undefined || str === null) {
        return "null";
    }
    if (typeof str !== "string") {
        return String(str);
    }
    return '"' + str.replace(/\\/g, "\\\\")
        .replace(/"/g, '\\"')
        .replace(/\n/g, "\\n")
        .replace(/\r/g, "\\r")
        .replace(/\t/g, "\\t") + '"';
}

function applyPlaceholdersToTab(tabId) {
    var tabPane = document.getElementById("pane-" + tabId.replace("tab-", ""));
    if (!tabPane) return;
    var elementos = tabPane.querySelectorAll("[placeholder]");
    for (var i = 0; i < elementos.length; i++) {
        setupElements(elementos[i]);
    }
}

// ======================
// INICIALIZACIÓN
// ======================
window.onload = function () {
    // Añadir la primera pestaña
    addNewTab();
};