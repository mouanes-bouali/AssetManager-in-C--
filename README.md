# AssetManager

Single-file C++17 library for discovering and loading game assets. Scan folders, load textures/meshes/sounds/scripts, and cache them in memory.

---

## Quick Start

```cpp
#include "AssetManager.hpp"

auto& am = AssetManager::getInstance();

// 1. Scan folder
am.ReadAssetFile("assets/");

// 2. List what was found
for (const auto& asset : am.GetRegistry())
    std::cout << "[" << asset.id << "] " << asset.name << std::endl;

// 3. Load by ID
TextureFile tex;
if (am.LoadFile(1, &tex)) {
    std::cout << tex.width << "x" << tex.height << std::endl;
}
```

---

## What It Does

| Feature | |
|---------|---|
| Scans folders recursively | Assigns unique IDs starting at 1 |
| Caches loaded assets | Loads each file only once |
| Returns raw data | You upload to GPU, play audio, etc. |

## What It Does NOT Do

| | |
|---|---|
| No rendering | No audio playback |
| No GPU uploads | No script execution |
| No file modification | |

You load the raw data. You decide what to do with it.

---

## Supported Types

| Type | Struct | Extensions | Parser |
|------|--------|-----------|--------|
| Texture | `TextureFile` | .png .jpg .jpeg .tga .bmp | stb_image |
| Mesh | `MeshFile` | .obj .fbx .gltf .glb .stl .ply | tinyobjloader |
| Sound | `SoundFile` | .wav | built-in |
| Script | `ScriptFile` | .lua .py .as .js | built-in |

> Only `.obj` and `.wav` have full parsers. Other extensions are recognized but return empty structs.

---

## Installation

Copy the folder into your project. Add `impl.cpp` to your build. Include the header. Done.

```
your-project/
└── AssetManager/
    ├── AssetManager.hpp
    ├── ILoader.hpp
    ├── Structs.hpp
    ├── impl.cpp          <-- compile this
    ├── stb_image.h
    └── tiny_obj_loader.h
```

```cpp
#include "AssetManager/AssetManager.hpp"
```

No CMake. No package manager. No setup.

---

## Documentation

| Document | |
|----------|---|
| [API.md](API.md) | Complete reference with examples |
| [Quick Start](#quick-start) | Copy-paste example above |

---

## API Overview

```cpp
// Lifecycle
static AssetManager& getInstance();
void ReadAssetFile(const std::string& path);

// Loading
template <typename FILE>
bool LoadFile(size_t id, FILE* out);

// Memory
bool IsLoaded(size_t id) const;
void Unload(size_t id);
void UnloadAll();

// Queries
const std::vector<AssetMetadata>& GetRegistry() const;
bool FileExistInRegistry(size_t id) const;
std::string GetPathById(size_t id) const;
std::string GetExtensionById(size_t id) const;
```

Full documentation with examples: [API.md](API.md)

---

## Workflow

```
Scan → Load → Use → Unload
```

1. **Scan** — `ReadAssetFile("assets/")` walks the folder, assigns IDs, stores metadata
2. **Load** — `LoadFile(id, &out)` reads and parses the file, caches the result
3. **Use** — Access struct fields: pixel data, vertices, samples, source code
4. **Unload** — `Unload(id)` or `UnloadAll()` frees cached memory

---

## Error Handling

Every `LoadFile` returns `bool`. Always check:

```cpp
MeshFile mesh;
if (!am.LoadFile(id, &mesh)) {
    // Failed — check ID, file path, type match
    return;
}
```

---

## Thread Safety

Not thread-safe. Wrap calls in a mutex if needed:

```cpp
std::lock_guard<std::mutex> lock(mtx);
am.LoadFile(id, &mesh);
```

---

## Requirements

- C++17 compiler
- No external dependencies (stb_image and tiny_obj_loader included)

---

## License

stb_image — Public domain (Sean Barrett)
tiny_obj_loader — MIT (Syoyo Fujita)
AssetManager — Public domain