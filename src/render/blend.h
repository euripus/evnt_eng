#ifndef BLEND_H
#define BLEND_H

#include <cstdint>

namespace evnt
{
/// Blend factors
/// [D3D11_BLEND]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476086(v=vs.85).aspx
/// [D3D12_BLEND]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770338(v=vs.85).aspx
/// [glBlendFuncSeparate]: https://www.opengl.org/wiki/GLAPI/glBlendFuncSeparate
/// This enumeration defines blend factors for alpha-blending.
/// It generatlly mirrors [D3D11_BLEND][] and [D3D12_BLEND][] enumerations and is used by
/// RenderTargetBlendDesc structure to define source and destination blend factors for color and alpha
/// channels. \sa [D3D11_BLEND on MSDN][D3D11_BLEND], [D3D12_BLEND on MSDN][D3D12_BLEND], [glBlendFuncSeparate
/// on OpenGL.org][glBlendFuncSeparate]
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

/// Blending operation
/// [D3D11_BLEND_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476088(v=vs.85).aspx
/// [D3D12_BLEND_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770340(v=vs.85).aspx
/// [glBlendEquationSeparate]: https://www.opengl.org/wiki/GLAPI/glBlendEquationSeparate
/// This enumeration describes blending operation for RGB or Alpha channels and generally mirrors
/// [D3D11_BLEND_OP][] and [D3D12_BLEND_OP][] enums. It is used by RenderTargetBlendDesc structure to define
/// RGB and Alpha blending operations \sa [D3D11_BLEND_OP on MSDN][D3D11_BLEND_OP], [D3D12_BLEND_OP on
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

/// Color component write flags
/// These flags are used by RenderTargetBlendDesc structure to define
/// writable components of the render target
enum COLOR_MASK : int8_t
{
    /// Allow data to be stored in the red component.
    COLOR_MASK_RED = 1,
    /// Allow data to be stored in the green component.
    COLOR_MASK_GREEN = 2,
    /// Allow data to be stored in the blue component.
    COLOR_MASK_BLUE = 4,
    /// Allow data to be stored in the alpha component.
    COLOR_MASK_ALPHA = 8,
    /// Allow data to be stored in all components.
    COLOR_MASK_ALL = (((COLOR_MASK_RED | COLOR_MASK_GREEN) | COLOR_MASK_BLUE) | COLOR_MASK_ALPHA)
};

/// Logic operation
/// [D3D12_LOGIC_OP]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770379(v=vs.85).aspx
/// This enumeration describes logic operation and generally mirrors [D3D12_LOGIC_OP][] enum.
/// It is used by RenderTargetBlendDesc structure to define logic operation.
/// Only available on D3D12 engine
/// \sa [D3D12_LOGIC_OP on MSDN][D3D12_LOGIC_OP]
enum LOGIC_OPERATION : int8_t
{
    /// Clear the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_CLEAR.
    LOGIC_OP_CLEAR = 0,
    /// Set the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_SET.
    LOGIC_OP_SET,
    /// Copy the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_COPY.
    LOGIC_OP_COPY,
    /// Perform an inverted-copy of the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_COPY_INVERTED.
    LOGIC_OP_COPY_INVERTED,
    /// No operation is performed on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_NOOP.
    LOGIC_OP_NOOP,
    /// Invert the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_INVERT.
    LOGIC_OP_INVERT,
    /// Perform a logical AND operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_AND.
    LOGIC_OP_AND,
    /// Perform a logical NAND operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_NAND.
    LOGIC_OP_NAND,
    /// Perform a logical OR operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_OR.
    LOGIC_OP_OR,
    /// Perform a logical NOR operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_NOR.
    LOGIC_OP_NOR,
    /// Perform a logical XOR operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_XOR.
    LOGIC_OP_XOR,
    /// Perform a logical equal operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_EQUIV.
    LOGIC_OP_EQUIV,
    /// Perform a logical AND and reverse operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_AND_REVERSE.
    LOGIC_OP_AND_REVERSE,
    /// Perform a logical AND and invert operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_AND_INVERTED.
    LOGIC_OP_AND_INVERTED,
    /// Perform a logical OR and reverse operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_OR_REVERSE.
    LOGIC_OP_OR_REVERSE,
    /// Perform a logical OR and invert operation on the render target.
    /// Direct3D12 counterpart: D3D12_LOGIC_OP_OR_INVERTED.
    LOGIC_OP_OR_INVERTED,
    /// Helper value that stores the total number of logical operations in the enumeration.
    LOGIC_OP_NUM_OPERATIONS
};

/// Describes a blend state for a single render target
/// This structure is used by BlendStateDesc to describe
/// blend states for render targets
struct RenderTargetBlendDesc
{
    /// Enable or disable blending for this render target. Default value: false.
    bool blend_enable = false;
    /// Enable or disable a logical operation for this render target. Default value: false.
    bool logic_operation_enable = false;
    /// Specifies the blend factor to apply to the RGB value output from the pixel shader
    /// Default value: Diligent::BLEND_FACTOR_ONE.
    BLEND_FACTOR src_blend = BLEND_FACTOR_ONE;
    /// Specifies the blend factor to apply to the RGB value in the render target
    /// Default value: Diligent::BLEND_FACTOR_ZERO.
    BLEND_FACTOR dest_blend = BLEND_FACTOR_ZERO;
    /// Defines how to combine the source and destination RGB values
    /// after applying the SrcBlend and DestBlend factors.
    /// Default value: Diligent::BLEND_OPERATION_ADD.
    BLEND_OPERATION blend_op = BLEND_OPERATION_ADD;
    /// Specifies the blend factor to apply to the alpha value output from the pixel shader.
    /// Blend factors that end in _COLOR are not allowed.
    /// Default value: Diligent::BLEND_FACTOR_ONE.
    BLEND_FACTOR src_blend_alpha = BLEND_FACTOR_ONE;
    /// Specifies the blend factor to apply to the alpha value in the render target.
    /// Blend factors that end in _COLOR are not allowed.
    /// Default value: Diligent::BLEND_FACTOR_ZERO.
    BLEND_FACTOR dest_blend_alpha = BLEND_FACTOR_ZERO;
    /// Defines how to combine the source and destination alpha values
    /// after applying the SrcBlendAlpha and DestBlendAlpha factors.
    /// Default value: Diligent::BLEND_OPERATION_ADD.
    BLEND_OPERATION blend_op_alpha = BLEND_OPERATION_ADD;
    /// Defines logical operation for the render target.
    /// Default value: Diligent::LOGIC_OP_NOOP.
    LOGIC_OPERATION logic_op = LOGIC_OP_NOOP;
    /// Render target write mask.
    /// Default value: Diligent::COLOR_MASK_ALL.
    uint8_t render_target_write_mask = COLOR_MASK_ALL;

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     RenderTargetBlendDesc{false, false}

