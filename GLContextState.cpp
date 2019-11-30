/*     Copyright 2015-2019 Egor Yusov
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

#include "GLContextState.h"
#include "TextureBaseGL.h"
#include "SamplerGLImpl.h"
#include "AsyncWritableResource.h"
#include "GLTypeConversions.h"
#include "BufferViewGLImpl.h"
#include "RenderDeviceGLImpl.h"

using namespace GLObjectWrappers;
using namespace Diligent;

namespace Diligent
{
    GLContextState::GLContextState( RenderDeviceGLImpl *pDeviceGL )
    {
        const DeviceCaps &DeviceCaps = pDeviceGL->GetDeviceCaps();
        m_Caps.bFillModeSelectionSupported = DeviceCaps.bWireframeFillSupported;

        {
            m_Caps.m_iMaxCombinedTexUnits = 0;
            glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_Caps.m_iMaxCombinedTexUnits );
            CHECK_GL_ERROR( "Failed to get max combined tex image units count" );
            VERIFY_EXPR(m_Caps.m_iMaxCombinedTexUnits > 0);

            m_Caps.m_iMaxDrawBuffers = 0;
            glGetIntegerv( GL_MAX_DRAW_BUFFERS, &m_Caps.m_iMaxDrawBuffers );
            CHECK_GL_ERROR( "Failed to get max draw buffers count" );
            VERIFY_EXPR(m_Caps.m_iMaxDrawBuffers > 0);
        }

        m_BoundTextures.reserve( m_Caps.m_iMaxCombinedTexUnits );
        m_BoundSamplers.reserve( 32 );
        m_BoundImages.reserve( 32 );

        Invalidate();

        m_CurrentGLContext = pDeviceGL->m_GLContext.GetCurrentNativeGLContext();
    }

    void GLContextState::Invalidate()
    {
#if !PLATFORM_ANDROID
        // On Android this results in OpenGL error, so we will not
        // clear the barriers. All the required barriers will be
        // executed next frame when needed
        if(m_PendingMemoryBarriers != 0)
            EnsureMemoryBarrier(m_PendingMemoryBarriers);
        m_PendingMemoryBarriers = 0;
#endif

        // Unity messes up at least VAO left in the context,
        // so unbid what we bound
        glUseProgram( 0 );
        glBindProgramPipeline( 0 );
        glBindVertexArray( 0 );
        glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
        glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
        CHECK_GL_ERROR( "Failed to reset GL context state" );

        m_GLProgId = -1;
        m_GLPipelineId = -1;
        m_VAOId = -1;
        m_FBOId = -1;
        
        m_BoundTextures.clear();
        m_BoundSamplers.clear();
        m_BoundImages.clear();

        m_DSState = DepthStencilGLState();
        m_RSState = RasterizerGLState();

        for( Uint32 rt = 0; rt < _countof( m_ColorWriteMasks ); ++rt )
            m_ColorWriteMasks[rt] = 0xFF;

        m_bIndependentWriteMasks = EnableStateHelper();

        m_iActiveTexture = -1;
        m_NumPatchVertices = -1;
    }

    void GLContextState::BindImage( Uint32 Index,
        TextureViewGLImpl *pTexView,
        GLint MipLevel,
        GLboolean IsLayered,
        GLint Layer,
        GLenum Access,
        GLenum Format )
    {
#if GL_ARB_shader_image_load_store
        BoundImageInfo NewImageInfo(
            pTexView->GetUniqueID(),
            MipLevel,
            IsLayered,
            Layer,
            Access,
            Format
            );
        if( Index >= m_BoundImages.size() )
            m_BoundImages.resize( Index + 1 );
        if( !(m_BoundImages[Index] == NewImageInfo) )
        {
            m_BoundImages[Index] = NewImageInfo;
            GLint GLTexHandle = pTexView->GetHandle();
            glBindImageTexture( Index, GLTexHandle, MipLevel, IsLayered, Layer, Access, Format );
            CHECK_GL_ERROR( "glBindImageTexture() failed" );
        }
#else
        UNSUPPORTED("GL_ARB_shader_image_load_store is not supported");
#endif
    }

    void GLContextState::EnsureMemoryBarrier( Uint32 RequiredBarriers, AsyncWritableResource *pRes/* = nullptr */ )
    {
#if GL_ARB_shader_image_load_store
        // Every resource tracks its own pending memory barriers.
        // Device context also tracks which barriers have not been executed
        // When a resource with pending memory barrier flag is bound to the context,
        // the context checks if the same flag is set in its own pending barriers.
        // Thus a memory barrier is only executed if some resource required that barrier
        // and it has not been executed yet. This is almost optimal strategy, but slightly
        // imperfect as the following scenario shows:

        // Draw 1: Barriers_A |= BARRIER_FLAG, Barrier_Ctx |= BARRIER_FLAG
        // Draw 2: Barriers_B |= BARRIER_FLAG, Barrier_Ctx |= BARRIER_FLAG
        // Draw 3: Bind B, execute BARRIER: Barriers_B = 0, Barrier_Ctx = 0 (Barriers_A == BARRIER_FLAG)
        // Draw 4: Barriers_B |= BARRIER_FLAG, Barrier_Ctx |= BARRIER_FLAG
        // Draw 5: Bind A, execute BARRIER, Barriers_A = 0, Barrier_Ctx = 0 (Barriers_B == BARRIER_FLAG)

        // In the last draw call, barrier for resource A has already been executed when resource B was
        // bound to the pipeline. Since Resource A has not been bound since then, its flag has not been
        // cleared.
        // This situation does not seem to be a problem though since a barier cannot be executed
        // twice in any situation

        Uint32 ResourcePendingBarriers = 0;
        if( pRes )
        {
            // If resource is specified, only set up memory barriers 
            // that are required by the resource
            ResourcePendingBarriers = pRes->GetPendingMemortBarriers();
            RequiredBarriers &= ResourcePendingBarriers;
        }

        // Leave only pending barriers
        RequiredBarriers &= m_PendingMemoryBarriers;
        if( RequiredBarriers )
        {
            glMemoryBarrier( RequiredBarriers );
            CHECK_GL_ERROR( "glMemoryBarrier() failed" );
            m_PendingMemoryBarriers &= ~RequiredBarriers;
        }

        // Leave only these barriers that are still pending
        if( pRes )
            pRes->ResetPendingMemoryBarriers( m_PendingMemoryBarriers & ResourcePendingBarriers );
#else
        UNSUPPORTED("GL_ARB_shader_image_load_store is not supported");
#endif
    }
}
