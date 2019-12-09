#ifndef IDEVICECONTEXT_H
#define IDEVICECONTEXT_H

#include "ibuffer.h"
#include "ifence.h"
#include "ipipelinestate.h"
#include "iswapchain.h"
#include "itexture.h"

namespace evnt
{
// class Box;
class ICommandList;
/// Draw command flags
enum DRAW_FLAGS : uint8_t
{
    /// No flags.
    DRAW_FLAG_NONE = 0x00,
    /// Verify the sate of index and vertex buffers (if any) used by the draw
    /// command. State validation is only performed in debug and development builds
    /// and the flag has no effect in release build.
    DRAW_FLAG_VERIFY_STATES = 0x01
};

/// Defines resource state transition mode performed by various commands.
/// Refer to http://evntgraphics.com/2018/12/09/resource-state-management/ for detailed explanation
/// of resource state management in evnt Engine.
enum RESOURCE_STATE_TRANSITION_MODE : uint8_t
{
    /// Perform no state transitions and no state validation.
    /// Resource states are not accessed (either read or written) by the command.
    RESOURCE_STATE_TRANSITION_MODE_NONE = 0,
    /// Transition resources to the states required by the specific command.
    /// Resources in unknown state are ignored.
    /// \note    Any method that uses this mode may alter the state of the resources it works with.
    ///          As automatic state management is not thread-safe, no other thread is allowed to read
    ///          or write the state of the resources being transitioned.
    ///          If the application intends to use the same resources in other threads simultaneously, it
    ///          needs to explicitly manage the states using IDeviceContext::TransitionResourceStates()
    ///          method. Refer to http://evntgraphics.com/2018/12/09/resource-state-management/ for
    ///          detailed explanation of resource state management in evnt Engine.
    RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
    /// Do not transition, but verify that states are correct.
    /// No validation is performed if the state is unknown to the engine.
    /// This mode only has effect in debug and development builds. No validation
    /// is performed in release build.
    /// \note    Any method that uses this mode will read the state of resources it works with.
    ///          As automatic state management is not thread-safe, no other thread is allowed to alter
    ///          the state of resources being used by the command. It is safe to read these states.
    RESOURCE_STATE_TRANSITION_MODE_VERIFY
};

/// Defines the draw command attributes
/// This structure is used by IRenderDevice::Draw().
struct DrawAttribs
{
    union
    {
        /// For a non-indexed draw call, number of vertices to draw.
        uint32_t num_vertices = 0;
        /// For an indexed draw call, number of indices to draw.
        uint32_t num_indices;
    };
    /// Indicates if index buffer will be used to index input vertices.
    bool is_indexed = false;
    /// For an indexed draw call, type of elements in the index buffer.
    /// Allowed values: VT_UINT16 and VT_uint32_t. Ignored if DrawAttribs::IsIndexed is False.
    VALUE_TYPE index_type = VT_UNDEFINED;
    /// Additional flags, see evnt::DRAW_FLAGS.
    DRAW_FLAGS flags = DRAW_FLAG_NONE;
    /// State transition mode for indirect draw arguments buffer. Ignored if pIndirectDrawAttribs member is
    /// null.
    RESOURCE_STATE_TRANSITION_MODE indirect_attribs_buffer_state_transition_mode =
        RESOURCE_STATE_TRANSITION_MODE_NONE;
    /// Number of instances to draw. If more than one instance is specified,
    /// instanced draw call will be performed.
    uint32_t num_instances = 1;
    /// For indexed rendering, a constant which is added to each index before
    /// accessing the vertex buffer.
    uint32_t base_vertex = 0;
    /// For indirect rendering, offset from the beginning of the buffer to the location
    /// of draw command attributes. Ignored if DrawAttribs::pIndirectDrawAttribs is null.
    uint32_t indirect_draw_args_offset = 0;
    union
    {
        /// For non-indexed rendering, LOCATION (or INDEX, but NOT the byte offset) of the
        /// first vertex in the vertex buffer to start reading vertices from.
        uint32_t start_vertex_location = 0;
        /// For indexed rendering, LOCATION (NOT the byte offset) of the first index in
        /// the index buffer to start reading indices from.
        uint32_t first_index_location;
    };
    /// For instanced rendering, LOCATION (or INDEX, but NOT the byte offset) in the vertex
    /// buffer to start reading instance data from.
    uint32_t first_instance_location = 0;
    /// For indirect rendering, pointer to the buffer, from which
    /// draw attributes will be read.
    IBuffer * p_indirect_draw_attribs = nullptr;

    /// Initializes the structure members with default values
    /// Default values:
    /// Member                                   | Default value
    /// -----------------------------------------|--------------------------------------
    /// NumVertices                              | 0
    /// IsIndexed                                | False
    /// IndexType                                | VT_UNDEFINED
    /// IndirectAttribsBufferStateTransitionMode | RESOURCE_STATE_TRANSITION_MODE_NONE
    /// Flags                                    | DRAW_FLAG_NONE
    /// NumInstances                             | 1
    /// BaseVertex                               | 0
    /// IndirectDrawArgsOffset                   | 0
    /// StartVertexLocation                      | 0
    /// FirstInstanceLocation                    | 0
    /// pIndirectDrawAttribs                     | nullptr
    DrawAttribs() noexcept {}
    /// Initializes the structure to perform non-indexed draw call.
    DrawAttribs(uint32_t _NumVertices, DRAW_FLAGS _Flags, uint32_t _NumInstances = 1,
                uint32_t _BaseVertex = 0, uint32_t _StartVertexLocation = 0,
                uint32_t _FirstInstanceLocation = 0) noexcept :
        num_vertices(_NumVertices),
        flags(_Flags),
        num_instances(_NumInstances),
        base_vertex(_BaseVertex),
        start_vertex_location(_StartVertexLocation),
        first_instance_location(_FirstInstanceLocation)
    {}

