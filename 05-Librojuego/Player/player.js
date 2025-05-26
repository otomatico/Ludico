// Estado del player
const gameState = {
    currentStory: null,
    currentNode: null,
    history: []
};

// Elementos del DOM
const elements = {
    storyTitle: document.getElementById('story-title'),
    storyAuthor: document.getElementById('story-author'),
    storyCover: document.getElementById('story-cover'),
    nodeText: document.getElementById('node-text'),
    nodeImage: document.getElementById('node-image-container'),
    nodeOptions: document.getElementById('node-options'),
    loadButton: document.getElementById('load-button'),
    restartButton: document.getElementById('restart-button'),
    storyFile: document.getElementById('story-file')
};

// Inicialización
function init() {
    setupEventListeners();
}

// Configurar eventos
function setupEventListeners() {
    elements.loadButton.addEventListener('click', () => elements.storyFile.click());
    elements.storyFile.addEventListener('change', handleFileUpload);
    elements.restartButton.addEventListener('click', restartStory);
}

// Cargar archivo JSON
function handleFileUpload(event) {
    const file = event.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = (e) => {
        try {
            const storyData = JSON.parse(e.target.result);
            loadStory(storyData);
        } catch (error) {
            alert("Error al cargar el archivo: No es un JSON válido");
            console.error(error);
        }
    };
    reader.readAsText(file);
}

// Cargar la historia
function loadStory(storyData) {
    gameState.currentStory = storyData;
    gameState.history = [];
    
    // Actualizar metadatos
    elements.storyTitle.textContent = storyData.meta?.title || "Librojuego sin título";
    elements.storyAuthor.textContent = `Por: ${storyData.meta?.author || "Anónimo"}`;
    
    if (storyData.meta?.coverImage) {
        elements.storyCover.style.backgroundImage = `url(${storyData.meta.coverImage})`;
    } else {
        elements.storyCover.style.backgroundImage = 'none';
    }

    // Ir al nodo inicial
    gotoNode(storyData.start_node_id);
    elements.restartButton.disabled = false;
}

// Navegar a un nodo
function gotoNode(nodeId) {
    if (!gameState.currentStory) return;

    const node = gameState.currentStory.nodes.find(n => n.id === nodeId);
    if (!node) {
        alert("¡Fin de la historia!");
        return;
    }

    gameState.currentNode = node;
    gameState.history.push(nodeId);

    // Actualizar la UI
    elements.nodeText.textContent = node.text || "(Nodo sin texto)";
    
    // Mostrar imagen si existe
    elements.nodeImage.innerHTML = '';
    if (node.image) {
        const img = document.createElement('img');
        img.src = node.image;
        img.alt = "Imagen del nodo";
        elements.nodeImage.appendChild(img);
    }

    // Mostrar opciones
    elements.nodeOptions.innerHTML = '';
    if (node.options && node.options.length > 0) {
        node.options.forEach(option => {
            const button = document.createElement('button');
            button.className = 'option-button';
            button.textContent = option.text || "(Opción sin texto)";
            button.addEventListener('click', () => gotoNode(option.nextNodeId));
            elements.nodeOptions.appendChild(button);
        });
    } else {
        const endMessage = document.createElement('p');
        endMessage.textContent = "¡Fin de esta historia!";
        endMessage.style.fontStyle = 'italic';
        elements.nodeOptions.appendChild(endMessage);
    }
}

// Reiniciar la historia
function restartStory() {
    if (gameState.currentStory) {
        gotoNode(gameState.currentStory.start_node_id);
    }
}

// Iniciar la aplicación
init();