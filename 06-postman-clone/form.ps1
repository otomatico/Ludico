# Importar ensamblados necesarios
Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName Microsoft.VisualBasic

# Crear formulario principal
$form = New-Object System.Windows.Forms.Form
$form.Text = "HTTP Request Manager"
$form.Size = New-Object System.Drawing.Size(900, 700)
$form.StartPosition = "CenterScreen"
$form.Font = New-Object System.Drawing.Font("Open Sans", 10)
$form.MinimumSize = New-Object System.Drawing.Size(800, 600)

# Cargar el panel central pasando la referencia del formulario
. "$PSScriptRoot\panel-central.ps1"
Initialize-CenterPanel -parentForm $form

# Mostrar el formulario
$form.ShowDialog() | Out-Null
#Set-ExecutionPolicy RemoteSigned -Scope CurrentUser -Force