    /// Initializes the structure to perform indexed draw call.
    DrawAttribs(uint32_t _NumIndices, VALUE_TYPE _IndexType, DRAW_FLAGS _Flags, uint32_t _NumInstances = 1,
                uint32_t _BaseVertex = 0, uint32_t _FirstIndexLocation = 0,
                uint32_t _FirstInstanceLocation = 0) noexcept :
        num_indices(_NumIndices),
        is_indexed(true),
        index_type(_IndexType),
        flags(_Flags),
        num_instances(_NumInstances),
        base_vertex(_BaseVertex),
        first_index_location(_FirstIndexLocation),
        first_instance_location(_FirstInstanceLocation)
    {}

    /// Initializes the structure to perform non-indexed indirect draw call.
    DrawAttribs(IBuffer * _pIndirectDrawAttribs, DRAW_FLAGS _Flags,
                RESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode,
                uint32_t                       _IndirectDrawArgsOffset = 0) noexcept :
        flags(_Flags),
        indirect_attribs_buffer_state_transition_mode(_IndirectAttribsBufferStateTransitionMode),
        indirect_draw_args_offset(_IndirectDrawArgsOffset),
        p_indirect_draw_attribs(_pIndirectDrawAttribs)
    {}

    /// Initializes the structure to perform indirect indexed draw call.
    DrawAttribs(IBuffer * _pIndirectDrawAttribs, VALUE_TYPE _IndexType, DRAW_FLAGS _Flags,
                RESOURCE_STATE_TRANSITION_MODE _IndirectAttribsBufferStateTransitionMode,
                uint32_t                       _IndirectDrawArgsOffset = 0) noexcept :
        is_indexed(true),
        index_type(_IndexType),
        flags(_Flags),
        indirect_attribs_buffer_state_transition_mode(_IndirectAttribsBufferStateTransitionMode),
        indirect_draw_args_offset(_IndirectDrawArgsOffset),
        p_indirect_draw_attribs(_pIndirectDrawAttribs)
    {}
};

/// Defines which parts of the depth-stencil buffer to clear.
/// These flags are used by IDeviceContext::ClearDepthStencil().
enum CLEAR_DEPTH_STENCIL_FLAGS : uint32_t
{
    /// Perform no clear.
    CLEAR_DEPTH_FLAG_NONE = 0x00,
    /// Clear depth part of the buffer.
    CLEAR_DEPTH_FLAG = 0x01,
    /// Clear stencil part of the buffer.
    CLEAR_STENCIL_FLAG = 0x02
};

/// Describes dispatch command arguments
/// This structure is used by IDeviceContext::DispatchCompute().
struct DispatchComputeAttribs
{
    uint32_t thread_group_count_x = 1;   ///< Number of groups dispatched in X direction.
    uint32_t thread_group_count_y = 1;   ///< Number of groups dispatched in Y direction.
    uint32_t thread_group_count_z = 1;   ///< Number of groups dispatched in Z direction.

    /// Pointer to the buffer containing dispatch arguments.
    /// If not nullptr, then indirect dispatch command is executed, and
    /// ThreadGroupCountX, ThreadGroupCountY, and ThreadGroupCountZ are ignored.
    IBuffer * p_indirect_dispatch_attribs = nullptr;

    /// If pIndirectDispatchAttribs is not nullptr, indicates offset from the beginning
    /// of the buffer to the dispatch command arguments. Ignored otherwise.
    uint32_t dispatch_args_byte_offset = 0;

    /// State transition mode for indirect dispatch attributes buffer. This member is ignored if
    /// pIndirectDispatchAttribs member is null.
    RESOURCE_STATE_TRANSITION_MODE indirect_attribs_buffer_state_transition_mode =
        RESOURCE_STATE_TRANSITION_MODE_NONE;

    DispatchComputeAttribs() noexcept {}

    /// Initializes the structure to perform non-indirect dispatch command.

    /// \param [in] GroupsX - Number of groups dispatched in X direction. Default value is 1.
    /// \param [in] GroupsY - Number of groups dispatched in Y direction. Default value is 1.
    /// \param [in] GroupsZ - Number of groups dispatched in Z direction. Default value is 1.
    explicit DispatchComputeAttribs(uint32_t GroupsX, uint32_t GroupsY = 1, uint32_t GroupsZ = 1) noexcept :
        thread_group_count_x(GroupsX),
        thread_group_count_y(GroupsY),
        thread_group_count_z(GroupsZ),
        p_indirect_dispatch_attribs(nullptr),
        dispatch_args_byte_offset(0)
    {}

    /// Initializes the structure to perform indirect dispatch command.

