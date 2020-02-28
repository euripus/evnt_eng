#ifndef PIPELINE_H
#define PIPELINE_H

#include "graphics_types.h"
#include "vertexarray.h"

namespace evnt
{
struct DrawArraysIndirectCommand
{
    uint32_t Count;
    uint32_t InstanceCount;
    uint32_t First;
    uint32_t BaseInstance;
};

struct DrawElementsIndirectCommand
{
    uint32_t Count;
    uint32_t InstanceCount;
    uint32_t FirstIndex;
    uint32_t BaseVertex;
    uint32_t BaseInstance;
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

    bool _keepAttributes = false;
};

class TessellationStage
{
public:
    TessellationStage() {}
    int patch_size = 1;
};

class Pipeline
{
public:
    Pipeline();
};
}   // namespace evnt
#endif   // PIPELINE_H
