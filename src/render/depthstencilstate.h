#ifndef DEPTHSTENCILSTATE_H
#define DEPTHSTENCILSTATE_H

#include "graphics_types.h"

namespace evnt
{
/// Stencil operation
/// [D3D11_STENCIL_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476219(v=vs.85).aspx
/// [D3D12_STENCIL_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770409(v=vs.85).aspx
/// This enumeration describes the stencil operation and generally mirrors
/// [D3D11_STENCIL_OP][]/[D3D12_STENCIL_OP][] enumeration.
/// It is used by Diligent::StencilOpDesc structure to describe the stencil fail, depth fail
/// and stencil pass operations
enum STENCIL_OP : int8_t
{
    /// Undefined operation.
    STENCIL_OP_UNDEFINED = 0,
    /// Keep the existing stencil data.\n
    /// Direct3D counterpart: D3D11_STENCIL_OP_KEEP/D3D12_STENCIL_OP_KEEP. OpenGL counterpart: GL_KEEP.
    STENCIL_OP_KEEP = 1,
    /// Set the stencil data to 0.\n
    /// Direct3D counterpart: D3D11_STENCIL_OP_ZERO/D3D12_STENCIL_OP_ZERO. OpenGL counterpart: GL_ZERO.
    STENCIL_OP_ZERO = 2,
    /// Set the stencil data to the reference value set by calling IDeviceContext::SetStencilRef().\n
    /// Direct3D counterpart: D3D11_STENCIL_OP_REPLACE/D3D12_STENCIL_OP_REPLACE. OpenGL counterpart:
    /// GL_REPLACE.
    STENCIL_OP_REPLACE = 3,
    /// Increment the current stencil value, and clamp to the maximum representable unsigned value.\n
    /// Direct3D counterpart: D3D11_STENCIL_OP_INCR_SAT/D3D12_STENCIL_OP_INCR_SAT. OpenGL counterpart:
    /// GL_INCR.
    STENCIL_OP_INCR_SAT = 4,
    /// Decrement the current stencil value, and clamp to 0.\n
    /// Direct3D counterpart: D3D11_STENCIL_OP_DECR_SAT/D3D12_STENCIL_OP_DECR_SAT. OpenGL counterpart:
    /// GL_DECR.
    STENCIL_OP_DECR_SAT = 5,
    /// Bitwise invert the current stencil buffer value.\n
    /// Direct3D counterpart: D3D11_STENCIL_OP_INVERT/D3D12_STENCIL_OP_INVERT. OpenGL counterpart: GL_INVERT.
    STENCIL_OP_INVERT = 6,
    /// Increment the current stencil value, and wrap the value to zero when incrementing
    /// the maximum representable unsigned value. \n
    /// Direct3D counterpart: D3D11_STENCIL_OP_INCR/D3D12_STENCIL_OP_INCR. OpenGL counterpart: GL_INCR_WRAP.
    STENCIL_OP_INCR_WRAP = 7,
    /// Decrement the current stencil value, and wrap the value to the maximum representable
    /// unsigned value when decrementing a value of zero.\n
    /// Direct3D counterpart: D3D11_STENCIL_OP_DECR/D3D12_STENCIL_OP_DECR. OpenGL counterpart: GL_DECR_WRAP.
    STENCIL_OP_DECR_WRAP = 8,
    /// Helper value that stores the total number of stencil operations in the enumeration.
    STENCIL_OP_NUM_OPS
};

/// Describes stencil operations that are performed based on the results of depth test.
/// [D3D11_DEPTH_STENCILOP_DESC]:
/// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476109(v=vs.85).aspx
/// [D3D12_DEPTH_STENCILOP_DESC]:
/// https://msdn.microsoft.com/en-us/library/windows/desktop/dn770355(v=vs.85).aspx The structure generally
/// mirrors [D3D11_DEPTH_STENCILOP_DESC][]/[D3D12_DEPTH_STENCILOP_DESC][] structure. It is used by
/// Diligent::DepthStencilStateDesc structure to describe the stencil operations for the front and back facing
/// polygons.
struct StencilOpDesc
{
    /// The stencil operation to perform when stencil testing fails.
    /// Default value: Diligent::STENCIL_OP_KEEP.
    STENCIL_OP StencilFailOp = STENCIL_OP_KEEP;
    /// The stencil operation to perform when stencil testing passes and depth testing fails.
    /// Default value: Diligent::STENCIL_OP_KEEP.
    STENCIL_OP StencilDepthFailOp = STENCIL_OP_KEEP;
    /// The stencil operation to perform when stencil testing and depth testing both pass.
    /// Default value: Diligent::STENCIL_OP_KEEP.
    STENCIL_OP StencilPassOp = STENCIL_OP_KEEP;
    /// A function that compares stencil data against existing stencil data.
    /// Default value: Diligent::COMPARISON_FUNC_ALWAYS. See Diligent::COMPARISON_FUNCTION.
    COMPARISON_FUNCTION StencilFunc = COMPARISON_FUNC_ALWAYS;

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     StencilOpDesc{STENCIL_OP_KEEP, STENCIL_OP_KEEP, STENCIL_OP_KEEP, COMPARISON_FUNC_ALWAYS}