    /// \param [in] pDispatchAttribs - Pointer to the buffer containing dispatch arguments.
    /// \param [in] Offset - Offset from the beginning of the buffer to the dispatch command
    ///                 arguments. Default value is 0.
    DispatchComputeAttribs(IBuffer * pDispatchAttribs, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode,
                           uint32_t Offset = 0) :
        thread_group_count_x(0),
        thread_group_count_y(0),
        thread_group_count_z(0),
        p_indirect_dispatch_attribs(pDispatchAttribs),
        dispatch_args_byte_offset(Offset),
        indirect_attribs_buffer_state_transition_mode(StateTransitionMode)
    {}
};

/// Defines allowed flags for IDeviceContext::SetVertexBuffers() function.
enum SET_VERTEX_BUFFERS_FLAGS : uint8_t
{
    /// No extra operations.
    SET_VERTEX_BUFFERS_FLAG_NONE = 0x00,
    /// Reset the vertex buffers to only the buffers specified in this
    /// call. All buffers previously bound to the pipeline will be unbound.
    SET_VERTEX_BUFFERS_FLAG_RESET = 0x01
};

/// Describes the viewport.
/// This structure is used by IDeviceContext::SetViewports().
struct Viewport
{
    /// X coordinate of the left boundary of the viewport.
    float top_left_x = 0.f;
    /// Y coordinate of the top boundary of the viewport.
    /// When defining a viewport, DirectX convention is used:
    /// window coordinate systems originates in the LEFT TOP corner
    /// of the screen with Y axis pointing down.
    float top_left_y = 0.f;
    /// Viewport width.
    float width = 0.f;
    /// Viewport Height.
    float height = 0.f;
    /// Minimum depth of the viewport. Ranges between 0 and 1.
    float min_depth = 0.f;
    /// Maximum depth of the viewport. Ranges between 0 and 1.
    float max_depth = 1.f;

    /// Initializes the structure.
    Viewport(float _TopLeftX, float _TopLeftY, float _Width, float _Height, float _MinDepth = 0,
             float _MaxDepth = 1) noexcept :
        top_left_x(_TopLeftX),
        top_left_y(_TopLeftY),
        width(_Width),
        height(_Height),
        min_depth(_MinDepth),
        max_depth(_MaxDepth)
    {}

    Viewport() noexcept {}
};

/// Box
struct Box
{
    uint32_t min_x = 0;   ///< Minimal X coordinate. Default value is 0
    uint32_t max_x = 0;   ///< Maximal X coordinate. Default value is 0
    uint32_t min_y = 0;   ///< Minimal Y coordinate. Default value is 0
    uint32_t max_y = 0;   ///< Maximal Y coordinate. Default value is 0
    uint32_t min_z = 0;   ///< Minimal Z coordinate. Default value is 0
    uint32_t max_z = 1;   ///< Maximal Z coordinate. Default value is 1

    Box(uint32_t _MinX, uint32_t _MaxX, uint32_t _MinY, uint32_t _MaxY, uint32_t _MinZ, uint32_t _MaxZ) :
        min_x(_MinX), max_x(_MaxX), min_y(_MinY), max_y(_MaxY), min_z(_MinZ), max_z(_MaxZ)
    {}
    Box(uint32_t _MinX, uint32_t _MaxX, uint32_t _MinY, uint32_t _MaxY) :
        Box(_MinX, _MaxX, _MinY, _MaxY, 0, 1)
    {}
    Box(uint32_t _MinX, uint32_t _MaxX) : Box(_MinX, _MaxX, 0, 0, 0, 1) {}
    Box() {}
};

/// Describes the rectangle.
/// This structure is used by IDeviceContext::SetScissorRects().
/// \remarks When defining a viewport, Windows convention is used:
///          window coordinate systems originates in the LEFT TOP corner
///          of the screen with Y axis pointing down.
struct Rect
{
    int32_t left   = 0;   ///< X coordinate of the left boundary of the viewport.
    int32_t top    = 0;   ///< Y coordinate of the top boundary of the viewport.
    int32_t right  = 0;   ///< X coordinate of the right boundary of the viewport.
    int32_t bottom = 0;   ///< Y coordinate of the bottom boundary of the viewport.

    /// Initializes the structure
    Rect(int32_t _left, int32_t _top, int32_t _right, int32_t _bottom) noexcept :
        left(_left), top(_top), right(_right), bottom(_bottom)
    {}
    Rect() noexcept {}
};

/// Defines copy texture command attributes.
/// This structure is used by IDeviceContext::CopyTexture().
struct CopyTextureAttribs
{
    /// Source texture to copy data from.
    ITexture * p_src_texture = nullptr;
    /// Mip level of the source texture to copy data from.
    uint32_t src_mip_level = 0;
    /// Array slice of the source texture to copy data from. Must be 0 for non-array textures.
    uint32_t src_slice = 0;
    /// Source region to copy. Use nullptr to copy the entire subresource.
    const Box * p_src_box = nullptr;
    /// Source texture state transition mode (see evnt::RESOURCE_STATE_TRANSITION_MODE).
    RESOURCE_STATE_TRANSITION_MODE src_texture_transition_mode = RESOURCE_STATE_TRANSITION_MODE_NONE;
    /// Destination texture.
    ITexture * p_dst_texture = nullptr;
    /// Destination mip level.
    uint32_t dst_mip_level = 0;
    /// Destination array slice. Must be 0 for non-array textures.
    uint32_t dst_slice = 0;
    /// X offset on the destination subresource.
    uint32_t dst_x = 0;
    /// Y offset on the destination subresource.
    uint32_t dst_y = 0;
    /// Z offset on the destination subresource
    uint32_t dst_z = 0;
    /// Destination texture state transition mode (see evnt::RESOURCE_STATE_TRANSITION_MODE).
    RESOURCE_STATE_TRANSITION_MODE dst_texture_transition_mode = RESOURCE_STATE_TRANSITION_MODE_NONE;

