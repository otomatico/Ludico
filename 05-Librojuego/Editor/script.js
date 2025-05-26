// Estado de la aplicación
const state = {
    meta: {
        title: "Mi Aventura Interactiva",
        author: "Anónimo",
        created_at: new Date().toISOString().split('T')[0],
        coverImage: null
    },
    nodes: [],
    selectedNode: null,
    nextId: 1,
    dragging: null,
    offset: { x: 0, y: 0 }
};

// Elementos del DOM
const elements = {
    nodeText: document.getElementById('node-text'),
    optionsContainer: document.getElementById('options-container'),
    nodesContainer: document.getElementById('nodes-container'),
    canvas: document.getElementById('graph-canvas'),
    ctx: document.getElementById('graph-canvas').getContext('2d'),
    nodeImageInput: document.getElementById('node-image'),
    nodeImagePreview: document.getElementById('node-image-preview')
};

// Inicialización
function init() {
    setupEventListeners();
    setupMetadataPanel();
    setupJsonImport(); // ← Nueva línea
    resizeCanvas();
    createNewNode();
	 document.getElementById('delete-node').disabled = true;
}

// Configuración de eventos
function setupEventListeners() {
    document.getElementById('add-option').addEventListener('click', addOption);
    document.getElementById('save-node').addEventListener('click', saveNode);
    document.getElementById('new-node').addEventListener('click', createNewNode);
    document.getElementById('export-json').addEventListener('click', exportToJson);
    window.addEventListener('resize', resizeCanvas);
    
    // Eventos para arrastrar nodos
    document.addEventListener('mousedown', startDrag);
    document.addEventListener('mousemove', drag);
    document.addEventListener('mouseup', endDrag);

    // Evento para imagen de nodo
    elements.nodeImageInput.addEventListener('change', handleNodeImageUpload);
	
	//Borrar un nodo
	document.getElementById('delete-node').addEventListener('click', deleteSelectedNode);
	document.addEventListener('keydown', (e) => {
		if (e.key === 'Delete' && state.selectedNode) {
			deleteSelectedNode();
		}
	})
}

function setupMetadataPanel() {
    document.getElementById('book-title').value = state.meta.title;
    document.getElementById('book-author').value = state.meta.author;
    
    document.getElementById('book-title').addEventListener('change', (e) => {
        state.meta.title = e.target.value;
    });
    
    document.getElementById('book-author').addEventListener('change', (e) => {
        state.meta.author = e.target.value;
    });

    // Configurar el input de portada
    document.getElementById('book-cover').addEventListener('change', async (e) => {
        try {
            const base64 = await convertToBase64(e.target.files[0]);
            state.meta.coverImage = base64;
            document.getElementById('cover-preview').style.backgroundImage = `url(${base64})`;
        } catch (error) {
            alert(error);
        }
    });
}

// ========== FUNCIONES PARA NODOS ==========
function createNewNode() {
    const newNode = {
        id: `node-${state.nextId++}`,
        text: '',
        image: null,
        options: [],
        x: Math.random() * (elements.canvas.width - 200) + 100,
        y: Math.random() * (elements.canvas.height - 100) + 50
    };
    state.nodes.push(newNode);
    selectNode(newNode);
    renderNode(newNode);
    updateForm();
}

function selectNode(node) {
    // Deseleccionar el nodo actual
    if (state.selectedNode) {
        const prevNode = document.getElementById(state.selectedNode.id);
        if (prevNode) prevNode.classList.remove('selected');
    }
    
    state.selectedNode = node;
    const nodeEl = document.getElementById(node.id);
    if (nodeEl) nodeEl.classList.add('selected');
    updateForm();
	 document.getElementById('delete-node').disabled = state.nodes.length <= 1;
}

function renderNode(node) {
    let nodeEl = document.getElementById(node.id);
    if (!nodeEl) {
        nodeEl = document.createElement('div');
        nodeEl.className = 'node';
        nodeEl.id = node.id;
        elements.nodesContainer.appendChild(nodeEl);
        
        // Eventos para el nodo
        nodeEl.addEventListener('click', (e) => {
            e.stopPropagation();
            selectNode(node);
        });
    }
    
    nodeEl.innerHTML = `
        ${node.image ? `<img src="${node.image}" class="node-image">` : ''}
        <div class="node-content">${node.text || 'Nuevo nodo'}</div>
    `;
    nodeEl.style.left = `${node.x}px`;
    nodeEl.style.top = `${node.y}px`;
    
    drawConnections();
}

