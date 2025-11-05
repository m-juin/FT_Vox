# **FT_Vox**

### Procedural Minecraft-like World Generator (42 Project)

[![Made with C++17](https://img.shields.io/badge/Made%20with-C%2B%2B17-blue.svg?style=for-the-badge&logo=c%2B%2B)]()
[![Graphics API: Vulkan](https://img.shields.io/badge/Graphics-Vulkan-red.svg?style=for-the-badge&logo=vulkan)]()
[![Platform](https://img.shields.io/badge/Platform-Linux-green.svg?style=for-the-badge&logo=linux)]()


---

## Introduction

**FT_Vox** is a voxel world generator inspired by **Minecraft**, developed from scratch as part of the **42 post common core**.  
Unlike a traditional game clone, FT_Vox focuses exclusively on **procedural generation**, **engine design**, and **rendering performance**.

This project is written in **C++17**, powered by **Vulkan**, and uses a fully custom stack — from the rendering pipeline to the UI and tools.  
No external engine or framework was used: every system, from **noise generation** to **JSON parsing**, was handcrafted.

---

## Core Objectives

- Build a **real-time voxel renderer** using Vulkan’s modern GPU pipeline  
- Recreate **Minecraft’s terrain generation**: seeded, biome-based, and continuous  
- Implement a **stable 150+ block render distance**  
- Optimize memory, chunk generation, and rendering pipelines  

---

## Additionnal Objectives

- Design **custom tools** for generation, debugging, and visualization  
- Develop an **entire UI layer** from scratch using GLFW  

---

## Main Features

### Procedural World Generation
- Fully **seeded** terrain generation (reproducible worlds)
- Supports **all base Minecraft biomes**:
  - Plains, Desert, Forest, Jungle, Mountains, Snow, Ocean, and more  
- Procedural **heightmaps** built from multi-octave **Perlin noise**
- **Spline-based transitions** 

### Rendering & Performance
- Render distance: **≥ 150 blocks**
- Optimized **frustum culling** and chunk batching (Ongoing)
- Threaded **chunk generation** to maintain stable framerates
- Texture atlas management using **stb_image**

### Debug & Development Tools
- Custom **in-game debug UI**
  - Display biome maps
  - Visualize Perlin noise layers
  - Inspect terrain generation parameters
  - Change seed **at runtime**

### World Management Menu
- UI for:
  - Creating or deleting worlds
  - Setting custom seeds and name before generation
- Persistent data storage through **Save file**

### Custom Tools & Libraries
- **Spline Designer**
  - Used to define elevation curves
  - Exports custom spline data to be used in terrain height calculation
- **Custom JSON Library**
  - Written from scratch in C++
  - Lightweight, header-only implementation
  - Handle generation rules.

### Visual & Rendering Features
- Font rendering via **FreeType**
- Vulkan texture sampling and mipmapping
- Support for **texture pack switching** (Ongoing)

### Gameplay Features
- Creative mode movement (free-fly camera, no gravity)
- Full 6-axis control with smooth mouse rotation
- Configurable input system (GLFW) (OnGoing)

---

## Rendering Pipeline (Vulkan)

1. **Instance & Physical Device Selection**  
   → Query supported extensions, surface formats, and queues.  
2. **Logical Device & Swapchain Setup**  
   → Double buffering, and command pool creation.  
3. **Mesh Generation (Chunks)**  
   → Each chunk generates its mesh from voxel data on the CPU.  
4. **Uniform & Descriptor Management**  
   → View/projection matrices, biome data.  
5. **Graphics Pipeline**  
    → Vertex + Fragment shaders for terrain rendering.   
    → Transparency for water and leaves.   
    → Vertex + Fragment shaders for terrain rendering.   
6. **Frame Rendering Loop**  
   → Command buffer submission, synchronization, and presentation.  

---

## To Do List

Current state of the project.

| Name | Description | Status |
|------|-------------|--------|
|**Skybox**| Implementation of a cubemap skybox. |<span style="color:green">**Completed**</span>| 
|**Biomes Decoration**| Implement some biomes decoration such as Tree, lacs, flower ...|<span style="color:yellow">**Started**</span>|
|**Better generation values**| Improve the current generation value for a better result.  |<span style="color:yellow">**Started**</span>|
|**Camera Frustrum**| Display chunck only if they are on the camera frustrum. |<span style="color:red">**Not Started**</span>| 
|**Caves**| Implementation of a caving system. |<span style="color:red">**Not Started**</span>|
|**Chunck Modification**| Allowing user to add / break blocks. |<span style="color:red">**Not Started**</span>|
|**Saving System**| Allowing a saving of generated chuncks with their blocks modifications. |<span style="color:red">**Not Started**</span>|
|**Day / Night system**| Creation of a day and night cycle with visibles sun and moon.  |<span style="color:red">**Not Started**</span>|

---

## Screenshots

Some screenshots of in game content. Still WIP:

<img width="3836" height="2053" alt="Capture d&#39;écran 2025-11-05 151307" src="https://github.com/user-attachments/assets/62ebb4f9-871f-49dc-884a-ee747698fa0d" />
<img width="3834" height="2055" alt="Capture d&#39;écran 2025-11-05 151223" src="https://github.com/user-attachments/assets/634231d5-3df6-4616-b431-289110d7fce1" />
<img width="3836" height="2055" alt="Capture d&#39;écran 2025-11-05 151055" src="https://github.com/user-attachments/assets/a7e66198-284d-487e-b54a-80819f27007e" />


---

## Controls

| Action | Key |
|--------|-----|
| Move forward / backward | W / S |
| Move left / right | A / D |
| Move up / down | Space / Left Shift |
| Look around | Mouse |
| Open debug interfaces | F3 / F4 |
| Quit | alt+F4 |

---

## Build & Run

### Requirements
- Vulkan SDK (1.1+)  
- GLFW  
- C++17 compiler clang++  
- GNU Make  
- Debian based distribution  

### Build
```bash
git clone https://github.com/m-juin/NeoVox.git
cd FT_Vox
make -j && ./Ft_Vox
