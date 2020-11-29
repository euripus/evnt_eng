#ifndef TEXTURERESOURCE_H
#define TEXTURERESOURCE_H

#include "imagedata.h"
#include "resource.h"

namespace evnt
{
class TextureResource : public Resource
{
public:
    explicit TextureResource(std::string name);
    ~TextureResource() override = default;

    // Resource interface
public:
    bool load() override;
    void release() override;

    static ResourceType                      GetTypeID() { return ResourceType::Texture; }
    static ResourceManager::ResourceRegEntry GetRegEntry();
    // regEntry functions
    static Resource::ResourceSharedPtr LoadTexture(std::string const & name);
    static ResourceSharedPtr           GetDefaultTexture();
    static bool                        WriteTga(Resource const & texture);

protected:
    ImageData m_data;
};
}   // namespace evnt
#endif   // TEXTURERESOURCE_H
