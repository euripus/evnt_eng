#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "ideviceobject.h"

namespace evnt
{
class IDeviceContext;

/// Defines optimized depth-stencil clear value.
struct DepthStencilClearValue
{
    /// Depth clear value
    float depth = 1.f;
    /// Stencil clear value
    uint8_t stencil = 0;

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     DepthStencilClearValue{1, 0}

    DepthStencilClearValue() noexcept {}
    DepthStencilClearValue(float _Depth, uint8_t _Stencil) noexcept : depth(_Depth), stencil(_Stencil) {}
};

/// Defines optimized clear value.
struct OptimizedClearValue
{
    /// Format
    TEXTURE_FORMAT format = TEX_FORMAT_UNKNOWN;
    /// Render target clear value
    float color[4] = {0, 0, 0, 0};
    /// Depth stencil clear value
    DepthStencilClearValue depth_stencil;

    bool operator==(const OptimizedClearValue & rhs) const
    {
        return format == rhs.format && color[0] == rhs.color[0] && color[1] == rhs.color[1]
               && color[2] == rhs.color[2] && color[3] == rhs.color[3]
               && depth_stencil.depth == rhs.depth_stencil.depth
               && depth_stencil.stencil == rhs.depth_stencil.stencil;
    }
};

/// Texture description
struct TextureDesc : DeviceObjectAttribs
{
    /// Texture type. See evnt::RESOURCE_DIMENSION for details.
    RESOURCE_DIMENSION type = RESOURCE_DIM_UNDEFINED;
    /// Texture width, in pixels.
    uint32_t width = 0;
    /// Texture height, in pixels.
    uint32_t height = 0;
    union
    {
        /// For a 1D array or 2D array, number of array slices
        uint32_t array_size = 1;
        /// For a 3D texture, number of depth slices
        uint32_t depth;
    };
    /// Texture format, see evnt::TEXTURE_FORMAT.
    TEXTURE_FORMAT format = TEX_FORMAT_UNKNOWN;
    /// Number of Mip levels in the texture. Multisampled textures can only have 1 Mip level.
    /// Specify 0 to generate full mipmap chain.
    uint32_t mip_levels = 1;
    /// Number of samples.
    /// Only 2D textures or 2D texture arrays can be multisampled.
    uint32_t sample_count = 1;
    /// Texture usage. See evnt::USAGE for details.
    USAGE usage = USAGE_DEFAULT;
    /// Bind flags, see evnt::BIND_FLAGS for details. 
    /// The following bind flags are allowed:
    /// evnt::BIND_SHADER_RESOURCE, evnt::BIND_RENDER_TARGET, evnt::BIND_DEPTH_STENCIL,
    /// evnt::and BIND_UNORDERED_ACCESS. 
    /// Multisampled textures cannot have evnt::BIND_UNORDERED_ACCESS flag set
    BIND_FLAGS bind_flags = BIND_NONE;
    /// CPU access flags or 0 if no CPU access is allowed,
    /// see evnt::CPU_ACCESS_FLAGS for details.
    CPU_ACCESS_FLAGS cpu_access_flags = CPU_ACCESS_NONE;
    /// Miscellaneous flags, see evnt::MISC_TEXTURE_FLAGS for details.
    MISC_TEXTURE_FLAGS misc_flags = MISC_TEXTURE_FLAG_NONE;
    /// Optimized clear value
    OptimizedClearValue clear_value;
    /// Defines which command queues this texture can be used with
    uint64_t command_queue_mask = 1;

    TextureDesc() noexcept {}
    TextureDesc(RESOURCE_DIMENSION _Type, uint32_t _Width, uint32_t _Height, uint32_t _ArraySizeOrDepth,
                TEXTURE_FORMAT _Format, uint32_t _MipLevels = TextureDesc{}.mip_levels,
                uint32_t _SampleCount = TextureDesc{}.sample_count, USAGE _Usage = TextureDesc{}.usage,
                BIND_FLAGS          _BindFlags        = TextureDesc{}.bind_flags,
                CPU_ACCESS_FLAGS    _CPUAccessFlags   = TextureDesc{}.cpu_access_flags,
                MISC_TEXTURE_FLAGS  _MiscFlags        = TextureDesc{}.misc_flags,
                OptimizedClearValue _ClearValue       = TextureDesc{}.clear_value,
                uint64_t            _CommandQueueMask = TextureDesc{}.command_queue_mask) :
        type(_Type),
        width(_Width),
        height(_Height),
        array_size(_ArraySizeOrDepth),
        format(_Format),
        mip_levels(_MipLevels),
        sample_count(_SampleCount),
        usage(_Usage),
        bind_flags(_BindFlags),
        cpu_access_flags(_CPUAccessFlags),
        misc_flags(_MiscFlags),
        clear_value(_ClearValue),
        command_queue_mask(_CommandQueueMask)
    {}

