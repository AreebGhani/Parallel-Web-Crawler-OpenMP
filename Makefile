# Project settings
TARGET = WebCrawlerApp
BUILD_DIR = build
VCPKG_TOOLCHAIN = ./pkg/scripts/buildsystems/vcpkg.cmake

# Default target
all: configure build run

# Configure project with CMake
configure:
	@echo "🛠️ Configuring with CMake..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN)

# Build project
build:
	@echo "🔨 Building project..."
	@cmake --build $(BUILD_DIR)

# Run project
run:
	@echo "🚀 Running project..."
	@./$(BUILD_DIR)/$(TARGET)

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
