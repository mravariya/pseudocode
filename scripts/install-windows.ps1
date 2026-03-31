# Build with CMake, then copy pseudocode.exe and pkg.exe to %LOCALAPPDATA%\Pseudocode\bin.
# Run from repo root in PowerShell:  .\scripts\install-windows.ps1
# Add that folder to your user PATH (see docs/installation.md).
$Root = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
Set-Location $Root
if (Get-Command cmake -ErrorAction SilentlyContinue) {
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build --config Release
} else {
  Write-Host "Install CMake or compile manually; see README."
  exit 1
}
$Dest = "$env:LOCALAPPDATA\Pseudocode\bin"
New-Item -ItemType Directory -Force -Path $Dest | Out-Null
Copy-Item build\Release\pseudocode.exe, build\Release\pkg.exe -Destination $Dest -ErrorAction SilentlyContinue
Copy-Item build\pseudocode.exe, build\pkg.exe -Destination $Dest -ErrorAction SilentlyContinue
Write-Host "Copied to $Dest — add that folder to your user PATH."
