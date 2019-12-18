#include "texturebase.h"
#include "../../core/exception.h"
#include "./interface/idevicecontext.h"

namespace evnt
{
void ValidateTextureDesc(const TextureDesc & tex_desc)
{
#define LOG_TEXTURE_ERROR_AND_THROW(...)                                   \
    do                                                                     \
    {                                                                      \
        std::ostringstream ss;                                             \
        Log::stream_print(ss, ##__VA_ARGS__);                              \
        std::string msg = ss.str();                                        \
        ss.str({});                                                        \
        ss << "Texture \"" << tex_desc.name << "\": " << msg << std::endl; \
        Log::Log(Log::error, ss.str());                                    \
        EV_EXCEPT(ss.str());                                               \
    } while(false)

    if(tex_desc.type == RESOURCE_DIM_UNDEFINED)
    {
        LOG_TEXTURE_ERROR_AND_THROW("Resource dimension is undefined");
    }

    if(!(tex_desc.type >= RESOURCE_DIM_TEX_1D && tex_desc.type <= RESOURCE_DIM_TEX_CUBE_ARRAY))
    {
        LOG_TEXTURE_ERROR_AND_THROW("Unexpected resource dimension");
    }

    if(tex_desc.width == 0)
    {
        LOG_TEXTURE_ERROR_AND_THROW("Texture width cannot be zero");
    }

    // Perform some parameter correctness check
    if(tex_desc.type == RESOURCE_DIM_TEX_1D || tex_desc.type == RESOURCE_DIM_TEX_1D_ARRAY)
    {
        if(tex_desc.height != 1)
            LOG_TEXTURE_ERROR_AND_THROW("Height (", tex_desc.height,
                                        ") of Texture 1D/Texture 1D Array must be 1");
    }
    else
    {
        if(tex_desc.height == 0)
            LOG_TEXTURE_ERROR_AND_THROW("Texture height cannot be zero");
    }

    if(tex_desc.type == RESOURCE_DIM_TEX_3D && tex_desc.depth == 0)
    {
        LOG_TEXTURE_ERROR_AND_THROW("3D texture depth cannot be zero");
    }

    if(tex_desc.type == RESOURCE_DIM_TEX_1D || tex_desc.type == RESOURCE_DIM_TEX_2D)
    {
        if(tex_desc.array_size != 1)
            LOG_TEXTURE_ERROR_AND_THROW(
                "Texture 1D/2D must have one array slice (", tex_desc.array_size,
                " provided). Use Texture 1D/2D array if you need more than one slice.");
    }

    if(tex_desc.type == RESOURCE_DIM_TEX_CUBE || tex_desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        if(tex_desc.width != tex_desc.height)
            LOG_TEXTURE_ERROR_AND_THROW("For cube map textures, texture width (", tex_desc.width,
                                        " provided) must match texture height (", tex_desc.height,
                                        " provided)");

        if(tex_desc.array_size < 6)
            LOG_TEXTURE_ERROR_AND_THROW("Texture cube/cube array must have at least 6 slices (",
                                        tex_desc.array_size, " provided).");
    }

    uint32_t max_dim = 0;
    if(tex_desc.type == RESOURCE_DIM_TEX_1D || tex_desc.type == RESOURCE_DIM_TEX_1D_ARRAY)
        max_dim = tex_desc.width;
    else if(tex_desc.type == RESOURCE_DIM_TEX_2D || tex_desc.type == RESOURCE_DIM_TEX_2D_ARRAY
            || tex_desc.type == RESOURCE_DIM_TEX_CUBE || tex_desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
        max_dim = std::max(tex_desc.width, tex_desc.height);
    else if(tex_desc.type == RESOURCE_DIM_TEX_3D)
        max_dim = std::max(std::max(tex_desc.width, tex_desc.height), tex_desc.depth);
    assert(max_dim >= (1U << (tex_desc.mip_levels - 1)));   // "Texture: Incorrect number of Mip levels"

    if(tex_desc.sample_count > 1)
    {
        if(!(tex_desc.type == RESOURCE_DIM_TEX_2D || tex_desc.type == RESOURCE_DIM_TEX_2D_ARRAY))
            LOG_TEXTURE_ERROR_AND_THROW("Only Texture 2D/Texture 2D Array can be multisampled");

        if(tex_desc.mip_levels != 1)
            LOG_TEXTURE_ERROR_AND_THROW("Multisampled textures must have one mip level (",
                                        tex_desc.mip_levels, " levels specified)");

        if(tex_desc.bind_flags & BIND_UNORDERED_ACCESS)
            LOG_TEXTURE_ERROR_AND_THROW("UAVs are not allowed for multisampled resources");
    }

    if((tex_desc.bind_flags & BIND_RENDER_TARGET)
       && (tex_desc.format == TEX_FORMAT_R8_SNORM || tex_desc.format == TEX_FORMAT_RG8_SNORM
           || tex_desc.format == TEX_FORMAT_RGBA8_SNORM || tex_desc.format == TEX_FORMAT_R16_SNORM
           || tex_desc.format == TEX_FORMAT_RG16_SNORM || tex_desc.format == TEX_FORMAT_RGBA16_SNORM))
    {
        const auto &       fmt_name = GetTextureFormatInfo(tex_desc.format).name;
        std::ostringstream ss;
        ss << fmt_name
           << " texture is created with BIND_RENDER_TARGET flag set.\nThere might be an issue in OpenGL "
              "driver on NVidia hardware: when rendering to SNORM textures, all negative values are clamped "
              "to zero.\nUse UNORM format instead."
           << std::endl;
        Log::Log(Log::warning, ss.str());
    }

    if(tex_desc.usage == USAGE_CPU_ACCESSIBLE)
    {
        if(tex_desc.bind_flags != 0)
            LOG_TEXTURE_ERROR_AND_THROW("Staging textures cannot be bound to any GPU pipeline stage");

        if(tex_desc.misc_flags & MISC_TEXTURE_FLAG_GENERATE_MIPS)
            LOG_TEXTURE_ERROR_AND_THROW("Mipmaps cannot be autogenerated for staging textures");

        if(tex_desc.cpu_access_flags == 0)
            LOG_TEXTURE_ERROR_AND_THROW("Staging textures must specify CPU access flags");

        if((tex_desc.cpu_access_flags & (CPU_ACCESS_READ | CPU_ACCESS_WRITE))
           == (CPU_ACCESS_READ | CPU_ACCESS_WRITE))
            LOG_TEXTURE_ERROR_AND_THROW(
                "Staging textures must use exactly one of ACESS_READ or ACCESS_WRITE flags");
    }
}

void ValidateTextureRegion(const TextureDesc & tex_desc, uint32_t mip_level, uint32_t slice, const Box & box)
{
#define VERIFY_TEX_PARAMS(expr, ...)                                           \
    if(!(expr))                                                                \
        do                                                                     \
        {                                                                      \
            std::ostringstream ss;                                             \
            Log::stream_print(ss, ##__VA_ARGS__);                              \
            std::string msg = ss.str();                                        \
            ss.str({});                                                        \
            ss << "Texture \"" << tex_desc.name << "\": " << msg << std::endl; \
            Log::Log(Log::error, ss.str());                                    \
    } while(false)
#ifdef DEBUG
    VERIFY_TEX_PARAMS(mip_level < tex_desc.mip_levels, "Mip level (", mip_level,
                      ") is out of allowed range [0, ", tex_desc.mip_levels - 1, "]");
    VERIFY_TEX_PARAMS(box.min_x < box.max_x, "Invalid X range: ", box.min_x, "..", box.max_x);
    VERIFY_TEX_PARAMS(box.min_y < box.max_y, "Invalid Y range: ", box.min_y, "..", box.max_y);
    VERIFY_TEX_PARAMS(box.min_z < box.max_z, "Invalid Z range: ", box.min_z, "..", box.max_z);

    if(tex_desc.type == RESOURCE_DIM_TEX_1D_ARRAY || tex_desc.type == RESOURCE_DIM_TEX_2D_ARRAY
       || tex_desc.type == RESOURCE_DIM_TEX_CUBE || tex_desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        VERIFY_TEX_PARAMS(slice < tex_desc.array_size, "Array slice (", slice, ") is out of range [0,",
                          tex_desc.array_size - 1, "]");
    }
    else
    {
        VERIFY_TEX_PARAMS(slice == 0, "Array slice (", slice, ") must be 0 for non-array textures");
    }

    const auto & fmt_info = GetTextureFormatInfo(tex_desc.format);

    uint32_t mip_width = std::max(tex_desc.width >> mip_level, 1U);
    if(fmt_info.component_type == COMPONENT_TYPE_COMPRESSED)
    {
        assert((fmt_info.block_width & (fmt_info.block_width - 1)) == 0);
        uint32_t block_aligned_mip_width =
            (mip_width + (fmt_info.block_width - 1)) & ~(fmt_info.block_width - 1);
        VERIFY_TEX_PARAMS(box.max_x <= block_aligned_mip_width, "Region max X coordinate (", box.max_x,
                          ") is out of allowed range [0, ", block_aligned_mip_width, "]");
        VERIFY_TEX_PARAMS((box.min_x % fmt_info.block_width) == 0,
                          "For compressed formats, the region min X coordinate (", box.min_x,
                          ") must be a multiple of block width (", uint32_t{fmt_info.block_width}, ")");
        VERIFY_TEX_PARAMS((box.max_x % fmt_info.block_width) == 0 || box.max_x == mip_width,
                          "For compressed formats, the region max X coordinate (", box.max_x,
                          ") must be a multiple of block width (", uint32_t{fmt_info.block_width},
                          ") or equal the mip level width (", mip_width, ")");
    }
    else
        VERIFY_TEX_PARAMS(box.max_x <= mip_width, "Region max X coordinate (", box.max_x,
                          ") is out of allowed range [0, ", mip_width, "]");

    if(tex_desc.type != RESOURCE_DIM_TEX_1D && tex_desc.type != RESOURCE_DIM_TEX_1D_ARRAY)
    {
        uint32_t mip_height = std::max(tex_desc.height >> mip_level, 1U);
        if(fmt_info.component_type == COMPONENT_TYPE_COMPRESSED)
        {
            assert((fmt_info.block_height & (fmt_info.block_height - 1)) == 0);
            uint32_t block_aligned_mip_height =
                (mip_height + (fmt_info.block_height - 1)) & ~(fmt_info.block_height - 1);
            VERIFY_TEX_PARAMS(box.max_y <= block_aligned_mip_height, "Region max Y coordinate (", box.max_y,
                              ") is out of allowed range [0, ", block_aligned_mip_height, "]");
            VERIFY_TEX_PARAMS((box.min_y % fmt_info.block_height) == 0,
                              "For compressed formats, the region min Y coordinate (", box.min_y,
                              ") must be a multiple of block height (", uint32_t{fmt_info.block_height}, ")");
            VERIFY_TEX_PARAMS((box.max_y % fmt_info.block_height) == 0 || box.max_y == mip_height,
                              "For compressed formats, the region max Y coordinate (", box.max_y,
                              ") must be a multiple of block height (", uint32_t{fmt_info.block_height},
                              ") or equal the mip level height (", mip_height, ")");
        }
        else
            VERIFY_TEX_PARAMS(box.max_y <= mip_height, "Region max Y coordinate (", box.max_y,
                              ") is out of allowed range [0, ", mip_height, "]");
    }

    if(tex_desc.type == RESOURCE_DIM_TEX_3D)
    {
        uint32_t mip_depth = std::max(tex_desc.depth >> mip_level, 1U);
        VERIFY_TEX_PARAMS(box.max_z <= mip_depth, "Region max Z coordinate (", box.max_z,
                          ") is out of allowed range  [0, ", mip_depth, "]");
    }
    else
    {
        VERIFY_TEX_PARAMS(box.min_z == 0, "Region min Z (", box.min_z, ") must be 0 for all but 3D textures");
        VERIFY_TEX_PARAMS(box.max_z == 1, "Region max Z (", box.max_z, ") must be 1 for all but 3D textures");
    }
#endif
}

void ValidateUpdateTextureParams(const TextureDesc & tex_desc, uint32_t mip_level, uint32_t slice,
                                 const Box & dst_box, const TextureSubResData & subres_data)
{
    assert((subres_data.data != nullptr)
           ^ (subres_data.src_buffer != nullptr));   //"Either CPU data pointer (data) or GPU buffer
                                                     //(src_buffer) must not be null, but not both"
    ValidateTextureRegion(tex_desc, mip_level, slice, dst_box);

#ifdef DEBUG
    VERIFY_TEX_PARAMS(tex_desc.sample_count == 1,
                      "Only non-multisampled textures can be updated with UpdateData()");
    VERIFY_TEX_PARAMS((subres_data.stride & 0x03) == 0, "Texture data stride (", subres_data.stride,
                      ") must be at least 32-bit aligned");
    VERIFY_TEX_PARAMS((subres_data.depth_stride & 0x03) == 0, "Texture data depth stride (",
                      subres_data.depth_stride, ") must be at least 32-bit aligned");

    auto         update_region_width  = dst_box.max_x - dst_box.min_x;
    auto         update_region_height = dst_box.max_y - dst_box.min_y;
    auto         update_region_depth  = dst_box.max_z - dst_box.min_z;
    const auto & fmt_info             = GetTextureFormatInfo(tex_desc.format);
    uint32_t     row_size             = 0;
    uint32_t     row_count            = 0;
    if(fmt_info.component_type == COMPONENT_TYPE_COMPRESSED)
    {
        // Align update region size by the block size. This is only necessary when updating
        // coarse mip levels. Otherwise UpdateRegionWidth/Height should be multiples of block size
        assert((fmt_info.block_width & (fmt_info.block_width - 1)) == 0);
        assert((fmt_info.block_height & (fmt_info.block_height - 1)) == 0);
        update_region_width =
            (update_region_width + (fmt_info.block_width - 1)) & ~(fmt_info.block_width - 1);
        update_region_height =
            (update_region_height + (fmt_info.block_height - 1)) & ~(fmt_info.block_height - 1);
        row_size  = update_region_width / uint32_t{fmt_info.block_width} * uint32_t{fmt_info.component_size};
        row_count = update_region_height / fmt_info.block_height;
    }
    else
    {
        row_size =
            update_region_width * uint32_t{fmt_info.component_size} * uint32_t{fmt_info.num_components};
        row_count = update_region_height;
    }
    assert(subres_data.stride >= row_size);   // "Source data stride is below the image row size"
    const uint32_t plane_size = subres_data.stride * row_count;
    assert(update_region_depth == 1
           || subres_data.depth_stride
                  >= plane_size);   // "Source data depth stride is below the image plane size"
#endif
}

void ValidateCopyTextureParams(const CopyTextureAttribs & copy_attribs)
{
    assert(copy_attribs.p_src_texture != nullptr && copy_attribs.p_dst_texture != nullptr);
    Box          src_box;
    const auto & src_tex_desc = copy_attribs.p_src_texture->getDesc();
    const auto & dst_tex_desc = copy_attribs.p_dst_texture->getDesc();
    auto         p_src_box    = copy_attribs.p_src_box;
    if(p_src_box == nullptr)
    {
        auto mip_level_attribs = GetMipLevelProperties(src_tex_desc, copy_attribs.src_mip_level);
        src_box.max_x          = mip_level_attribs.logical_width;
        src_box.max_y          = mip_level_attribs.logical_height;
        src_box.max_z          = mip_level_attribs.depth;
        p_src_box              = &src_box;
    }
    ValidateTextureRegion(src_tex_desc, copy_attribs.src_mip_level, copy_attribs.src_slice, *p_src_box);

    Box dst_box;
    dst_box.min_x = copy_attribs.dst_x;
    dst_box.max_x = dst_box.min_x + (p_src_box->max_x - p_src_box->min_x);
    dst_box.min_y = copy_attribs.dst_y;
    dst_box.max_y = dst_box.min_y + (p_src_box->max_y - p_src_box->min_y);
    dst_box.min_z = copy_attribs.dst_z;
    dst_box.max_z = dst_box.min_z + (p_src_box->max_z - p_src_box->min_z);
    ValidateTextureRegion(dst_tex_desc, copy_attribs.dst_mip_level, copy_attribs.dst_slice, dst_box);
}

void ValidateMapTextureParams(const TextureDesc & tex_desc, uint32_t mip_level, uint32_t array_slice,
                              MAP_TYPE MapType, uint32_t MapFlags, const Box * p_map_region)
{
    VERIFY_TEX_PARAMS(mip_level < tex_desc.mip_levels, "Mip level (", mip_level,
                      ") is out of allowed range [0, ", tex_desc.mip_levels - 1, "]");
    if(tex_desc.type == RESOURCE_DIM_TEX_1D_ARRAY || tex_desc.type == RESOURCE_DIM_TEX_2D_ARRAY
       || tex_desc.type == RESOURCE_DIM_TEX_CUBE || tex_desc.type == RESOURCE_DIM_TEX_CUBE_ARRAY)
    {
        VERIFY_TEX_PARAMS(array_slice < tex_desc.array_size, "Array slice (", array_slice,
                          ") is out of range [0,", tex_desc.array_size - 1, "]");
    }
    else
    {
        VERIFY_TEX_PARAMS(array_slice == 0, "Array slice (", array_slice,
                          ") must be 0 for non-array textures");
    }

    if(p_map_region != nullptr)
    {
        ValidateTextureRegion(tex_desc, mip_level, array_slice, *p_map_region);
    }
}
}   // namespace evnt
