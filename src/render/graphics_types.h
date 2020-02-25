#ifndef GRAPHICS_TYPES_H
#define GRAPHICS_TYPES_H

#include <stdint.h>

namespace evnt
{
/// Stencil operation
/// [D3D11_STENCIL_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476219(v=vs.85).aspx
/// [D3D12_STENCIL_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770409(v=vs.85).aspx
/// This enumeration describes the stencil operation and generally mirrors
/// [D3D11_STENCIL_OP][]/[D3D12_STENCIL_OP][] enumeration.
/// It is used by evnt::StencilOpDesc structure to describe the stencil fail, depth fail
/// and stencil pass operations
enum STENCIL_OP : int8_t
{
    /// Undefined operation.
    STENCIL_OP_UNDEFINED = 0,
    /// Keep the existing stencil data.
    /// Direct3D counterpart: D3D11_STENCIL_OP_KEEP/D3D12_STENCIL_OP_KEEP. OpenGL counterpart: GL_KEEP.
    STENCIL_OP_KEEP = 1,
    /// Set the stencil data to 0.
    /// Direct3D counterpart: D3D11_STENCIL_OP_ZERO/D3D12_STENCIL_OP_ZERO. OpenGL counterpart: GL_ZERO.
    STENCIL_OP_ZERO = 2,
    /// Set the stencil data to the reference value set by calling IDeviceContext::SetStencilRef().
    /// Direct3D counterpart: D3D11_STENCIL_OP_REPLACE/D3D12_STENCIL_OP_REPLACE. OpenGL counterpart:
    /// GL_REPLACE.
    STENCIL_OP_REPLACE = 3,
    /// Increment the current stencil value, and clamp to the maximum representable unsigned value.
    /// Direct3D counterpart: D3D11_STENCIL_OP_INCR_SAT/D3D12_STENCIL_OP_INCR_SAT. OpenGL counterpart:
    /// GL_INCR.
    STENCIL_OP_INCR_SAT = 4,
    /// Decrement the current stencil value, and clamp to 0.
    /// Direct3D counterpart: D3D11_STENCIL_OP_DECR_SAT/D3D12_STENCIL_OP_DECR_SAT. OpenGL counterpart:
    /// GL_DECR.
    STENCIL_OP_DECR_SAT = 5,
    /// Bitwise invert the current stencil buffer value.
    /// Direct3D counterpart: D3D11_STENCIL_OP_INVERT/D3D12_STENCIL_OP_INVERT. OpenGL counterpart: GL_INVERT.
    STENCIL_OP_INVERT = 6,
    /// Increment the current stencil value, and wrap the value to zero when incrementing
    /// the maximum representable unsigned value.
    /// Direct3D counterpart: D3D11_STENCIL_OP_INCR/D3D12_STENCIL_OP_INCR. OpenGL counterpart: GL_INCR_WRAP.
    STENCIL_OP_INCR_WRAP = 7,
    /// Decrement the current stencil value, and wrap the value to the maximum representable
    /// unsigned value when decrementing a value of zero.
    /// Direct3D counterpart: D3D11_STENCIL_OP_DECR/D3D12_STENCIL_OP_DECR. OpenGL counterpart: GL_DECR_WRAP.
    STENCIL_OP_DECR_WRAP = 8,
    /// Helper value that stores the total number of stencil operations in the enumeration.
    STENCIL_OP_NUM_OPS
};

/// Blending operation
/// [D3D11_BLEND_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476088(v=vs.85).aspx
/// [D3D12_BLEND_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770340(v=vs.85).aspx
/// [glBlendEquationSeparate]: https://www.opengl.org/wiki/GLAPI/glBlendEquationSeparate
/// This enumeration describes blending operation for RGB or Alpha channels and generally mirrors
/// [D3D11_BLEND_OP][] and [D3D12_BLEND_OP][] enums. It is used by RenderTargetBlendDesc structure to define
/// RGB and Alpha blending operations [D3D11_BLEND_OP on MSDN][D3D11_BLEND_OP], [D3D12_BLEND_OP on
/// MSDN][D3D12_BLEND_OP], [glBlendEquationSeparate on OpenGL.org][glBlendEquationSeparate]
enum BLEND_OPERATION : int8_t
{
    /// Undefined blend operation
    BLEND_OPERATION_UNDEFINED = 0,
    /// Add source and destination color components.
    /// Direct3D counterpart: D3D11_BLEND_OP_ADD/D3D12_BLEND_OP_ADD. OpenGL counterpart: GL_FUNC_ADD.
    BLEND_OPERATION_ADD,
    /// Subtract destination color components from source color components.
    /// Direct3D counterpart: D3D11_BLEND_OP_SUBTRACT/D3D12_BLEND_OP_SUBTRACT. OpenGL counterpart:
    /// GL_FUNC_SUBTRACT.
    BLEND_OPERATION_SUBTRACT,
    /// Subtract source color components from destination color components.
    /// Direct3D counterpart: D3D11_BLEND_OP_REV_SUBTRACT/D3D12_BLEND_OP_REV_SUBTRACT. OpenGL counterpart:
    /// GL_FUNC_REVERSE_SUBTRACT.
    BLEND_OPERATION_REV_SUBTRACT,
    /// Compute the minimum of source and destination color components.
    /// Direct3D counterpart: D3D11_BLEND_OP_MIN/D3D12_BLEND_OP_MIN. OpenGL counterpart: GL_MIN.
    BLEND_OPERATION_MIN,
    /// Compute the maximum of source and destination color components.
    /// Direct3D counterpart: D3D11_BLEND_OP_MAX/D3D12_BLEND_OP_MAX. OpenGL counterpart: GL_MAX.
    BLEND_OPERATION_MAX,
    /// Helper value that stores the total number of blend operations in the enumeration.
    BLEND_OPERATION_NUM_OPERATIONS
};

/// Blend factors
/// [D3D11_BLEND]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476086(v=vs.85).aspx
/// [D3D12_BLEND]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770338(v=vs.85).aspx
/// [glBlendFuncSeparate]: https://www.opengl.org/wiki/GLAPI/glBlendFuncSeparate
/// This enumeration defines blend factors for alpha-blending.
/// It generatlly mirrors [D3D11_BLEND][] and [D3D12_BLEND][] enumerations and is used by
/// RenderTargetBlendDesc structure to define source and destination blend factors for color and alpha
/// channels.
/// [D3D11_BLEND on MSDN][D3D11_BLEND], [D3D12_BLEND on MSDN][D3D12_BLEND],
/// [glBlendFuncSeparate on OpenGL.org][glBlendFuncSeparate]
enum BLEND_FACTOR : int8_t
{
    /// Undefined blend factor
    BLEND_FACTOR_UNDEFINED = 0,
    /// The blend factor is zero.
    /// Direct3D counterpart: D3D11_BLEND_ZERO/D3D12_BLEND_ZERO. OpenGL counterpart: GL_ZERO.
    BLEND_FACTOR_ZERO,
    /// The blend factor is one.
    /// Direct3D counterpart: D3D11_BLEND_ONE/D3D12_BLEND_ONE. OpenGL counterpart: GL_ONE.
    BLEND_FACTOR_ONE,
    /// The blend factor is RGB data from a pixel shader.
    /// Direct3D counterpart: D3D11_BLEND_SRC_COLOR/D3D12_BLEND_SRC_COLOR. OpenGL counterpart: GL_SRC_COLOR.
    BLEND_FACTOR_SRC_COLOR,
    /// The blend factor is 1-RGB, where RGB is the data from a pixel shader.
    /// Direct3D counterpart: D3D11_BLEND_INV_SRC_COLOR/D3D12_BLEND_INV_SRC_COLOR. OpenGL counterpart:
    /// GL_ONE_MINUS_SRC_COLOR.
    BLEND_FACTOR_INV_SRC_COLOR,
    /// The blend factor is alpha (A) data from a pixel shader.
    /// Direct3D counterpart: D3D11_BLEND_SRC_ALPHA/D3D12_BLEND_SRC_ALPHA. OpenGL counterpart: GL_SRC_ALPHA.
    BLEND_FACTOR_SRC_ALPHA,
    /// The blend factor is 1-A, where A is alpha data from a pixel shader.
    /// Direct3D counterpart: D3D11_BLEND_INV_SRC_ALPHA/D3D12_BLEND_INV_SRC_ALPHA. OpenGL counterpart:
    /// GL_ONE_MINUS_SRC_ALPHA.
    BLEND_FACTOR_INV_SRC_ALPHA,
    /// The blend factor is alpha (A) data from a render target.
    /// Direct3D counterpart: D3D11_BLEND_DEST_ALPHA/D3D12_BLEND_DEST_ALPHA. OpenGL counterpart: GL_DST_ALPHA.
    BLEND_FACTOR_DEST_ALPHA,
    /// The blend factor is 1-A, where A is alpha data from a render target.
    /// Direct3D counterpart: D3D11_BLEND_INV_DEST_ALPHA/D3D12_BLEND_INV_DEST_ALPHA. OpenGL counterpart:
    /// GL_ONE_MINUS_DST_ALPHA.
    BLEND_FACTOR_INV_DEST_ALPHA,
    /// The blend factor is RGB data from a render target.
    /// Direct3D counterpart: D3D11_BLEND_DEST_COLOR/D3D12_BLEND_DEST_COLOR. OpenGL counterpart: GL_DST_COLOR.
    BLEND_FACTOR_DEST_COLOR,
    /// The blend factor is 1-RGB, where RGB is the data from a render target.
    /// Direct3D counterpart: D3D11_BLEND_INV_DEST_COLOR/D3D12_BLEND_INV_DEST_COLOR. OpenGL counterpart:
    /// GL_ONE_MINUS_DST_COLOR.
    BLEND_FACTOR_INV_DEST_COLOR,
    /// The blend factor is (f,f,f,1), where f = min(As, 1-Ad),
    /// As is alpha data from a pixel shader, and Ad is alpha data from a render target.
    /// Direct3D counterpart: D3D11_BLEND_SRC_ALPHA_SAT/D3D12_BLEND_SRC_ALPHA_SAT. OpenGL counterpart:
    /// GL_SRC_ALPHA_SATURATE.
    BLEND_FACTOR_SRC_ALPHA_SAT,
    /// The blend factor is the constant blend factor set with IDeviceContext::SetBlendFactors().
    /// Direct3D counterpart: D3D11_BLEND_BLEND_FACTOR/D3D12_BLEND_BLEND_FACTOR. OpenGL counterpart:
    /// GL_CONSTANT_COLOR.
    BLEND_FACTOR_BLEND_FACTOR,
    /// The blend factor is one minus constant blend factor set with IDeviceContext::SetBlendFactors().
    /// Direct3D counterpart: D3D11_BLEND_INV_BLEND_FACTOR/D3D12_BLEND_INV_BLEND_FACTOR. OpenGL counterpart:
    /// GL_ONE_MINUS_CONSTANT_COLOR.
    BLEND_FACTOR_INV_BLEND_FACTOR,
    /// The blend factor is the second RGB data output from a pixel shader.
    /// Direct3D counterpart: D3D11_BLEND_SRC1_COLOR/D3D12_BLEND_SRC1_COLOR. OpenGL counterpart:
    /// GL_SRC1_COLOR.
    BLEND_FACTOR_SRC1_COLOR,
    /// The blend factor is 1-RGB, where RGB is the second RGB data output from a pixel shader.
    /// Direct3D counterpart: D3D11_BLEND_INV_SRC1_COLOR/D3D12_BLEND_INV_SRC1_COLOR. OpenGL counterpart:
    /// GL_ONE_MINUS_SRC1_COLOR.
    BLEND_FACTOR_INV_SRC1_COLOR,
    /// The blend factor is the second alpha (A) data output from a pixel shader.
    /// Direct3D counterpart: D3D11_BLEND_SRC1_ALPHA/D3D12_BLEND_SRC1_ALPHA. OpenGL counterpart:
    /// GL_SRC1_ALPHA.
    BLEND_FACTOR_SRC1_ALPHA,
    /// The blend factor is 1-A, where A is the second alpha data output from a pixel shader.
    /// Direct3D counterpart: D3D11_BLEND_INV_SRC1_ALPHA/D3D12_BLEND_INV_SRC1_ALPHA. OpenGL counterpart:
    /// GL_ONE_MINUS_SRC1_ALPHA.
    BLEND_FACTOR_INV_SRC1_ALPHA,
    /// Helper value that stores the total number of blend factors in the enumeration.
    BLEND_FACTOR_NUM_FACTORS
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

    BLEND_FACTOR source_color;
    BLEND_FACTOR destination_color;
    BLEND_FACTOR source_alpha;
    BLEND_FACTOR destination_alpha;

    BLEND_OPERATION color_equation;
    BLEND_OPERATION alpha_equation;
};
}   // namespace evnt

#endif   // GRAPHICS_TYPES_H
