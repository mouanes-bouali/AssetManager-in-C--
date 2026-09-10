# AssetManager

**Single-file C++17 library for discovering and loading game assets.**

Scans folders, assigns IDs, loads textures/meshes/sounds/scripts into memory, and caches them. You get raw data — you decide what to do with it.

---

## Quick Start

```cpp
#include "AssetManager.hpp"

auto& am = AssetManager::getInstance();

// 1. Scan folder
am.ReadAssetFile("assets/");

// 2. Load by ID
TextureFile tex;
if (am.LoadFile(1, &tex)) {
    // Use tex.width, tex.height, tex.pixelData
}
```

---

## Supported Types

| Type | Struct | Extensions | Parser |
|------|--------|-----------|--------|
| Texture | `TextureFile` | .png .jpg .jpeg .tga .bmp | stb_image |
| Mesh | `MeshFile` | .obj .fbx .gltf .glb .stl .ply | tinyobjloader (.obj only) |
| Sound | `SoundFile` | .wav | built-in |
| Script | `ScriptFile` | .lua .py .as .js | built-in |

> Only `.obj` and `.wav` have full parsers. Other extensions are recognized but return empty structs.

---

## API

### Lifecycle

```cpp
static AssetManager& getInstance();        // Get singleton
void ReadAssetFile(const std::string& path); // Scan folder, populate registry
```

### Loading

```cpp
template <typename FILE>
bool LoadFile(size_t id, FILE* out);  // Load from disk (cached on success)
```

Returns `true` on success. If already cached, returns instantly.

### Memory Management

```cpp
bool  IsLoaded(size_t id) const;  // Check if cached
void  Unload(size_t id);          // Free one asset
void  UnloadAll();                // Free everything
```

### Registry Queries

```cpp
const std::vector<AssetMetadata>& GetRegistry() const;  // Full catalog
bool FileExistInRegistry(size_t id) const;              // ID exists?
std::string GetPathById(size_t id) const;               // File path or ""
std::string GetExtensionById(size_t id) const;          // Extension or ""
```

---

## Workflow

```
Scan → Load → Use → Unload
  1      2     3      4
```

1. **Scan** — `ReadAssetFile("assets/")` walks the folder. Each file gets a unique ID (starting at 1). Only metadata is stored — no file contents yet.

2. **Load** — `LoadFile(id, &out)` reads and parses the file. Result is cached. Second call with same ID is instant.

3. **Use** — Work with the loaded struct's fields. Upload to GPU, play audio, etc.

4. **Unload** — `Unload(id)` or `UnloadAll()` frees cached memory.

---

## Data Structures

### TextureFile
```cpp
AssetID id;                              // Unique identifier
std::string name;                        // Filename
int width, height;                       // Dimensions
int channels;                            // Always 4 (RGBA)
std::vector<unsigned char> pixelData;    // Size = width * height * 4
```

### MeshFile
```cpp
AssetID id;
std::string name;
std::vector<float> vertices;    // [x, y, z, x, y, z, ...]
std::vector<uint32_t> indices;  // Triangle indices
std::vector<float> normals;     // May be empty
std::vector<float> uvs;         // May be empty
int vertexCount, faceCount;
```

### SoundFile
```cpp
AssetID id;
std::string name;
int sampleRate;                          // e.g. 44100
int channels;                            // 1 or 2
std::vector<int16_t> samples;            // PCM data
```

### ScriptFile
```cpp
AssetID id;
std::string name;
std::string language;                    // "Lua", "Python", etc.
std::string sourceCode;                  // Raw file contents
```

### AssetMetadata (from registry)
```cpp
AssetID id;
std::string name;
std::string filepath;
std::string extension;
```

---

## Finding IDs

IDs start at 1. List everything to find what you need:

```cpp
for (const auto& asset : am.GetRegistry()) {
    std::cout << "[" << asset.id << "] " << asset.name << std::endl;
}
```

Or search by name:

```cpp
AssetID FindID(const AssetManager& am, const std::string& name) {
    for (const auto& a : am.GetRegistry())
        if (a.name == name) return a.id;
    return 0;  // Not found
}
```

---

## Error Handling

**Always check the return value:**

```cpp
MeshFile mesh;
if (!am.LoadFile(id, &mesh)) {
    // Failed: bad ID, wrong type, file missing/corrupt, null pointer
    return;
}
```

**Common failures:**

| Cause | Solution |
|-------|----------|
| ID not in registry | Call `ReadAssetFile()` first |
| Wrong type requested | Match struct to file extension |
| File missing/corrupt | Verify path and file integrity |
| Null pointer passed | Pass valid pointer to stack object |

---

## Thread Safety

**Not thread-safe.** Wrap calls in a mutex:

```cpp
std::mutex mtx;
{
    std::lock_guard<std::mutex> lock(mtx);
    am.LoadFile(id, &mesh);
}
```

Standard pattern: load everything on main thread during loading screen, never during gameplay.

---

## Files

```
AssetManager/
├── AssetManager.hpp      // Main class
├── ILoader.hpp           // Loader interface + implementations
├── Structs.hpp           // All data structures
├── impl.cpp              // Compile this (stb + tinyobj implementations)
├── stb_image.h           // Image decoding
└── tiny_obj_loader.h     // OBJ parsing
```

Add `impl.cpp` to your build. Include `AssetManager.hpp`. Done.

---

## Limitations

- Only `.obj` meshes fully parsed (not .fbx, .gltf)
- Only `.wav` sounds fully parsed (not .mp3, .ogg)
- Textures always converted to RGBA (4 channels)
- Full file loaded into RAM (no streaming)
- Not thread-safe

---

## Extending

To add a new asset type:

1. Add struct to `Structs.hpp`
2. Add to `AssetData` variant
3. Create loader inheriting `ILoader`
4. Register in `CreateLoadChecker()`

Everything else works automatically.