    RenderTargetBlendDesc() noexcept {}

    explicit RenderTargetBlendDesc(
        bool _BlendEnable, bool _LogicOperationEnable = RenderTargetBlendDesc{}.logic_operation_enable,
        BLEND_FACTOR    _SrcBlend              = RenderTargetBlendDesc{}.src_blend,
        BLEND_FACTOR    _DestBlend             = RenderTargetBlendDesc{}.dest_blend,
        BLEND_OPERATION _BlendOp               = RenderTargetBlendDesc{}.blend_op,
        BLEND_FACTOR    _SrcBlendAlpha         = RenderTargetBlendDesc{}.src_blend_alpha,
        BLEND_FACTOR    _DestBlendAlpha        = RenderTargetBlendDesc{}.dest_blend_alpha,
        BLEND_OPERATION _BlendOpAlpha          = RenderTargetBlendDesc{}.blend_op_alpha,
        LOGIC_OPERATION _LogicOp               = RenderTargetBlendDesc{}.logic_op,
        uint8_t         _RenderTargetWriteMask = RenderTargetBlendDesc{}.render_target_write_mask) :
        blend_enable(_BlendEnable),
        logic_operation_enable(_LogicOperationEnable),
        src_blend(_SrcBlend),
        dest_blend(_DestBlend),
        blend_op(_BlendOp),
        src_blend_alpha(_SrcBlendAlpha),
        dest_blend_alpha(_DestBlendAlpha),
        blend_op_alpha(_BlendOpAlpha),
        logic_op(_LogicOp),
        render_target_write_mask(_RenderTargetWriteMask)
    {}

    /// Comparison operator tests if two structures are equivalent
    /// \param [in] rhs - reference to the structure to perform comparison with
    /// \return
    /// - True if all members of the two structures are equal.
    /// - false otherwise
    bool operator==(const RenderTargetBlendDesc & rhs) const
    {
        return blend_enable == rhs.blend_enable && logic_operation_enable == rhs.logic_operation_enable
               && src_blend == rhs.src_blend && dest_blend == rhs.dest_blend && blend_op == rhs.blend_op
               && src_blend_alpha == rhs.src_blend_alpha && dest_blend_alpha == rhs.dest_blend_alpha
               && blend_op_alpha == rhs.blend_op_alpha && logic_op == rhs.logic_op
               && render_target_write_mask == rhs.render_target_write_mask;
    }
};

/// Blend state description

/// This structure describes the blend state and is part of the GraphicsPipelineDesc.
struct BlendStateDesc
{
    /// Specifies whether to use alpha-to-coverage as a multisampling technique
    /// when setting a pixel to a render target. Default value: false.
    bool alpha_to_coverage_enable = false;
    /// Specifies whether to enable independent blending in simultaneous render targets.
    /// If set to false, only RenderTargets[0] is used. Default value: false.
    bool independent_blend_enable = false;
    /// Constant member defining the maximum number of render targets
    static constexpr int s_max_render_targets = 8;
    /// An array of RenderTargetBlendDesc structures that describe the blend
    /// states for render targets
    RenderTargetBlendDesc render_targets[s_max_render_targets];

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     BlendStateDesc{false, false}

    BlendStateDesc() noexcept {}

    BlendStateDesc(bool _AlphaToCoverageEnable, bool _IndependentBlendEnable,
                   const RenderTargetBlendDesc & RT0 = RenderTargetBlendDesc{}) noexcept :
        alpha_to_coverage_enable(_AlphaToCoverageEnable),
        independent_blend_enable(_IndependentBlendEnable),
        render_targets{RT0}
    {}

    /// Comparison operator tests if two structures are equivalent
    /// \param [in] RHS - reference to the structure to perform comparison with
    /// \return
    /// - True if all members are of the two structures equal.
    ///   ote The operator performs *bitwise comparison* of the two structures.
    ///   That is if for instance both structures have IndependentBlendEnable set to false,
    ///   but differ in render target other than 0, the operator will return false
    ///   even though the two blend states created from these structures will be identical.
    /// - false otherwise
    bool operator==(const BlendStateDesc & RHS) const
    {
        bool rt_is_equal = true;
        for(int i = 0; i < s_max_render_targets; ++i)
            if(!(render_targets[i] == RHS.render_targets[i]))
            {
                rt_is_equal = false;
                break;
            }

        return rt_is_equal && alpha_to_coverage_enable == RHS.alpha_to_coverage_enable
               && independent_blend_enable == RHS.independent_blend_enable;
    }
};
}   // namespace evnt

#endif   // BLEND_H
