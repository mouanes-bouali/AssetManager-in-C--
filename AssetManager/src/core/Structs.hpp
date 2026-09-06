#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

using AssetID = uint64_t;

// ── Concrete, typed representations of a LOADED asset ──
struct MeshFile {
    AssetID id = 0;
    std::string name;
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
};

struct TextureFile {
    AssetID id = 0;
    std::string name;
    int width = 0;
    int height = 0;
    int channels = 4;
    std::vector<unsigned char> pixelData;
};

struct SoundFile {
    AssetID id = 0;
    std::string name;
    int sampleRate = 44100;
    int channels = 2;
    std::vector<int16_t> samples;
};

struct ScriptFile {
    AssetID id = 0;
    std::string name;
    std::string language;
    std::string sourceCode;
};

// ── One type-safe box that can hold any ONE of the four ──
using AssetData = std::variant<MeshFile, TextureFile, SoundFile, ScriptFile>;

// ── Discovery-only metadata, before anything is loaded ──
struct AssetMetadata {
    AssetID id = 0;
    std::string name;
    std::string filepath;
    std::string extension;
};