document.addEventListener('DOMContentLoaded', () => {
    // Obtener los elementos del DOM
    const wallLengthInput = document.getElementById('wall-length');
    const wallHeightInput = document.getElementById('wall-height');
    const brickLengthInput = document.getElementById('brick-length');
    const brickWidthInput = document.getElementById('brick-width');
    const brickHeightInput = document.getElementById('brick-height');
    const mortarThicknessInput = document.getElementById('mortar-thickness');
    const calculateBtn = document.getElementById('calculate-btn');
    const resultsDiv = document.getElementById('results');
    const wallAreaSpan = document.getElementById('wall-area');
    const brickAreaWithJointSpan = document.getElementById('brick-area-with-joint');
    const requiredBricksSpan = document.getElementById('required-bricks');
    const errorMessageDiv = document.getElementById('error-message');

    // Función para validar los campos de entrada
    const validateInputs = () => {
        const inputs = [
            wallLengthInput,
            wallHeightInput,
            brickLengthInput,
            brickWidthInput,
            brickHeightInput,
            mortarThicknessInput
        ];
        
        // Comprobar que todos los campos tienen un valor numérico positivo
        const allValid = inputs.every(input => {
            const value = parseFloat(input.value);
            return !isNaN(value) && value > 0;
        });

        return allValid;
    };

    // Función principal para calcular
    const calculateBricks = () => {
        // Ocultar mensajes de error y resultados previos
        errorMessageDiv.style.display = 'none';
        resultsDiv.classList.remove('show');

        // Validar los campos de entrada
        if (!validateInputs()) {
            errorMessageDiv.style.display = 'block';
            return;
        }

        // Obtener los valores de los campos (ya validados)
        const wallLength = parseFloat(wallLengthInput.value);
        const wallHeight = parseFloat(wallHeightInput.value);
        const brickLength = parseFloat(brickLengthInput.value);
        const brickHeight = parseFloat(brickHeightInput.value);
        const mortarThickness = parseFloat(mortarThicknessInput.value);

        // --- Realizar los cálculos ---

        // 1. Calcular el área total de la pared
        const wallArea = wallLength * wallHeight;

        // 2. Calcular el área de un ladrillo incluyendo el mortero
        // El mortero se añade a la longitud y altura del ladrillo
        const brickLengthWithJoint = brickLength + mortarThickness;
        const brickHeightWithJoint = brickHeight + mortarThickness;
        const brickAreaWithJoint = brickLengthWithJoint * brickHeightWithJoint;
        
        // 3. Calcular la cantidad de ladrillos necesarios
        // Redondeamos hacia arriba para asegurarnos de tener suficientes
        const requiredBricks = Math.ceil(wallArea / brickAreaWithJoint);

        // 4. Mostrar los resultados en la página
        wallAreaSpan.textContent = wallArea.toFixed(2);
        brickAreaWithJointSpan.textContent = brickAreaWithJoint.toFixed(4);
        requiredBricksSpan.textContent = requiredBricks.toLocaleString('es-ES'); // Formato de número con separador de miles

        // Mostrar el div de resultados con una transición
        resultsDiv.style.display = 'block';
        setTimeout(() => {
            resultsDiv.classList.add('show');
        }, 10);
    };

    // Asignar el evento click al botón
    calculateBtn.addEventListener('click', calculateBricks);

    // Opcional: Calcular al presionar Enter en cualquier campo de entrada
    const inputFields = document.querySelectorAll('input[type="number"]');
    inputFields.forEach(input => {
        input.addEventListener('keypress', (event) => {
            if (event.key === 'Enter') {
                calculateBricks();
            }
        });
    });
});