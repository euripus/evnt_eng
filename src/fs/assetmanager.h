#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "memory_stream.h"
#include <cassert>
#include <ctime>

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
    virtual AssetReadingPriority     getPriority() { return AssetReadingPriority::Middle; }
    virtual std::vector<std::string> getExtensions()                                                  = 0;
    virtual void loadAssets(std::string ext, InputMemoryStream & stream, AssetManager & assetManager) = 0;
};

class AssetManager
{
public:
    AssetManager();
};
}   // namespace evnt

#endif   // ASSETMANAGER_H
