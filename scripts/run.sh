#!/bin/bash

# Exit on any command failure
set -e

# Define app name
APP_NAME="WebCrawlerApp"

# Define vcpkg directory
VCPKG_DIR="./pkg"

echo "🔧 Checking for pkg..."

# Download and bootstrap vcpkg if not already present
if [ ! -d "$VCPKG_DIR" ]; then
    echo "📥 Cloning pkg..."
    git clone https://github.com/microsoft/vcpkg "$VCPKG_DIR"
    "$VCPKG_DIR/bootstrap-vcpkg.sh"
fi

# Enable manifest mode
export VCPKG_ROOT="$VCPKG_DIR"
export VCPKG_FEATURE_FLAGS="manifests"

# Install dependencies via vcpkg
echo "📦 Installing dependencies..."
"$VCPKG_DIR/vcpkg" install

# Create build directory if it doesn't exist
echo "📁 Setting up build directory..."
mkdir -p build
cd build

# Run CMake with vcpkg toolchain file
echo "⚙️ Running CMake..."
cmake .. -DCMAKE_TOOLCHAIN_FILE="../pkg/scripts/buildsystems/vcpkg.cmake"

# Build the project
echo "🏗️ Building $APP_NAME..."
make

# Run the application
echo "🚀 Running $APP_NAME..."
./$APP_NAME
