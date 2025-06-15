# Exit on error
$ErrorActionPreference = "Stop"

# App name
$AppName = "WebCrawlerApp"
$VcpkgDir = "pkg"
$BuildDir = "build"

Write-Host "🔧 Checking for pkg..."

# Clone and bootstrap vcpkg if not present
if (!(Test-Path -Path $VcpkgDir)) {
    Write-Host "📥 Cloning pkg..."
    git clone https://github.com/microsoft/vcpkg $VcpkgDir
    & "$VcpkgDir\bootstrap-vcpkg.bat"
}

# Enable manifest mode
$env:VCPKG_ROOT = Resolve-Path $VcpkgDir
$env:VCPKG_FEATURE_FLAGS = "manifests"

# Install required packages
Write-Host "📦 Installing dependencies..."
& "$VcpkgDir\vcpkg.exe" install

# Delete old build directory (clean build)
if (Test-Path $BuildDir) {
    Write-Host "🧹 Removing old build directory..."
    Remove-Item -Recurse -Force $BuildDir
}
Write-Host "📁 Creating build directory..."
New-Item -ItemType Directory -Path $BuildDir | Out-Null

Set-Location -Path $BuildDir

# Run CMake with vcpkg toolchain
Write-Host "⚙️ Running CMake..."
& cmake .. -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=../$VcpkgDir/scripts/buildsystems/vcpkg.cmake

# Build project
Write-Host "🏗️ Building project..."
& cmake --build . --config Release

# Run the executable
$exePath = ".\Release\$AppName.exe"
if (Test-Path $exePath) {
    Write-Host "🚀 Launching app..."
    Start-Process $exePath
} else {
    Write-Host "❌ Executable not found: $exePath"
}
