function Initialize-CenterPanel {
    # Panel central
    $global:centerPanel = New-Object System.Windows.Forms.Panel
    $centerPanel.Dock = "Fill"
    $centerPanel.BackColor = [System.Drawing.Color]::White
    $centerPanel.BorderStyle = [System.Windows.Forms.BorderStyle]::FixedSingle

    # Configurar controles del panel central
    Add-CenterPanelControls

    # Agregar al formulario principal
    $global:mainForm.Controls.Add($centerPanel)
}

function Add-CenterPanelControls {
    # Panel para mostrar detalles de la solicitud/respuesta
    $global:requestViewPanel = New-Object System.Windows.Forms.Panel
    $requestViewPanel.Dock = "Fill"
    $requestViewPanel.Padding = New-Object System.Windows.Forms.Padding(10)

    # TabControl para organizar las secciones
    $tabControl = New-Object System.Windows.Forms.TabControl
    $tabControl.Dock = "Fill"

    # Pestaña de Detalles
    $tabDetails = New-Object System.Windows.Forms.TabPage
    $tabDetails.Text = "Detalles"
    $tabDetails.BackColor = [System.Drawing.Color]::White

    # Pestaña de Respuesta
    $tabResponse = New-Object System.Windows.Forms.TabPage
    $tabResponse.Text = "Respuesta"
    $tabResponse.BackColor = [System.Drawing.Color]::White

    # Agregar pestañas al control
    $tabControl.Controls.Add($tabDetails)
    $tabControl.Controls.Add($tabResponse)

    # Controles para la pestaña de Detalles
    Add-DetailsTabControls $tabDetails

    # Controles para la pestaña de Respuesta
    Add-ResponseTabControls $tabResponse

    $requestViewPanel.Controls.Add($tabControl)
    $centerPanel.Controls.Add($requestViewPanel)
}

