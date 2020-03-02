#ifndef PIPELINE_H
#define PIPELINE_H

#include "buffer.h"
#include "framebuffer.h"
#include "graphics_types.h"
#include "quad2d.h"
#include "shader.h"
#include "vertexarray.h"

namespace evnt
{
struct DrawArraysIndirectCommand
{
    uint32_t count;
    uint32_t instance_count;
    uint32_t first;
    uint32_t base_instance;
};

struct DrawElementsIndirectCommand
{
    uint32_t count;
    uint32_t instance_count;
    uint32_t first_index;
    uint32_t base_vertex;
    uint32_t base_instance;
};

struct ColorMask
{
    uint32_t buffer;
    bool     red;
    bool     green;
    bool     blue;
    bool     alpha;
};

struct BufferBlending
{
    uint32_t buffer;

    BlendFactor source_color;
    BlendFactor destination_color;
    BlendFactor source_alpha;
    BlendFactor destination_alpha;

    BlendOperation color_equation;
    BlendOperation alpha_equation;
};

class VertexStage
{
public:
    VertexStage() {}

    IVertexArray * vertex_array;

    bool front_face_culling = false;
    bool back_face_culling  = false;
    bool depth_clamping     = false;

    bool _keep_attributes = false;
};

class TessellationStage
{
public:
    TessellationStage() {}
    int patch_size = 1;
};

class FragmentStage
{
public:
    IFramebuffer *   framebuffer = nullptr;
    std::vector<int> draw_targets;

    IQuad2D viewport;
    IQuad2D scissor_region;
    bool    scissor_test       = false;
    bool    rasterizer_discard = false;
    bool    depth_test         = false;
    bool    stencil_test       = false;

    bool                   depth_mask = false;
    uint32_t               front_stencil_mask;
    uint32_t               back_stencil_mask;
    std::vector<ColorMask> color_masks;

    ComparisonFunc front_stencil_func = ComparisonFunc::COMPARISON_FUNC_LESS;
    ComparisonFunc back_stencil_func  = ComparisonFunc::COMPARISON_FUNC_LESS;
    uint32_t       stencil_compare_reference;
    uint32_t       stencil_compare_mask;

    StencilOperation front_stencil_fail;
    StencilOperation front_depth_fail;
    StencilOperation front_depth_pass;

    StencilOperation back_stencil_fail;
    StencilOperation back_depth_fail;
    StencilOperation back_depth_pass;

    bool                        blending = false;
    std::vector<BufferBlending> buffer_blends;

    bool _keep_fbo     = false;
    bool _keep_targets = false;
};

class TransformFeedbackStage
{
public:
    TransformFeedbackStage() {}
    std::vector<std::pair<int, IBuffer *>> buffer_bindings;

    bool _keep_bindings = false;
};

class Pipeline
{
public:
    Pipeline() = default;

    IShader *              shader = nullptr;
    VertexStage            vertex_stage;
    TessellationStage      tessellation_stage;
    TransformFeedbackStage transform_feedback_stage;
    FragmentStage          fragment_stage;

    void optimize(const Pipeline & previous);

    virtual void bindPipeline() = 0;

protected:
    bool m_keep_shader;
};
}   // namespace evnt
#endif   // PIPELINE_H
