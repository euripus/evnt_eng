#ifndef IRENDERDEVICE_H
#define IRENDERDEVICE_H

#include "../devicecaps.h"
#include "../graphics_types.h"
#include "ibuffer.h"
#include "ifence.h"
#include "ipipelinestate.h"
#include "itexture.h"

namespace evnt
{
/// Render device interface
class IRenderDevice
{
public:
    /// Creates a new buffer object
    /// \param [in] BuffDesc   - Buffer description, see evnt::BufferDesc for details.
    /// \param [in] pBuffData  - Pointer to evnt::BufferData structure that describes
    ///                          initial buffer data or nullptr if no data is provided.
    ///                          Static buffers (USAGE_STATIC) must be initialized at creation time.
    /// \param [out] ppBuffer  - Address of the memory location where the pointer to the
    ///                          buffer interface will be stored. The function calls AddRef(),
    ///                          so that the new buffer will contain one refernce and must be
    ///                          released by a call to Release().
    /// \remarks
    /// Size of a uniform buffer (BIND_UNIFORM_BUFFER) must be multiple of 16.
    /// Stride of a formatted buffer will be computed automatically from the format if
    /// ElementByteStride member of buffer description is set to default value (0).
    virtual void createBuffer(const BufferDesc & BuffDesc, const BufferData * pBuffData,
                              IBuffer ** ppBuffer) = 0;

    /// Creates a new shader object
    /// \param [in] ShaderCI  - Shader create info, see evnt::ShaderCreateInfo for details.
    /// \param [out] ppShader - Address of the memory location where the pointer to the
    ///                         shader interface will be stored.
    ///                         The function calls AddRef(), so that the new object will contain
    ///                         one refernce.
    virtual void createShader(const ShaderCreateInfo & ShaderCI, IShader ** ppShader) = 0;

    /// Creates a new texture object
    /// \param [in] TexDesc - Texture description, see evnt::TextureDesc for details.
    /// \param [in] pData   - Pointer to evnt::TextureData structure that describes
    ///                       initial texture data or nullptr if no data is provided.
    ///                       Static textures (USAGE_STATIC) must be initialized at creation time.
    /// \param [out] ppTexture - Address of the memory location where the pointer to the
    ///                          texture interface will be stored.
    ///                          The function calls AddRef(), so that the new object will contain
    ///                          one refernce.
    /// \remarks
    /// To create all mip levels, set the TexDesc.MipLevels to zero.
    /// Multisampled resources cannot be initialzed with data when they are created.
    /// If initial data is provided, number of subresources must exactly match the number
    /// of subresources in the texture (which is the number of mip levels times the number of array slices.
    /// For a 3D texture, this is just the number of mip levels).
    /// For example, for a 15 x 6 x 2 2D texture array, the following array of subresources should be
    /// provided: 15x6, 7x3, 3x1, 1x1, 15x6, 7x3, 3x1, 1x1.
    /// For a 15 x 6 x 4 3D texture, the following array of subresources should be provided:
    /// 15x6x4, 7x3x2, 3x1x1, 1x1x1
    virtual void createTexture(const TextureDesc & TexDesc, const TextureData * pData,
                               ITexture ** ppTexture) = 0;

    /// Creates a new sampler object
    /// \param [in]  SamDesc   - Sampler description, see evnt::SamplerDesc for details.
    /// \param [out] ppSampler - Address of the memory location where the pointer to the
    ///                          sampler interface will be stored.
    ///                          The function calls AddRef(), so that the new object will contain
    ///                          one refernce.
    /// \remark If an application attempts to create a sampler interface with the same attributes
    ///         as an existing interface, the same interface will be returned.
    /// \note   In D3D11, 4096 unique sampler state objects can be created on a device at a time.
    virtual void createSampler(const SamplerDesc & SamDesc, ISampler ** ppSampler) = 0;

    /// Creates a new resource mapping
    /// \param [in]  MappingDesc - Resource mapping description, see evnt::ResourceMappingDesc for
    ///                            details.
    /// \param [out] ppMapping   - Address of the memory location where the pointer to the
    ///                            resource mapping interface will be stored.
    ///                            The function calls AddRef(), so that the new object will contain
    ///                            one refernce.
    virtual void createResourceMapping(const ResourceMappingDesc & MappingDesc,
                                       IResourceMapping **         ppMapping) = 0;

    /// Creates a new pipeline state object
    /// \param [in]  PipelineDesc    - Pipeline state description, see evnt::PipelineStateDesc for
    ///                                details.
    /// \param [out] ppPipelineState - Address of the memory location where the pointer to the
    ///                                pipeline state interface will be stored.
    ///                                The function calls AddRef(), so that the new object will contain
    ///                                one refernce.
    virtual void createPipelineState(const PipelineStateDesc & PipelineDesc,
                                     IPipelineState **         ppPipelineState) = 0;

    /// Creates a new pipeline state object
    /// \param [in]  Desc    - Fence description, see evnt::FenceDesc for details.
    /// \param [out] ppFence - Address of the memory location where the pointer to the
    ///                        fence interface will be stored.
    ///                        The function calls AddRef(), so that the new object will contain
    ///                        one refernce.
    virtual void createFence(const FenceDesc & Desc, IFence ** ppFence) = 0;

    /// Gets the device capabilities, see evnt::DeviceCaps for details
    virtual const DeviceCaps & getDeviceCaps() const = 0;

    /// Returns the basic texture format information.
    /// See evnt::TextureFormatInfo for details on the provided information.
    /// \param [in] TexFormat - Texture format for which to provide the information
    /// \return Const reference to the TextureFormatInfo structure containing the
    ///         texture format description.
    virtual const TextureFormatInfo & getTextureFormatInfo(TEXTURE_FORMAT TexFormat) = 0;

    /// Purges device release queues and releases all stale resources.
    /// This method is automatically called by ISwapChain::Present().
    /// \param [in]  ForceRelease - Forces release of all objects. Use this option with
    ///                             great care only if you are sure the resources are not
    ///                             in use by the GPU (such as when the device has just been idled).
    virtual void releaseStaleResources(bool ForceRelease = false) = 0;
};
}   // namespace evnt

#endif   // IRENDERDEVICE_H
