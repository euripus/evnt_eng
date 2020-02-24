#include "glcontextstate.h"
#include "asyncwritableresource.h"
#include "gldebug.h"
#include "renderdeviceglimpl.h"
#include "textureviewglimpl.h"
#include "typeconversions.h"

namespace evnt
{

//GLContextState::GLContextState(RenderDeviceGLImpl * p_device_gl)
//{
//    const DeviceCaps & device_caps         = p_device_gl->getDeviceCaps();
//    m_caps.m_fill_mode_selection_supported = device_caps.is_wireframe_fill_supported;

//    {
//        m_caps.m_max_combined_tex_units = 0;
//        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_caps.m_max_combined_tex_units);
//        CHECK_GL_ERROR("Failed to get max combined tex image units count");
//        assert(m_caps.m_max_combined_tex_units > 0);

//        m_caps.m_max_draw_buffers = 0;
//        glGetIntegerv(GL_MAX_DRAW_BUFFERS, &m_caps.m_max_draw_buffers);
//        CHECK_GL_ERROR("Failed to get max draw buffers count");
//        assert(m_caps.m_max_draw_buffers > 0);
//    }

//    m_bound_textures.reserve(m_caps.m_max_combined_tex_units);
//    m_bound_samplers.reserve(32);
//    m_bound_images.reserve(32);

//    invalidate();

//    m_current_gl_context = p_device_gl->m_gl_context.GetCurrentNativeGLContext();
//}

//void GLContextState::invalidate()
//{
//#if !PLATFORM_ANDROID
//    // On Android this results in OpenGL error, so we will not
//    // clear the barriers. All the required barriers will be
//    // executed next frame when needed
//    if(m_pending_memory_barriers != 0)
//        ensureMemoryBarrier(m_pending_memory_barriers);
//    m_pending_memory_barriers = 0;
//#endif

//    // Unity messes up at least VAO left in the context,
//    // so unbid what we bound
//    glUseProgram(0);
//    glBindProgramPipeline(0);
//    glBindVertexArray(0);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
//    CHECK_GL_ERROR("Failed to reset GL context state");

//    m_prog_id     = static_cast<GLuint>(-1);
//    m_pipeline_id = static_cast<GLuint>(-1);
//    m_vao_id      = static_cast<GLuint>(-1);
//    m_fbo_id      = static_cast<GLuint>(-1);

//    m_bound_textures.clear();
//    m_bound_samplers.clear();
//    m_bound_images.clear();

//    m_ds_state = DepthStencilGLState();
//    m_rs_state = RasterizerGLState();

//    for(unsigned int rt = 0; rt < std::size(m_color_write_masks); ++rt)
//        m_color_write_masks[rt] = 0xFF;

//    m_independent_write_masks = EnableStateHelper();

//    m_active_texture     = -1;
//    m_num_patch_vertices = -1;
//}

//bool UpdateBoundObject(GLuint & current_object_id, GLuint & new_handle)
//{
//    if(current_object_id != new_handle)
//    {
//        current_object_id = new_handle;
//        return true;
//    }
//    return false;
//}

//template<class ObjectType>
//bool UpdateBoundObjectsArr(std::vector<ObjectType> & bound_object_ids, uint32_t index, GLuint & new_handle)
//{
//    if(index >= bound_object_ids.size())
//        bound_object_ids.resize(index + 1, static_cast<ObjectType>(-1));

//    return UpdateBoundObject(bound_object_ids[index], new_handle);
//}

//void GLContextState::setProgram(const GLObjectWrappers::GLProgramObj & program)
//{
//    GLuint prog_handle = program;
//    if(UpdateBoundObject(m_prog_id, prog_handle))
//    {
//        glUseProgram(prog_handle);
//        CHECK_GL_ERROR("Failed to set GL program");
//    }
//}

//void GLContextState::setPipeline(const GLObjectWrappers::GLPipelineObj & pipeline)
//{
//    GLuint handle = pipeline;
//    if(UpdateBoundObject(m_pipeline_id, handle))
//    {
//        glBindProgramPipeline(handle);
//        CHECK_GL_ERROR("Failed to bind program pipeline");
//    }
//}

//void GLContextState::bindVAO(const GLObjectWrappers::GLVertexArrayObj & vao)
//{
//    GLuint handle = vao;
//    if(UpdateBoundObject(m_vao_id, handle))
//    {
//        glBindVertexArray(handle);
//        CHECK_GL_ERROR("Failed to set VAO");
//    }
//}

//void GLContextState::bindFBO(const GLObjectWrappers::GLFrameBufferObj & fbo)
//{
//    GLuint handle = fbo;
//    if(UpdateBoundObject(m_fbo_id, handle))
//    {
//        // Even though the write mask only applies to writes to a framebuffer, the mask state is NOT
//        // Framebuffer state. So it is NOT part of a Framebuffer Object or the Default Framebuffer.
//        // Binding a new framebuffer will NOT affect the mask.

//        // NOTE: if attachment image is a NON-immutable format texture and the selected
//        // level is NOT level_base, the texture MUST BE MIPMAP COMPLETE
//        // If image is part of a cubemap texture, the texture must also be mipmap cube complete.
//        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);
//        CHECK_GL_ERROR("Failed to bind FBO as draw framebuffer");
//        glBindFramebuffer(GL_READ_FRAMEBUFFER, handle);
//        CHECK_GL_ERROR("Failed to bind FBO as read framebuffer");
//    }
//}

//void GLContextState::setActiveTexture(int32_t index)
//{
//    if(index < 0)
//    {
//        index += m_caps.m_max_combined_tex_units;
//    }
//    assert(0 <= index && index < m_caps.m_max_combined_tex_units);

//    if(m_active_texture != index)
//    {
//        glActiveTexture(GL_TEXTURE0 + index);
//        CHECK_GL_ERROR("Failed to activate texture slot ", index);
//        m_active_texture = index;
//    }
//}

//void GLContextState::bindTexture(int32_t index, GLenum bind_target,
//                                 const GLObjectWrappers::GLTextureObj & tex)
//{
//    if(index < 0)
//    {
//        index += m_caps.m_max_combined_tex_units;
//    }
//    assert(0 <= index && index < m_caps.m_max_combined_tex_units);

//    // Always update active texture unit
//    setActiveTexture(index);

//    GLuint tex_handle = tex;
//    if(UpdateBoundObjectsArr(m_bound_textures, index, tex_handle))
//    {
//        glBindTexture(bind_target, tex_handle);
//        CHECK_GL_ERROR("Failed to bind texture to slot ", index);
//    }
//}

//void GLContextState::bindSampler(uint32_t index, const GLObjectWrappers::GLSamplerObj & sampler)
//{
//    GLuint sampler_handle = sampler;
//    if(UpdateBoundObjectsArr(m_bound_samplers, index, sampler_handle))
//    {
//        glBindSampler(index, sampler_handle);
//        CHECK_GL_ERROR("Failed to bind sampler to slot ", index);
//    }
//}

//void GLContextState::bindImage(uint32_t index, class TextureViewGLImpl * tex_view, GLint mip_level,
//                               GLboolean is_layered, GLint layer, GLenum access, GLenum format)
//{
//#if GL_ARB_shader_image_load_store
//    BoundImageInfo new_image_info(mip_level, is_layered, layer, access, format);
//    if(index >= m_bound_images.size())
//        m_bound_images.resize(index + 1);
//    if(!(m_bound_images[index] == new_image_info))
//    {
//        m_bound_images[index] = new_image_info;
//        GLint gl_tex_handle   = tex_view->getHandle();
//        glBindImageTexture(index, gl_tex_handle, mip_level, is_layered, layer, access, format);
//        CHECK_GL_ERROR("glBindImageTexture() failed");
//    }
//#else
//    UNEXPECTED("GL_ARB_shader_image_load_store is not supported");
//#endif
//}

//void GLContextState::ensureMemoryBarrier(uint32_t required_barriers, AsyncWritableResource * res)
//{
//#if GL_ARB_shader_image_load_store
//    // Every resource tracks its own pending memory barriers.
//    // Device context also tracks which barriers have not been executed
//    // When a resource with pending memory barrier flag is bound to the context,
//    // the context checks if the same flag is set in its own pending barriers.
//    // Thus a memory barrier is only executed if some resource required that barrier
//    // and it has not been executed yet. This is almost optimal strategy, but slightly
//    // imperfect as the following scenario shows:

//    // Draw 1: Barriers_A |= BARRIER_FLAG, Barrier_Ctx |= BARRIER_FLAG
//    // Draw 2: Barriers_B |= BARRIER_FLAG, Barrier_Ctx |= BARRIER_FLAG
//    // Draw 3: Bind B, execute BARRIER: Barriers_B = 0, Barrier_Ctx = 0 (Barriers_A == BARRIER_FLAG)
//    // Draw 4: Barriers_B |= BARRIER_FLAG, Barrier_Ctx |= BARRIER_FLAG
//    // Draw 5: Bind A, execute BARRIER, Barriers_A = 0, Barrier_Ctx = 0 (Barriers_B == BARRIER_FLAG)

//    // In the last draw call, barrier for resource A has already been executed when resource B was
//    // bound to the pipeline. Since Resource A has not been bound since then, its flag has not been
//    // cleared.
//    // This situation does not seem to be a problem though since a barier cannot be executed
//    // twice in any situation

//    uint32_t resource_pending_barriers = 0;
//    if(res)
//    {
//        // If resource is specified, only set up memory barriers
//        // that are required by the resource
//        resource_pending_barriers = res->GetPendingMemortBarriers();
//        required_barriers &= resource_pending_barriers;
//    }

//    // Leave only pending barriers
//    required_barriers &= m_pending_memory_barriers;
//    if(required_barriers)
//    {
//        glMemoryBarrier(required_barriers);
//        CHECK_GL_ERROR("glMemoryBarrier() failed");
//        m_pending_memory_barriers &= ~required_barriers;
//    }

//    // Leave only these barriers that are still pending
//    if(res)
//        res->ResetPendingMemoryBarriers(m_pending_memory_barriers & resource_pending_barriers);
//#else
//    UNEXPECTED("GL_ARB_shader_image_load_store is not supported");
//#endif
//}

//void GLContextState::setPendingMemoryBarriers(uint32_t pending_barriers)
//{
//    m_pending_memory_barriers |= pending_barriers;
//}

//void GLContextState::enableDepthTest(bool enable)
//{
//    if(m_ds_state.m_depth_enable_state != enable)
//    {
//        if(enable)
//        {
//            glEnable(GL_DEPTH_TEST);
//            CHECK_GL_ERROR("Failed to enable detph test");
//        }
//        else
//        {
//            glDisable(GL_DEPTH_TEST);
//            CHECK_GL_ERROR("Failed to disable detph test");
//        }
//        m_ds_state.m_depth_enable_state = enable;
//    }
//}

//void GLContextState::enableDepthWrites(bool enable)
//{
//    if(m_ds_state.m_depth_writes_enable_state != enable)
//    {
//        // If mask is non-zero, the depth buffer is enabled for writing; otherwise, it is disabled.
//        glDepthMask(enable ? 1 : 0);
//        CHECK_GL_ERROR("Failed to enale/disable depth writes");
//        m_ds_state.m_depth_writes_enable_state = enable;
//    }
//}

//void GLContextState::setDepthFunc(COMPARISON_FUNCTION cmp_func)
//{
//    if(m_ds_state.m_depth_cmp_func != cmp_func)
//    {
//        auto glcmp_func = CompareFuncToGLCompareFunc(cmp_func);
//        glDepthFunc(glcmp_func);
//        CHECK_GL_ERROR("Failed to set GL comparison function");
//        m_ds_state.m_depth_cmp_func = cmp_func;
//    }
//}

//void GLContextState::enableStencilTest(bool enable)
//{
//    if(m_ds_state.m_stencil_test_enable_state != enable)
//    {
//        if(enable)
//        {
//            glEnable(GL_STENCIL_TEST);
//            CHECK_GL_ERROR("Failed to enable stencil test");
//        }
//        else
//        {
//            glDisable(GL_STENCIL_TEST);
//            CHECK_GL_ERROR("Failed to disable stencil test");
//        }
//        m_ds_state.m_stencil_test_enable_state = enable;
//    }
//}

//void GLContextState::setStencilWriteMask(uint8_t stencil_write_mask)
//{
//    if(m_ds_state.m_stencil_write_mask != stencil_write_mask)
//    {
//        glStencilMask(stencil_write_mask);
//        m_ds_state.m_stencil_write_mask = stencil_write_mask;
//    }
//}

//void GLContextState::setStencilRef(GLenum face, int32_t ref)
//{
//    auto & face_stencil_op = m_ds_state.m_stencil_op_state[face == GL_FRONT ? 0 : 1];
//    auto   glstencil_func  = CompareFuncToGLCompareFunc(face_stencil_op.func);
//    glStencilFuncSeparate(face, glstencil_func, ref, face_stencil_op.mask);
//    CHECK_GL_ERROR("Failed to set stencil function");
//}

//void GLContextState::setStencilFunc(GLenum face, COMPARISON_FUNCTION func, int32_t ref, uint32_t mask)
//{
//    auto & face_stencil_op = m_ds_state.m_stencil_op_state[face == GL_FRONT ? 0 : 1];
//    if(face_stencil_op.func != func || face_stencil_op.ref != ref || face_stencil_op.mask != mask)
//    {
//        face_stencil_op.func = func;
//        face_stencil_op.ref  = ref;
//        face_stencil_op.mask = mask;

//        setStencilRef(face, ref);
//    }
//}

//void GLContextState::setStencilOp(GLenum face, STENCIL_OP stencil_fail_op, STENCIL_OP stencil_depth_fail_op,
//                                  STENCIL_OP stencil_pass_op)
//{
//    auto & face_stencil_op = m_ds_state.m_stencil_op_state[face == GL_FRONT ? 0 : 1];
//    if(face_stencil_op.stencil_fail_op != stencil_fail_op
//       || face_stencil_op.stencil_depth_fail_op != stencil_depth_fail_op
//       || face_stencil_op.stencil_pass_op != stencil_pass_op)
//    {
//        auto glsfail = StencilOp2GlStencilOp(stencil_fail_op);
//        auto dpfail  = StencilOp2GlStencilOp(stencil_depth_fail_op);
//        auto dppass  = StencilOp2GlStencilOp(stencil_pass_op);

//        glStencilOpSeparate(face, glsfail, dpfail, dppass);
//        CHECK_GL_ERROR("Failed to set stencil operation");

//        face_stencil_op.stencil_fail_op       = stencil_fail_op;
//        face_stencil_op.stencil_depth_fail_op = stencil_depth_fail_op;
//        face_stencil_op.stencil_pass_op       = stencil_pass_op;
//    }
//}

//void GLContextState::setFillMode(FILL_MODE fill_mode)
//{
//    if(m_caps.m_fill_mode_selection_supported)
//    {
//        if(m_rs_state.fill_mode != fill_mode)
//        {
//            if(glPolygonMode != nullptr)
//            {
//                auto PolygonMode = fill_mode == FILL_MODE_WIREFRAME ? GL_LINE : GL_FILL;
//                glPolygonMode(GL_FRONT_AND_BACK, PolygonMode);
//                CHECK_GL_ERROR("Failed to set polygon mode");
//            }
//            else
//            {
//                if(fill_mode != FILL_MODE_SOLID)
//                    Log::Log(Log::error, "This API/device only supports solid fill mode");
//            }

//            m_rs_state.fill_mode = fill_mode;
//        }
//    }
//    else
//    {
//        if(fill_mode == FILL_MODE_WIREFRAME)
//            Log::Log(Log::warning, "Wireframe fill mode is not supported on this device\n");
//    }
//}

//void GLContextState::setCullMode(CULL_MODE cull_mode)
//{
//    if(m_rs_state.cull_mode != cull_mode)
//    {
//        if(cull_mode == CULL_MODE_NONE)
//        {
//            glDisable(GL_CULL_FACE);
//            CHECK_GL_ERROR("Failed to disable face culling");
//        }
//        else
//        {
//            assert(cull_mode == CULL_MODE_FRONT || cull_mode == CULL_MODE_BACK);   // Unexpected cull mode
//            glEnable(GL_CULL_FACE);
//            CHECK_GL_ERROR("Failed to enable face culling");
//            auto cull_face = cull_mode == CULL_MODE_BACK ? GL_BACK : GL_FRONT;
//            glCullFace(cull_face);
//            CHECK_GL_ERROR("Failed to set cull face");
//        }

//        m_rs_state.cull_mode = cull_mode;
//    }
//}

//void GLContextState::setFrontFace(bool front_counter_clockwise)
//{
//    if(m_rs_state.front_counter_clockwise != front_counter_clockwise)
//    {
//        auto front_face = front_counter_clockwise ? GL_CCW : GL_CW;
//        glFrontFace(front_face);
//        CHECK_GL_ERROR("Failed to set front face");
//        m_rs_state.front_counter_clockwise = front_counter_clockwise;
//    }
//}

//void GLContextState::setDepthBias(float depth_bias, float slope_scaled_depth_bias)
//{
//    if(m_rs_state.depth_bias != depth_bias || m_rs_state.slope_scaled_depth_bias != slope_scaled_depth_bias)
//    {
//        if(depth_bias != 0 || slope_scaled_depth_bias != 0)
//        {
//            glEnable(GL_POLYGON_OFFSET_FILL);
//            CHECK_GL_ERROR("Failed to enable polygon offset fill");
//        }
//        else
//        {
//            glDisable(GL_POLYGON_OFFSET_FILL);
//            CHECK_GL_ERROR("Failed to disable polygon offset fill");
//        }

//        glPolygonOffset(slope_scaled_depth_bias, depth_bias);
//        CHECK_GL_ERROR("Failed to set polygon offset");

//        m_rs_state.depth_bias              = depth_bias;
//        m_rs_state.slope_scaled_depth_bias = slope_scaled_depth_bias;
//    }
//}

//void GLContextState::setDepthClamp(bool enable_depth_clamp)
//{
//    if(m_rs_state.depth_clamp_enable != enable_depth_clamp)
//    {
//        if(enable_depth_clamp)
//        {
//            if(GL_DEPTH_CLAMP)
//            {
//                glEnable(GL_DEPTH_CLAMP);
//                CHECK_GL_ERROR("Failed to enable depth clamp");
//            }
//        }
//        else
//        {
//            if(GL_DEPTH_CLAMP)
//            {
//                // WARNING: on OpenGL, depth clamping is disabled against
//                // both far and near clip planes. On DirectX, only clipping
//                // against far clip plane can be disabled
//                glDisable(GL_DEPTH_CLAMP);
//                CHECK_GL_ERROR("Failed to disable depth clamp");
//            }
//            else
//            {
//                Log::Log(Log::warning, "Disabling depth clamp is not supported");
//            }
//        }
//        m_rs_state.depth_clamp_enable = enable_depth_clamp;
//    }
//}

//void GLContextState::enableScissorTest(bool enable_scissor_test)
//{
//    if(m_rs_state.scissor_test_enable != enable_scissor_test)
//    {
//        if(enable_scissor_test)
//        {
//            glEnable(GL_SCISSOR_TEST);
//            CHECK_GL_ERROR("Failed to enable scissor test");
//        }
//        else
//        {
//            glDisable(GL_SCISSOR_TEST);
//            CHECK_GL_ERROR("Failed to disable scissor clamp");
//        }

//        m_rs_state.scissor_test_enable = enable_scissor_test;
//    }
//}

//void GLContextState::setBlendFactors(const float * blend_factors)
//{
//    glBlendColor(blend_factors[0], blend_factors[1], blend_factors[2], blend_factors[3]);
//    CHECK_GL_ERROR("Failed to set blend color");
//}

//void GLContextState::setBlendState(const BlendStateDesc & bs_dsc, uint32_t sample_mask)
//{
//    assert(sample_mask == 0xFFFFFFFF);   // Sample mask is not currently implemented in GL

//    bool enable_blend = false;
//    if(bs_dsc.independent_blend_enable)
//    {
//        for(int i = 0; i < bs_dsc.s_max_render_targets; ++i)
//        {
//            const auto & rt = bs_dsc.render_targets[i];
//            if(rt.blend_enable)
//                enable_blend = true;

//            if(i < m_caps.m_max_draw_buffers)
//            {
//                setColorWriteMask(i, rt.render_target_write_mask, true);
//            }
//            else
//            {
//                assert(rt.render_target_write_mask == RenderTargetBlendDesc{}.render_target_write_mask);
//            }
//        }
//    }
//    else
//    {
//        const auto & rt0 = bs_dsc.render_targets[0];
//        enable_blend     = rt0.blend_enable;
//        setColorWriteMask(0, rt0.render_target_write_mask, false);
//    }

//    if(enable_blend)
//    {
//        //  Sets the blend enable flag for ALL color buffers.
//        glEnable(GL_BLEND);
//        CHECK_GL_ERROR("Failed to enable alpha blending");

//        if(bs_dsc.alpha_to_coverage_enable)
//        {
//            glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
//            CHECK_GL_ERROR("Failed to enable alpha to coverage");
//        }
//        else
//        {
//            glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
//            CHECK_GL_ERROR("Failed to disable alpha to coverage");
//        }

//        if(bs_dsc.independent_blend_enable)
//        {
//            for(int i = 0; i < bs_dsc.s_max_render_targets; ++i)
//            {
//                const auto & rt = bs_dsc.render_targets[i];

//                if(i >= m_caps.m_max_draw_buffers)
//                {
//                    if(rt.blend_enable)
//                    {
//                        std::ostringstream ss;
//                        ss << "Blend is enabled for render target " << i << " but this device only supports "
//                           << m_caps.m_max_draw_buffers << " draw buffers";

//                        Log::Log(Log::error, ss.str());
//                    }
//                    continue;
//                }

//                if(rt.blend_enable)
//                {
//                    glEnablei(GL_BLEND, i);
//                    CHECK_GL_ERROR("Failed to enable alpha blending");

//                    auto src_factor_RGB   = BlendFactor2GLBlend(rt.src_blend);
//                    auto dst_factor_RGB   = BlendFactor2GLBlend(rt.dest_blend);
//                    auto src_factor_alpha = BlendFactor2GLBlend(rt.src_blend_alpha);
//                    auto dst_factor_alpha = BlendFactor2GLBlend(rt.dest_blend_alpha);
//                    glBlendFuncSeparatei(i, src_factor_RGB, dst_factor_RGB, src_factor_alpha,
//                                         dst_factor_alpha);
//                    CHECK_GL_ERROR("Failed to set separate blending factors");
//                    auto mode_RGB   = BlendOperation2GLBlendOp(rt.blend_op);
//                    auto mode_alpha = BlendOperation2GLBlendOp(rt.blend_op_alpha);
//                    glBlendEquationSeparatei(i, mode_RGB, mode_alpha);
//                    CHECK_GL_ERROR("Failed to set separate blending equations");
//                }
//                else
//                {
//                    glDisablei(GL_BLEND, i);
//                    CHECK_GL_ERROR("Failed to disable alpha blending");
//                }
//            }
//        }
//        else
//        {
//            const auto & rt0              = bs_dsc.render_targets[0];
//            auto         src_factor_RGB   = BlendFactor2GLBlend(rt0.src_blend);
//            auto         dst_factor_RGB   = BlendFactor2GLBlend(rt0.dest_blend);
//            auto         src_factor_alpha = BlendFactor2GLBlend(rt0.src_blend_alpha);
//            auto         dst_factor_alpha = BlendFactor2GLBlend(rt0.dest_blend_alpha);
//            glBlendFuncSeparate(src_factor_RGB, dst_factor_RGB, src_factor_alpha, dst_factor_alpha);
//            CHECK_GL_ERROR("Failed to set blending factors");

//            auto mode_RGB   = BlendOperation2GLBlendOp(rt0.blend_op);
//            auto mode_alpha = BlendOperation2GLBlendOp(rt0.blend_op_alpha);
//            glBlendEquationSeparate(mode_RGB, mode_alpha);
//            CHECK_GL_ERROR("Failed to set blending equations");
//        }
//    }
//    else
//    {
//        //  Sets the blend disable flag for ALL color buffers.
//        glDisable(GL_BLEND);
//        CHECK_GL_ERROR("Failed to disable alpha blending");
//    }
//}

//void GLContextState::getColorWriteMask(uint32_t rt_index, uint32_t & write_mask, bool & is_independent)
//{
//    if(!m_independent_write_masks)
//        rt_index = 0;

//    write_mask     = m_color_write_masks[rt_index];
//    is_independent = m_independent_write_masks;
//}

//void GLContextState::setColorWriteMask(uint32_t rt_index, uint32_t write_mask, bool is_independent)
//{
//    // Even though the write mask only applies to writes to a framebuffer, the mask state is NOT
//    // Framebuffer state. So it is NOT part of a Framebuffer Object or the Default Framebuffer.
//    // Binding a new framebuffer will NOT affect the mask.

//    if(!is_independent)
//        rt_index = 0;

//    if(m_color_write_masks[rt_index] != write_mask || m_independent_write_masks != is_independent)
//    {
//        if(is_independent)
//        {
//            // Note that glColorMaski() does not set color mask for the framebuffer
//            // attachment point RTIndex. Rather it sets the mask for what was set
//            // by the glDrawBuffers() function for the i-th output
//            glColorMaski(rt_index, (write_mask & COLOR_MASK_RED) ? GL_TRUE : GL_FALSE,
//                         (write_mask & COLOR_MASK_GREEN) ? GL_TRUE : GL_FALSE,
//                         (write_mask & COLOR_MASK_BLUE) ? GL_TRUE : GL_FALSE,
//                         (write_mask & COLOR_MASK_ALPHA) ? GL_TRUE : GL_FALSE);
//            CHECK_GL_ERROR("Failed to set GL color mask");

//            m_color_write_masks[rt_index] = write_mask;
//        }
//        else
//        {
//            // glColorMask() sets the mask for ALL draw buffers
//            glColorMask((write_mask & COLOR_MASK_RED) ? GL_TRUE : GL_FALSE,
//                        (write_mask & COLOR_MASK_GREEN) ? GL_TRUE : GL_FALSE,
//                        (write_mask & COLOR_MASK_BLUE) ? GL_TRUE : GL_FALSE,
//                        (write_mask & COLOR_MASK_ALPHA) ? GL_TRUE : GL_FALSE);
//            CHECK_GL_ERROR("Failed to set GL color mask");

//            for(unsigned int rt = 0; rt < std::size(m_color_write_masks); ++rt)
//                m_color_write_masks[rt] = write_mask;
//        }
//        m_independent_write_masks = is_independent;
//    }
//}

//void GLContextState::setNumPatchVertices(int32_t num_vertices)
//{
//#if GL_ARB_tessellation_shader
//    if(num_vertices != m_num_patch_vertices)
//    {
//        m_num_patch_vertices = num_vertices;
//        glPatchParameteri(GL_PATCH_VERTICES, static_cast<GLint>(num_vertices));
//        CHECK_GL_ERROR("Failed to set the number of patch vertices");
//    }
//#endif
//}
}   // namespace evnt
