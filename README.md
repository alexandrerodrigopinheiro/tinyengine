# Tiny Engine

> 🕹️ Minimalist 2D Game Engine in C++ using SDL2  
> Open-source, lightweight, and built for clarity, speed, and simplicity.

---

## 🧭 Overview

**Tiny Engine** is a compact and extensible 2D game engine written in modern C++, using SDL2 as its foundation.  
It was created to offer a clean and minimal alternative for indie developers, prototypes, and learning projects.

Its goal: **No frills, just a simple and functional game loop.**

---

## ✨ Features

- 2D rendering via SDL2
- Simple game loop with fixed timestep
- Sprite rendering and animation
- Input handling (keyboard/mouse)
- Asset loading (images, fonts)
- Scene management and entity loop
- Minimal external dependencies

---

## ⚙️ Requirements

- C++17 compiler (GCC, Clang, MSVC)
- [SDL2](https://github.com/libsdl-org/SDL) development libraries
- [SDL_image](https://github.com/libsdl-org/SDL_image)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
- CMake 3.15+

---

## 🔧 Build Instructions

### 1. Clone the repository

```bash
git clone https://github.com/your-org/tiny-engine.git
cd tiny-engine
```

### 2. Configure and build with CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### 3. Run the demo

```bash
./tiny-engine-demo
```

---

## 🧪 Project Structure

```plaintext
src/
├── core/         → Engine core (loop, timing, config)
├── graphics/     → Renderer, textures, sprites
├── input/        → Keyboard & mouse handling
├── scenes/       → Scene and entity logic
examples/         → Sample games
include/          → Public headers
```

---

## 🚀 Getting Started

To start your own project:

1. Copy the `src/` and `include/` folders
2. Use the provided `main.cpp` template
3. Extend the engine with your own components

---

## 🧾 License

This project is released under the **MIT License**.  
© Gaming Innovators — Open-source with ❤️.  
Feel free to fork, modify, and use in personal or commercial projects.

---

## 🤝 Credits

- [SDL2](https://github.com/libsdl-org/SDL)
- [Dear ImGui](https://github.com/ocornut/imgui) (optional)
- Contributors & indie developers
