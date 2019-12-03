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
}
