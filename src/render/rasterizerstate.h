#ifndef RASTERIZERSTATE_H
#define RASTERIZERSTATE_H

#include "graphics_types.h"

namespace evnt
{
/// Fill mode
/// [D3D11_FILL_MODE]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476131(v=vs.85).aspx
/// [D3D12_FILL_MODE]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770366(v=vs.85).aspx
/// This enumeration determines the fill mode to use when rendering triangles and mirrors the
/// [D3D11_FILL_MODE][]/[D3D12_FILL_MODE][] enum. It is used by RasterizerStateDesc structure to define the
/// fill mode.
enum FILL_MODE : int8_t
{
    /// Undefined fill mode.
    FILL_MODE_UNDEFINED = 0,
    /// Rasterize triangles using wireframe fill.
    /// Direct3D counterpart: D3D11_FILL_WIREFRAME/D3D12_FILL_MODE_WIREFRAME. OpenGL counterpart: GL_LINE.
    FILL_MODE_WIREFRAME,
    /// Rasterize triangles using solid fill.
    /// Direct3D counterpart: D3D11_FILL_SOLID/D3D12_FILL_MODE_SOLID. OpenGL counterpart: GL_FILL.
    FILL_MODE_SOLID,
    /// Helper value that stores the total number of fill modes in the enumeration.
    FILL_MODE_NUM_MODES
};

/// Cull mode
/// [D3D11_CULL_MODE]: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476108(v=vs.85).aspx
/// [D3D12_CULL_MODE]: https://msdn.microsoft.com/en-us/library/windows/desktop/dn770354(v=vs.85).aspx
/// This enumeration defines which triangles are not drawn during the rasterization and mirrors
/// [D3D11_CULL_MODE][]/[D3D12_CULL_MODE][] enum. It is used by RasterizerStateDesc structure to define the
/// polygon cull mode.
enum CULL_MODE : int8_t
{
    /// Undefined cull mode.
    CULL_MODE_UNDEFINED = 0,
    /// Draw all triangles.
    /// Direct3D counterpart: D3D11_CULL_NONE/D3D12_CULL_MODE_NONE. OpenGL counterpart: glDisable(
    /// GL_CULL_FACE ).
    CULL_MODE_NONE,
    /// Do not draw triangles that are front-facing. Front- and back-facing triangles are determined
    /// by the RasterizerStateDesc::FrontCounterClockwise member.
    /// Direct3D counterpart: D3D11_CULL_FRONT/D3D12_CULL_MODE_FRONT. OpenGL counterpart: GL_FRONT.
    CULL_MODE_FRONT,
    /// Do not draw triangles that are back-facing. Front- and back-facing triangles are determined
    /// by the RasterizerStateDesc::FrontCounterClockwise member.
    /// Direct3D counterpart: D3D11_CULL_BACK/D3D12_CULL_MODE_BACK. OpenGL counterpart: GL_BACK.
    CULL_MODE_BACK,
    /// Helper value that stores the total number of cull modes in the enumeration.
    CULL_MODE_NUM_MODES
};

/// Rasterizer state description
/// This structure describes the rasterizer state and is part of the GraphicsPipelineDesc.
struct RasterizerStateDesc
{
    /// Determines traingle fill mode, see evnt::FILL_MODE for details.
    /// Default value: evnt::FILL_MODE_SOLID.
    FILL_MODE fill_mode = FILL_MODE_SOLID;
    /// Determines traingle cull mode, see evnt::CULL_MODE for details.
    /// Default value: evnt::CULL_MODE_BACK.
    CULL_MODE cull_mode = CULL_MODE_BACK;
    /// Determines if a triangle is front- or back-facing. If this parameter is true,
    /// a triangle will be considered front-facing if its vertices are counter-clockwise
    /// on the render target and considered back-facing if they are clockwise.
    /// If this parameter is false, the opposite is true.
    /// Default value: false.
    bool front_counter_clockwise = false;
    /// Enable clipping based on distance.
    /// \warning On DirectX this only disables clipping against far clipping plane,
    ///          while on OpenGL this disables clipping against both far and near clip planes.
    /// Default value: true.
    bool depth_clip_enable = true;
    /// Enable scissor-rectangle culling. All pixels outside an active scissor rectangle are culled.
    /// Default value: false.
    bool scissor_enable = false;
    /// Specifies whether to enable line antialiasing.
    /// Default value: false.
    bool antialiased_line_enable = false;
    /// Constant value added to the depth of a given pixel.
    /// Default value: 0.
    int32_t depth_bias = 0;
    /// Maximum depth bias of a pixel.
    /// \warning Depth bias clamp is not available in OpenGL
    /// Default value: 0.
    float depth_bias_clamp = 0.f;
    /// Scalar that scales the given pixel's slope before adding to the pixel's depth.
    /// Default value: 0.
    float slope_scaled_depth_bias = 0.f;

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     RasterizerStateDesc{FILL_MODE_SOLID, CULL_MODE_BACK}

    RasterizerStateDesc() noexcept {}

    RasterizerStateDesc(
        FILL_MODE _FillMode, CULL_MODE _CullMode,
        bool    _FrontCounterClockwise = RasterizerStateDesc{}.front_counter_clockwise,
        bool    _DepthClipEnable       = RasterizerStateDesc{}.depth_clip_enable,
        bool    _ScissorEnable         = RasterizerStateDesc{}.scissor_enable,
        bool    _AntialiasedLineEnable = RasterizerStateDesc{}.antialiased_line_enable,
        int32_t _DepthBias             = RasterizerStateDesc{}.depth_bias,
        float   _DepthBiasClamp        = RasterizerStateDesc{}.depth_bias_clamp,
        float   _SlopeScaledDepthBias  = RasterizerStateDesc{}.slope_scaled_depth_bias) noexcept :
        fill_mode(_FillMode),
        cull_mode(_CullMode),
        front_counter_clockwise(_FrontCounterClockwise),
        depth_clip_enable(_DepthClipEnable),
        scissor_enable(_ScissorEnable),
        antialiased_line_enable(_AntialiasedLineEnable),
        depth_bias(_DepthBias),
        depth_bias_clamp(_DepthBiasClamp),
        slope_scaled_depth_bias(_SlopeScaledDepthBias)
    {}

    /// Tests if two structures are equivalent
    /// \param [in] RHS - reference to the structure to perform comparison with
    /// \return
    /// - true if all members of the two structures are equal.
    /// - false otherwise
    bool operator==(const RasterizerStateDesc & rhs) const
    {
        return fill_mode == rhs.fill_mode && cull_mode == rhs.cull_mode
               && front_counter_clockwise == rhs.front_counter_clockwise && depth_bias == rhs.depth_bias
               && depth_bias_clamp == rhs.depth_bias_clamp
               && slope_scaled_depth_bias == rhs.slope_scaled_depth_bias
               && depth_clip_enable == rhs.depth_clip_enable && scissor_enable == rhs.scissor_enable
               && antialiased_line_enable == rhs.antialiased_line_enable;
    }
};
}   // namespace evnt

#endif   // RASTERIZERSTATE_H