    /// Tests if two structures are equivalent
    /// \param [in] RHS - reference to the structure to perform comparison with
    /// \return
    /// - True if all members of the two structures except for the Name are equal.
    /// - False otherwise.
    /// The operator ignores DeviceObjectAttribs::Name field as it does not affect
    /// the texture description state.
    bool operator==(const TextureDesc & RHS) const
    {
        // Name is primarily used for debug purposes and does not affect the state.
        // It is ignored in comparison operation.
        return   // strcmp(Name, RHS.Name) == 0          &&
            type == RHS.type && width == RHS.width && height == RHS.height && array_size == RHS.array_size
            && format == RHS.format && mip_levels == RHS.mip_levels && sample_count == RHS.sample_count
            && usage == RHS.usage && bind_flags == RHS.bind_flags && cpu_access_flags == RHS.cpu_access_flags
            && misc_flags == RHS.misc_flags && clear_value == RHS.clear_value
            && command_queue_mask == RHS.command_queue_mask;
    }
};

/// Describes data for one subresource
struct TextureSubResData
{
    /// Pointer to the subresource data in CPU memory.
    /// If provided, pSrcBuffer must be null
    const void * data = nullptr;
    /// Pointer to the GPU buffer that contains subresource data.
    /// If provided, pData must be null
    class IBuffer * src_buffer = nullptr;
    /// When updating data from the buffer (pSrcBuffer is not null),
    /// offset from the beginning of the buffer to the data start
    uint32_t src_offset = 0;
    /// For 2D and 3D textures, row stride in bytes
    uint32_t stride = 0;
    /// For 3D textures, depth slice stride in bytes
    /// \note On OpenGL, this must be a mutliple of Stride
    uint32_t depth_stride = 0;

    /// Initializes the structure members with default values
    /// Default values:
    /// Member          | Default value
    /// ----------------|--------------
    /// pData           | nullptr
    /// SrcOffset       | 0
    /// Stride          | 0
    /// DepthStride     | 0
    TextureSubResData() noexcept {}

    /// Initializes the structure members to perform copy from the CPU memory
    TextureSubResData(void * _pData, uint32_t _Stride, uint32_t _DepthStride = 0) noexcept :
        data(_pData), src_buffer(nullptr), src_offset(0), stride(_Stride), depth_stride(_DepthStride)
    {}

    /// Initializes the structure members to perform copy from the GPU buffer
    TextureSubResData(IBuffer * _pBuffer, uint32_t _SrcOffset, uint32_t _Stride,
                      uint32_t _DepthStride = 0) noexcept :
        data(nullptr),
        src_buffer(_pBuffer),
        src_offset(_SrcOffset),
        stride(_Stride),
        depth_stride(_DepthStride)
    {}
};

/// Describes the initial data to store in the texture
struct TextureData
{
    /// Pointer to the array of the TextureSubResData elements containing
    /// information about each subresource.
    TextureSubResData * sub_resources = nullptr;
    /// Number of elements in pSubResources array.
    /// NumSubresources must exactly match the number
    /// of subresources in the texture. Otherwise an error
    /// occurs.
    uint32_t num_subresources = 0;
};

struct MappedTextureSubresource
{
    void *   data         = nullptr;
    uint32_t stride       = 0;
    uint32_t depth_stride = 0;
};

/// Texture inteface
class ITexture : public IDeviceObject
{
public:
    /// Returns the texture description used to create the object
    const TextureDesc & getDesc() const override = 0;
    /// Creates a new texture view
    /// \param [in] ViewDesc - View description. See evnt::TextureViewDesc for details.
    /// \param [out] ppView - Address of the memory location where the pointer to the view interface will be
    /// written to.
    ///
    /// \remarks To create a shader resource view addressing the entire texture, set only
    /// TextureViewDesc::ViewType
    ///          member of the ViewDesc parameter to evnt::TEXTURE_VIEW_SHADER_RESOURCE and leave all
    ///          other members in their default values. Using the same method, you can create render target or
    ///          depth stencil view addressing the largest mip level. If texture view format is
    ///          evnt::TEX_FORMAT_UNKNOWN, the view format will match the texture format. If texture
    ///          view type is evnt::TEXTURE_VIEW_UNDEFINED, the type will match the texture type. If the
    ///          number of mip levels is 0, and the view type is shader resource, the view will address all
    ///          mip levels. For other view types it will address one mip level. If the number of slices is
    ///          0, all slices from FirstArraySlice or FirstDepthSlice will be referenced by the view. For
    ///          non-array textures, the only allowed values for the number of slices are 0 and 1. Texture
    ///          view will contain strong reference to the texture, so the texture will not be destroyed until
    ///          all views are released. The function calls AddRef() for the created interface, so it must
    ///          be released by a call to Release() when it is no longer needed.
    virtual void createView(const struct TextureViewDesc & ViewDesc, class ITextureView ** ppView) = 0;
    /// Returns the pointer to the default view.
    /// \param [in] ViewType - Type of the requested view. See evnt::TEXTURE_VIEW_TYPE.
    /// \return Pointer to the interface
    ///
    /// \note The function does not increase the reference counter for the returned interface, so
    ///       Release() must *NOT* be called.
    virtual ITextureView * getDefaultView(TEXTURE_VIEW_TYPE ViewType) = 0;
    /// Returns native texture handle specific to the underlying graphics API
    /// \return pointer to ID3D11Resource interface, for D3D11 implementation
    ///         pointer to ID3D12Resource interface, for D3D12 implementation
    ///         GL buffer handle, for GL implementation
    virtual void * getNativeHandle() = 0;
    /// Sets the usage state for all texture subresources.
    /// \note This method does not perform state transition, but
    ///       resets the internal texture state to the given value.
    ///       This method should be used after the application finished
    ///       manually managing the texture state and wants to hand over
    ///       state management back to the engine.
    virtual void setState(RESOURCE_STATE State) = 0;
    /// Returns the internal texture state
    virtual RESOURCE_STATE getState() const = 0;
};

}   // namespace evnt

#endif   // ITEXTURE_H
