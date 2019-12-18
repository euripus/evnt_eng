#ifndef DEVICECONTEXTBASE_H
#define DEVICECONTEXTBASE_H

#include "./interface/ibuffer.h"
#include "./interface/idevicecontext.h"
#include "./interface/irenderdevice.h"
#include "./interface/iswapchain.h"
#include "./interface/itextureview.h"
#include "objectbase.h"

namespace evnt
{
/// Base implementation of the device context.
/// \tparam BaseInterface         - base interface that this class will inheret.
/// \tparam ImplementationTraits  - implementation traits that define specific implementation details
///                                 (texture implemenation type, buffer implementation type, etc.)
/// \remarks Device context keeps strong references to all objects currently bound to
///          the pipeline: buffers, states, samplers, shaders, etc.
///          The context also keeps strong references to the device and
///          the swap chain.
template<typename BaseInterface, typename ImplementationTraits>
class DeviceContextBase : public ObjectBase<BaseInterface>
{
public:
    using TObjectBase           = ObjectBase<BaseInterface>;
    using BufferImplType        = typename ImplementationTraits::BufferType;
    using TextureImplType       = typename ImplementationTraits::TextureType;
    using PipelineStateImplType = typename ImplementationTraits::PipelineStateType;
    using TextureViewImplType   = typename TextureImplType::ViewImplType;

    DeviceContextBase();

    /// Base implementation of IDeviceContext::SetVertexBuffers(); validates parameters and
    /// caches references to the buffers.
    inline virtual void SetVertexBuffers(uint32_t StartSlot, uint32_t NumBuffersSet, IBuffer ** ppBuffers,
                                         uint32_t *                     pOffsets,
                                         RESOURCE_STATE_TRANSITION_MODE StateTransitionMode,
                                         SET_VERTEX_BUFFERS_FLAGS       Flags) override = 0;

    inline virtual void InvalidateState() override = 0;

    /// Base implementation of IDeviceContext::CommitShaderResources(); validates parameters.
    inline bool CommitShaderResources(IShaderResourceBinding *       pShaderResourceBinding,
                                      RESOURCE_STATE_TRANSITION_MODE StateTransitionMode, int);

    /// Base implementation of IDeviceContext::SetIndexBuffer(); caches the strong reference to the index
    /// buffer
    inline virtual void SetIndexBuffer(IBuffer * pIndexBuffer, uint32_t ByteOffset,
                                       RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) override = 0;

    /// Caches the viewports
    inline void SetViewports(uint32_t NumViewports, const Viewport * pViewports, uint32_t & RTWidth,
                             uint32_t & RTHeight);

    /// Caches the scissor rects
    inline void SetScissorRects(uint32_t NumRects, const Rect * pRects, uint32_t & RTWidth,
                                uint32_t & RTHeight);

    /// Caches the render target and depth stencil views. Returns true if any view is different
    /// from the cached value and false otherwise.
    inline bool SetRenderTargets(uint32_t NumRenderTargets, ITextureView * ppRenderTargets[],
                                 ITextureView * pDepthStencil);

    /// Base implementation of IDeviceContext::UpdateBuffer(); validates input parameters.
    virtual void UpdateBuffer(IBuffer * pBuffer, uint32_t Offset, uint32_t Size, const void * pData,
                              RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) override = 0;

    /// Base implementation of IDeviceContext::CopyBuffer(); validates input parameters.
    virtual void CopyBuffer(IBuffer * pSrcBuffer, uint32_t SrcOffset,
                            RESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode, IBuffer * pDstBuffer,
                            uint32_t DstOffset, uint32_t Size,
                            RESOURCE_STATE_TRANSITION_MODE DstBufferTransitionMode) override = 0;

    /// Base implementation of IDeviceContext::MapBuffer(); validates input parameters.
    virtual void MapBuffer(IBuffer * pBuffer, MAP_TYPE MapType, MAP_FLAGS MapFlags,
                           void *& pMappedData) override = 0;

    /// Base implementation of IDeviceContext::UnmapBuffer()
    virtual void UnmapBuffer(IBuffer * pBuffer, MAP_TYPE MapType) override = 0;

    /// Base implementaiton of IDeviceContext::UpdateData(); validates input parameters
    virtual void UpdateTexture(ITexture * pTexture, uint32_t MipLevel, uint32_t Slice, const Box & DstBox,
                               const TextureSubResData &      SubresData,
                               RESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode,
                               RESOURCE_STATE_TRANSITION_MODE TextureTransitionMode) override = 0;

    /// Base implementaiton of IDeviceContext::CopyTexture(); validates input parameters
    virtual void CopyTexture(const CopyTextureAttribs & CopyAttribs) override = 0;

    /// Base implementaiton of IDeviceContext::MapTextureSubresource()
    virtual void MapTextureSubresource(ITexture * pTexture, uint32_t MipLevel, uint32_t ArraySlice,
                                       MAP_TYPE MapType, MAP_FLAGS MapFlags, const Box * pMapRegion,
                                       MappedTextureSubresource & MappedData) override = 0;

    /// Base implementaiton of IDeviceContext::UnmapTextureSubresource()
    virtual void UnmapTextureSubresource(ITexture * pTexture, uint32_t MipLevel,
                                         uint32_t ArraySlice) override = 0;

    virtual void GenerateMips(ITextureView * pTexView) override = 0;

    /// Sets the strong pointer to the swap chain
    virtual void SetSwapChain(ISwapChain * pSwapChain) override;

    /// Returns the swap chain
    ISwapChain * GetSwapChain();

    /// Returns true if currently bound frame buffer is the default frame buffer
    inline bool IsDefaultFBBound();

    /// Returns currently bound pipeline state and blend factors
    inline void GetPipelineState(IPipelineState ** ppPSO, float * BlendFactors, uint32_t & StencilRef);

    /// Returns currently bound render targets
    inline void GetRenderTargets(uint32_t & NumRenderTargets, ITextureView ** ppRTVs, ITextureView ** ppDSV);

    /// Returns currently set viewports
    inline void GetViewports(uint32_t & NumViewports, Viewport * pViewports);

    /// Returns the render device
    IRenderDevice * GetDevice();

    inline void ResetRenderTargets();

    bool IsDeferred() const;
};
}   // namespace evnt
#endif   // DEVICECONTEXTBASE_H
