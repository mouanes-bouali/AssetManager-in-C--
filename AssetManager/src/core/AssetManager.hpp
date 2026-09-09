#include "ILoader.hpp"
#include "Structs.hpp"
#include "json.hpp"
#include <cstddef>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

using json = nlohmann::json;

enum CurrentMode { EDIT, RUNTIME };

class AssetManager {
public:
  CurrentMode currentmode = EDIT;
  void CreateLoadChecker() {
    loadchecker_.push_back(std::make_unique<TextureLoader>());
    loadchecker_.push_back(std::make_unique<MeshLoader>());
    loadchecker_.push_back(std::make_unique<SoundLoader>());
    loadchecker_.push_back(std::make_unique<ScriptLoader>());
  };
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
        }
      }
    }
  };
  bool FileExistInRegistry(size_t id) {
    for (const auto &file : assetsRegistry_) {
      if (file.id == id) {
        return true;
      }
    }
    return false;
  };
  std::string GetPathById(size_t id) {
    for (const auto &file : assetsRegistry_) {
      if (file.id == id) {

        return file.filepath;
      }
    }
  };
  std::string GetExtentionById(size_t id) {
    for (const auto &file : assetsRegistry_) {
      if (file.id == id) {

        return file.extension;
      }
    }
  };

  template <typename FILE>
  void LoadFile(size_t id,FILE* inputfileptr) {
    if (!FileExistInRegistry(id))
      return;
    std::string filepath = GetPathById(id);
    std::string extention = GetExtentionById(id);
    if (filepath.empty()) {
      return;
    }
    for (const auto &loader : loadchecker_) {
      if (loader->CanBeLoaded(extention)) {
        AssetData asset = loader->Load(id, filepath);
        if(const auto& fileptr = std::get_if<FILE>(&asset)) {
          *inputfileptr=*fileptr;
    
    }
      }

    }
    
    return ;
  };
  
  
  // bool is fileloaded()
  //  unload all()
  //  unload(string path)
  std::vector<AssetMetadata> GetRegistry() { return assetsRegistry_; };

private:
  std::vector<AssetMetadata> assetsRegistry_;
  std::vector<std::unique_ptr<ILoader>> loadchecker_;
  size_t ID_ = 0;
  std::string assetsFilePqth_;
  AssetManager(){
    CreateLoadChecker();
    
  };
  ~AssetManager() = default;
};
