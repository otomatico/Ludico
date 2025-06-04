function Update-RequestList {
    param(
        [string]$CollectionName
    )
    # Actualizar lista de solicitudes en el panel central
}

function Show-RequestDetails {
    param(
        [hashtable]$Request
    )
    # Mostrar detalles de la solicitud
}

function Load-SampleData {
    # Cargar datos de ejemplo (opcional)
}

function Save-CollectionsToFile {
    param(
        [string]$FilePath
    )
    # Guardar colecciones a archivo JSON
}

function Load-CollectionsFromFile {
    param(
        [string]$FilePath
    )
    # Cargar colecciones desde archivo JSON
}