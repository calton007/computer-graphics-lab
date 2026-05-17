[CmdletBinding()]
param(
    [switch]$Clean
)

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$Root = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $Root "build-vs2026"
$MinimumCompilerVersion = [version]"19.50"
$VswherePath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

function Fail($Message) {
    Write-Error $Message
    exit 1
}

function Get-VS2026Instance {
    if (-not (Test-Path -LiteralPath $VswherePath)) {
        Fail "vswhere not found. Install Visual Studio 2026 Build Tools first."
    }

    $json = & $VswherePath -all -products * -format json
    $instances = @($json | ConvertFrom-Json)
    $instance = $instances |
        Where-Object { $_.installationVersion -like "18.*" -and $_.isComplete -eq $true -and $_.isLaunchable -eq $true } |
        Select-Object -First 1

    if (-not $instance) {
        Fail @"
Visual Studio 2026 Build Tools was not found.
Download Build Tools from:
https://visualstudio.microsoft.com/downloads/

Then run the downloaded bootstrapper as administrator:
.\vs_BuildTools.exe --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --passive --norestart
"@
    }

    return $instance
}

function Get-CMakePath($Instance) {
    $candidates = @(
        (Join-Path $Instance.installationPath "Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"),
        "${env:ProgramFiles(x86)}\Microsoft Visual Studio\2019\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
        "$env:ProgramFiles\CMake\bin\cmake.exe"
    )

    foreach ($candidate in $candidates) {
        if ($candidate -and (Test-Path -LiteralPath $candidate)) {
            return $candidate
        }
    }

    $cmd = Get-Command cmake -ErrorAction SilentlyContinue
    if ($cmd) {
        return $cmd.Source
    }

    Fail "CMake not found. Install CMake or the Visual Studio CMake component."
}

function Invoke-InVS2026($Instance, $Command) {
    $vcvars = Join-Path $Instance.installationPath "VC\Auxiliary\Build\vcvars64.bat"
    if (-not (Test-Path -LiteralPath $vcvars)) {
        Fail "vcvars64.bat not found under $($Instance.installationPath). Install the C++ workload."
    }

    & cmd.exe /d /s /c "call `"$vcvars`" >nul && $Command"
    if ($LASTEXITCODE -ne 0) {
        Fail "Command failed with exit code ${LASTEXITCODE}: $Command"
    }
}

function Assert-RequiredTool($Instance, $ToolName) {
    $vcvars = Join-Path $Instance.installationPath "VC\Auxiliary\Build\vcvars64.bat"
    & cmd.exe /d /s /c "call `"$vcvars`" >nul && where $ToolName >nul 2>&1"
    if ($LASTEXITCODE -ne 0) {
        Fail "$ToolName was not found in the VS2026 x64 build environment. Install the Windows SDK C++ desktop tools."
    }
}

$instance = Get-VS2026Instance
$clOutput = Invoke-InVS2026 $instance "cl 2>&1"
$clText = $clOutput -join "`n"
if ($clText -notmatch "(\d+)\.(\d+)\.(\d+)") {
    Fail "Unable to read cl.exe version."
}

$compilerVersion = [version]"$($Matches[1]).$($Matches[2]).$($Matches[3])"
if ($compilerVersion -lt $MinimumCompilerVersion) {
    Fail "MSVC compiler $compilerVersion found, but this project requires 19.50+ / MSVC 14.50+."
}

Assert-RequiredTool $instance "rc.exe"
Assert-RequiredTool $instance "mt.exe"

if ($Clean -and (Test-Path -LiteralPath $BuildDir)) {
    Remove-Item -LiteralPath $BuildDir -Recurse -Force
}

$cmakePath = Get-CMakePath $instance
$configure = "`"$cmakePath`" -S `"$Root`" -B `"$BuildDir`" -G `"NMake Makefiles`""
$build = "`"$cmakePath`" --build `"$BuildDir`""

Invoke-InVS2026 $instance $configure
Invoke-InVS2026 $instance $build

Write-Host "Build complete: $BuildDir"