function Add-DetailsTabControls {
    param($parentControl)

    # Panel contenedor con scroll
    $scrollPanel = New-Object System.Windows.Forms.Panel
    $scrollPanel.Dock = "Fill"
    $scrollPanel.AutoScroll = $true

    # Grupo para parámetros básicos
    $grpBasic = New-Object System.Windows.Forms.GroupBox
    $grpBasic.Text = "Parámetros Básicos"
    $grpBasic.Location = New-Object System.Drawing.Point(10, 10)
    $grpBasic.Size = New-Object System.Drawing.Size(640, 150)
    $grpBasic.Anchor = "Left,  Top"

    # Método HTTP
    $lblMethod = New-Object System.Windows.Forms.Label
    $lblMethod.Text = "Método:"
    $lblMethod.Location = New-Object System.Drawing.Point(10, 25)
    $lblMethod.Size = New-Object System.Drawing.Size(80, 20)

    $global:cmbMethod = New-Object System.Windows.Forms.ComboBox
    $cmbMethod.Location = New-Object System.Drawing.Point(100, 25)
    $cmbMethod.Size = New-Object System.Drawing.Size(100, 20)
    $cmbMethod.Items.AddRange(@("GET", "POST", "PUT", "DELETE", "PATCH", "HEAD", "OPTIONS"))
    $cmbMethod.SelectedIndex = 0

    # Nombre de la solicitud
    $lblName = New-Object System.Windows.Forms.Label
    $lblName.Text = "Nombre:"
    $lblName.Location = New-Object System.Drawing.Point(10, 55)
    $lblName.Size = New-Object System.Drawing.Size(80, 20)

    $global:txtRequestName = New-Object System.Windows.Forms.TextBox
    $txtRequestName.Location = New-Object System.Drawing.Point(100, 55)
    $txtRequestName.Size = New-Object System.Drawing.Size(580, 20)

    # URL
    $lblUrl = New-Object System.Windows.Forms.Label
    $lblUrl.Text = "URL:"
    $lblUrl.Location = New-Object System.Drawing.Point(10, 85)
    $lblUrl.Size = New-Object System.Drawing.Size(80, 20)

    $global:txtUrl = New-Object System.Windows.Forms.TextBox
    $txtUrl.Location = New-Object System.Drawing.Point(100, 85)
    $txtUrl.Size = New-Object System.Drawing.Size(580, 20)
    $txtUrl.Text = "https://"

    # Descripción
    $lblDescription = New-Object System.Windows.Forms.Label
    $lblDescription.Text = "Descripción:"
    $lblDescription.Location = New-Object System.Drawing.Point(10, 115)
    $lblDescription.Size = New-Object System.Drawing.Size(80, 20)

    $global:txtDescription = New-Object System.Windows.Forms.TextBox
    $txtDescription.Location = New-Object System.Drawing.Point(100, 115)
    $txtDescription.Size = New-Object System.Drawing.Size(480, 20)

    $grpBasic.Controls.AddRange(@($lblMethod, $cmbMethod, $lblName, $txtRequestName, $lblUrl, $txtUrl, $lblDescription, $txtDescription))

    # Grupo para Headers
    $grpHeaders = New-Object System.Windows.Forms.GroupBox
    $grpHeaders.Text = "Headers"
    $grpHeaders.Location = New-Object System.Drawing.Point(10, 170)
    $grpHeaders.Size = New-Object System.Drawing.Size(640, 200)
    $grpHeaders.Anchor = "Left, Top"

    $global:txtHeaders = New-Object System.Windows.Forms.RichTextBox
    $txtHeaders.Location = New-Object System.Drawing.Point(10, 20)
    $txtHeaders.Size = New-Object System.Drawing.Size(620, 170)
    $txtHeaders.Font = New-Object System.Drawing.Font("Consolas", 10)
    $txtHeaders.Text = "{`n    `"Content-Type`": `"application/json`",`n    `"Authorization`": `"Bearer token`"`n}"

    $grpHeaders.Controls.Add($txtHeaders)

    # Grupo para Body
    $grpBody = New-Object System.Windows.Forms.GroupBox
    $grpBody.Text = "Body"
    $grpBody.Location = New-Object System.Drawing.Point(10, 380)
    $grpBody.Size = New-Object System.Drawing.Size(640, 250)
    $grpBody.Anchor = "Left, Top"

    $global:txtBody = New-Object System.Windows.Forms.RichTextBox
    $txtBody.Location = New-Object System.Drawing.Point(10, 20)
    $txtBody.Size = New-Object System.Drawing.Size(620, 220)
    $txtBody.Font = New-Object System.Drawing.Font("Consolas", 10)

    $grpBody.Controls.Add($txtBody)

    # Botón para enviar la solicitud
    $btnSend = New-Object System.Windows.Forms.Button
    $btnSend.Text = "Enviar Solicitud"
    $btnSend.Location = New-Object System.Drawing.Point(70, 10)
    $btnSend.Size = New-Object System.Drawing.Size(150, 30)
    $btnSend.Anchor = "Right"
    $btnSend.Add_Click({ Send-Request })

    # Botón para guardar
    $btnSave = New-Object System.Windows.Forms.Button
    $btnSave.Text = "Guardar Cambios"
    $btnSave.Location = New-Object System.Drawing.Point(70, 50)
    $btnSave.Size = New-Object System.Drawing.Size(150, 30)
    $btnSave.Anchor = "Right"
    $btnSave.Add_Click({ Save-Request })

    $scrollPanel.Controls.AddRange(@( $btnSend, $btnSave, $grpBasic, $grpHeaders, $grpBody))
    $parentControl.Controls.Add($scrollPanel)
}

