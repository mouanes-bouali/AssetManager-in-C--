class AssetManager {
public:
    static AssetManager& getInstance() {
        static AssetManager instance;
        return instance;
    }

    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;
    //string fetchfolder() ,,, to show contents 
    //<template file>loadfile(string path) or just use an interface and each loadfile in its way
    //bool is fileloaded()
    // unload all()
    // unload(string path)
private:
    AssetManager() = default;
    ~AssetManager() = default;
};