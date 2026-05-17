[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet("E1_1", "E1_2", "E1_3", "E2_1", "E3_1", "E4", "E5", "E6", "E7")]
    [string]$Target
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$Root = Split-Path -Parent $PSScriptRoot
$Exe = Join-Path $Root "build-vs2026\$Target.exe"

if (-not (Test-Path -LiteralPath $Exe)) {
    Write-Error "Executable not found: $Exe. Run scripts\build-vs2026.ps1 first."
    exit 1
}

Start-Process -FilePath $Exe -WorkingDirectory (Split-Path -Parent $Exe)