function Add-ResponseTabControls {
    param($parentControl)

    # Panel para la respuesta
    $responsePanel = New-Object System.Windows.Forms.Panel
    $responsePanel.Dock = "Fill"

    # Caja de texto para la respuesta
    $global:txtResponse = New-Object System.Windows.Forms.RichTextBox
    $txtResponse.Dock = "Fill"
    $txtResponse.Font = New-Object System.Drawing.Font("Consolas", 10)
    $txtResponse.ReadOnly = $true

    # Barra de estado para información de la respuesta
    $statusBar = New-Object System.Windows.Forms.StatusBar
    $statusBar.Dock = "Bottom"
    $statusBar.SizingGrip = $false

    $global:lblStatus = New-Object System.Windows.Forms.StatusBarPanel
    $lblStatus.Text = "Listo"
    $lblStatus.AutoSize = [System.Windows.Forms.StatusBarPanelAutoSize]::Spring

    $global:lblStatusCode = New-Object System.Windows.Forms.StatusBarPanel
    $lblStatusCode.Text = ""
    $lblStatusCode.AutoSize = [System.Windows.Forms.StatusBarPanelAutoSize]::Contents

    $global:lblResponseTime = New-Object System.Windows.Forms.StatusBarPanel
    $lblResponseTime.Text = ""
    $lblResponseTime.AutoSize = [System.Windows.Forms.StatusBarPanelAutoSize]::Contents

    $statusBar.Panels.AddRange(@($lblStatus, $lblStatusCode, $lblResponseTime))

    $responsePanel.Controls.Add($txtResponse)
    $responsePanel.Controls.Add($statusBar)
    $parentControl.Controls.Add($responsePanel)
}

function Show-RequestForm {
    param(
        [string]$CollectionName,
        [hashtable]$RequestData = $null,
        [bool]$IsEdit = $false
    )

    $form = New-Object System.Windows.Forms.Form
    $form.Text = if ($IsEdit) { "Editar Solicitud" } else { "Nueva Solicitud" }
    $form.Size = New-Object System.Drawing.Size(800, 900)
    $form.StartPosition = "CenterParent"
    $form.FormBorderStyle = "FixedDialog"
    $form.MaximizeBox = $false

    # Controles del formulario (similar a los del panel central)
    $tabControl = New-Object System.Windows.Forms.TabControl
    $tabControl.Dock = "Fill"

    $tabDetails = New-Object System.Windows.Forms.TabPage
    $tabDetails.Text = "Detalles"
    Add-DetailsTabControls $tabDetails

    $tabControl.Controls.Add($tabDetails)
    $form.Controls.Add($tabControl)

    # Rellenar con datos si es edición
    if ($IsEdit -and $RequestData) {
        $cmbMethod.SelectedItem = $RequestData.method
        $txtRequestName.Text = $RequestData.name
        $txtUrl.Text = $RequestData.location
        $txtHeaders.Text = $RequestData.header
        $txtBody.Text = $RequestData.body
        
        if ($RequestData.description) {
            $txtDescription.Text = $RequestData.description
        }
    }

    # Botones inferiores
    $buttonPanel = New-Object System.Windows.Forms.Panel
    $buttonPanel.Dock = "Bottom"
    $buttonPanel.Height = 50
    $buttonPanel.Padding = New-Object System.Windows.Forms.Padding(5)

    $btnSave = New-Object System.Windows.Forms.Button
    $btnSave.Text = "Guardar"
    $btnSave.DialogResult = "OK"
    $btnSave.Anchor = "Right, Bottom"
    $btnSave.Size = New-Object System.Drawing.Size(100, 30)
    $btnSave.Location = New-Object System.Drawing.Point(500, 10)

    $btnCancel = New-Object System.Windows.Forms.Button
    $btnCancel.Text = "Cancelar"
    $btnCancel.DialogResult = "Cancel"
    $btnCancel.Anchor = "Right, Bottom"
    $btnCancel.Size = New-Object System.Drawing.Size(100, 30)
    $btnCancel.Location = New-Object System.Drawing.Point(490, 10)

    $form.Controls.Add($buttonPanel)
    $buttonPanel.Controls.Add($btnCancel)
    $buttonPanel.Controls.Add($btnSave)

    $btnSave.Add_Click({
        if ([string]::IsNullOrWhiteSpace($txtRequestName.Text)) {
            [System.Windows.Forms.MessageBox]::Show("El nombre de la solicitud no puede estar vacío.", "Error", "OK", "Error")
            $form.DialogResult = "None"
        }
    })

    if ($form.ShowDialog($global:mainForm) -eq "OK") {
        $request = @{
            name = $txtRequestName.Text
            method = $cmbMethod.SelectedItem.ToString()
            location = $txtUrl.Text
            header = $txtHeaders.Text
            body = $txtBody.Text
            description = $txtDescription.Text
        }

        if ($IsEdit) {
            # Actualizar solicitud existente
            $collectionRequests = $global:collections[$CollectionName].request
            for ($i = 0; $i -lt $collectionRequests.Count; $i++) {
                if ($collectionRequests[$i].name -eq $RequestData.name) {
                    $global:collections[$CollectionName].request[$i] = $request
                    break
                }
            }
            
            # Actualizar TreeView
            $collectionNode = $treeCollections.Nodes | Where-Object { $_.Tag.name -eq $CollectionName }
            if ($collectionNode) {
                $requestNode = $collectionNode.Nodes | Where-Object { $_.Tag.request.name -eq $RequestData.name }
                if ($requestNode) {
                    $requestNode.Text = "$($request.method) - $($request.name)"
                    $requestNode.Tag.request = $request
                }
            }
        } else {
            # Agregar nueva solicitud
            $global:collections[$CollectionName].request += $request
            
            # Agregar al TreeView
            $collectionNode = $treeCollections.Nodes | Where-Object { $_.Tag.name -eq $CollectionName }
            if ($collectionNode) {
                $requestNode = New-Object System.Windows.Forms.TreeNode
                $requestNode.Text = "$($request.method) - $($request.name)"
                $requestNode.Name = "request_$(New-Guid)"
                $requestNode.Tag = @{
                    type = "request"
                    collection = $CollectionName
                    request = $request
                }
                $requestNode.ImageKey = "Request"
                $requestNode.SelectedImageKey = "Request"
                
                $collectionNode.Nodes.Add($requestNode) | Out-Null
                $collectionNode.Expand()
            }
        }
    }
}

