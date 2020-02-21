#include "assetmanager.h"

namespace evnt
{
IAsset * AssetManager::locateAsset(std::string path)
{
    auto asset = m_assets.find(path);
    return asset == m_assets.end() ? nullptr : asset->second.get();
}

IAssetReader * AssetManager::locateAssetReader(std::string ext)
{
    auto asset_reader = m_readers.find(ext);
    return asset_reader == m_readers.end() ? nullptr : asset_reader->second.get();
}

void AssetManager::registerReader(AssetReaderPtr reader)
{
    std::string ext = reader->getExtension();
    m_readers.insert_or_assign(ext, std::move(reader));
}

void AssetManager::addAsset(AssetPtr asset)
{
    m_assets.insert_or_assign(asset->getPath(), std::move(asset));
}

void AssetManager::deleteAsset(IAsset * asset)
{
    m_assets.erase(asset->getPath());
}
}   // namespace evnt
