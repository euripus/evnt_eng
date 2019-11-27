#ifndef GLCONTEXTSTATE_H
#define GLCONTEXTSTATE_H

#include "../blend.h"
#include "../depthstencilstate.h"
#include "../graphics_types.h"
#include "../rasterizerstate.h"
#include "globjwrapper.h"
#include <vector>

namespace evnt
{
class GLContextState
{
public:
    GLContextState();

    void setProgram(const GLObjectWrappers::GLProgramObj & program);
    void setPipeline(const GLObjectWrappers::GLPipelineObj & pipeline);
    void bindVAO(const GLObjectWrappers::GLVertexArrayObj & vao);
    void bindFBO(const GLObjectWrappers::GLFrameBufferObj & fbo);
    void setActiveTexture(int32_t index);
    void bindTexture(int32_t index, GLenum bind_target, const GLObjectWrappers::GLTextureObj & tex);
    void bindSampler(uint32_t index, const GLObjectWrappers::GLSamplerObj & sampler);
    void bindImage(uint32_t index, class TextureViewGLImpl * tex_view, GLint mip_level, GLboolean is_layered,
                   GLint layer, GLenum access, GLenum format);
    void ensureMemoryBarrier(uint32_t required_barriers, class AsyncWritableResource * res = nullptr);
    void setPendingMemoryBarriers(uint32_t pending_barriers);

    void enableDepthTest(bool enable);
    void enableDepthWrites(bool enable);
    void setDepthFunc(COMPARISON_FUNCTION cmp_func);
    void enableStencilTest(bool enable);
    void setStencilWriteMask(uint8_t stencil_writeMask);
    void setStencilRef(GLenum face, int32_t ref);
    void setStencilFunc(GLenum face, COMPARISON_FUNCTION func, int32_t ref, uint32_t mask);
    void setStencilOp(GLenum face, STENCIL_OP stencil_fail_op, STENCIL_OP stencil_depth_fail_op,
                      STENCIL_OP stencil_pass_op);

    void setFillMode(FILL_MODE fill_mode);
    void setCullMode(CULL_MODE cull_mode);
    void setFrontFace(bool front_counter_clockwise);
    void setDepthBias(float depth_bias, float slope_scaled_depth_bias);
    void setDepthClamp(bool enable_depth_clamp);
    void enableScissorTest(bool enable_scissor_test);

    void setBlendFactors(const float * blend_factors);
    void setBlendState(const BlendStateDesc & bs_dsc, uint32_t sample_mask);

    bool getDepthWritesEnabled() const { return m_ds_state.m_depth_writes_enable_state; }
    bool getScissorTestEnabled() const { return m_rs_state.scissor_test_enable; }
    void getColorWriteMask(uint32_t rt_index, uint32_t & write_mask, bool & is_independent);
    void setColorWriteMask(uint32_t rt_index, uint32_t write_mask, bool is_independent);

    void setNumPatchVertices(int32_t num_vertices);
    void invalidate();

    /*void SetCurrentGLContext(GLContext::NativeGLContextType Context) { m_CurrentGLContext = Context; }
    GLContext::NativeGLContextType GetCurrentGLContext() const { return m_CurrentGLContext; }*/

    struct ContextCaps
    {
        bool  m_fill_mode_selection_supported = true;
        GLint m_max_combined_tex_units        = 0;
        GLint m_max_draw_buffers              = 0;
    };
    const ContextCaps & GetContextCaps() { return m_caps; }

private:
    struct BoundImageInfo
    {
        GLint     mip_level  = 0;
        GLboolean is_layered = 0;
        GLint     layer      = 0;
        GLenum    access     = 0;
        GLenum    format     = 0;

        BoundImageInfo() {}

        BoundImageInfo(GLint _MipLevel, GLboolean _IsLayered, GLint _Layer, GLenum _Access, GLenum _Format) :
            mip_level(_MipLevel), is_layered(_IsLayered), layer(_Layer), access(_Access), format(_Format)
        {}

        bool operator==(const BoundImageInfo & rhs) const
        {
            return mip_level == rhs.mip_level && is_layered == rhs.is_layered && layer == rhs.layer
                   && access == rhs.access && format == rhs.format;
        }
    };

    class EnableStateHelper
    {
    public:
        enum class ENABLE_STATE : int32_t
        {
            UNKNOWN,
            ENABLED,
            DISABLED
        };

        bool operator==(bool enabled) const
        {
            return (enabled && m_enable_state == ENABLE_STATE::ENABLED)
                   || (!enabled && m_enable_state == ENABLE_STATE::DISABLED);
        }
        bool operator!=(bool enabled) const { return !(*this == enabled); }

        const EnableStateHelper & operator=(bool enabled)
        {
            m_enable_state = enabled ? ENABLE_STATE::ENABLED : ENABLE_STATE::DISABLED;
            return *this;
        }

        operator bool() const { return m_enable_state == ENABLE_STATE::ENABLED; }

    private:
        ENABLE_STATE m_enable_state = ENABLE_STATE::UNKNOWN;
    };

    struct DepthStencilGLState
    {
        EnableStateHelper   m_depth_enable_state;
        EnableStateHelper   m_depth_writes_enable_state;
        COMPARISON_FUNCTION m_depth_cmp_func = COMPARISON_FUNC_UNKNOWN;
        EnableStateHelper   m_stencil_test_enable_state;
        uint16_t            m_stencil_read_mask  = 0xFFFF;
        uint16_t            m_stencil_write_mask = 0xFFFF;
        struct StencilOpState
        {
            COMPARISON_FUNCTION func                  = COMPARISON_FUNC_UNKNOWN;
            STENCIL_OP          stencil_fail_op       = STENCIL_OP_UNDEFINED;
            STENCIL_OP          stencil_depth_fail_op = STENCIL_OP_UNDEFINED;
            STENCIL_OP          stencil_pass_op       = STENCIL_OP_UNDEFINED;
            int32_t             ref                   = std::numeric_limits<int32_t>::min();
            uint32_t            mask                  = static_cast<uint32_t>(-1);
        } m_StencilOpState[2];
    };

    struct RasterizerGLState
    {
        FILL_MODE         fill_mode = FILL_MODE_UNDEFINED;
        CULL_MODE         cull_mode = CULL_MODE_UNDEFINED;
        EnableStateHelper front_counter_clockwise;
        float             depth_bias              = std::numeric_limits<float>::max();
        float             slope_scaled_depth_bias = std::numeric_limits<float>::max();
        EnableStateHelper depth_clamp_enable;
        EnableStateHelper scissor_test_enable;
    };

    GLuint              m_prog_id     = static_cast<GLuint>(-1);
    GLuint              m_pipeline_id = static_cast<GLuint>(-1);
    GLuint              m_vao_id      = static_cast<GLuint>(-1);
    GLuint              m_fbo_id      = static_cast<GLuint>(-1);
    std::vector<GLuint> m_bound_textures;
    std::vector<GLuint> m_bound_samplers;

    std::vector<BoundImageInfo> m_bound_images;
    uint32_t                    m_pending_memory_barriers = 0;

    DepthStencilGLState m_ds_state;
    RasterizerGLState   m_rs_state;
    ContextCaps         m_caps;

    uint32_t m_color_write_masks[MaxRenderTargets] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    EnableStateHelper m_independent_write_masks;
    int32_t           m_active_texture     = -1;
    int32_t           m_num_patch_vertices = -1;

    // GLContext::NativeGLContextType m_CurrentGLContext = {};
};

}   // namespace evnt

#endif   // GLCONTEXTSTATE_H
