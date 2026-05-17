[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

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

function Invoke-InVS2026($Instance, $Command) {
    $vcvars = Join-Path $Instance.installationPath "VC\Auxiliary\Build\vcvars64.bat"
    if (-not (Test-Path -LiteralPath $vcvars)) {
        Fail "vcvars64.bat not found under $($Instance.installationPath). Install the C++ workload."
    }

    & cmd.exe /d /s /c "call `"$vcvars`" >nul && $Command"
}

function Get-CompilerVersion($Instance) {
    $output = Invoke-InVS2026 $Instance "cl 2>&1"
    $text = $output -join "`n"
    if ($text -notmatch "(\d+)\.(\d+)\.(\d+)") {
        Fail "Unable to read cl.exe version."
    }
    return [version]"$($Matches[1]).$($Matches[2]).$($Matches[3])"
}

function Get-RequiredToolPath($Instance, $ToolName) {
    $output = Invoke-InVS2026 $Instance "where $ToolName 2>&1"
    if ($LASTEXITCODE -ne 0) {
        Fail "$ToolName was not found in the VS2026 x64 build environment. Install the Windows SDK C++ desktop tools."
    }

    $path = @($output | Where-Object { $_ -and ($_ -notmatch "^\s*$") })[0]
    if (-not $path) {
        Fail "$ToolName was not found in the VS2026 x64 build environment. Install the Windows SDK C++ desktop tools."
    }

    return $path
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

$instance = Get-VS2026Instance
$compilerVersion = Get-CompilerVersion $instance
if ($compilerVersion -lt $MinimumCompilerVersion) {
    Fail "MSVC compiler $compilerVersion found, but this project requires 19.50+ / MSVC 14.50+."
}

$rcPath = Get-RequiredToolPath $instance "rc.exe"
$mtPath = Get-RequiredToolPath $instance "mt.exe"
$cmakePath = Get-CMakePath $instance
$cmakeVersion = (& $cmakePath --version | Select-Object -First 1)
$sdkRoots = Get-ChildItem "${env:ProgramFiles(x86)}\Windows Kits\10\Lib" -Directory -ErrorAction SilentlyContinue |
    Sort-Object { [version]$_.Name } -Descending
$sdkVersion = if ($sdkRoots) { $sdkRoots[0].Name } else { "not found" }

Write-Host "Visual Studio: $($instance.displayName)"
Write-Host "VS path: $($instance.installationPath)"
Write-Host "MSVC cl.exe: $compilerVersion"
Write-Host "CMake: $cmakeVersion"
Write-Host "Windows SDK: $sdkVersion"
Write-Host "Resource compiler: $rcPath"
Write-Host "Manifest tool: $mtPath"
