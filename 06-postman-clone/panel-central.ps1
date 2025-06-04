function Initialize-CenterPanel {
    param( [System.Windows.Forms.Form]$parentForm  )
    
    # Panel central
    $global:centerPanel = New-Object System.Windows.Forms.Panel
    $centerPanel.Dock = "Fill"
    $centerPanel.BackColor = [System.Drawing.Color]::White
    $centerPanel.BorderStyle = [System.Windows.Forms.BorderStyle]::FixedSingle

    # TabControl principal
    $tabControl = New-Object System.Windows.Forms.TabControl
    $tabControl.Dock = "Fill"

    # Pestaña de solicitud
    $tabRequest = New-Object System.Windows.Forms.TabPage
    $tabRequest.Text = "Solicitud"
    Initialize-RequestTab $tabRequest

    # Pestaña de respuesta
    $tabResponse = New-Object System.Windows.Forms.TabPage
    $tabResponse.Text = "Respuesta"
    Initialize-ResponseTab $tabResponse

    $tabControl.Controls.AddRange(@($tabRequest, $tabResponse))
    $centerPanel.Controls.Add($tabControl)
    $parentForm.Controls.Add($centerPanel)
}

function Initialize-RequestTab {
    param($parentControl)

    # Panel con scroll
    $scrollPanel = New-Object System.Windows.Forms.Panel
    $scrollPanel.Dock = "Fill"
    $scrollPanel.AutoScroll = $true
    $scrollPanel.Padding = New-Object System.Windows.Forms.Padding(10)

    # Grupo de parámetros básicos
    $grpBasic = New-Object System.Windows.Forms.Panel
    $grpBasic.Dock = "Top"
    #$grpBasic.AutoSize = $true
    $grpBasic.Size = New-Object System.Drawing.Size(800, 50)
    $grpBasic.Padding = New-Object System.Windows.Forms.Padding(10)
    $grpBasic.Margin = New-Object System.Windows.Forms.Padding(20)

    # Controles básicos
    $global:cmbMethod = New-Object System.Windows.Forms.ComboBox
    $cmbMethod.Location = New-Object System.Drawing.Point(10, 15)
    $cmbMethod.Size = New-Object System.Drawing.Size(100, 20)
    $cmbMethod.Anchor = "Left"
    $cmbMethod.Items.AddRange(@("GET", "POST", "PUT", "DELETE", "PATCH", "HEAD", "OPTIONS"))
    $cmbMethod.SelectedIndex = 0

    $global:txtUrl = New-Object System.Windows.Forms.TextBox
    $txtUrl.Location = New-Object System.Drawing.Point(115, 15)
    #$txtUrl.Size = New-Object System.Drawing.Size(625, 20)
    $txtUrl.AutoSize = $true
    $txtUrl.Anchor = "Left, Right"
    $txtUrl.Padding = New-Object System.Windows.Forms.Padding(10)
    $txtUrl.Margin = New-Object System.Windows.Forms.Padding(50)

    $txtUrl.Text = "https://"
    
    $btnSend = New-Object System.Windows.Forms.Button
    $btnSend.Text = "Enviar"
    $btnSend.Location = New-Object System.Drawing.Point(690, 12)
    $btnSend.Size = New-Object System.Drawing.Size(100, 30)
    $btnSend.Anchor = "Right"
    #$btnSend.BorderStyle = [System.Windows.Forms.BorderStyle]::None;
    $btnSend.Add_Click({ Send-Request })

    $grpBasic.Controls.AddRange(@($cmbMethod, $txtUrl, $btnSend))

    # Grupo de headers
    $grpHeaders = New-Object System.Windows.Forms.GroupBox
    $grpHeaders.Text = "Headers"
    $grpHeaders.AutoSize = $true
    #$grpHeaders.Padding = New-Object System.Windows.Forms.Padding(10)
    $grpHeaders.Margin = New-Object System.Windows.Forms.Padding(10)
    $grpHeaders.Dock = "Top"    
    
    $global:txtHeaders = New-Object System.Windows.Forms.RichTextBox
    $txtHeaders.Location = New-Object System.Drawing.Point(10, 30)
    $txtHeaders.AutoSize = $true
    $txtHeaders.Anchor = "Left, Right, Top"
    $txtHeaders.Font = New-Object System.Drawing.Font("Consolas", 10)
    $txtHeaders.Text = "{`n    `"Content-Type`": `"application/json`",`n    `"Authorization`": `"Bearer token`"`n}"

    $grpHeaders.Controls.Add($txtHeaders)

    # Grupo de body
    $grpBody = New-Object System.Windows.Forms.GroupBox
    $grpBody.Text = "Body"
    $grpBody.AutoSize = $true
    $grpBody.Padding = New-Object System.Windows.Forms.Padding(0)
    $grpBody.Margin = New-Object System.Windows.Forms.Padding(3)
    $grpBody.Dock = "Top"

    $global:txtBody = New-Object System.Windows.Forms.RichTextBox
    $txtBody.Location = New-Object System.Drawing.Point(10, 30)
    #$txtBody.Size = New-Object System.Drawing.Size(580, 150)
    $txtBody.AutoSize = $true
    $txtBody.Anchor = "Left, Right, Top"
    $txtBody.Font = New-Object System.Drawing.Font("Consolas", 10)

    $grpBody.Controls.Add($txtBody)

    # Barra de botones
    $buttonPanel = New-Object System.Windows.Forms.Panel
    $buttonPanel.Padding = New-Object System.Windows.Forms.Padding(5)
    $buttonPanel.Margin = New-Object System.Windows.Forms.Padding(5)
    $buttonPanel.Size = New-Object System.Drawing.Size(800, 50)
    $buttonPanel.Dock = "Bottom"

    $btnSave = New-Object System.Windows.Forms.Button
    $btnSave.Text = "Exportar"
    $btnSave.Location = New-Object System.Drawing.Point(140, 5)
    #$btnSave.Size = New-Object System.Drawing.Size(120, 30)
    $btnSave.Dock = "Right"
    $btnSave.AutoSize = $true
    $btnSave.Padding = New-Object System.Windows.Forms.Padding(5)
    $btnSave.Margin = New-Object System.Windows.Forms.Padding(5)
    
    $btnSave.Add_Click({ Save-RequestToFile })

    $btnLoad = New-Object System.Windows.Forms.Button
    $btnLoad.Text = "Importar"
    $btnLoad.Location = New-Object System.Drawing.Point(270, 5)
    #$btnLoad.Size = New-Object System.Drawing.Size(120, 30)
    $btnLoad.Dock = "Right"
    $btnLoad.Add_Click({ Load-RequestFromFile })


    $buttonPanel.Controls.AddRange(@($btnSave, $btnLoad))
    $scrollPanel.Controls.AddRange(@($grpBody, $grpHeaders, $grpBasic, $buttonPanel))
    $parentControl.Controls.Add($scrollPanel)
}

