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
    $grpBasic = New-Object System.Windows.Forms.GroupBox
    $grpBasic.Text = "Parámetros Básicos"
    #$grpBasic.Location = New-Object System.Drawing.Point(10, 10)
    #$grpBasic.Size = New-Object System.Drawing.Size(800, 130)
    $grpBasic.AutoSize = $true
    $grpBasic.Padding = New-Object System.Windows.Forms.Padding(10)
    $grpBasic.Margin = New-Object System.Windows.Forms.Padding(20)
    #$grpBasic.Anchor = "Left, Right, Top"
    $grpBasic.Dock = "Top"

    # Controles básicos
    $lblMethod = New-Object System.Windows.Forms.Label
    $lblMethod.Text = "Método:"
    $lblMethod.Location = New-Object System.Drawing.Point(10, 25)
    $lblMethod.Size = New-Object System.Drawing.Size(80, 20)

    $global:cmbMethod = New-Object System.Windows.Forms.ComboBox
    $cmbMethod.Location = New-Object System.Drawing.Point(100, 25)
    $cmbMethod.Size = New-Object System.Drawing.Size(100, 20)
    $cmbMethod.Items.AddRange(@("GET", "POST", "PUT", "DELETE", "PATCH", "HEAD", "OPTIONS"))
    $cmbMethod.SelectedIndex = 0

    $lblName = New-Object System.Windows.Forms.Label
    $lblName.Text = "Nombre:"
    $lblName.Location = New-Object System.Drawing.Point(10, 55)
    $lblName.Size = New-Object System.Drawing.Size(80, 20)

    $global:txtRequestName = New-Object System.Windows.Forms.TextBox
    $txtRequestName.Location = New-Object System.Drawing.Point(100, 55)
    $txtRequestName.Size = New-Object System.Drawing.Size(680, 20)

    $lblUrl = New-Object System.Windows.Forms.Label
    $lblUrl.Text = "URL:"
    $lblUrl.Location = New-Object System.Drawing.Point(10, 85)
    $lblUrl.Size = New-Object System.Drawing.Size(80, 20)

    $global:txtUrl = New-Object System.Windows.Forms.TextBox
    $txtUrl.Location = New-Object System.Drawing.Point(100, 85)
    $txtUrl.Size = New-Object System.Drawing.Size(680, 20)
    $txtUrl.Text = "https://"

    $grpBasic.Controls.AddRange(@($lblMethod, $cmbMethod, $lblName, $txtRequestName, $lblUrl, $txtUrl))

    # Grupo de headers
    $grpHeaders = New-Object System.Windows.Forms.GroupBox
    $grpHeaders.Text = "Headers"
    #$grpHeaders.Location = New-Object System.Drawing.Point(10, 160)
    #$grpHeaders.Size = New-Object System.Drawing.Size(800, 200)
    #$grpHeaders.Anchor = "Left, Top"
    $grpHeaders.AutoSize = $true
    $grpHeaders.Padding = New-Object System.Windows.Forms.Padding(10)
    $grpHeaders.Margin = New-Object System.Windows.Forms.Padding(10)
    $grpHeaders.Dock = "Top"
    

    $global:txtHeaders = New-Object System.Windows.Forms.RichTextBox
    $txtHeaders.Location = New-Object System.Drawing.Point(10, 30)
    $txtHeaders.Size = New-Object System.Drawing.Size(780, 100)
    $txtHeaders.Font = New-Object System.Drawing.Font("Consolas", 10)
    $txtHeaders.Text = "{`n    `"Content-Type`": `"application/json`",`n    `"Authorization`": `"Bearer token`"`n}"

    $grpHeaders.Controls.Add($txtHeaders)

    # Grupo de body
    $grpBody = New-Object System.Windows.Forms.GroupBox
    $grpBody.Text = "Body"
    #$grpBody.Location = New-Object System.Drawing.Point(10, 330)
    #$grpBody.Size = New-Object System.Drawing.Size(800, 230)
    #$grpBody.Anchor = "Left, Top"
    $grpBody.AutoSize = $true
    $grpBody.Padding = New-Object System.Windows.Forms.Padding(0)
    $grpBody.Margin = New-Object System.Windows.Forms.Padding(3)
    $grpBody.Dock = "Top"

    $global:txtBody = New-Object System.Windows.Forms.RichTextBox
    $txtBody.Location = New-Object System.Drawing.Point(10, 30)
    $txtBody.Size = New-Object System.Drawing.Size(780, 150)
    $txtBody.Font = New-Object System.Drawing.Font("Consolas", 10)

    $grpBody.Controls.Add($txtBody)

    # Barra de botones
    $buttonPanel = New-Object System.Windows.Forms.Panel
    #$buttonPanel.Location = New-Object System.Drawing.Point(10, 640)
    #$buttonPanel.Size = New-Object System.Drawing.Size(800, 40)
    #$buttonPanel.Anchor = "Left, Right, Bottom"
    $buttonPanel.Padding = New-Object System.Windows.Forms.Padding(10)
    $buttonPanel.Margin = New-Object System.Windows.Forms.Padding(10)
    $buttonPanel.Dock = "Bottom"
    
    $btnSend = New-Object System.Windows.Forms.Button
    $btnSend.Text = "Enviar Solicitud"
    $btnSend.Location = New-Object System.Drawing.Point(10, 5)
    $btnSend.Size = New-Object System.Drawing.Size(120, 30)
    $btnSend.Add_Click({ Send-Request })

    $btnSave = New-Object System.Windows.Forms.Button
    $btnSave.Text = "Guardar Solicitud"
    $btnSave.Location = New-Object System.Drawing.Point(140, 5)
    $btnSave.Size = New-Object System.Drawing.Size(120, 30)
    $btnSave.Add_Click({ Save-RequestToFile })

    $btnLoad = New-Object System.Windows.Forms.Button
    $btnLoad.Text = "Cargar Solicitud"
    $btnLoad.Location = New-Object System.Drawing.Point(270, 5)
    $btnLoad.Size = New-Object System.Drawing.Size(120, 30)
    $btnLoad.Add_Click({ Load-RequestFromFile })

    $buttonPanel.Controls.AddRange(@($btnSend, $btnSave, $btnLoad))
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