#include "textureresource.h"
#include "../core/core.h"
#include "../log/log.h"

namespace evnt
{
ResourceManager::ResourceRegEntry TextureResource::GetRegEntry()
{
    static ResourceManager::ResourceRegEntry reg;

    reg.res_type_name            = "TextureResource type";
    reg.res_get_default_function = std::bind(TextureResource::GetDefaultTexture);
    reg.res_write_function       = std::bind(TextureResource::WriteTga, std::placeholders::_1);
    reg.res_read_function        = std::bind(TextureResource::LoadTexture, std::placeholders::_1);

    return reg;
}

Resource::ResourceSharedPtr TextureResource::LoadTexture(const std::string & name)
{
    auto tex = std::make_shared<TextureResource>(name);
    auto fs  = Core::instance().getFileSystem();

    auto file = fs.getFile(name);   // exception if not found

    if(file.getNameExt() == ".tga")
    {
        if(ReadTGA(file, tex->m_data))
        {
            tex->m_state = State::state_mem;
            return tex;
        }
    }
    else if(file.getNameExt() == ".bmp")
    {
        if(ReadBMP(file, tex->m_data))
        {
            tex->m_state = State::state_mem;
            return tex;
        }
    }

    std::stringstream ss;
    ss << "Warning. Texture: \"" << name << "\" "
       << "not loaded!" << std::endl;
    Log::Log(Log::warning, ss.str());
    tex->m_state = State::state_invalid;
    return tex;
}

Resource::ResourceSharedPtr TextureResource::GetDefaultTexture()
{
    static auto tex = [] {
        uint8_t texData[] = {128, 192, 255, 255, 128, 192, 255, 255, 255, 192, 128, 255, 255, 192, 128, 255,
                             128, 192, 255, 255, 128, 192, 255, 255, 255, 192, 128, 255, 255, 192, 128, 255,
                             255, 192, 128, 255, 255, 192, 128, 255, 128, 192, 255, 255, 128, 192, 255, 255,
                             255, 192, 128, 255, 255, 192, 128, 255, 128, 192, 255, 255, 128, 192, 255, 255};

        auto ptr           = std::make_shared<TextureResource>("default_texture");
        ptr->m_data.height = 4;
        ptr->m_data.width  = 4;
        ptr->m_data.type   = ImageData::PixelType::pt_rgba;

        ptr->m_data.data = std::make_unique<uint8_t[]>(4 * 4 * 4);
        std::memcpy(ptr->m_data.data.get(), texData, 4 * 4 * 4);

        return ptr;
    }();

    return tex;
}

bool TextureResource::WriteTga(Resource const & texture)
{
    auto fs = Core::instance().getFileSystem();

    auto & tex      = static_cast<TextureResource const &>(texture);
    auto   out_file = WriteTGA(tex.getName(), tex.m_data);

    return fs.writeFile({}, &out_file);
}

TextureResource::TextureResource(std::string name) :
    Resource(ResourceType::Texture, std::move(name)), m_data{}
{}

bool TextureResource::load()
{
    if(m_state == Resource::State::state_mem)
        return true;

    if(!Core::instance().getFileSystem().isExist(m_name))
    {
        m_state = Resource::State::state_invalid;
        return false;
    }

    auto              new_tex = LoadTexture(m_name);
    TextureResource * tex_ptr = static_cast<TextureResource *>(new_tex.get());

    if(tex_ptr->m_state == Resource::State::state_invalid)
    {
        m_state = Resource::State::state_invalid;
        return false;
    }

    std::swap(m_state, tex_ptr->m_state);
    std::swap(m_type, tex_ptr->m_type);
    std::swap(m_name, tex_ptr->m_name);
    std::swap(m_data, tex_ptr->m_data);

    return true;
}

void TextureResource::release()
{
    m_state = Resource::State::state_path;
    ImageData temp;
    std::swap(m_data, temp);
}
}   // namespace evnt