function Initialize-ResponseTab {
    param($parentControl)

    $global:txtResponse = New-Object System.Windows.Forms.RichTextBox
    $txtResponse.Dock = "Fill"
    $txtResponse.Font = New-Object System.Drawing.Font("Consolas", 10)
    $txtResponse.ReadOnly = $true

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
    $parentControl.Controls.Add($txtResponse)
    $parentControl.Controls.Add($statusBar)
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

function Save-RequestToFile {
    $saveFileDialog = New-Object System.Windows.Forms.SaveFileDialog
    $saveFileDialog.Filter = "JSON files (*.json)|*.json|All files (*.*)|*.*"
    $saveFileDialog.Title = "Guardar solicitud HTTP"
    $saveFileDialog.DefaultExt = "json"
    $saveFileDialog.AddExtension = $true
    
    if ($saveFileDialog.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
        $requestData = @{
            Name = $txtRequestName.Text
            Method = $cmbMethod.SelectedItem.ToString()
            Url = $txtUrl.Text
            Headers = $txtHeaders.Text
            Body = $txtBody.Text
        }
        
        try {
            $requestData | ConvertTo-Json -Depth 10 | Out-File -FilePath $saveFileDialog.FileName -Encoding UTF8
            [System.Windows.Forms.MessageBox]::Show("Solicitud guardada correctamente.", "Éxito", "OK", "Information")
        } catch {
            [System.Windows.Forms.MessageBox]::Show("Error al guardar el archivo: $_", "Error", "OK", "Error")
        }
    }
}

function Load-RequestFromFile {
    $openFileDialog = New-Object System.Windows.Forms.OpenFileDialog
    $openFileDialog.Filter = "JSON files (*.json)|*.json|All files (*.*)|*.*"
    $openFileDialog.Title = "Cargar solicitud HTTP"
    
    if ($openFileDialog.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK) {
        try {
            $requestData = Get-Content -Path $openFileDialog.FileName -Raw -Encoding UTF8 | ConvertFrom-Json
            
            $txtRequestName.Text = $requestData.Name
            $cmbMethod.SelectedItem = $requestData.Method
            $txtUrl.Text = $requestData.Url
            $txtHeaders.Text = $requestData.Headers
            $txtBody.Text = $requestData.Body
            
            [System.Windows.Forms.MessageBox]::Show("Solicitud cargada correctamente.", "Éxito", "OK", "Information")
        } catch {
            [System.Windows.Forms.MessageBox]::Show("Error al cargar el archivo: $_", "Error", "OK", "Error")
        }
    }
}