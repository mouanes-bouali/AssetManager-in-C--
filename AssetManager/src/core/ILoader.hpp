#pragma once

#include "Structs.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

// ──────────────────────────────────────────────────────────────
// STB_IMAGE for texture loading
// ──────────────────────────────────────────────────────────────
#include "stb_image.h"

// ──────────────────────────────────────────────────────────────
// TINYOBJLOADER for mesh loading
// ──────────────────────────────────────────────────────────────
#include "tiny_obj_loader.h"

// ──────────────────────────────────────────────────────────────
// ILoader Interface
// ──────────────────────────────────────────────────────────────
class ILoader {
public:
    virtual bool CanBeLoaded(const std::string &extension) const = 0;
    virtual AssetData Load(AssetID id, const std::string &path) const = 0;
    virtual ~ILoader() = default;
};

// ──────────────────────────────────────────────────────────────
// ScriptLoader — Reads raw text files
// ──────────────────────────────────────────────────────────────
class ScriptLoader : public ILoader {
    std::vector<std::string> extensions_ = {".lua", ".py", ".as", ".js"};

    std::string DetectLanguage(const std::string &ext) const {
        if (ext == ".lua") return "Lua";
        if (ext == ".py")  return "Python";
        if (ext == ".as")  return "AngelScript";
        if (ext == ".js")  return "JavaScript";
        return "Unknown";
    }

public:
    bool CanBeLoaded(const std::string &extension) const override {
        return std::find(extensions_.begin(), extensions_.end(), extension) != extensions_.end();
    }

    AssetData Load(AssetID id, const std::string &path) const override {
        std::ifstream file(path);
        if (!file.is_open()) {
           
            ScriptFile script;
            script.id = id;
            script.name = std::filesystem::path(path).filename().string();
            return script;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        ScriptFile script;
        script.id = id;
        script.name = std::filesystem::path(path).filename().string();
        script.language = DetectLanguage(std::filesystem::path(path).extension().string());
        script.sourceCode = buffer.str();

       
        return script;
    }
};

// ──────────────────────────────────────────────────────────────
// TextureLoader — Decodes PNG, JPG, TGA, BMP with stb_image
// ──────────────────────────────────────────────────────────────
class TextureLoader : public ILoader {
    std::vector<std::string> extensions_ = {".png", ".jpg", ".jpeg", ".tga", ".bmp"};

public:
    bool CanBeLoaded(const std::string &extension) const override {
        return std::find(extensions_.begin(), extensions_.end(), extension) != extensions_.end();
    }

    AssetData Load(AssetID id, const std::string &path) const override {
        TextureFile tex;
        tex.id = id;
        tex.name = std::filesystem::path(path).filename().string();

        int width, height, channels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4); // Force RGBA

        if (data) {
            tex.width = width;
            tex.height = height;
            tex.channels = 4;
            tex.pixelData.assign(data, data + (width * height * 4));
            stbi_image_free(data);

           
        } else {
        }

        return tex;
    }
};

// ──────────────────────────────────────────────────────────────
// MeshLoader — Parses OBJ, FBX, GLTF, etc. with tinyobjloader
// ──────────────────────────────────────────────────────────────
class MeshLoader : public ILoader {
    std::vector<std::string> extensions_ = {".obj", ".fbx", ".gltf", ".glb", ".stl", ".ply"};

public:
    bool CanBeLoaded(const std::string &extension) const override {
        return std::find(extensions_.begin(), extensions_.end(), extension) != extensions_.end();
    }

    AssetData Load(AssetID id, const std::string &path) const override {
        MeshFile mesh;
        mesh.id = id;
        mesh.name = std::filesystem::path(path).filename().string();

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
            
            return mesh;
        }

        if (!warn.empty()) {
            std::cout << "⚠️ " << warn << std::endl;
        }

        // ─── Extract vertices, normals, UVs, indices ───────
        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                // Position (x, y, z)
                mesh.vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
                mesh.vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
                mesh.vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

                // Normals (if available)
                if (!attrib.normals.empty() && index.normal_index >= 0) {
                    mesh.normals.push_back(attrib.normals[3 * index.normal_index + 0]);
                    mesh.normals.push_back(attrib.normals[3 * index.normal_index + 1]);
                    mesh.normals.push_back(attrib.normals[3 * index.normal_index + 2]);
                }

                // UVs (if available)
                if (!attrib.texcoords.empty() && index.texcoord_index >= 0) {
                    mesh.uvs.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                    mesh.uvs.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
                }

                mesh.indices.push_back(static_cast<uint32_t>(mesh.indices.size()));
            }
        }

        mesh.vertexCount = static_cast<int>(mesh.vertices.size() / 3);
        mesh.faceCount = static_cast<int>(mesh.indices.size() / 3);

        std::cout << "🔧 Loaded Mesh: " << mesh.name
                  << " (vertices: " << mesh.vertexCount
                  << ", faces: " << mesh.faceCount << ")"
                  << std::endl;

        return mesh;
    }
};

// ──────────────────────────────────────────────────────────────
// SoundLoader — Parses WAV files (real implementation)
// ──────────────────────────────────────────────────────────────
class SoundLoader : public ILoader {
    std::vector<std::string> extensions_ = {".wav"};

public:
    bool CanBeLoaded(const std::string &extension) const override {
        return std::find(extensions_.begin(), extensions_.end(), extension) != extensions_.end();
    }

    AssetData Load(AssetID id, const std::string &path) const override {
        SoundFile sound;
        sound.id = id;
        sound.name = std::filesystem::path(path).filename().string();

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            std::cout << "❌ Failed to open sound: " << path << std::endl;
            return sound;
        }

        char header[44];
        file.read(header, 44);

        if (std::string(header, 4) != "RIFF") {
            std::cout << "❌ Not a valid WAV file: " << path << std::endl;
            return sound;
        }

        sound.sampleRate = *reinterpret_cast<int32_t*>(&header[24]);
        sound.channels   = *reinterpret_cast<int16_t*>(&header[22]);

        std::vector<char> rawBytes((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());

        sound.samples.assign(
            reinterpret_cast<int16_t*>(rawBytes.data()),
            reinterpret_cast<int16_t*>(rawBytes.data() + (rawBytes.size() / 2) * 2)
        );

        float duration = static_cast<float>(sound.samples.size()) / sound.sampleRate / sound.channels;

        std::cout << "🔊 Loaded Sound: " << sound.name
                  << " (" << duration << "s, " << sound.sampleRate << "Hz)"
                  << std::endl;

        return sound;
    }
};