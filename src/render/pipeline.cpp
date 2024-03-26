#include "pipeline.h"

namespace evnt
{
void Pipeline::optimize(Pipeline const & previous)
{
    // Minimize state changes
    if(shader->getID() == previous.shader->getID())
        m_keep_shader = true;

    if(vertex_stage.vertex_array->getID() == previous.vertex_stage.vertex_array->getID())
        vertex_stage._keep_attributes = true;

    if(transform_feedback_stage.buffer_bindings == previous.transform_feedback_stage.buffer_bindings)
        transform_feedback_stage._keep_bindings = true;

    if(fragment_stage.framebuffer->getID() == previous.fragment_stage.framebuffer->getID())
        fragment_stage._keep_fbo = true;

    if(fragment_stage.draw_targets == previous.fragment_stage.draw_targets)
        fragment_stage._keep_targets = true;
}
}   // namespace evnt
