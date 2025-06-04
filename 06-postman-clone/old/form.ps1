# Importar ensamblados necesarios
Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing
Add-Type -AssemblyName Microsoft.VisualBasic

# Cargar módulos
. "$PSScriptRoot\helpers.ps1" -Encoding utf8
. "$PSScriptRoot\panel-izquierdo.ps1" -Encoding utf8
. "$PSScriptRoot\panel-central.ps1" -Encoding utf8

# Crear formulario principal
$global:mainForm = New-Object System.Windows.Forms.Form
$mainForm.Text = "PowerShell Postman Clone"
$mainForm.Font = New-Object System.Drawing.Font("Open Sans", 10)
$mainForm.Size = New-Object System.Drawing.Size(1200, 800)
$mainForm.StartPosition = "CenterScreen"
$mainForm.MinimumSize = New-Object System.Drawing.Size(800, 600)

# Configurar paneles
Initialize-CenterPanel
Initialize-LeftPanel

# Mostrar formulario
$mainForm.ShowDialog() | Out-Null
#Set-ExecutionPolicy RemoteSigned -Scope CurrentUser -Force