    CopyTextureAttribs() noexcept {}
    CopyTextureAttribs(ITexture * _pSrcTexture, RESOURCE_STATE_TRANSITION_MODE _SrcTextureTransitionMode,
                       ITexture *                     _pDstTexture,
                       RESOURCE_STATE_TRANSITION_MODE _DstTextureTransitionMode) noexcept :
        p_src_texture(_pSrcTexture),
        src_texture_transition_mode(_SrcTextureTransitionMode),
        p_dst_texture(_pDstTexture),
        dst_texture_transition_mode(_DstTextureTransitionMode)
    {}
};

/// Device context interface.
/// \remarks Device context keeps strong references to all objects currently bound to
///          the pipeline: buffers, states, samplers, shaders, etc.
///          The context also keeps strong reference to the device and
///          the swap chain.
class IDeviceContext
{
public:
    /// Sets the pipeline state.
    /// \param [in] pPipelineState - Pointer to IPipelineState interface to bind to the context.
    virtual void setPipelineState(IPipelineState * pPipelineState) = 0;

    /// Transitions shader resources to the states required by Draw or Dispatch command.
    /// \param [in] pPipelineState         - Pipeline state object that was used to create the shader resource
    ///                                      binding.
    /// \param [in] pShaderResourceBinding - Shader resource binding whose resources will be
    ///                                      transitioned.
    /// \remarks This method explicitly transitiones all resources except ones in unknown state to the states
    ///          required by Draw or Dispatch command.
    ///          If this method was called, there is no need to use
    ///          evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION when calling
    ///          IDeviceContext::CommitShaderResources()
    /// \remarks Resource state transitioning is not thread safe. As the method may alter the states
    ///          of resources referenced by the shader resource binding, no other thread is allowed to read or
    ///          write these states.
    ///          If the application intends to use the same resources in other threads simultaneously, it
    ///          needs to explicitly manage the states using IDeviceContext::TransitionResourceStates()
    ///          method.
    virtual void transitionShaderResources(IPipelineState *         pPipelineState,
                                           IShaderResourceBinding * pShaderResourceBinding) = 0;

