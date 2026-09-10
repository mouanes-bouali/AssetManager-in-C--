#include "ILoader.hpp"
#include "Structs.hpp"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>



class AssetManager {
public:

  static AssetManager &getInstance() {
    static AssetManager instance;
    return instance;
  }

  AssetManager(const AssetManager &) = delete;
  AssetManager &operator=(const AssetManager &) = delete;
  
  void ReadAssetFile(const std::string &path) {
    for (const auto & entry :
         std::filesystem::recursive_directory_iterator(path)) {

      if (!entry.is_regular_file()) {
        continue;
      }
      std::filesystem::path filepath = entry.path();
      std::string extention = filepath.extension().string();
      for (const auto &load : loadchecker_) {
        if (load->CanBeLoaded(extention)) {
          AssetMetadata file;
          file.name = filepath.filename().string();
          file.id = ID_;
          ID_++;
          file.extension = extention;
          file.filepath = filepath.string();
          assetsRegistry_.push_back(file);
          registryIndex_.emplace(file.id, assetsRegistry_.size() - 1);
          break;
        }
      }
    }
  };
  bool FileExistInRegistry(size_t id) const {
    return registryIndex_.find(id) != registryIndex_.end();
  };
  std::string GetPathById(size_t id) const {
    auto it = registryIndex_.find(id);
    if (it != registryIndex_.end()) {
      return assetsRegistry_[it->second].filepath;
    }
    return "";
  };
  std::string GetExtentionById(size_t id) const {
    auto it = registryIndex_.find(id);
    if (it != registryIndex_.end()) {
      return assetsRegistry_[it->second].extension;
    }
    return "";
  };

  template <typename FILE>
  bool LoadFile(size_t id, FILE* inputfileptr) {
    if (!inputfileptr)
      return false;
    auto cacheIt = loadedAssets_.find(id);
    if (cacheIt != loadedAssets_.end()) {
      if (const auto* cached = std::get_if<FILE>(&cacheIt->second)) {
        *inputfileptr = *cached;
        return true;
      }
      return false;
    }
    if (!FileExistInRegistry(id))
      return false;
    std::string filepath = GetPathById(id);
    std::string extention = GetExtentionById(id);
    if (filepath.empty()) {
      return false;
    }
    for (const auto &loader : loadchecker_) {
      if (loader->CanBeLoaded(extention)) {
        AssetData asset = loader->Load(id, filepath);
        if (const auto& fileptr = std::get_if<FILE>(&asset)) {
          *inputfileptr = *fileptr;
          loadedAssets_.emplace(id, asset);
          return true;
        }
      }
    }
    return false;
  };

  bool IsLoaded(size_t id) const {
    return loadedAssets_.find(id) != loadedAssets_.end();
  };
  void Unload(size_t id) {
    loadedAssets_.erase(id);
  };
  void UnloadAll() {
    loadedAssets_.clear();
  };
  
  
  // bool is fileloaded()
  //  unload all()
  //  unload(string path)
  const std::vector<AssetMetadata>& GetRegistry() const { return assetsRegistry_; };

private:
  void CreateLoadChecker() {
    loadchecker_.push_back(std::make_unique<TextureLoader>());
    loadchecker_.push_back(std::make_unique<MeshLoader>());
    loadchecker_.push_back(std::make_unique<SoundLoader>());
    loadchecker_.push_back(std::make_unique<ScriptLoader>());
  };
  std::vector<AssetMetadata> assetsRegistry_;
  std::unordered_map<AssetID, size_t> registryIndex_;
  std::vector<std::unique_ptr<ILoader>> loadchecker_;
  std::unordered_map<AssetID, AssetData> loadedAssets_;
  size_t ID_ = 1;
  AssetManager(){
    CreateLoadChecker();
  };
  ~AssetManager() = default;
};
