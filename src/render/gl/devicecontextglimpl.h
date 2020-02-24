#ifndef DEVICECONTEXTGLIMPL_H
#define DEVICECONTEXTGLIMPL_H

#include "../common/devicecontextbase.h"
#include "./interface/idevicecontextgl.h"
#include "glcontextstate.h"
#include "textureviewglimpl.h"

namespace evnt
{
/*struct DeviceContextGLImplTraits
{
    using BufferType        = BufferGLImpl;
    using TextureType       = TextureBaseGL;
    using PipelineStateType = PipelineStateGLImpl;
    using ViewImplType      = TextureViewGLImpl;
};

/// Implementation of the Diligent::IDeviceContextGL interface
class DeviceContextGLImpl final : public DeviceContextBase<IDeviceContextGL, DeviceContextGLImplTraits>
{
public:
    using TDeviceContextBase = DeviceContextBase<IDeviceContextGL, DeviceContextGLImplTraits>;

    // DeviceContextGLImpl() {}

    void setPipelineState(IPipelineState * pPipelineState) override final;

    virtual void transitionShaderResources(IPipelineState *         pPipelineState,
                                           IShaderResourceBinding * pShaderResourceBinding) override final;

    virtual void commitShaderResources(IShaderResourceBinding *       pShaderResourceBinding,
                                       RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) override final;

    virtual void setStencilRef(uint32_t StencilRef) override final;

    virtual void setBlendFactors(const float * pBlendFactors = nullptr) override final;

    virtual void setVertexBuffers(uint32_t StartSlot, uint32_t NumBuffersSet, IBuffer ** ppBuffers,
                                  uint32_t * pOffsets, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode,
                                  SET_VERTEX_BUFFERS_FLAGS Flags) override final;

    virtual void invalidateState() override final;

    virtual void setIndexBuffer(IBuffer * pIndexBuffer, uint32_t ByteOffset,
                                RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) override final;

    virtual void setViewports(uint32_t NumViewports, const Viewport * pViewports, uint32_t RTWidth,
                              uint32_t RTHeight) override final;

    virtual void setScissorRects(uint32_t NumRects, const Rect * pRects, uint32_t RTWidth,
                                 uint32_t RTHeight) override final;

    virtual void setRenderTargets(uint32_t NumRenderTargets, ITextureView * ppRenderTargets[],
                                  ITextureView *                 pDepthStencil,
                                  RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) override final;

    virtual void draw(DrawAttribs & DrawAttribs) override final;

    virtual void dispatchCompute(const DispatchComputeAttribs & DispatchAttrs) override final;

    virtual void clearDepthStencil(ITextureView * pView, CLEAR_DEPTH_STENCIL_FLAGS ClearFlags, float fDepth,
                                   uint8_t                        Stencil,
                                   RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) override final;

    virtual void clearRenderTarget(ITextureView * pView, const float * RGBA,
                                   RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) override final;

    virtual void flush() override final;

    virtual void updateBuffer(IBuffer * pBuffer, uint32_t Offset, uint32_t Size, const void * pData,
                              RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) override final;

    virtual void copyBuffer(IBuffer * pSrcBuffer, uint32_t SrcOffset,
                            RESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode, IBuffer * pDstBuffer,
                            uint32_t DstOffset, uint32_t Size,
                            RESOURCE_STATE_TRANSITION_MODE DstBufferTransitionMode) override final;

    virtual void mapBuffer(IBuffer * pBuffer, MAP_TYPE MapType, MAP_FLAGS MapFlags,
                           void *& pMappedData) override final;

    virtual void unmapBuffer(IBuffer * pBuffer, MAP_TYPE MapType) override final;

    virtual void updateTexture(ITexture * pTexture, uint32_t MipLevel, uint32_t Slice, const Box & DstBox,
                               const TextureSubResData &      SubresData,
                               RESOURCE_STATE_TRANSITION_MODE SrcBufferStateTransitionMode,
                               RESOURCE_STATE_TRANSITION_MODE TextureStateTransitionMode) override final;

    virtual void copyTexture(const CopyTextureAttribs & CopyAttribs) override final;

    virtual void mapTextureSubresource(ITexture * pTexture, uint32_t MipLevel, uint32_t ArraySlice,
                                       MAP_TYPE MapType, MAP_FLAGS MapFlags, const Box * pMapRegion,
                                       MappedTextureSubresource & MappedData) override final;

    virtual void unmapTextureSubresource(ITexture * pTexture, uint32_t MipLevel,
                                         uint32_t ArraySlice) override final;

    virtual void generateMips(ITextureView * pTexView) override;

    virtual void finishFrame() override final;

    virtual void transitionResourceStates(uint32_t              BarrierCount,
                                          StateTransitionDesc * pResourceBarriers) override final;

    virtual void finishCommandList(class ICommandList ** ppCommandList) override final;

    virtual void executeCommandList(class ICommandList * pCommandList) override final;

    virtual void signalFence(IFence * pFence, uint64_t Value) override final;

    virtual bool updateCurrentGLContext() override final;

    void bindProgramResources(uint32_t & NewMemoryBarriers, IShaderResourceBinding * pResBinding);

    GLContextState & getContextState() { return m_context_state; }

    void commitRenderTargets();

protected:
    GLContextState m_context_state;
};*/
}   // namespace evnt
#endif   // DEVICECONTEXTGLIMPL_H
