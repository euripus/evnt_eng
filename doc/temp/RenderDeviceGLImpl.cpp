/*     Copyright 2019 Diligent Graphics LLC
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF ANY PROPRIETARY RIGHTS.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#include "pch.h"

#include "RenderDeviceGLImpl.h"

#include "BufferGLImpl.h"
#include "DeviceContextGLImpl.h"
#include "EngineMemory.h"
#include "FenceGLImpl.h"
#include "GLTypeConversions.h"
#include "PipelineStateGLImpl.h"
#include "SamplerGLImpl.h"
#include "ShaderGLImpl.h"
#include "ShaderResourceBindingGLImpl.h"
#include "StringTools.h"
#include "Texture1DArray_OGL.h"
#include "Texture1D_OGL.h"
#include "Texture2DArray_OGL.h"
#include "Texture2D_OGL.h"
#include "Texture3D_OGL.h"
#include "TextureCubeArray_OGL.h"
#include "TextureCube_OGL.h"
#include "VAOCache.h"

namespace Diligent
{
RenderDeviceGLImpl ::RenderDeviceGLImpl(IReferenceCounters * pRefCounters, IMemoryAllocator & RawMemAllocator,
                                        IEngineFactory *           pEngineFactory,
                                        const EngineGLCreateInfo & InitAttribs,
                                        const SwapChainDesc *      pSCDesc) :
    TRenderDeviceBase{pRefCounters, RawMemAllocator, pEngineFactory, 0,
                      DeviceObjectSizes{sizeof(TextureBaseGL), sizeof(TextureViewGLImpl),
                                        sizeof(BufferGLImpl), sizeof(BufferViewGLImpl), sizeof(ShaderGLImpl),
                                        sizeof(SamplerGLImpl), sizeof(PipelineStateGLImpl),
                                        sizeof(ShaderResourceBindingGLImpl), sizeof(FenceGLImpl)}},
    // Device caps must be filled in before the constructor of Pipeline Cache is called!
    m_GLContext{InitAttribs, m_DeviceCaps, pSCDesc}
{
    GLint NumExtensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &NumExtensions);
    CHECK_GL_ERROR("Failed to get the number of extensions");
    m_ExtensionStrings.reserve(NumExtensions);
    for(int Ext = 0; Ext < NumExtensions; ++Ext)
    {
        auto CurrExtension = glGetStringi(GL_EXTENSIONS, Ext);
        CHECK_GL_ERROR("Failed to get extension string #", Ext);
        m_ExtensionStrings.emplace(reinterpret_cast<const Char *>(CurrExtension));
    }

    FlagSupportedTexFormats();
    QueryDeviceCaps();

    std::basic_string<GLubyte> glstrVendor = glGetString(GL_VENDOR);
    std::string                Vendor      = StrToLower(std::string(glstrVendor.begin(), glstrVendor.end()));
    LOG_INFO_MESSAGE("GPU Vendor: ", Vendor);

    if(Vendor.find("intel") != std::string::npos)
        m_GPUInfo.Vendor = GPU_VENDOR::INTEL;
    else if(Vendor.find("nvidia") != std::string::npos)
        m_GPUInfo.Vendor = GPU_VENDOR::NVIDIA;
    else if(Vendor.find("ati") != std::string::npos || Vendor.find("amd") != std::string::npos)
        m_GPUInfo.Vendor = GPU_VENDOR::ATI;
    else if(Vendor.find("qualcomm"))
        m_GPUInfo.Vendor = GPU_VENDOR::QUALCOMM;
}

RenderDeviceGLImpl ::~RenderDeviceGLImpl() {}

IMPLEMENT_QUERY_INTERFACE(RenderDeviceGLImpl, IID_RenderDeviceGL, TRenderDeviceBase)

void RenderDeviceGLImpl ::InitTexRegionRender()
{
    m_pTexRegionRender.reset(new TexRegionRender(this));
}

void RenderDeviceGLImpl ::CreateBuffer(const BufferDesc & BuffDesc, const BufferData * pBuffData,
                                       IBuffer ** ppBuffer, bool bIsDeviceInternal)
{
    CreateDeviceObject("buffer", BuffDesc, ppBuffer, [&]() {
        auto spDeviceContext = GetImmediateContext();
        VERIFY(spDeviceContext, "Immediate device context has been destroyed");
        auto * pDeviceContextGL = spDeviceContext.RawPtr<DeviceContextGLImpl>();

        BufferGLImpl * pBufferOGL(NEW_RC_OBJ(m_BufObjAllocator, "BufferGLImpl instance", BufferGLImpl)(
            m_BuffViewObjAllocator, this, pDeviceContextGL->GetContextState(), BuffDesc, pBuffData,
            bIsDeviceInternal));
        pBufferOGL->QueryInterface(IID_Buffer, reinterpret_cast<IObject **>(ppBuffer));
        pBufferOGL->CreateDefaultViews();
        OnCreateDeviceObject(pBufferOGL);
    });
}

void RenderDeviceGLImpl ::CreateBuffer(const BufferDesc & BuffDesc, const BufferData * BuffData,
                                       IBuffer ** ppBuffer)
{
    CreateBuffer(BuffDesc, BuffData, ppBuffer, false);
}

void RenderDeviceGLImpl ::CreateBufferFromGLHandle(Uint32 GLHandle, const BufferDesc & BuffDesc,
                                                   RESOURCE_STATE InitialState, IBuffer ** ppBuffer)
{
    VERIFY(GLHandle, "GL buffer handle must not be null");
    CreateDeviceObject("buffer", BuffDesc, ppBuffer, [&]() {
        auto spDeviceContext = GetImmediateContext();
        VERIFY(spDeviceContext, "Immediate device context has been destroyed");
        auto * pDeviceContextGL = spDeviceContext.RawPtr<DeviceContextGLImpl>();

        BufferGLImpl * pBufferOGL(NEW_RC_OBJ(m_BufObjAllocator, "BufferGLImpl instance", BufferGLImpl)(
            m_BuffViewObjAllocator, this, pDeviceContextGL->GetContextState(), BuffDesc, GLHandle, false));
        pBufferOGL->QueryInterface(IID_Buffer, reinterpret_cast<IObject **>(ppBuffer));
        pBufferOGL->CreateDefaultViews();
        OnCreateDeviceObject(pBufferOGL);
    });
}

void RenderDeviceGLImpl ::CreateShader(const ShaderCreateInfo & ShaderCreateInfo, IShader ** ppShader,
                                       bool bIsDeviceInternal)
{
    CreateDeviceObject("shader", ShaderCreateInfo.Desc, ppShader, [&]() {
        ShaderGLImpl * pShaderOGL(NEW_RC_OBJ(m_ShaderObjAllocator, "ShaderGLImpl instance",
                                             ShaderGLImpl)(this, ShaderCreateInfo, bIsDeviceInternal));
        pShaderOGL->QueryInterface(IID_Shader, reinterpret_cast<IObject **>(ppShader));

        OnCreateDeviceObject(pShaderOGL);
    });
}

void RenderDeviceGLImpl ::CreateShader(const ShaderCreateInfo & ShaderCreateInfo, IShader ** ppShader)
{
    CreateShader(ShaderCreateInfo, ppShader, false);
}

void RenderDeviceGLImpl ::CreateTexture(const TextureDesc & TexDesc, const TextureData * pData,
                                        ITexture ** ppTexture, bool bIsDeviceInternal)
{
    CreateDeviceObject("texture", TexDesc, ppTexture, [&]() {
        auto spDeviceContext = GetImmediateContext();
        VERIFY(spDeviceContext, "Immediate device context has been destroyed");
        auto & GLState = spDeviceContext.RawPtr<DeviceContextGLImpl>()->GetContextState();

        const auto & FmtInfo = GetTextureFormatInfo(TexDesc.Format);
        if(!FmtInfo.Supported)
        {
            LOG_ERROR_AND_THROW(FmtInfo.Name, " is not supported texture format");
        }

        TextureBaseGL * pTextureOGL = nullptr;
        switch(TexDesc.Type)
        {
            case RESOURCE_DIM_TEX_1D:
                pTextureOGL = NEW_RC_OBJ(m_TexObjAllocator, "Texture1D_OGL instance", Texture1D_OGL)(
                    m_TexViewObjAllocator, this, GLState, TexDesc, pData, bIsDeviceInternal);
                break;

            case RESOURCE_DIM_TEX_1D_ARRAY:
                pTextureOGL =
                    NEW_RC_OBJ(m_TexObjAllocator, "Texture1DArray_OGL instance", Texture1DArray_OGL)(
                        m_TexViewObjAllocator, this, GLState, TexDesc, pData, bIsDeviceInternal);
                break;

            case RESOURCE_DIM_TEX_2D:
                pTextureOGL = NEW_RC_OBJ(m_TexObjAllocator, "Texture2D_OGL instance", Texture2D_OGL)(
                    m_TexViewObjAllocator, this, GLState, TexDesc, pData, bIsDeviceInternal);
                break;

            case RESOURCE_DIM_TEX_2D_ARRAY:
                pTextureOGL =
                    NEW_RC_OBJ(m_TexObjAllocator, "Texture2DArray_OGL instance", Texture2DArray_OGL)(
                        m_TexViewObjAllocator, this, GLState, TexDesc, pData, bIsDeviceInternal);
                break;

            case RESOURCE_DIM_TEX_3D:
                pTextureOGL = NEW_RC_OBJ(m_TexObjAllocator, "Texture3D_OGL instance", Texture3D_OGL)(
                    m_TexViewObjAllocator, this, GLState, TexDesc, pData, bIsDeviceInternal);
                break;

            case RESOURCE_DIM_TEX_CUBE:
                pTextureOGL = NEW_RC_OBJ(m_TexObjAllocator, "TextureCube_OGL instance", TextureCube_OGL)(
                    m_TexViewObjAllocator, this, GLState, TexDesc, pData, bIsDeviceInternal);
                break;

            case RESOURCE_DIM_TEX_CUBE_ARRAY:
                pTextureOGL =
                    NEW_RC_OBJ(m_TexObjAllocator, "TextureCubeArray_OGL instance", TextureCubeArray_OGL)(
                        m_TexViewObjAllocator, this, GLState, TexDesc, pData, bIsDeviceInternal);
                break;

            default:
                LOG_ERROR_AND_THROW("Unknown texture type. (Did you forget to initialize the Type member of "
                                    "TextureDesc structure?)");
        }

        pTextureOGL->QueryInterface(IID_Texture, reinterpret_cast<IObject **>(ppTexture));
        pTextureOGL->CreateDefaultViews();
        OnCreateDeviceObject(pTextureOGL);
    });
}

void RenderDeviceGLImpl::CreateTexture(const TextureDesc & TexDesc, const TextureData * Data,
                                       ITexture ** ppTexture)
{
    CreateTexture(TexDesc, Data, ppTexture, false);
}

void RenderDeviceGLImpl::CreateTextureFromGLHandle(Uint32 GLHandle, const TextureDesc & TexDesc,
                                                   RESOURCE_STATE InitialState, ITexture ** ppTexture)
{
    VERIFY(GLHandle, "GL texture handle must not be null");
    CreateDeviceObject("texture", TexDesc, ppTexture, [&]() {
        auto spDeviceContext = GetImmediateContext();
        VERIFY(spDeviceContext, "Immediate device context has been destroyed");
        auto & GLState = spDeviceContext.RawPtr<DeviceContextGLImpl>()->GetContextState();

        TextureBaseGL * pTextureOGL = nullptr;
        switch(TexDesc.Type)
        {
            case RESOURCE_DIM_TEX_1D:
                pTextureOGL = NEW_RC_OBJ(m_TexObjAllocator, "Texture1D_OGL instance", Texture1D_OGL)(
                    m_TexViewObjAllocator, this, GLState, TexDesc, GLHandle);
                break;

            case RESOURCE_DIM_TEX_1D_ARRAY:
                pTextureOGL =
                    NEW_RC_OBJ(m_TexObjAllocator, "Texture1DArray_OGL instance",
                               Texture1DArray_OGL)(m_TexViewObjAllocator, this, GLState, TexDesc, GLHandle);
                break;

            case RESOURCE_DIM_TEX_2D:
                pTextureOGL = NEW_RC_OBJ(m_TexObjAllocator, "Texture2D_OGL instance", Texture2D_OGL)(
                    m_TexViewObjAllocator, this, GLState, TexDesc, GLHandle);
                break;

            case RESOURCE_DIM_TEX_2D_ARRAY:
                pTextureOGL =
                    NEW_RC_OBJ(m_TexObjAllocator, "Texture2DArray_OGL instance",
                               Texture2DArray_OGL)(m_TexViewObjAllocator, this, GLState, TexDesc, GLHandle);
                break;

            case RESOURCE_DIM_TEX_3D:
                pTextureOGL = NEW_RC_OBJ(m_TexObjAllocator, "Texture3D_OGL instance", Texture3D_OGL)(
                    m_TexViewObjAllocator, this, GLState, TexDesc, GLHandle);
                break;

            case RESOURCE_DIM_TEX_CUBE:
                pTextureOGL = NEW_RC_OBJ(m_TexObjAllocator, "TextureCube_OGL instance", TextureCube_OGL)(
                    m_TexViewObjAllocator, this, GLState, TexDesc, GLHandle);
                break;

            case RESOURCE_DIM_TEX_CUBE_ARRAY:
                pTextureOGL =
                    NEW_RC_OBJ(m_TexObjAllocator, "TextureCubeArray_OGL instance",
                               TextureCubeArray_OGL)(m_TexViewObjAllocator, this, GLState, TexDesc, GLHandle);
                break;

            default:
                LOG_ERROR_AND_THROW("Unknown texture type. (Did you forget to initialize the Type member of "
                                    "TextureDesc structure?)");
        }

        pTextureOGL->QueryInterface(IID_Texture, reinterpret_cast<IObject **>(ppTexture));
        pTextureOGL->CreateDefaultViews();
        OnCreateDeviceObject(pTextureOGL);
    });
}

void RenderDeviceGLImpl ::CreateSampler(const SamplerDesc & SamplerDesc, ISampler ** ppSampler,
                                        bool bIsDeviceInternal)
{
    CreateDeviceObject("sampler", SamplerDesc, ppSampler, [&]() {
        m_SamplersRegistry.Find(SamplerDesc, reinterpret_cast<IDeviceObject **>(ppSampler));
        if(*ppSampler == nullptr)
        {
            SamplerGLImpl * pSamplerOGL(NEW_RC_OBJ(m_SamplerObjAllocator, "SamplerGLImpl instance",
                                                   SamplerGLImpl)(this, SamplerDesc, bIsDeviceInternal));
            pSamplerOGL->QueryInterface(IID_Sampler, reinterpret_cast<IObject **>(ppSampler));
            OnCreateDeviceObject(pSamplerOGL);
            m_SamplersRegistry.Add(SamplerDesc, *ppSampler);
        }
    });
}

void RenderDeviceGLImpl::CreateSampler(const SamplerDesc & SamplerDesc, ISampler ** ppSampler)
{
    CreateSampler(SamplerDesc, ppSampler, false);
}

void RenderDeviceGLImpl::CreatePipelineState(const PipelineStateDesc & PipelineDesc,
                                             IPipelineState **         ppPipelineState)
{
    CreatePipelineState(PipelineDesc, ppPipelineState, false);
}

void RenderDeviceGLImpl::CreatePipelineState(const PipelineStateDesc & PipelineDesc,
                                             IPipelineState ** ppPipelineState, bool bIsDeviceInternal)
{
    CreateDeviceObject("Pipeline state", PipelineDesc, ppPipelineState, [&]() {
        PipelineStateGLImpl * pPipelineStateOGL(
            NEW_RC_OBJ(m_PSOAllocator, "PipelineStateGLImpl instance",
                       PipelineStateGLImpl)(this, PipelineDesc, bIsDeviceInternal));
        pPipelineStateOGL->QueryInterface(IID_PipelineState, reinterpret_cast<IObject **>(ppPipelineState));
        OnCreateDeviceObject(pPipelineStateOGL);
    });
}

void RenderDeviceGLImpl::CreateFence(const FenceDesc & Desc, IFence ** ppFence)
{
    CreateDeviceObject("Fence", Desc, ppFence, [&]() {
        FenceGLImpl * pFenceOGL(
            NEW_RC_OBJ(m_FenceAllocator, "FenceGLImpl instance", FenceGLImpl)(this, Desc));
        pFenceOGL->QueryInterface(IID_Fence, reinterpret_cast<IObject **>(ppFence));
        OnCreateDeviceObject(pFenceOGL);
    });
}

void RenderDeviceGLImpl::TestTextureFormat(TEXTURE_FORMAT TexFormat)
{
    auto & TexFormatInfo = m_TextureFormatsInfo[TexFormat];
    VERIFY(TexFormatInfo.Supported, "Texture format is not supported");

    auto GLFmt = TexFormatToGLInternalTexFormat(TexFormat);
    VERIFY(GLFmt != 0, "Incorrect internal GL format");

    auto spDeviceContext = GetImmediateContext();
    VERIFY(spDeviceContext, "Immediate device context has been destroyed");
    auto * pContextGL   = spDeviceContext.RawPtr<DeviceContextGLImpl>();
    auto & ContextState = pContextGL->GetContextState();

    const int TestTextureDim   = 32;
    const int TestTextureDepth = 8;

    // Create test texture 1D
    TexFormatInfo.Tex1DFmt = false;
    if(m_DeviceCaps.TexCaps.bTexture1DSupported && TexFormatInfo.ComponentType != COMPONENT_TYPE_COMPRESSED)
    {
        GLObjectWrappers::GLTextureObj TestGLTex(true);
        TexFormatInfo.Tex1DFmt = CreateTestGLTexture(ContextState, GL_TEXTURE_1D, TestGLTex, [&]() {
            glTexStorage1D(GL_TEXTURE_1D, 1, GLFmt, TestTextureDim);
        });
    }

    // Create test texture 2D
    TexFormatInfo.Tex2DFmt        = false;
    TexFormatInfo.TexCubeFmt      = false;
    TexFormatInfo.ColorRenderable = false;
    TexFormatInfo.DepthRenderable = false;
    {
        GLObjectWrappers::GLTextureObj TestGLTex(true);
        TexFormatInfo.Tex2DFmt = CreateTestGLTexture(ContextState, GL_TEXTURE_2D, TestGLTex, [&]() {
            glTexStorage2D(GL_TEXTURE_2D, 1, GLFmt, TestTextureDim, TestTextureDim);
        });

        if(TexFormatInfo.Tex2DFmt)
        {
            {
                GLObjectWrappers::GLTextureObj TestGLCubeTex(true);
                TexFormatInfo.TexCubeFmt =
                    CreateTestGLTexture(ContextState, GL_TEXTURE_CUBE_MAP, TestGLCubeTex, [&]() {
                        glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, GLFmt, TestTextureDim, TestTextureDim);
                    });
            }

            bool bTestDepthAttachment = TexFormatInfo.ComponentType == COMPONENT_TYPE_DEPTH
                                        || TexFormatInfo.ComponentType == COMPONENT_TYPE_DEPTH_STENCIL;
            bool bTestColorAttachment =
                !bTestDepthAttachment && TexFormatInfo.ComponentType != COMPONENT_TYPE_COMPRESSED;

            GLObjectWrappers::GLFrameBufferObj NewFBO(false);

            GLint CurrentFramebuffer = -1;
            if(bTestColorAttachment || bTestDepthAttachment)
            {
                glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &CurrentFramebuffer);
                CHECK_GL_ERROR("Failed to get current framebuffer");

                NewFBO.Create();
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, NewFBO);
                CHECK_GL_ERROR("Failed to bind the framebuffer");
            }

            if(bTestDepthAttachment)
            {
                GLenum Attachment = TexFormatInfo.ComponentType == COMPONENT_TYPE_DEPTH
                                        ? GL_DEPTH_ATTACHMENT
                                        : GL_DEPTH_STENCIL_ATTACHMENT;
                glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, Attachment, GL_TEXTURE_2D, TestGLTex, 0);
                if(glGetError() == GL_NO_ERROR)
                {
                    // Create dummy texture2D since some older version do not allow depth only
                    // attachments
                    GLObjectWrappers::GLTextureObj ColorTex(true);
                    bool Success = CreateTestGLTexture(ContextState, GL_TEXTURE_2D, ColorTex, [&]() {
                        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, TestTextureDim, TestTextureDim);
                    });
                    VERIFY(Success, "Failed to create dummy render target texture");
                    (void)Success;
                    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTex,
                                           0);
                    CHECK_GL_ERROR("Failed to set bind dummy render target to framebuffer");

                    static const GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0};
                    glDrawBuffers(_countof(DrawBuffers), DrawBuffers);
                    CHECK_GL_ERROR("Failed to set draw buffers via glDrawBuffers()");

                    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                    TexFormatInfo.DepthRenderable =
                        (glGetError() == GL_NO_ERROR) && (Status == GL_FRAMEBUFFER_COMPLETE);
                }
            }
            else if(bTestColorAttachment)
            {
                glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TestGLTex,
                                       0);
                if(glGetError() == GL_NO_ERROR)
                {
                    static const GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0};
                    glDrawBuffers(_countof(DrawBuffers), DrawBuffers);
                    CHECK_GL_ERROR("Failed to set draw buffers via glDrawBuffers()");

                    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
                    TexFormatInfo.ColorRenderable =
                        (glGetError() == GL_NO_ERROR) && (Status == GL_FRAMEBUFFER_COMPLETE);
                }
            }

            if(bTestColorAttachment || bTestDepthAttachment)
            {
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, CurrentFramebuffer);
                CHECK_GL_ERROR("Failed to bind the framebuffer");
            }
        }
    }

    TexFormatInfo.SampleCounts = 0x01;
    if(TexFormatInfo.ComponentType != COMPONENT_TYPE_COMPRESSED && m_DeviceCaps.TexCaps.bTexture2DMSSupported)
    {
#if GL_ARB_texture_storage_multisample
        for(GLsizei SampleCount = 2; SampleCount <= 8; SampleCount *= 2)
        {
            GLObjectWrappers::GLTextureObj TestGLTex(true);
            auto                           SampleCountSupported =
                CreateTestGLTexture(ContextState, GL_TEXTURE_2D_MULTISAMPLE, TestGLTex, [&]() {
                    glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, SampleCount, GLFmt, TestTextureDim,
                                              TestTextureDim, GL_TRUE);
                });
            if(SampleCountSupported)
                TexFormatInfo.SampleCounts |= SampleCount;
        }
#endif
    }

    // Create test texture 3D
    TexFormatInfo.Tex3DFmt = false;
    // 3D textures do not support depth formats
    if(!(TexFormatInfo.ComponentType == COMPONENT_TYPE_DEPTH
         || TexFormatInfo.ComponentType == COMPONENT_TYPE_DEPTH_STENCIL))
    {
        GLObjectWrappers::GLTextureObj TestGLTex(true);
        TexFormatInfo.Tex3DFmt = CreateTestGLTexture(ContextState, GL_TEXTURE_3D, TestGLTex, [&]() {
            glTexStorage3D(GL_TEXTURE_3D, 1, GLFmt, TestTextureDim, TestTextureDim, TestTextureDepth);
        });
    }
}

void RenderDeviceGLImpl ::QueryDeviceCaps()
{
    if(glPolygonMode == nullptr)
        m_DeviceCaps.bWireframeFillSupported = false;

    if(m_DeviceCaps.bWireframeFillSupported)
    {
        // Test glPolygonMode() function to check if it fails
        // (It does fail on NVidia Shield tablet, but works fine
        // on Intel hw)
        VERIFY(glGetError() == GL_NO_ERROR, "Unhandled gl error encountered");
        m_DeviceCaps.bWireframeFillSupported = True;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if(glGetError() != GL_NO_ERROR)
            m_DeviceCaps.bWireframeFillSupported = False;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if(glGetError() != GL_NO_ERROR)
            m_DeviceCaps.bWireframeFillSupported = False;
    }
}

FBOCache & RenderDeviceGLImpl::GetFBOCache(GLContext::NativeGLContextType Context)
{
    ThreadingTools::LockHelper FBOCacheLock(m_FBOCacheLockFlag);
    return m_FBOCache[Context];
}

void RenderDeviceGLImpl::OnReleaseTexture(ITexture * pTexture)
{
    ThreadingTools::LockHelper FBOCacheLock(m_FBOCacheLockFlag);
    for(auto & FBOCacheIt : m_FBOCache)
        FBOCacheIt.second.OnReleaseTexture(pTexture);
}

VAOCache & RenderDeviceGLImpl::GetVAOCache(GLContext::NativeGLContextType Context)
{
    ThreadingTools::LockHelper VAOCacheLock(m_VAOCacheLockFlag);
    return m_VAOCache[Context];
}

void RenderDeviceGLImpl::OnDestroyPSO(IPipelineState * pPSO)
{
    ThreadingTools::LockHelper VAOCacheLock(m_VAOCacheLockFlag);
    for(auto & VAOCacheIt : m_VAOCache)
        VAOCacheIt.second.OnDestroyPSO(pPSO);
}

void RenderDeviceGLImpl::OnDestroyBuffer(IBuffer * pBuffer)
{
    ThreadingTools::LockHelper VAOCacheLock(m_VAOCacheLockFlag);
    for(auto & VAOCacheIt : m_VAOCache)
        VAOCacheIt.second.OnDestroyBuffer(pBuffer);
}

void RenderDeviceGLImpl::IdleGPU()
{
    glFinish();
}

}   // namespace Diligent