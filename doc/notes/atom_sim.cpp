// https://github.com/neketka/AtomSim

// https://habr.com/ru/post/210894/
// https://www.fluentcpp.com/2018/05/22/how-to-transform-a-hierarchy-of-virtual-methods-into-a-crtp/

void Render()
{
    // ...
    Pipeline        pipe;
    ShaderConstants consts;
    CommandBuffer   cmd;

    pipe.SetPipelineData{...} consts
        .SetConstans{...}

    cmd.ClearAllBuffers();
    cmd.MemoryBarrier();

    cmd.PipeBind(pipe) cmd.ConstsBind(consts)
        cmd.DrawCommand()

            cmd.ExecuteCommands();
}

ShaderConstants
{
    std::vector<std::pair<int, std::pair<Texture, Sampler>>> m_uniformT;
    std::vector<std::pair<int, Image>>                       m_uniformI;
    std::vector<std::pair<int, int>>                         m_uniform1i;
    std::vector<std::pair<int, float>>                       m_uniform1f;
    std::vector<std::pair<int, glm::vec2>>                   m_uniform2f;
    std::vector<std::pair<int, glm::vec3>>                   m_uniform3f;
    std::vector<std::pair<int, glm::vec4>>                   m_uniform4f;
    std::vector<std::pair<int, glm::mat3>>                   m_uniform3fvMat;
    std::vector<std::pair<int, glm::mat4>>                   m_uniform4fvMat;
}

Pipeline
{
    Shader shader;
    VertexStage
    {
        VertexArray vertexArray;
        bool        FrontFaceCulling;
        bool        BackFaceCulling;
        bool        DepthClamping;
    }
    TessellationStage
    {
        int PatchSize;
    }
    TransformFeedbackStage
    {
        std::vector<std::pair<int, TBuffer>> BufferBindings;
    }
    FragmentStage
    {
        Framebuffer      framebuffer;
        std::vector<int> DrawTargets;

        IQuad2D Viewport;
        IQuad2D ScissorRegion;
        bool    ScissorTest;
        bool    RasterizerDiscard;
        bool    DepthTest;
        bool    StencilTest;

        bool                   DepthMask = true;
        GLuint                 FrontStencilMask;
        GLuint                 BackStencilMask;
        std::vector<ColorMask> ColorMasks;

        ComparisonFunc FrontStencilFunc;
        ComparisonFunc BackStencilFunc;
        GLint          StencilCompareReference;
        GLuint         StencilCompareMask;

        StencilOperation FrontStencilFail;
        StencilOperation FrontDepthFail;
        StencilOperation FrontDepthPass;

        StencilOperation BackStencilFail;
        StencilOperation BackDepthFail;
        StencilOperation BackDepthPass;

        bool                        Blending;
        std::vector<BufferBlending> BufferBlends;
    }
}

CommandBuffer
{
    enum class MemoryBarrierType;

    std::vector<std::function<void()>> m_commands;

    std::vector<ShaderConstants>  m_constants;
    std::vector<ShaderDescriptor> m_descriptors;
    std::vector<Pipeline>         m_pipelines;
    std::vector<CommandBuffer>    m_subBuffers;
}

//==================================================================================//
void Initialize()
{
    GenerateRenderBuffers();
    GenerateShaders();
}

GenerateRenderBuffers()
{
    sphereVertices = Buffer<Vertex>(..., BufferOptions(false, false, false, false, false, false));

    std::vector<BufferStructure> bufStruct = {
        BufferStructure({Attribute(0, 0, 3, 12, true), Attribute(1, 12, 3, 12, true)}),
        BufferStructure({Attribute(2, 0, 4, 16, true, 1), Attribute(3, 16, 4, 16, true, 1),
                         Attribute(4, 32, 4, 16, true, 1), Attribute(5, 48, 4, 16, true, 1)})};

    vao = VertexArray(bufStruct);

    vao.SetBuffer(0, sphereVertices);
}

GenerateShaders()
{
    renderingShader = Shader(renderShader, {});
}