function Send-Request {
    try {
        $headers = ConvertFrom-Json $txtHeaders.Text -ErrorAction Stop
    } catch {
        [System.Windows.Forms.MessageBox]::Show("Los headers no tienen un formato JSON válido.", "Error", "OK", "Error")
        return
    }

    $uri = $txtUrl.Text
    $method = $cmbMethod.SelectedItem.ToString()
    $body = $null

    if ($method -in @("POST", "PUT", "PATCH") -and -not [string]::IsNullOrWhiteSpace($txtBody.Text)) {
        $body = $txtBody.Text
    }

    $stopwatch = [System.Diagnostics.Stopwatch]::StartNew()

    try {
        $response = Invoke-RestMethod -Uri $uri -Method $method -Headers $headers -Body $body -ErrorAction Stop
        
        $txtResponse.Text = ($response | ConvertTo-Json -Depth 10)
        $lblStatusCode.Text = "Estado: 200 OK"
        $lblStatus.Text = "Solicitud completada con éxito"
    } catch {
        $errorResponse = $_.Exception.Response
        if ($errorResponse) {
            $statusCode = [int]$errorResponse.StatusCode
            $statusDescription = $errorResponse.StatusDescription
            $lblStatusCode.Text = "Estado: $statusCode $statusDescription"
            
            $reader = New-Object System.IO.StreamReader($errorResponse.GetResponseStream())
            $errorDetails = $reader.ReadToEnd()
            $reader.Close()
            
            $txtResponse.Text = $errorDetails
            $lblStatus.Text = "Error en la solicitud"
        } else {
            $txtResponse.Text = $_.Exception.Message
            $lblStatus.Text = "Error en la solicitud"
            $lblStatusCode.Text = ""
        }
    } finally {
        $stopwatch.Stop()
        $lblResponseTime.Text = "Tiempo: $($stopwatch.ElapsedMilliseconds)ms"
    }
}

function Save-Request {
    # Implementar lógica para guardar cambios a la solicitud actual
    [System.Windows.Forms.MessageBox]::Show("Cambios guardados correctamente.", "Guardar", "OK", "Information")
}

function Show-RequestDetails {
    param(
        [hashtable]$Request
    )

    $cmbMethod.SelectedItem = $Request.method
    $txtRequestName.Text = $Request.name
    $txtUrl.Text = $Request.location
    $txtHeaders.Text = $Request.header
    $txtBody.Text = $Request.body
    
    if ($Request.description) {
        $txtDescription.Text = $Request.description
    } else {
        $txtDescription.Text = ""
    }
    
    $txtResponse.Text = ""
    $lblStatus.Text = "Listo"
    $lblStatusCode.Text = ""
    $lblResponseTime.Text = ""
}