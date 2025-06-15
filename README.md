# 🕸️ Web Crawler App

A **Parallel Web Crawler** written in **C++**, powered by **OpenMP**, with a basic **GUI** built using `wxWidgets`. The crawler scrapes product data from websites (e.g., Amazon) using `libcurl` and parses HTML using `gumbo`.

This project uses **vcpkg** for dependency management and **Node.js** for cross-platform project launch scripting.

---

## 📌 Features

- Parallel crawling using OpenMP
- Cross-platform support: Linux, Windows, macOS
- GUI frontend with wxWidgets
- Web scraping via libcurl
- HTML parsing with Google's Gumbo parser
- Modern dependency management with vcpkg (manifest mode)
- Simple build via CMake
- Unified launch script using Node.js

---

## 🛠️ Technologies Used

| Technology   | Purpose                             |
| ------------ | ----------------------------------- |
| C++          | Core language for logic and GUI     |
| OpenMP       | Parallel crawling                   |
| wxWidgets    | Cross-platform GUI framework        |
| libcurl      | HTTP requests and web scraping      |
| gumbo        | HTML parser library                 |
| vcpkg        | C++ package manager (manifest mode) |
| CMake        | Build system                        |
| Node.js      | Cross-platform launch script        |

---

## 📁 Project Structure
```
├── lib/ # Source code (main.cpp)
├── scripts/ # Platform-specific scripts (run.sh, run.ps1)
├── pkg/ # vcpkg (auto-downloaded if missing)
├── build/ # CMake build directory
├── index.js # Node.js-based launcher
├── vcpkg.json # Dependency manifest (vcpkg)
├── CMakeLists.txt # CMake configuration
├── Makefile # Wrapper for CMake build
└── README.md # You're reading this :)
```

---

## 📦 Dependencies

### Managed via [vcpkg](https://github.com/microsoft/vcpkg)

```json
{
  "name": "web-crawler",
  "version": "1.0.0",
  "description": "Parallel Web Crawler using C++ and OpenMP",
  "license": "ISC",
  "dependencies": ["wxwidgets", "curl", "gumbo"]
}
```

---

## 🚀 Getting Started

### 🔧 Requirements

- C++17 compiler (g++ / clang / Visual Studio)
- cmake (version ≥ 3.31)
- node (for running index.js script)
- git (for cloning vcpkg)

### 🧪 Run the App

Automatically detects OS and runs the appropriate script.

```bash
npm start
```

### 🧰 Manual Build (Alternative)

- Unix/macOS:

```bash
bash ./scripts/run.sh
```

- Windows:

```bash
.\scripts\run.ps1
```

### 🧹 Clean Build

```bash
make clean
```

---

## 🤝 Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your improvements or ideas.

---

## 📄 License

MIT License — feel free to use, modify, and distribute.

---

## 👨‍💻 Author

Areeb Ghani
BS Computer Science — COMSATS Lahore
Passionate about system programming and full-stack dev.

---

Let me know if you'd like a badge section (build status, license, etc.) or CI setup added to this `README.md`.