    StencilOpDesc() noexcept {}
    StencilOpDesc(STENCIL_OP _StencilFailOp, STENCIL_OP _StencilDepthFailOp, STENCIL_OP _StencilPassOp,
                  COMPARISON_FUNCTION _StencilFunc) noexcept :
        StencilFailOp(_StencilFailOp),
        StencilDepthFailOp(_StencilDepthFailOp),
        StencilPassOp(_StencilPassOp),
        StencilFunc(_StencilFunc)
    {}

    /// Tests if two structures are equivalent
    /// \param [in] rhs - reference to the structure to perform comparison with
    /// \return
    /// - true if all members of the two structures are equal.
    /// - false otherwise
    bool operator==(const StencilOpDesc & rhs) const
    {
        return StencilFailOp == rhs.StencilFailOp && StencilDepthFailOp == rhs.StencilDepthFailOp
               && StencilPassOp == rhs.StencilPassOp && StencilFunc == rhs.StencilFunc;
    }
};

/// Depth stencil state description
/// [D3D11_DEPTH_STENCIL_DESC]:
/// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476110(v=vs.85).aspx
/// [D3D12_DEPTH_STENCIL_DESC]:
/// https://msdn.microsoft.com/en-us/library/windows/desktop/dn770356(v=vs.85).aspx This structure describes
/// the depth stencil state and is part of the GraphicsPipelineDesc. The structure generally mirrors
/// [D3D11_DEPTH_STENCIL_DESC][]/[D3D12_DEPTH_STENCIL_DESC][] structure.
struct DepthStencilStateDesc
{
    /// Enable depth-stencil operations. When it is set to false,
    /// depth test always passes, depth writes are disabled,
    /// and no stencil operations are performed. Default value: true.
    bool depth_enable = true;
    /// Enable or disable writes to a depth buffer. Default value: true.
    bool depth_write_enable = true;
    /// A function that compares depth data against existing depth data.
    /// See Diligent::COMPARISON_FUNCTION for details.
    /// Default value: Diligent::COMPARISON_FUNC_LESS.
    COMPARISON_FUNCTION depth_func = COMPARISON_FUNC_LESS;
    /// Enable stencil opertaions. Default value: false.
    bool stencil_enable = false;
    /// Identify which bits of the depth-stencil buffer are accessed when reading stencil data.
    /// Default value: 0xFF.
    uint8_t stencil_read_mask = 0xFF;
    /// Identify which bits of the depth-stencil buffer are accessed when writing stencil data.
    /// Default value: 0xFF.
    uint8_t stenci_write_mask = 0xFF;
    /// Identify stencil operations for the front-facing triangles, see Diligent::StencilOpDesc.
    StencilOpDesc front_face;
    /// Identify stencil operations for the back-facing triangles, see Diligent::StencilOpDesc.
    StencilOpDesc back_face;

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     DepthStencilStateDesc{false, false}

    DepthStencilStateDesc() noexcept {}
    DepthStencilStateDesc(bool _DepthEnable, bool _DepthWriteEnable,
                          COMPARISON_FUNCTION _DepthFunc        = DepthStencilStateDesc{}.depth_func,
                          bool                _StencilEnable    = DepthStencilStateDesc{}.stencil_enable,
                          uint8_t             _StencilReadMask  = DepthStencilStateDesc{}.stencil_read_mask,
                          uint8_t             _StencilWriteMask = DepthStencilStateDesc{}.stenci_write_mask,
                          StencilOpDesc       _FrontFace        = StencilOpDesc{},
                          StencilOpDesc       _BackFace         = StencilOpDesc{}) noexcept :
        depth_enable(_DepthEnable),
        depth_write_enable(_DepthWriteEnable),
        depth_func(_DepthFunc),
        stencil_enable(_StencilEnable),
        stencil_read_mask(_StencilReadMask),
        stenci_write_mask(_StencilWriteMask),
        front_face(_FrontFace),
        back_face(_BackFace)
    {}

    /// Tests if two structures are equivalent
    /// \param [in] rhs - reference to the structure to perform comparison with
    /// \return
    /// - true if all members of the two structures are equal.
    /// - false otherwise
    bool operator==(const DepthStencilStateDesc & rhs) const
    {
        return depth_enable == rhs.depth_enable && depth_write_enable == rhs.depth_write_enable
               && depth_func == rhs.depth_func && stencil_enable == rhs.stencil_enable
               && stencil_read_mask == rhs.stencil_read_mask && stenci_write_mask == rhs.stenci_write_mask
               && front_face == rhs.front_face && back_face == rhs.back_face;
    }
};

}   // namespace evnt

#endif   // DEPTHSTENCILSTATE_H