    /// Commits shader resources to the device context
    /// \param [in] pShaderResourceBinding - Shader resource binding whose resources will be committed.
    ///                                      If pipeline state contains no shader resources, this parameter
    ///                                      can be null.
    /// \param [in] StateTransitionMode    - State transition mode (see
    ///                                      evnt::RESOURCE_STATE_TRANSITION_MODE).
    /// \remarks Pipeline state object that was used to create the shader resource binding must be bound
    ///          to the pipeline when CommitShaderResources() is called. If no pipeline state object is bound
    ///          or the pipeline state object does not match the shader resource binding, the method will
    ///          fail. If evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode is used, the engine will
    ///          also transition all shader resources to required states. If the flag is not set, it is
    ///          assumed that all resources are already in correct states. Resources can be explicitly
    ///          transitioned to required states by calling IDeviceContext::TransitionShaderResources() or
    ///          IDeviceContext::TransitionResourceStates().
    /// \remarks Automatic resource state transitioning is not thread-safe.
    ///          - If evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode is used, the method may alter
    ///          the states
    ///            of resources referenced by the shader resource binding and no other thread is allowed to
    ///            read or write these states.
    ///          - If evnt::RESOURCE_STATE_TRANSITION_MODE_VERIFY mode is used, the method will read the
    ///          states, so no other thread
    ///            should alter the states by calling any of the methods that use
    ///            evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION mode. It is safe for other threads to
    ///            read the states.
    ///          - If evnt::RESOURCE_STATE_TRANSITION_MODE_NONE mode is used, the method does not access
    ///          the states of resources.
    ///          If the application intends to use the same resources in other threads simultaneously, it
    ///          should manage the states manually by setting the state to evnt::RESOURCE_STATE_UNKNOWN
    ///          (which will disable automatic state management) using IBuffer::SetState() or
    ///          ITexture::SetState() and explicitly transitioning the states with
    ///          IDeviceContext::TransitionResourceStates().
    virtual void commitShaderResources(IShaderResourceBinding *       pShaderResourceBinding,
                                       RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;

    /// Sets the stencil reference value
    /// \param [in] StencilRef - Stencil reference value.
    virtual void setStencilRef(uint32_t StencilRef) = 0;

    /// \param [in] pBlendFactors - Array of four blend factors, one for each RGBA component.
    ///                             Theses factors are used if the blend state uses one of the
    ///                             evnt::BLEND_FACTOR_BLEND_FACTOR or
    ///                             evnt::BLEND_FACTOR_INV_BLEND_FACTOR
    ///                             blend factors. If nullptr is provided,
    ///                             default blend factors array {1,1,1,1} will be used.
    virtual void setBlendFactors(const float * pBlendFactors = nullptr) = 0;

    /// Binds vertex buffers to the pipeline
    /// \param [in] StartSlot           - The first input slot for binding. The first vertex buffer is
    ///                                   explicitly bound to the start slot; each additional vertex buffer
    ///                                   in the array is implicitly bound to each subsequent input slot.
    /// \param [in] NumBuffersSet       - The number of vertex buffers in the array.
    /// \param [in] ppBuffers           - A pointer to an array of vertex buffers.
    ///                                   The buffers must have been created with the
    ///                                   evnt::BIND_VERTEX_BUFFER flag.
    /// \param [in] pOffsets            - Pointer to an array of offset values; one offset value for each
    /// buffer
    ///                                   in the vertex-buffer array. Each offset is the number of bytes
    ///                                   between the first element of a vertex buffer and the first element
    ///                                   that will be used. If this parameter is nullptr, zero offsets for
    ///                                   all buffers will be used.
    /// \param [in] StateTransitionMode - State transition mode for buffers being set (see
    ///                                   evnt::RESOURCE_STATE_TRANSITION_MODE).
    /// \param [in] Flags               - Additional flags. See
    ///                                   evnt::SET_VERTEX_BUFFERS_FLAGS for a list of allowed values.
    /// \remarks The device context keeps strong references to all bound vertex buffers.
    ///          Thus a buffer cannot be released until it is unbound from the context.
    ///          It is suggested to specify evnt::SET_VERTEX_BUFFERS_FLAG_RESET flag
    ///          whenever possible. This will assure that no buffers from previous draw calls
    ///          are bound to the pipeline.
    /// \remarks When StateTransitionMode is evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, the method
    ///          will transition all buffers in known states to evnt::RESOURCE_STATE_VERTEX_BUFFER. Resource
    ///          state transitioning is not thread safe, so no other thread is allowed to read or write the
    ///          states of these buffers.
    ///          If the application intends to use the same resources in other threads simultaneously, it
    ///          needs to explicitly manage the states using IDeviceContext::TransitionResourceStates()
    ///          method.
    virtual void setVertexBuffers(uint32_t StartSlot, uint32_t NumBuffersSet, IBuffer ** ppBuffers,
                                  uint32_t * pOffsets, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode,
                                  SET_VERTEX_BUFFERS_FLAGS Flags) = 0;

    /// Invalidates the cached context state
    /// This method should be called by say Unity plugin before (or after)
    /// issuing draw commands to invalidate cached states.
    virtual void invalidateState() = 0;

    /// Binds an index buffer to the pipeline
    /// \param [in] pIndexBuffer        - Pointer to the index buffer. The buffer must have been created
    ///                                   with the evnt::BIND_INDEX_BUFFER flag.
    /// \param [in] ByteOffset          - Offset from the beginning of the buffer to
    ///                                   the start of index data.
    /// \param [in] StateTransitionMode - State transiton mode for the index buffer to bind (see
    ///                                   evnt::RESOURCE_STATE_TRANSITION_MODE).
    /// \remarks The device context keeps strong reference to the index buffer.
    ///          Thus an index buffer object cannot be released until it is unbound
    ///          from the context.
    /// \remarks When StateTransitionMode is evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, the method
    ///          will transition the buffer to evnt::RESOURCE_STATE_INDEX_BUFFER (if its state is not
    ///          unknown). Resource state transitioning is not thread safe, so no other thread is allowed to
    ///          read or write the state of the buffer. If the application intends to use the same resource in
    ///          other threads simultaneously, it needs to explicitly manage the states using
    ///          IDeviceContext::TransitionResourceStates() method.
    virtual void setIndexBuffer(IBuffer * pIndexBuffer, uint32_t ByteOffset,
                                RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;

    /// Sets an array of viewports
    /// \param [in] NumViewports - Number of viewports to set.
    /// \param [in] pViewports   - An array of Viewport structures describing the viewports to bind.
    /// \param [in] RTWidth      - Render target width. If 0 is provided, width of the currently bound render
    ///                            target will be used.
    /// \param [in] RTHeight     - Render target height. If 0 is provided, height of the
    ///                            currently bound render target will be used.
    /// \remarks
    /// DirectX and OpenGL use different window coordinate systems. In DirectX, the coordinate system origin
    /// is in the left top corner of the screen with Y axis pointing down. In OpenGL, the origin
    /// is in the left bottom corener of the screen with Y axis pointing up. Render target size is
    /// required to convert viewport from DirectX to OpenGL coordinate system if OpenGL device is used.
    /// All viewports must be set atomically as one operation. Any viewports not
    /// defined by the call are disabled.
    /// You can set the viewport size to match the currently bound render target using the
    /// following call:
    ///     pContext->SetViewports(1, nullptr, 0, 0);
    virtual void setViewports(uint32_t NumViewports, const Viewport * pViewports, uint32_t RTWidth,
                              uint32_t RTHeight) = 0;

    /// Sets active scissor rects
    /// \param [in] NumRects - Number of scissor rectangles to set.
    /// \param [in] pRects   - An array of Rect structures describing the scissor rectangles to bind.
    /// \param [in] RTWidth  - Render target width. If 0 is provided, width of the currently bound render
    ///                        target will be used.
    /// \param [in] RTHeight - Render target height. If 0 is provided, height of the
    ///                        currently bound render target will be used.
    /// \remarks
    /// DirectX and OpenGL use different window coordinate systems. In DirectX, the coordinate system origin
    /// is in the left top corner of the screen with Y axis pointing down. In OpenGL, the origin
    /// is in the left bottom corener of the screen with Y axis pointing up. Render target size is
    /// required to convert viewport from DirectX to OpenGL coordinate system if OpenGL device is used.
    /// All scissor rects must be set atomically as one operation. Any rects not
    /// defined by the call are disabled.
    virtual void setScissorRects(uint32_t NumRects, const Rect * pRects, uint32_t RTWidth,
                                 uint32_t RTHeight) = 0;

    /// Binds one or more render targets and the depth-stencil buffer to the pipeline. It also
    /// sets the viewport to match the first non-null render target or depth-stencil buffer
    /// \param [in] NumRenderTargets    - Number of render targets to bind.
    /// \param [in] ppRenderTargets     - Array of pointers to ITextureView that represent the render
    ///                                   targets to bind to the device. The type of each view in the
    ///                                   array must be evnt::TEXTURE_VIEW_RENDER_TARGET.
    /// \param [in] pDepthStencil       - Pointer to the ITextureView that represents the depth stencil to
    ///                                   bind to the device. The view type must be
    ///                                   evnt::TEXTURE_VIEW_DEPTH_STENCIL.
    /// \param [in] StateTransitionMode - State transition mode of the render targets and depth stencil buffer
    /// being set (see evnt::RESOURCE_STATE_TRANSITION_MODE).
    /// \remarks     The device context will keep strong references to all bound render target
    ///              and depth-stencil views. Thus these views (and consequently referenced textures)
    ///              cannot be released until they are unbound from the context.
    ///              Any render targets not defined by this call are set to nullptr.
    ///              You can set the default render target and depth stencil using the
    ///              following call:
    ///                 pContext->SetRenderTargets(0, nullptr, nullptr);
    /// \remarks When StateTransitionMode is evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, the method
    ///          will transition all render targets in known states to evnt::RESOURCE_STATE_REDER_TARGET,
    ///          and the depth-stencil buffer to evnt::RESOURCE_STATE_DEPTH_WRITE state.
    ///          Resource state transitioning is not thread safe, so no other thread is allowed to read or
    ///          write the states of resources used by the command.
    ///          If the application intends to use the same resource in other threads simultaneously, it needs
    ///          to explicitly manage the states using IDeviceContext::TransitionResourceStates() method.
    virtual void setRenderTargets(uint32_t NumRenderTargets, ITextureView * ppRenderTargets[],
                                  ITextureView *                 pDepthStencil,
                                  RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;

    /// Executes a draw command
    /// \param [in] DrawAttribs - Structure describing draw command attributes, see evnt::DrawAttribs for
    ///             details.
    /// \remarks  If IndirectAttribsBufferStateTransitionMode member is
    ///           evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
    ///           the method may transition the state of indirect draw arguments buffer. This is not a thread
    ///           safe operation, so no other thread is allowed to read or write the state of the buffer.
    ///           If evnt::DRAW_FLAG_VERIFY_STATES flag is set, the method reads the state of vertex/index
    ///           buffers, so no other threads are allowed to alter the states of the same resources.
    ///           It is OK to read these states.
    ///           If the application intends to use the same resources in other threads simultaneously, it
    ///           needs to explicitly manage the states using IDeviceContext::TransitionResourceStates()
    ///           method.
    virtual void draw(DrawAttribs & DrawAttribs) = 0;

    /// Executes a dispatch compute command
    /// \param [in] DispatchAttrs - Structure describing dispatch command attributes,
    ///                             see evnt::DispatchComputeAttribs for details.
    /// \remarks  If IndirectAttribsBufferStateTransitionMode member is
    ///           evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION,
    ///           the method may transition the state of indirect dispatch arguments buffer. This is not a
    ///           thread safe operation, so no other thread is allowed to read or write the state of the same
    ///           resource.
    ///           If the application intends to use the same resources in other threads simultaneously, it
    ///           needs to explicitly manage the states using IDeviceContext::TransitionResourceStates()
    ///           method.
    virtual void dispatchCompute(const DispatchComputeAttribs & DispatchAttrs) = 0;

    /// Clears a depth-stencil view
    /// \param [in] pView               - Pointer to ITextureView interface to clear. The view type must be
    ///                                   evnt::TEXTURE_VIEW_DEPTH_STENCIL.
    /// \param [in] StateTransitionMode - state transition mode of the depth-stencil buffer to clear.
    /// \param [in] ClearFlags          - Idicates which parts of the buffer to clear, see
    ///                                   evnt::CLEAR_DEPTH_STENCIL_FLAGS.
    /// \param [in] fDepth              - Value to clear depth part of the view with.
    /// \param [in] Stencil             - Value to clear stencil part of the view with.
    /// \remarks The full extent of the view is always cleared. Viewport and scissor settings are not applied.
    /// \note The depth-stencil view must be bound to the pipeline for clear operation to be performed.
    /// \remarks When StateTransitionMode is evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, the method
    ///          will transition the state of the texture to the state required by clear operation.
    ///          In Direct3D12, this satate is always evnt::RESOURCE_STATE_DEPTH_WRITE, however in Vulkan
    ///          the state depends on whether the depth buffer is bound to the pipeline.
    ///          Resource state transitioning is not thread safe, so no other thread is allowed to read or
    ///          write the state of resources used by the command.
    virtual void clearDepthStencil(ITextureView * pView, CLEAR_DEPTH_STENCIL_FLAGS ClearFlags, float fDepth,
                                   uint8_t Stencil, RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;

    /// Clears a render target view
    /// \param [in] pView               - Pointer to ITextureView interface to clear. The view type must be
    ///                                   evnt::TEXTURE_VIEW_RENDER_TARGET.
    /// \param [in] RGBA                - A 4-component array that represents the color to fill the render
    ///                                   target with. If nullptr is provided, the default array {0,0,0,0}
    ///                                   will be used.
    /// \param [in] StateTransitionMode - Defines required state transitions (see
    ///                                   evnt::RESOURCE_STATE_TRANSITION_MODE)
    /// \remarks The full extent of the view is always cleared. Viewport and scissor settings are not applied.
    ///          The render target view must be bound to the pipeline for clear operation to be performed in
    ///          OpenGL backend.
    /// \remarks When StateTransitionMode is evnt::RESOURCE_STATE_TRANSITION_MODE_TRANSITION, the method
    ///          will transition the texture to the state required by the command. Resource state
    ///          transitioning is not thread safe, so no other thread is allowed to read or write the states
    ///          of the same textures.
    ///          If the application intends to use the same resource in other threads simultaneously, it needs
    ///          to explicitly manage the states using IDeviceContext::TransitionResourceStates() method.
    /// \note    In D3D12 backend clearing render targets requires textures to always be transitioned to
    ///          evnt::RESOURCE_STATE_RENDER_TARGET state. In Vulkan backend however this depends on
    ///          whether a render pass has been started. To clear render target outside of a render pass, the
    ///          texture must be transitioned to evnt::RESOURCE_STATE_COPY_DEST state. Inside a render
    ///          pass it must be in evnt::RESOURCE_STATE_RENDER_TARGET state. When using
    ///          evnt::RESOURCE_STATE_TRANSITION_TRANSITION mode, the engine takes care of proper resource
    ///          state transition, otherwise it is the responsibility of the application.
    virtual void clearRenderTarget(ITextureView * pView, const float * RGBA,
                                   RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;

    /// Finishes recording commands and generates a command list.
    /// \param [out] ppCommandList - Memory location where pointer to the recorded command list will be
    ///                              written.
    virtual void finishCommandList(ICommandList ** ppCommandList) = 0;

    /// Executes recorded commands in a command list.
    /// \param [in] pCommandList - Pointer to the command list to executre.
    /// \remarks After command list is executed, it is no longer valid and should be released.
    virtual void executeCommandList(ICommandList * pCommandList) = 0;

    /// Tells the GPU to set a fence to a specified value after all previous work has completed.
    /// \note The method does not flush the context (an application can do this explcitly if needed)
    ///       and the fence will be signalled only when the command context is flushed next time.
    ///       If an application needs to wait for the fence in a loop, it must flush the context
    ///       after signalling the fence.
    /// \param [in] pFence - The fence to signal
    /// \param [in] Value  - The value to set the fence to. This value must be greater than the
    ///                      previously signalled value on the same fence.
    virtual void signalFence(IFence * pFence, uint64_t Value) = 0;

    /// Flushes the command buffer.
    virtual void flush() = 0;

    /// Updates the data in the buffer.
    /// \param [in] pBuffer             - Pointer to the buffer to updates.
    /// \param [in] Offset              - Offset in bytes from the beginning of the buffer to the update
    ///                                   region.
    /// \param [in] Size                - Size in bytes of the data region to update.
    /// \param [in] pData               - Pointer to the data to write to the buffer.
    /// \param [in] StateTransitionMode - Buffer state transition mode (see
    ///                                   evnt::RESOURCE_STATE_TRANSITION_MODE)
    virtual void updateBuffer(IBuffer * pBuffer, uint32_t Offset, uint32_t Size, const void * pData,
                              RESOURCE_STATE_TRANSITION_MODE StateTransitionMode) = 0;

    /// Copies the data from one buffer to another.
    /// \param [in] pSrcBuffer              - Source buffer to copy data from.
    /// \param [in] SrcOffset               - Offset in bytes from the beginning of the source buffer to the
    ///                                       beginning of data to copy.
    /// \param [in] SrcBufferTransitionMode - State transition mode of the source
    ///                                       buffer (see evnt::RESOURCE_STATE_TRANSITION_MODE).
    /// \param [in] pDstBuffer              - Destination buffer to copy data to.
    /// \param [in] DstOffset               - Offset in bytes from the beginning of the destination buffer to
    ///                                       the beginning of the destination region.
    /// \param [in] Size                    - Size in bytes of data to copy.
    /// \param [in] DstBufferTransitionMode - State transition mode of the destination buffer (see
    ///                                       evnt::RESOURCE_STATE_TRANSITION_MODE).
    virtual void copyBuffer(IBuffer * pSrcBuffer, uint32_t SrcOffset,
                            RESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode, IBuffer * pDstBuffer,
                            uint32_t DstOffset, uint32_t Size,
                            RESOURCE_STATE_TRANSITION_MODE DstBufferTransitionMode) = 0;

    /// Maps the buffer.
    /// \param [in] pBuffer      - Pointer to the buffer to map.
    /// \param [in] MapType      - Type of the map operation. See evnt::MAP_TYPE.
    /// \param [in] MapFlags     - Special map flags. See evnt::MAP_FLAGS.
    /// \param [out] pMappedData - Reference to the void pointer to store the address of the mapped region.
    virtual void mapBuffer(IBuffer * pBuffer, MAP_TYPE MapType, MAP_FLAGS MapFlags, void *& pMappedData) = 0;

    /// Unmaps the previously mapped buffer.
    /// \param [in] pBuffer - Pointer to the buffer to unmap.
    /// \param [in] MapType - Type of the map operation. This parameter must match the type that was
    ///                       provided to the Map() method.
    virtual void unmapBuffer(IBuffer * pBuffer, MAP_TYPE MapType) = 0;

    /// Updates the data in the texture.
    /// \param [in] pTexture    - Pointer to the device context interface to be used to perform the operation.
    /// \param [in] MipLevel    - Mip level of the texture subresource to update.
    /// \param [in] Slice       - Array slice. Should be 0 for non-array textures.
    /// \param [in] DstBox      - Destination region on the texture to update.
    /// \param [in] SubresData  - Source data to copy to the texture.
    /// \param [in] SrcBufferTransitionMode - If pSrcBuffer member of TextureSubResData structure is not null,
    ///                                       this parameter defines state transition mode of the source
    ///                                       buffer. If pSrcBuffer is null, this parameter is ignored.
    /// \param [in] TextureTransitionMode   - Texture state transition mode (see
    ///                                       evnt::RESOURCE_STATE_TRANSITION_MODE)
    virtual void updateTexture(ITexture * pTexture, uint32_t MipLevel, uint32_t Slice, const Box & DstBox,
                               const TextureSubResData &      SubresData,
                               RESOURCE_STATE_TRANSITION_MODE SrcBufferTransitionMode,
                               RESOURCE_STATE_TRANSITION_MODE TextureTransitionMode) = 0;

    /// Copies data from one texture to another.
    /// \param [in] CopyAttribs - Structure describing copy command attributes, see
    ///                           evnt::CopyTextureAttribs for details.
    virtual void copyTexture(const CopyTextureAttribs & CopyAttribs) = 0;

    /// Maps the texture subresource.
    /// \param [in] pTexture    - Pointer to the texture to map.
    /// \param [in] MipLevel    - Mip level to map.
    /// \param [in] ArraySlice  - Array slice to map. This parameter must be 0 for non-array textures.
    /// \param [in] MapType     - Type of the map operation. See evnt::MAP_TYPE.
    /// \param [in] MapFlags    - Special map flags. See evnt::MAP_FLAGS.
    /// \param [in] pMapRegion  - Texture region to map. If this parameter is null, the entire subresource is
    ///                           mapped.
    /// \param [out] MappedData - Mapped texture region data
    /// \remarks This method is supported in D3D11, D3D12 and Vulkan backends. In D3D11 backend, only the
    ///          entire subresource can be mapped, so pMapRegion must either be null, or cover the entire
    ///          subresource. In D3D11 and Vulkan backends, dynamic textures are no different from non-dynamic
    ///          textures, and mapping with MAP_FLAG_DISCARD has exactly the same behavior.
    virtual void mapTextureSubresource(ITexture * pTexture, uint32_t MipLevel, uint32_t ArraySlice,
                                       MAP_TYPE MapType, MAP_FLAGS MapFlags, const Box * pMapRegion,
                                       MappedTextureSubresource & MappedData) = 0;

    /// Unmaps the texture subresource.
    virtual void unmapTextureSubresource(ITexture * pTexture, uint32_t MipLevel, uint32_t ArraySlice) = 0;

    /// Generates a mipmap chain.
    /// \param [in] pTextureView - Texture view to generate mip maps for.
    /// \remarks This function can only be called for a shader resource view.
    ///          The texture must be created with MISC_TEXTURE_FLAG_GENERATE_MIPS flag.
    virtual void generateMips(ITextureView * pTextureView) = 0;

    /// Sets the swap chain in the device context.
    /// The swap chain is used by the device context to work with the
    /// default framebuffer. Specifically, if the swap chain is set in the context,
    /// the following commands can be used:
    /// * SetRenderTargets(0, nullptr, nullptr) - to bind the default back buffer & depth buffer
    /// * SetViewports(1, nullptr, 0, 0) - to set the viewport to match the size of the back buffer
    /// * ClearRenderTarget(nullptr, color) - to clear the default back buffer
    /// * ClearDepthStencil(nullptr, ...) - to clear the default depth buffer
    /// The swap chain is automatically initialized for immediate and all deferred contexts
    /// by factory functions EngineFactoryD3D11Impl::CreateSwapChainD3D11(),
    /// EngineFactoryD3D12Impl::CreateSwapChainD3D12(), and
    /// EngineFactoryOpenGLImpl::CreateDeviceAndSwapChainGL(). However, when the engine is initialized by
    /// attaching to existing d3d11/d3d12 device or OpenGL/GLES context, the swap chain needs to be set
    /// manually if the device context will be using any of the commands above. Device context keeps strong
    /// reference to the swap chain.
    virtual void setSwapChain(ISwapChain * pSwapChain) = 0;

    /// Finishes the current frame and releases dynamic resources allocated by the context.
    /// For immediate context, this method is called automatically by Present(), but can
    /// also be called explicitly. For deferred context, the method must be called by the application to
    /// release dynamic resources. The method has some overhead, so it is better to call it once
    /// per frame, though it can be called with different frequency. Note that unless the GPU is idled,
    /// the resources may actually be released several frames after the one they were used in last time.
    /// \note After the call all dynamic resources become invalid and must be written again before the next
    /// use.
    ///       Also, all committed resources become invalid.
    ///       For deferred contexts, this method must be called after all command lists referencing dynamic
    ///       resources have been executed through immediate context. The method does not Flush() the
    ///       context.
    virtual void finishFrame() = 0;
};
}   // namespace evnt
#endif   // IDEVICECONTEXT_H
