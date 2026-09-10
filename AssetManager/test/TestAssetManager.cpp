#include "catch.hpp"
#include "../src/core/AssetManager.hpp"

TEST_CASE("ReadAssetFile populates registry from a folder")
{
    auto& manager = AssetManager::getInstance();
    manager.ReadAssetFile("C:/Users/ACER/Desktop/C++ Files/AssetManager in c++/AssetManager/test/Assets");

    auto registry = manager.GetRegistry();
    REQUIRE(!registry.empty());

    bool foundJpg = false;
    for (const auto& entry : registry) {
        if (entry.extension == ".jpg") {
            foundJpg = true;
            break;
        }
    }
    REQUIRE(foundJpg);
}

TEST_CASE("LoadFile loads a texture into memory")
{
    auto& manager = AssetManager::getInstance();
    manager.ReadAssetFile("C:/Users/ACER/Desktop/C++ Files/AssetManager in c++/AssetManager/test/Assets");

    auto registry = manager.GetRegistry();
    AssetID jpgId = 0;
    bool found = false;
    for (const auto& entry : registry) {
        if (entry.extension == ".jpg") {
            jpgId = entry.id;
            found = true;
            break;
        }
    }
    REQUIRE(found);

    TextureFile texture;
    manager.LoadFile(jpgId, &texture);

    std::cout << "\n=== Loaded Texture ===" << std::endl;
    std::cout << "Name:     " << texture.name << std::endl;
    std::cout << "ID:       " << texture.id << std::endl;
    std::cout << "Width:    " << texture.width << std::endl;
    std::cout << "Height:   " << texture.height << std::endl;
    std::cout << "Channels: " << texture.channels << std::endl;
    std::cout << "Pixels:   " << texture.pixelData.size() << " bytes" << std::endl;

    REQUIRE(texture.width > 0);
    REQUIRE(texture.height > 0);
    REQUIRE_FALSE(texture.pixelData.empty());
}

TEST_CASE("LoadFile(Mesh) loads a mesh into memory")
{
    auto& manager = AssetManager::getInstance();
    manager.ReadAssetFile("C:/Users/ACER/Desktop/C++ Files/AssetManager in c++/AssetManager/test/Assets");

    auto registry = manager.GetRegistry();
    REQUIRE_FALSE(registry.empty());

    AssetID meshId = 0;
    bool found = false;
    for (const auto& asset : registry) {
        if (asset.extension == ".obj" || asset.extension == ".fbx") {
            meshId = asset.id;
            found = true;
            break;
        }
    }

    if (!found) {
        SUCCEED("No mesh file (.obj/.fbx) found in test Assets folder - skipping mesh test");
        return;
    }

    MeshFile mesh;
    manager.LoadFile(meshId, &mesh);

    std::cout << "\n=== Loaded Mesh ===" << std::endl;
    std::cout << "Name:         " << mesh.name << std::endl;
    std::cout << "ID:           " << mesh.id << std::endl;
    std::cout << "Vertices:     " << mesh.vertices.size() << " floats ("
              << mesh.vertices.size() / 3 << " vertices)" << std::endl;
    std::cout << "Indices:      " << mesh.indices.size() << " ("
              << mesh.indices.size() / 3 << " triangles)" << std::endl;
    std::cout << "Normals:      " << mesh.normals.size() / 3 << std::endl;
    std::cout << "UVs:          " << mesh.uvs.size() / 2 << std::endl;

    std::cout << "\nFirst 5 vertices:" << std::endl;
    for (size_t i = 0; i < mesh.vertices.size() && i < 15; i += 3) {
        std::cout << "  (" << mesh.vertices[i]
                  << ", " << mesh.vertices[i + 1]
                  << ", " << mesh.vertices[i + 2] << ")" << std::endl;
    }

    REQUIRE_FALSE(mesh.vertices.empty());
    REQUIRE_FALSE(mesh.indices.empty());
    REQUIRE(mesh.vertices.size() % 3 == 0);
    REQUIRE(mesh.indices.size() % 3 == 0);
}