// Nueva función para eliminar nodos:
function deleteSelectedNode() {
    if (!state.selectedNode) {
        alert("No hay nodo seleccionado");
        return;
    }

    if (state.nodes.length <= 1) {
        alert("No puedes eliminar el último nodo");
        return;
    }

    // Confirmación antes de eliminar
    if (!confirm(`¿Eliminar el nodo "${state.selectedNode.id}"?`)) {
        return;
    }

    // 1. Eliminar referencias en las opciones de otros nodos
    state.nodes.forEach(node => {
        node.options = node.options.filter(opt => opt.nextNodeId !== state.selectedNode.id);
    });

    // 2. Eliminar el nodo del estado
    state.nodes = state.nodes.filter(n => n.id !== state.selectedNode.id);

    // 3. Eliminar visualmente
    const nodeEl = document.getElementById(state.selectedNode.id);
    if (nodeEl) nodeEl.remove();

    // 4. Seleccionar otro nodo (el primero disponible)
    selectNode(state.nodes[0]);

    // 5. Redibujar conexiones
    drawConnections();
}

// ========== FUNCIONES PARA OPCIONES ==========
function addOption() {
    if (!state.selectedNode) return;
    state.selectedNode.options.push({ text: '', nextNodeId: '' });
    updateForm();
}

function updateForm() {
    if (!state.selectedNode) return;
    
    // Actualizar texto e imagen
    elements.nodeText.value = state.selectedNode.text || '';
    elements.nodeImagePreview.style.backgroundImage = state.selectedNode.image 
        ? `url(${state.selectedNode.image})` 
        : '';
    
    // Actualizar opciones
    elements.optionsContainer.innerHTML = '<h3>Opciones:</h3>';
    state.selectedNode.options.forEach((option, index) => {
        const optionDiv = document.createElement('div');
        optionDiv.className = 'option';
        optionDiv.innerHTML = `
            <input type="text" class="option-text" value="${option.text}" 
                   placeholder="Texto de la opción" data-index="${index}">
            <select class="option-target" data-index="${index}">
                <option value="">-- Selecciona nodo --</option>
                ${state.nodes.map(n => `
                    <option value="${n.id}" ${option.nextNodeId === n.id ? 'selected' : ''}>
                        ${n.id}${n.text ? ` (${n.text.substring(0, 20)}...)` : ''}
                    </option>
                `).join('')}
            </select>
            <button class="delete-option" data-index="${index}">×</button>
        `;
        
        // Eventos para la opción
        optionDiv.querySelector('.delete-option').addEventListener('click', (e) => {
            e.preventDefault();
            state.selectedNode.options.splice(index, 1);
            updateForm();
        });
        
        elements.optionsContainer.appendChild(optionDiv);
    });
}

// ========== FUNCIONES PARA IMÁGENES ==========
async function handleNodeImageUpload(e) {
    if (!state.selectedNode) return;
    try {
        const base64 = await convertToBase64(e.target.files[0]);
        state.selectedNode.image = base64;
        updateForm();
        renderNode(state.selectedNode);
    } catch (error) {
        alert(error);
    }
}

async function convertToBase64(file) {
    return new Promise((resolve, reject) => {
        if (!file) reject("No se seleccionó archivo");
        
        if (file.size > 1 * 1024 * 1024) {
            reject("La imagen es demasiado grande (máx. 1MB)");
            return;
        }

        const reader = new FileReader();
        reader.onload = () => resolve(reader.result);
        reader.onerror = error => reject(error);
        reader.readAsDataURL(file);
    });
}

// ========== FUNCIONES PARA GUARDAR ==========
function saveNode() {
    if (!state.selectedNode) return;
    
    // Guardar texto
    state.selectedNode.text = elements.nodeText.value;
    
    // Guardar opciones
    const textInputs = document.querySelectorAll('.option-text');
    const targetSelects = document.querySelectorAll('.option-target');
    
    state.selectedNode.options = Array.from(textInputs).map((input, i) => ({
        text: input.value,
        nextNodeId: targetSelects[i].value
    }));
    
    renderNode(state.selectedNode);
}

// ========== FUNCIONES PARA ARRASTRAR NODOS ==========
function startDrag(e) {
    const nodeEl = e.target.closest('.node');
    if (!nodeEl) return;
    
    state.dragging = state.nodes.find(n => n.id === nodeEl.id);
    state.offset.x = e.clientX - state.dragging.x;
    state.offset.y = e.clientY - state.dragging.y;
}

function drag(e) {
    if (!state.dragging) return;
    
    state.dragging.x = e.clientX - state.offset.x;
    state.dragging.y = e.clientY - state.offset.y;
    
    const nodeEl = document.getElementById(state.dragging.id);
    nodeEl.style.left = `${state.dragging.x}px`;
    nodeEl.style.top = `${state.dragging.y}px`;
    
    drawConnections();
}

function endDrag() {
    state.dragging = null;
}

