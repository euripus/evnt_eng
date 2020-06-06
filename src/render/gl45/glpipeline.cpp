#include "glpipeline.h"
#include "typeconversions.h"

namespace evnt
{
GLPipeline::GLPipeline() {}

void GLPipeline::bindPipeline()
{
    if(!m_keep_shader)
        glUseProgram(shader->getID());
    // Vertex Stage
    if(!vertex_stage._keep_attributes)
        glBindVertexArray(vertex_stage.vertex_array->getID());

    if(vertex_stage.front_face_culling || vertex_stage.back_face_culling)
    {
        glEnable(GL_CULL_FACE);
        if(vertex_stage.front_face_culling && vertex_stage.back_face_culling)
            glCullFace(GL_FRONT_AND_BACK);
        else if(vertex_stage.front_face_culling)
            glCullFace(GL_FRONT);
        else if(vertex_stage.back_face_culling)
            glCullFace(GL_BACK);
    }
    else
        glDisable(GL_CULL_FACE);

    if(vertex_stage.depth_clamping)
        glEnable(GL_DEPTH_CLAMP);
    else
        glDisable(GL_DEPTH_CLAMP);

    // Tesselation Stage
    glPatchParameteri(GL_PATCH_VERTICES, tessellation_stage.patch_size);

    // Transform Feedback Stage
    if(!transform_feedback_stage._keep_bindings)
        for(auto & buffers : transform_feedback_stage.buffer_bindings)
            glBindBufferBase(GL_TRANSFORM_FEEDBACK, buffers.first, buffers.second->getID());

    // Fragment Stage
    if(!fragment_stage._keep_fbo)
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fragment_stage.framebuffer->getID());

    if(fragment_stage.scissor_test)
    {
        glEnable(GL_SCISSOR_TEST);
        glScissor(static_cast<GLint>(fragment_stage.scissor_region.m_position.x),
                  static_cast<GLint>(fragment_stage.scissor_region.m_position.y),
                  static_cast<GLsizei>(fragment_stage.scissor_region.m_size.x),
                  static_cast<GLsizei>(fragment_stage.scissor_region.m_size.y));
    }
    else
        glDisable(GL_SCISSOR_TEST);

    if(fragment_stage.rasterizer_discard)
        glEnable(GL_RASTERIZER_DISCARD);
    else
        glDisable(GL_RASTERIZER_DISCARD);

    if(fragment_stage.depth_test)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if(fragment_stage.stencil_test)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);

    glDepthMask(fragment_stage.depth_mask);
    glStencilMaskSeparate(GL_FRONT, fragment_stage.front_stencil_mask);
    glStencilMaskSeparate(GL_BACK, fragment_stage.back_stencil_mask);
    for(ColorMask mask : fragment_stage.color_masks)
        glColorMaski(mask.buffer, mask.red, mask.green, mask.blue, mask.alpha);

    /*
    glStencilFuncSeparate(static_cast<GLenum>(this->FragmentStage.FrontStencilFunc),
    static_cast<GLenum>(this->FragmentStage.BackStencilFunc), this->FragmentStage.StencilCompareReference,
    this->FragmentStage.StencilCompareMask);

    glStencilOpSeparate(GL_FRONT, static_cast<GLenum>(this->FragmentStage.FrontStencilFail),
                        static_cast<GLenum>(this->FragmentStage.FrontDepthFail),
                        static_cast<GLenum>(this->FragmentStage.FrontDepthPass));

    glStencilOpSeparate(GL_BACK, static_cast<GLenum>(this->FragmentStage.BackStencilFail),
                        static_cast<GLenum>(this->FragmentStage.BackDepthFail),
                        static_cast<GLenum>(this->FragmentStage.BackDepthPass));*/

    if(fragment_stage.blending)
    {
        glEnable(GL_BLEND);
        for(BufferBlending blending : fragment_stage.buffer_blends)
        {
            glBlendFuncSeparatei(blending.buffer, BlendFactorToGLBlend(blending.source_color),
                                 BlendFactorToGLBlend(blending.destination_color),
                                 BlendFactorToGLBlend(blending.source_alpha),
                                 BlendFactorToGLBlend(blending.destination_alpha));
            glBlendEquationSeparatei(blending.buffer, BlendOperationToGLBlendOp(blending.color_equation),
                                     BlendOperationToGLBlendOp(blending.alpha_equation));
        }
    }
    else
        glDisable(GL_BLEND);

    glViewport(static_cast<GLint>(fragment_stage.viewport.m_position.x),
               static_cast<GLint>(fragment_stage.viewport.m_position.y),
               static_cast<GLsizei>(fragment_stage.viewport.m_size.x),
               static_cast<GLsizei>(fragment_stage.viewport.m_size.y));

    if(!fragment_stage._keep_targets && fragment_stage.framebuffer->getID() != 0)
    {
        std::vector<GLenum> drawTargets;
        for(int32_t target : fragment_stage.draw_targets)
            drawTargets.push_back(GL_COLOR_ATTACHMENT0 + target);
        glDrawBuffers(static_cast<GLsizei>(fragment_stage.draw_targets.size()), drawTargets.data());
    }
}
}   // namespace evnt
