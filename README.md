# <p align="center">☂️ UmbrellaEngine</p>

<p align="center">
A modern game engine written in <strong>C++20</strong> using <strong>OpenGL</strong>.
</p>

<p align="center">

![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3+-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![Status](https://img.shields.io/badge/Status-Active%20Development-orange.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

</p>

---

> **UmbrellaEngine** is a game engine built from scratch to explore modern rendering techniques, engine architecture, and low-level graphics programming.
> The project focuses on writing clean, modular, and maintainable code while gaining a deeper understanding of how professional game engines work internally.

---

# ✨ Features

## Rendering

* OpenGL Renderer
* Physically Based Rendering (PBR) *(Work in Progress)*
* GLSL Shader System
* Material System
* Texture Management
* Model Loading
* Scene Graph

## Core

* Asset Manager
* Event System
* Logging System
* ECS Architecture (EnTT)
* File System Utilities
* Resource Handle System

## Assets

* glTF 2.0 Support
* OBJ Support
* Texture Importing
* Shader Hot Reloading *(Planned)*

---

# 🛠 Technologies

| Library   | Purpose         |
| --------- | --------------- |
| C++20     | Core Language   |
| OpenGL    | Rendering API   |
| GLFW      | Window & Input  |
| GLAD      | OpenGL Loader   |
| GLM       | Mathematics     |
| EnTT      | ECS Framework   |
| Assimp    | Model Importing |
| stb_image | Texture Loading |

---

# 🎯 Goals

UmbrellaEngine is built with the following objectives:

* Learn graphics programming from the ground up.
* Build a flexible and scalable engine architecture.
* Explore modern rendering techniques.
* Write clean, reusable, and maintainable code.
* Understand how commercial game engines are designed internally.

---

# 🏗 Architecture

```text
Application
      │
      ▼
Scene
      │
      ▼
Entity (EnTT)
      │
      ▼
Components
      │
      ▼
Renderer
      │
      ▼
OpenGL
```

---

# 📁 Project Structure

```text
UmbrellaEngine/
│
├── Engine/
│   ├── Core/
│   ├── Renderer/
│   ├── Assets/
│   ├── ECS/
│   ├── Scene/
│   ├── Events/
│   └── Platform/
│
├── Sandbox/
│
├── Resources/
│
├── ThirdParty/
│
└── Docs/
```

---

# 🚀 Current Progress

### Completed

* ✅ Asset Manager
* ✅ Shader System
* ✅ Texture Loader
* ✅ Model Importing
* ✅ Logging System
* ✅ ECS Integration

### Currently Working On

* 🚧 PBR Material System
* 🚧 Material Pipeline
* 🚧 Renderer Improvements

### Planned

* Shadow Mapping
* HDR Rendering
* Bloom
* SSAO
* Deferred Rendering
* Animation System
* Audio
* Physics
* Editor
* Scripting

---

# 🗺 Roadmap

* [x] Logger
* [x] Asset Manager
* [x] Shader System
* [x] Texture Loading
* [x] Model Importing
* [x] ECS
* [ ] PBR
* [ ] Animation
* [ ] Physics
* [ ] Audio
* [ ] Editor
* [ ] Vulkan Backend *(Future)*

---

# 📸 Screenshots

Screenshots and videos will be added as development progresses.
<img width="793" height="630" alt="Screenshot 2026-07-27 143816" src="https://github.com/user-attachments/assets/af9b7b1a-4a81-4f6e-ba63-ed449d60c747" />
<img width="1056" height="1007" alt="Screenshot 2026-07-27 143839" src="https://github.com/user-attachments/assets/9bb999b5-698c-4dad-939f-ee00b40f32a1" />


# 💡 Why UmbrellaEngine?

UmbrellaEngine is not intended to compete with engines like Unreal Engine or Unity.

Instead, the project serves as a learning platform for exploring graphics programming, rendering pipelines, engine architecture, and modern C++ through practical implementation.

Every subsystem is developed from scratch to better understand the design decisions behind professional game engines.

---

# 🔨 Building

## Requirements

* C++20 Compiler
* CMake
* OpenGL 3.3+
* Git

Clone the repository:

```bash
git clone https://github.com/<your-username>/UmbrellaEngine.git
```

Generate the project:

```bash
cmake -B build
cmake --build build
```

---

# 🤝 Contributing

Contributions, ideas, bug reports, and suggestions are always welcome.

If you'd like to contribute, feel free to open an issue or submit a pull request.

---

# 📄 License

This project is licensed under the MIT License.

---

<p align="center">

Made with Modern C++

If you like this project, consider giving it a ⭐ on GitHub!

</p>
