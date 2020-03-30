#include "glrenderdevice.h"
#include "typeconversions.h"
#include <GL/glew.h>

namespace evnt
{
GLRenderDevice::GLRenderDevice() {}

void GLRenderDevice::clearColorFramebufferCommand(int32_t attachment, glm::vec4 color)
{
    m_commands.push_back([color, attachment]() {
        float colorv[] = {color.x, color.y, color.z, color.w};
        glClearBufferfv(GL_COLOR, attachment, colorv);
    });
}

void GLRenderDevice::clearDepthFramebufferCommand(float value)
{
    m_commands.push_back([value]() { glClearBufferfv(GL_DEPTH, 0, &value); });
}

void GLRenderDevice::clearAllBuffers()
{
    m_commands.push_back(
        []() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); });
}

void GLRenderDevice::bindPixelUnpackBufferCommand(IBuffer * buffer)
{
    auto id = buffer->getID();
    m_commands.push_back([id]() { glBindBuffer(GL_PIXEL_UNPACK_BUFFER, id); });
}

void GLRenderDevice::bindPixelPackBufferCommand(IBuffer * buffer)
{
    auto id = buffer->getID();
    m_commands.push_back([id]() { glBindBuffer(GL_PIXEL_PACK_BUFFER, id); });
}

void GLRenderDevice::bindIndirectCommandBufferCommand(IBuffer * buffer)
{
    auto id = buffer->getID();
    m_commands.push_back([id]() { glBindBuffer(GL_DRAW_INDIRECT_BUFFER, id); });
}

void GLRenderDevice::bindComputeShaderCommand(IShader * shader)
{
    auto id = shader->getID();
    m_commands.push_back([id]() { glUseProgram(id); });
}

void GLRenderDevice::bindPipelineCommand(Pipeline * pipeline)
{
    m_pipelines.push_back(pipeline);
    int                       index     = m_pipelines.size() - 1;
    std::vector<Pipeline *> * pipelines = &m_pipelines;

    m_commands.push_back([index, pipelines]() { (*pipelines)[index]->bindPipeline(); });
}

void GLRenderDevice::bindConstantsCommand(ShaderConstants * constants)
{
    m_constants.push_back(constants);
    int                              index        = m_constants.size() - 1;
    std::vector<ShaderConstants *> * constantsVec = &m_constants;

    m_commands.push_back([index, constantsVec]() { (*constantsVec)[index]->bindConstants(); });
}

void GLRenderDevice::bindDescriptorCommand(ShaderDescriptor * descriptor)
{
    m_descriptors.push_back(descriptor);
    int                               index       = m_descriptors.size() - 1;
    std::vector<ShaderDescriptor *> * descriptors = &m_descriptors;

    m_commands.push_back([index, descriptors]() { (*descriptors)[index]->bindDescriptor(); });
}

void GLRenderDevice::drawIndexedCommand(PrimitiveType type, int32_t count, int32_t instances,
                                        int32_t baseIndex, int32_t baseVertex, int32_t baseInstance)
{
    auto pt = PrimitiveTypeToGLPrimitiveType(type);
    m_commands.push_back([pt, count, instances, baseIndex, baseVertex, baseInstance]() {
        glDrawElementsInstancedBaseVertexBaseInstance(pt, count, GL_UNSIGNED_INT,
                                                      reinterpret_cast<GLvoid *>(baseIndex), instances,
                                                      baseVertex, baseInstance);
    });
}

void GLRenderDevice::drawCommand(PrimitiveType type, int32_t first, int32_t count, int32_t instances,
                                 int32_t baseInstance)
{
    auto pt = PrimitiveTypeToGLPrimitiveType(type);
    m_commands.push_back([pt, count, instances, first, baseInstance]() {
        glDrawArraysInstancedBaseInstance(pt, first, count, instances, baseInstance);
    });
}

void GLRenderDevice::drawIndexedIndirectCommand(PrimitiveType type, int32_t offset, int32_t cmdCount,
                                                int32_t stride)
{
    auto pt = PrimitiveTypeToGLPrimitiveType(type);
    m_commands.push_back([pt, cmdCount, stride, offset]() {
        glMultiDrawElementsIndirect(pt, GL_UNSIGNED_INT, reinterpret_cast<const void *>(offset), cmdCount,
                                    stride);
    });
}

void GLRenderDevice::drawIndirectCommand(PrimitiveType type, int32_t offset, int32_t cmdCount, int32_t stride)
{
    auto pt = PrimitiveTypeToGLPrimitiveType(type);
    m_commands.push_back([pt, cmdCount, stride, offset]() {
        glMultiDrawArraysIndirect(pt, reinterpret_cast<const void *>(offset), cmdCount, stride);
    });
}

void GLRenderDevice::dispatchCommand(int32_t x, int32_t y, int32_t z)
{
    m_commands.push_back([x, y, z]() { glDispatchCompute(x, y, z); });
}

void GLRenderDevice::memoryBarrier(MemoryBarrierType type)
{
    auto mb = MemoryBarrierTypeToGLMemoryBarrierType(type);
    m_commands.push_back([mb]() { glMemoryBarrier(mb); });
}

void GLRenderDevice::textureBarrier()
{
    m_commands.push_back([]() { glTextureBarrier(); });
}
}   // namespace evnt
