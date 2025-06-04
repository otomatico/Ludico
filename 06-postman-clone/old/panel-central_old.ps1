function Initialize-CenterPanel {
    # Panel central
    $global:centerPanel = New-Object System.Windows.Forms.Panel
    $centerPanel.Dock = "Fill"
    $centerPanel.BackColor = [System.Drawing.Color]::White
    $centerPanel.BorderStyle = [System.Windows.Forms.BorderStyle]::FixedSingle

    # Agregar panel al formulario principal
    $mainForm.Controls.Add($centerPanel)
}

function Show-RequestForm {
    param(
        [string]$CollectionName,
        [hashtable]$RequestData = $null
    )

    $isEdit = $RequestData -ne $null
    $form = New-Object System.Windows.Forms.Form
    $form.Text = if ($isEdit) { "Editar Solicitud" } else { "Nueva Solicitud" }
    $form.Size = New-Object System.Drawing.Size(600, 700)
    $form.StartPosition = "CenterParent"

    # Controles del formulario (similar al anterior pero adaptado)
    # ... (código similar al que ya teníamos para solicitudes)

    if ($form.ShowDialog() -eq "OK") {
        # Lógica para guardar/actualizar la solicitud
    }
}