// ========== FUNCIONES PARA DIBUJAR CONEXIONES ==========
function drawConnections() {
    elements.ctx.clearRect(0, 0, elements.canvas.width, elements.canvas.height);
    
    state.nodes.forEach(node => {
        node.options.forEach(opt => {
            if (!opt.nextNodeId) return;
            const targetNode = state.nodes.find(n => n.id === opt.nextNodeId);
            if (!targetNode) return;
            
            // Dibujar línea
            elements.ctx.beginPath();
            elements.ctx.moveTo(node.x + 60, node.y + 40);
            elements.ctx.lineTo(targetNode.x + 60, targetNode.y);
            elements.ctx.strokeStyle = '#4CAF50';
            elements.ctx.lineWidth = 2;
            elements.ctx.stroke();
            
            // Dibujar flecha
            drawArrow(node.x + 60, node.y + 40, targetNode.x + 60, targetNode.y);
        });
    });
}

function drawArrow(fromX, fromY, toX, toY) {
    const headLength = 10;
    const angle = Math.atan2(toY - fromY, toX - fromX);
    
    elements.ctx.beginPath();
    elements.ctx.moveTo(toX, toY);
    elements.ctx.lineTo(toX - headLength * Math.cos(angle - Math.PI / 6), toY - headLength * Math.sin(angle - Math.PI / 6));
    elements.ctx.moveTo(toX, toY);
    elements.ctx.lineTo(toX - headLength * Math.cos(angle + Math.PI / 6), toY - headLength * Math.sin(angle + Math.PI / 6));
    elements.ctx.stroke();
}

// ========== EXPORTACIÓN ==========
function exportToJson() {
    const bookData = {
        meta: {
            title: state.meta.title,
            author: state.meta.author,
            created_at: state.meta.created_at,
            coverImage: state.meta.coverImage || undefined
        },
        start_node_id: state.nodes[0]?.id || "",
        nodes: state.nodes.map(node => ({
            id: node.id,
            text: node.text,
            image: node.image || undefined,
            options: node.options.filter(opt => opt.text && opt.nextNodeId)
        }))
    };
    
    const jsonStr = JSON.stringify(bookData, (key, value) => 
        value === undefined ? null : value, 2);
    
    const blob = new Blob([jsonStr], { type: "application/json" });
    const url = URL.createObjectURL(blob);
    
    const a = document.createElement("a");
    a.href = url;
    a.download = `${state.meta.title || 'librojuego'}.json`;
    a.click();
}
// ========== FUNCIONES PARA CARGAR JSON ==========
function setupJsonImport() {
    document.getElementById('import-json').addEventListener('click', () => {
        document.getElementById('json-loader').click();
    });

    document.getElementById('json-loader').addEventListener('change', async (e) => {
        const file = e.target.files[0];
        if (!file) return;

        try {
            const jsonData = await loadJsonFile(file);
            importBookData(jsonData);
        } catch (error) {
            alert(`Error al cargar el archivo: ${error.message}`);
            console.error(error);
        }
    });
}

function loadJsonFile(file) {
    return new Promise((resolve, reject) => {
        const reader = new FileReader();
        reader.onload = (e) => {
            try {
                resolve(JSON.parse(e.target.result));
            } catch (e) {
                reject(new Error("El archivo no es un JSON válido"));
            }
        };
        reader.onerror = () => reject(new Error("Error leyendo el archivo"));
        reader.readAsText(file);
    });
}

function importBookData(bookData) {
    // Limpiar estado actual
    state.nodes = [];
    state.nextId = 1;
    elements.nodesContainer.innerHTML = '';
    
    // Cargar metadatos
    state.meta = {
        title: bookData.meta?.title || "Sin título",
        author: bookData.meta?.author || "Anónimo",
        created_at: bookData.meta?.created_at || new Date().toISOString().split('T')[0],
        coverImage: bookData.meta?.coverImage || null
    };
    
    // Cargar nodos
    if (bookData.nodes && bookData.nodes.length > 0) {
	
        bookData.nodes.forEach(node => {
            const newNode = {
                id: node.id,
                text: node.text || '',
                image: node.image || null,
                options: node.options || [],
                x: Math.random() * (elements.canvas.width - 200) + 100,
                y: Math.random() * (elements.canvas.height - 100) + 50
            };
            state.nodes.push(newNode);
            renderNode(newNode);
            
            // Actualizar el nextId
            const nodeNum = parseInt(node.id.replace('node-', ''));
            if (!isNaN(nodeNum) && nodeNum >= state.nextId) {
                state.nextId = nodeNum + 1;
            }
        });
        
        // Ajustar conexiones
        drawConnections();
    }
    
    // Actualizar UI
    setupMetadataPanel();
    if (state.nodes.length > 0) {
        selectNode(state.nodes[0]);
    }
    
    // Actualizar vista de portada
    if (state.meta.coverImage) {
        document.getElementById('cover-preview').style.backgroundImage = 
            `url(${state.meta.coverImage})`;
    }
}
// ========== UTILIDADES ==========
function resizeCanvas() {
    elements.canvas.width = window.innerWidth - 340;
    elements.canvas.height = window.innerHeight;
    drawConnections();
}

// Iniciar la aplicación
init();