#pragma once

#include "Structs.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// ── ILoader: the port. WHAT a loader does, not HOW. ──
class ILoader {
public:
  virtual bool CanBeLoaded(const std::string &extension) const = 0;
  virtual AssetData Load(AssetID id, const std::string &path) const = 0;
  virtual ~ILoader() = default;
};

// ── ScriptLoader: REAL, fully working. Just reads raw text. ──
class ScriptLoader : public ILoader {
  std::vector<std::string> extensions_ = {".lua", ".py", ".as", ".js"};

  std::string DetectLanguage(const std::string &ext) const {
    if (ext == ".lua")
      return "Lua";
    if (ext == ".py")
      return "Python";
    if (ext == ".as")
      return "AngelScript";
    if (ext == ".js")
      return "JavaScript";
    return "Unknown";
  }

public:
  bool CanBeLoaded(const std::string &extension) const override {
    return std::find(extensions_.begin(), extensions_.end(), extension) !=
           extensions_.end();
  }

  AssetData Load(AssetID id, const std::string &path) const override {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();

    ScriptFile script;
    script.id = id;
    script.name = std::filesystem::path(path).filename().string();
    script.language =
        DetectLanguage(std::filesystem::path(path).extension().string());
    script.sourceCode = buffer.str();
    return script;
  }
};

// ── TextureLoader: extension-matching real, pixel decode is a marked
// placeholder ──
class TextureLoader : public ILoader {
  std::vector<std::string> extensions_ = {".png", ".jpg", ".jpeg", ".tga",
                                          ".bmp"};

public:
  bool CanBeLoaded(const std::string &extension) const override {
    return std::find(extensions_.begin(), extensions_.end(), extension) !=
           extensions_.end();
  }

  AssetData Load(AssetID id, const std::string &path) const override {
    TextureFile tex;
    tex.id = id;
    tex.name = std::filesystem::path(path).filename().string();

    // PLACEHOLDER: real decode needs stb_image.h:
    //   unsigned char* pixels = stbi_load(path.c_str(), &tex.width,
    //   &tex.height, &ch, 0);
    tex.width = 512;
    tex.height = 512;
    tex.channels = 4;
    return tex;
  }
};

// ── MeshLoader: same honesty, real parsing needs tinyobjloader ──
class MeshLoader : public ILoader {
  std::vector<std::string> extensions_ = {".obj", ".fbx", ".gltf",
                                          ".glb", ".stl", ".ply"};

public:
  bool CanBeLoaded(const std::string &extension) const override {
    return std::find(extensions_.begin(), extensions_.end(), extension) !=
           extensions_.end();
  }

  AssetData Load(AssetID id, const std::string &path) const override {
    MeshFile mesh;
    mesh.id = id;
    mesh.name = std::filesystem::path(path).filename().string();
    // PLACEHOLDER: real parsing needs tinyobjloader or similar
    return mesh;
  }
};

// ── SoundLoader: WAV header parsing is genuinely simple, done for real ──
class SoundLoader : public ILoader {
  std::vector<std::string> extensions_ = {".wav"};

public:
  bool CanBeLoaded(const std::string &extension) const override {
    return std::find(extensions_.begin(), extensions_.end(), extension) !=
           extensions_.end();
  }

  AssetData Load(AssetID id, const std::string &path) const override {
    SoundFile sound;
    sound.id = id;
    sound.name = std::filesystem::path(path).filename().string();

    std::ifstream file(path, std::ios::binary);
    if (file) {
      char header[44];
      file.read(header, 44);
      sound.sampleRate = *reinterpret_cast<int32_t *>(&header[24]);
      sound.channels = *reinterpret_cast<int16_t *>(&header[22]);

      std::vector<char> rawBytes((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
      sound.samples.assign(reinterpret_cast<int16_t *>(rawBytes.data()),
                           reinterpret_cast<int16_t *>(
                               rawBytes.data() + (rawBytes.size() / 2) * 2));
    }
    return sound;
  }
};
