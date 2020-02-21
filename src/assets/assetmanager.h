#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "../fs/file.h"
#include <unordered_map>

namespace evnt
{
class AssetManager;

enum class AssetReadingPriority
{
    First,
    Middle,
    Last
};

class IAssetReader
{
public:
    virtual AssetReadingPriority getPriority() { return AssetReadingPriority::Middle; }
    virtual std::string          getExtension()                                                       = 0;
    virtual void                 loadAssets(InputMemoryStream & stream, AssetManager & asset_manager) = 0;
};

class IAsset
{
public:
    virtual ~IAsset() {}

    virtual std::string getPath() const          = 0;
    virtual void        setStream(InFile stream) = 0;
    virtual void        load()                   = 0;
    virtual void        unload()                 = 0;
    virtual bool        isReady()                = 0;
    virtual bool        isValid()                = 0;

    virtual IAsset * createCopy(std::string new_path) = 0;
    virtual void     exportAsset()                    = 0;

    bool operator==(IAsset & asset) const { return getPath() == asset.getPath(); }
};

// Resources owner
class AssetManager
{
public:
    using AssetPtr       = std::unique_ptr<IAsset>;
    using AssetReaderPtr = std::unique_ptr<IAssetReader>;

    AssetManager();

    IAsset *       locateAsset(std::string path);
    IAssetReader * locateAssetReader(std::string ext);

    void registerReader(AssetReaderPtr reader);   // Pointer will be owned by the AssetManager instance
    void addAsset(AssetPtr asset);                // Pointer will be owned by the AssetManager instance
    void deleteAsset(IAsset * asset);

    template<typename T>
    void setDefaultAsset(std::unique_ptr<T> asset)   // Pointer will be owned by the AssetManager instance
    {
        m_defaultAssets.insert({typeid(T).name(), std::move(asset)});
    }

    template<typename T>
    T & getDefaultAsset()
    {
        return dynamic_cast<T &>(*m_defaultAssets.at(typeid(T).name()).get());
    }

private:
    std::unordered_map<std::string, AssetPtr>       m_assets;
    std::unordered_map<std::string, AssetReaderPtr> m_readers;
    std::unordered_map<std::string, AssetPtr>       m_defaultAssets;
};
}   // namespace evnt

#endif   // ASSETMANAGER_H
