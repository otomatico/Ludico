# Panel Izquierdo - Gestión de Colecciones y Solicitudes
function Initialize-LeftPanel {
    # Crear el panel contenedor
    $global:leftPanel = New-Object System.Windows.Forms.Panel
    $leftPanel.Dock = "Left"
    $leftPanel.Width = 300
    $leftPanel.BackColor = [System.Drawing.Color]::FromArgb(240, 240, 240)
    $leftPanel.BorderStyle = [System.Windows.Forms.BorderStyle]::FixedSingle

    # Configurar controles
    Add-LeftPanelControls
    
    # Cargar datos iniciales (opcional)
    Load-InitialData
    
    # Agregar al formulario principal
    $global:mainForm.Controls.Add($leftPanel)
}

function Add-LeftPanelControls {
    # Barra de herramientas superior
    $toolPanel = New-Object System.Windows.Forms.Panel
    $toolPanel.Dock = "Top"
    $toolPanel.Height = 40
    $toolPanel.BackColor = [System.Drawing.Color]::FromArgb(230, 230, 230)

    # Botón para nueva colección
    $btnNewCollection = New-Object System.Windows.Forms.Button
    $btnNewCollection.Text = "+ Colección"
    #$btnNewCollection.Text = "➕ Colección"
    #$btnNewCollection.Font = New-Object System.Drawing.Font("Consolas", 12)
    $btnNewCollection.Location = New-Object System.Drawing.Point(5, 5)
    $btnNewCollection.Size = New-Object System.Drawing.Size(100, 30)
    $btnNewCollection.FlatStyle = [System.Windows.Forms.FlatStyle]::Flat
    $btnNewCollection.FlatAppearance.BorderSize = 0
    $btnNewCollection.BackColor = [System.Drawing.Color]::White
    $btnNewCollection.Add_Click({ Show-CollectionForm })

    # Botón para nueva solicitud
    $global:btnNewRequest = New-Object System.Windows.Forms.Button
    $btnNewRequest.Text = "+ Solicitud"
    #$btnNewRequest.Text = "➕ Solicitud"
    #$btnNewRequest.Font = New-Object System.Drawing.Font("Consolas", 12)
    $btnNewRequest.Location = New-Object System.Drawing.Point(110, 5)
    $btnNewRequest.Size = New-Object System.Drawing.Size(100, 30)
    $btnNewRequest.FlatStyle = [System.Windows.Forms.FlatStyle]::Flat
    $btnNewRequest.FlatAppearance.BorderSize = 0
    $btnNewRequest.BackColor = [System.Drawing.Color]::White
    $btnNewRequest.Enabled = $false
    $btnNewRequest.Add_Click({
        if ($treeCollections.SelectedNode -and $treeCollections.SelectedNode.Tag.type -eq "collection") {
            Show-RequestForm -CollectionName $treeCollections.SelectedNode.Tag.name
        }
    })

    # Botón para eliminar
    $global:btnDeleteItem = New-Object System.Windows.Forms.Button
    #$btnDeleteItem.Text = "🗑️ Eliminar"
    $btnDeleteItem.Text = "X Eliminar"    
    $btnDeleteItem.Location = New-Object System.Drawing.Point(215, 5)
    $btnDeleteItem.Size = New-Object System.Drawing.Size(75, 30)
    $btnDeleteItem.FlatStyle = [System.Windows.Forms.FlatStyle]::Flat
    $btnDeleteItem.FlatAppearance.BorderSize = 0
    $btnDeleteItem.BackColor = [System.Drawing.Color]::White
    $btnDeleteItem.Enabled = $false
    $btnDeleteItem.Add_Click({ Remove-SelectedItem })

    # TreeView para mostrar colecciones y solicitudes
    $global:treeCollections = New-Object System.Windows.Forms.TreeView
    $treeCollections.Dock = "Fill"
    $treeCollections.BorderStyle = [System.Windows.Forms.BorderStyle]::None
    $treeCollections.FullRowSelect = $true
    $treeCollections.HideSelection = $false
    $treeCollections.ShowLines = $true
    $treeCollections.ShowPlusMinus = $true
    $treeCollections.ShowRootLines = $true
    $treeCollections.ImageList = New-Object System.Windows.Forms.ImageList
    $treeCollections.ImageList.Images.Add("Collection", [System.Drawing.SystemIcons]::Folder)
    $treeCollections.ImageList.Images.Add("Request", [System.Drawing.SystemIcons]::Document)

    # Configurar eventos
    $treeCollections.Add_AfterSelect({
        $isCollectionSelected = $treeCollections.SelectedNode -and $treeCollections.SelectedNode.Tag.type -eq "collection"
        $isRequestSelected = $treeCollections.SelectedNode -and $treeCollections.SelectedNode.Tag.type -eq "request"
        
        $btnNewRequest.Enabled = $isCollectionSelected
        $btnDeleteItem.Enabled = $isCollectionSelected -or $isRequestSelected
        
        if ($isRequestSelected) {
            Show-RequestDetails -Request $treeCollections.SelectedNode.Tag.request
        }
    })

    $treeCollections.Add_NodeMouseDoubleClick({
        param($sender, $e)
        if ($e.Node.Tag.type -eq "collection") {
            Show-CollectionForm -CollectionName $e.Node.Tag.name -IsEdit $true
        } elseif ($e.Node.Tag.type -eq "request") {
            Show-RequestForm -CollectionName $e.Node.Tag.collection -RequestData $e.Node.Tag.request -IsEdit $true
        }
    })

    # Menú contextual
    $contextMenu = New-Object System.Windows.Forms.ContextMenuStrip
    $editMenu = $contextMenu.Items.Add("Editar")
    $editMenu.Add_Click({
        if ($treeCollections.SelectedNode.Tag.type -eq "collection") {
            Show-CollectionForm -CollectionName $treeCollections.SelectedNode.Tag.name -IsEdit $true
        } else {
            Show-RequestForm -CollectionName $treeCollections.SelectedNode.Tag.collection `
                -RequestData $treeCollections.SelectedNode.Tag.request -IsEdit $true
        }
    })
    
    $deleteMenu = $contextMenu.Items.Add("Eliminar")
    $deleteMenu.Add_Click({ Remove-SelectedItem })
    
    $treeCollections.ContextMenuStrip = $contextMenu

    # Agregar controles al panel
    $toolPanel.Controls.Add($btnNewCollection)
    $toolPanel.Controls.Add($btnNewRequest)
    $toolPanel.Controls.Add($btnDeleteItem)
    
    $leftPanel.Controls.Add($treeCollections)
    $leftPanel.Controls.Add($toolPanel)
}

function Show-CollectionForm {
    param(
        [string]$CollectionName = "",
        [bool]$IsEdit = $false
    )

    $form = New-Object System.Windows.Forms.Form
    $form.Text = if ($IsEdit) { "Editar Colección" } else { "Nueva Colección" }
    $form.Size = New-Object System.Drawing.Size(350, 200)
    $form.StartPosition = "CenterParent"
    $form.FormBorderStyle = "FixedDialog"
    $form.MaximizeBox = $false
    $form.MinimizeBox = $false

    # Controles del formulario
    $lblName = New-Object System.Windows.Forms.Label
    $lblName.Text = "Nombre de la colección:"
    $lblName.Location = New-Object System.Drawing.Point(10, 20)
    $lblName.Size = New-Object System.Drawing.Size(200, 20)

    $txtName = New-Object System.Windows.Forms.TextBox
    $txtName.Location = New-Object System.Drawing.Point(10, 50)
    $txtName.Size = New-Object System.Drawing.Size(310, 20)
    $txtName.Text = $CollectionName

    $btnSave = New-Object System.Windows.Forms.Button
    $btnSave.Text = "Guardar"
    $btnSave.Location = New-Object System.Drawing.Point(100, 100)
    $btnSave.Size = New-Object System.Drawing.Size(100, 30)
    $btnSave.DialogResult = "OK"
    $btnSave.Add_Click({
        if ([string]::IsNullOrWhiteSpace($txtName.Text)) {
            [System.Windows.Forms.MessageBox]::Show("El nombre de la colección no puede estar vacío.", "Error", "OK", "Error")
            $form.DialogResult = "None"
        }
    })

    $form.AcceptButton = $btnSave
    $form.Controls.Add($lblName)
    $form.Controls.Add($txtName)
    $form.Controls.Add($btnSave)

    if ($form.ShowDialog($global:mainForm) -eq "OK") {
        if ($IsEdit) {
            # Actualizar colección existente
            $node = $treeCollections.Nodes | Where-Object { $_.Tag.name -eq $CollectionName }
            if ($node) {
                # Actualizar TreeView
                $node.Text = $txtName.Text
                $node.Tag.name = $txtName.Text
                
                # Actualizar estructura de datos
                if ($global:collections.ContainsKey($CollectionName)) {
                    $collectionData = $global:collections[$CollectionName]
                    $global:collections.Remove($CollectionName)
                    $global:collections[$txtName.Text] = $collectionData
                }
            }
        } else {
            # Crear nueva colección
            $collectionNode = New-Object System.Windows.Forms.TreeNode
            $collectionNode.Text = $txtName.Text
            $collectionNode.Name = "collection_$($txtName.Text)"
            $collectionNode.Tag = @{ type = "collection"; name = $txtName.Text }
            $collectionNode.ImageKey = "Collection"
            $collectionNode.SelectedImageKey = "Collection"
            
            $treeCollections.Nodes.Add($collectionNode) | Out-Null
            
            # Agregar a estructura de datos
            $global:collections[$txtName.Text] = @{
                name = $txtName.Text
                request = @()
            }
        }
    }
}

function Remove-SelectedItem {
    if ($treeCollections.SelectedNode -and $treeCollections.SelectedNode.Tag) {
        $confirm = [System.Windows.Forms.MessageBox]::Show(
            "¿Estás seguro de que deseas eliminar este elemento?", 
            "Confirmar eliminación", 
            [System.Windows.Forms.MessageBoxButtons]::YesNo, 
            [System.Windows.Forms.MessageBoxIcon]::Question
        )
        
        if ($confirm -eq "Yes") {
            $tag = $treeCollections.SelectedNode.Tag
            
            if ($tag.type -eq "collection") {
                # Eliminar colección
                $global:collections.Remove($tag.name)
                $treeCollections.Nodes.Remove($treeCollections.SelectedNode)
            } elseif ($tag.type -eq "request") {
                # Eliminar solicitud
                $collectionName = $tag.collection
                if ($global:collections.ContainsKey($collectionName)) {
                    $requests = $global:collections[$collectionName].request
                    $global:collections[$collectionName].request = $requests | Where-Object { $_.name -ne $tag.request.name }
                    $treeCollections.SelectedNode.Parent.Nodes.Remove($treeCollections.SelectedNode)
                }
            }
        }
    }
}

function Load-InitialData {
    # Datos de ejemplo (opcional)
    $sampleData = @{
        "Ejemplo API" = @{
            name = "Ejemplo API"
            request = @(
                @{
                    name = "Obtener usuarios"
                    method = "GET"
                    location = "https://api.example.com/users"
                    header = "{`n    `"Authorization`": `"Bearer token123`"`n}"
                    body = ""
                },
                @{
                    name = "Crear usuario"
                    method = "POST"
                    location = "https://api.example.com/users"
                    header = "{`n    `"Content-Type`": `"application/json`",`n    `"Authorization`": `"Bearer token123`"`n}"
                    body = "{`n    `"name`": `"John Doe`",`n    `"email`": `"john@example.com`"`n}"
                }
            )
        }
    }

    $global:collections = $sampleData.Clone()

    # Cargar datos en el TreeView
    foreach ($collectionName in $global:collections.Keys) {
        $collectionNode = New-Object System.Windows.Forms.TreeNode
        $collectionNode.Text = $collectionName
        $collectionNode.Name = "collection_$collectionName"
        $collectionNode.Tag = @{ type = "collection"; name = $collectionName }
        $collectionNode.ImageKey = "Collection"
        $collectionNode.SelectedImageKey = "Collection"

        foreach ($request in $global:collections[$collectionName].request) {
            $requestNode = New-Object System.Windows.Forms.TreeNode
            $requestNode.Text = "$($request.method) - $($request.name)"
            $requestNode.Name = "request_$(New-Guid)"
            $requestNode.Tag = @{
                type = "request"
                collection = $collectionName
                request = $request
            }
            $requestNode.ImageKey = "Request"
            $requestNode.SelectedImageKey = "Request"
            
            $collectionNode.Nodes.Add($requestNode) | Out-Null
        }

        $treeCollections.Nodes.Add($collectionNode) | Out-Null
    }
}

# Inicializar estructura de datos global
$global:collections = @{}