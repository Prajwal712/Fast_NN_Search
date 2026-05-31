<div align="center">

# ⚡ Fast NN Search

**A high-performance, interactive nearest-neighbor search visualizer built with C++ and SDL2**

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![SDL2](https://img.shields.io/badge/SDL2-Graphics-green?style=for-the-badge&logo=data:image/svg+xml;base64,&logoColor=white)](https://www.libsdl.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%2064--bit-lightgrey?style=for-the-badge&logo=windows&logoColor=white)](https://www.microsoft.com/windows)

*Explore, compare, and benchmark KD-Tree, Quad-Tree, and Linear search algorithms through a real-time interactive GUI.*

</div>

---

## 🎬 Demo

<div align="center">

[![Watch the Demo](https://img.shields.io/badge/▶_Watch_Demo-YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/watch?v=HUJq90Hpbcc)

🔗 **[https://www.youtube.com/watch?v=HUJq90Hpbcc](https://www.youtube.com/watch?v=HUJq90Hpbcc)**

</div>

---

## 📖 About

**Fast NN Search** is a desktop application that provides a hands-on way to visualize and benchmark nearest-neighbor search algorithms on 2D point datasets. Built as a course project for **Data Structures & Algorithms**, it allows users to interactively create point categories, add/remove points, and run nearest-neighbor queries using three different search strategies — comparing their performance in real time.

---

## ✨ Key Features

| Feature | Description |
|---------|-------------|
| 🌳 **KD-Tree Search** | Efficient axis-aligned binary space partitioning for O(log n) average-case nearest-neighbor queries |
| 🗺️ **Quad-Tree Search** | Region-based spatial decomposition with adaptive subdivision for fast spatial lookups |
| 📏 **Linear Search** | Brute-force baseline for direct comparison (O(n) per query) |
| ⏱️ **Real-Time Benchmarking** | Microsecond-precision timing displayed after each search query |
| 🎨 **Interactive Visualization** | Color-coded point categories on a 2D coordinate plane with axes and gridlines |
| ➕ **Dynamic Point Management** | Add/remove individual points or bulk-generate thousands of random points |
| 🔄 **Algorithm Toggle** | Instantly switch between KD-Tree, Quad-Tree, and Linear search modes |
| 📐 **Resizable Window** | Fully responsive layout that adapts to any window size |
| 🎚️ **Adjustable Rendering** | Control point size and max render limit via sliders for smooth performance at scale |

---

## 🏗️ Architecture

```
Fast_NN_Search/
├── framework.cpp          # Main application — SDL2 rendering, UI, and event handling
├── KD-Tree/
│   ├── kd_tree.h          # KD-Tree node structure and API declarations
│   └── kd_tree.cpp        # KD-Tree insert, delete, and nearest-neighbor search
├── Quad-Tree/
│   ├── quadtree.h         # Quad-Tree node structure and API declarations
│   └── quadtree.cpp       # Quad-Tree insert, delete, and nearest-neighbor search
├── libs/                  # Pre-bundled SDL2 & SDL2_ttf 64-bit libraries
├── compile.bat            # One-click build script
├── ARIAL.TTF              # Font asset for UI text rendering
├── SDL2.dll               # SDL2 runtime library
└── SDL2_ttf.dll           # SDL2_ttf runtime library
```

---

## ⚙️ Algorithm Complexity

| Operation | KD-Tree | Quad-Tree | Linear |
|-----------|---------|-----------|--------|
| **Insert** | O(log n) | O(log n) | — |
| **Delete** | O(log n) | O(log n) | O(n) |
| **Nearest Neighbor** | O(log n) avg | O(log n) avg | O(n) |
| **Space** | O(n) | O(n) | O(n) |

> **Note:** KD-Tree and Quad-Tree worst-case search can degrade to O(n) for pathological distributions, but average-case performance on random data is significantly faster than linear scan.

---

## 🚀 Getting Started

### Prerequisites

This project targets **64-bit Windows** with the **MinGW-w64 (GCC)** toolchain.

> [!IMPORTANT]
> A **64-bit** C++ compiler is required. If your MinGW installation is 32-bit (`mingw32`), compilation will fail.

**Download a compatible compiler:**
[MinGW-w64 GCC 15.2.0 (64-bit)](https://github.com/brechtsanders/winlibs_mingw/releases/download/15.2.0posix-13.0.0-ucrt-r3/winlibs-i686-posix-dwarf-gcc-15.2.0-mingw-w64ucrt-13.0.0-r3.zip)

> All SDL2 libraries, headers, and DLLs are **pre-bundled** in this repository — no additional dependencies to install.

### Build & Run

```bash
# 1. Clone the repository
git clone https://github.com/Prajwal712/Fast_NN_Search.git

# 2. Navigate to the project directory
cd Fast_NN_Search

# 3. Compile (Windows)
.\compile.bat

# 4. Run the application
.\my_map_app.exe
```

A successful build produces `my_map_app.exe` in the project root.

---

## 🕹️ Usage Guide

### Main View
- **Add Category** — Create named point groups, each with a unique color
- **Remove All** — Clear all categories and points
- **Point Size Slider** — Adjust the visual radius of rendered points
- **Max Render Limit Slider** — Cap the number of points drawn for performance

### Category View *(click a category to enter)*
- **Add Point** — Click anywhere on the map to place a new point
- **Remove Nearest** — Click to remove the closest point to your cursor
- **Search Nearest** — Click to find the nearest neighbor with real-time timing
- **Add N Random Points** — Bulk-generate points for large-scale testing
- **Algorithm Toggle** — Cycle through KD-Tree → Quad-Tree → Linear search
- **Delete Category** — Remove the entire category and its points
- **← Back** — Return to the main view

---

## 🛠️ Tech Stack

| Technology | Purpose |
|------------|---------|
| **C++17** | Core application logic and data structures |
| **SDL2** | Window management, rendering, and input handling |
| **SDL2_ttf** | TrueType font rendering for the UI |
| **MinGW-w64** | Compiler toolchain (GCC for Windows) |

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

<div align="center">

**Built with ❤️ as a DSA Course Project**

*If you found this project useful, consider giving it a ⭐ on GitHub!*

</div>
