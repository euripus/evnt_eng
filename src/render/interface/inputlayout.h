#ifndef INPUTLAYOUT_H
#define INPUTLAYOUT_H

#include "../graphics_types.h"

namespace evnt
{
static constexpr uint32_t s_max_layout_elements = 16;

/// Description of a single element of the input layout
struct LayoutElement
{
    static constexpr uint32_t s_auto_offset = static_cast<uint32_t>(-1);
    static constexpr uint32_t s_auto_stride = static_cast<uint32_t>(-1);

    /// Input index of the element, which is specified in the vertex shader.
    uint32_t input_index = 0;
    /// Buffer slot index that this element is read from.
    uint32_t buffer_slot = 0;
    /// Number of components in the element. Allowed values are 1, 2, 3, and 4.
    uint32_t num_components = 0;
    /// Type of the element components, see evnt::VALUE_TYPE for details.
    VALUE_TYPE value_type = VT_FLOAT32;
    /// For signed and unsigned integer value types
    /// (VT_INT8, VT_INT16, VT_INT32, VT_UINT8, VT_UINT16, VT_uint32_t)
    /// indicates if the value should be normalized to [-1,+1] or
    /// [0, 1] range respectively. For floating point types
    /// (VT_FLOAT16 and VT_FLOAT32), this member is ignored.
    bool is_normalized = true;
    /// Relative offset, in bytes, to the element bits.
    /// If this value is set to LayoutElement::AutoOffset (default value), the offset will
    /// be computed automatically by placing the element right after the previous one.
    uint32_t relative_offset = s_auto_offset;
    /// Stride, in bytes, between two elements, for this buffer slot.
    /// If this value is set to LayoutElement::AutoStride, the stride will be
    /// computed automatically assuming that all elements in the same buffer slot are
    /// packed one after another. If the buffer slot contains multiple layout elements,
    /// they all must specify the same stride or use AutoStride value.
    uint32_t stride = s_auto_stride;
    /// Input frequency
    enum FREQUENCY : int32_t
    {
        /// Frequency is undefined.
        FREQUENCY_UNDEFINED = 0,

        /// Input data is per-vertex data.
        FREQUENCY_PER_VERTEX,

        /// Input data is per-instance data.
        FREQUENCY_PER_INSTANCE,

        /// Helper value that stores the total number of frequencies in the enumeration.
        FREQUENCY_NUM_FREQUENCIES
    };
    FREQUENCY frequency = FREQUENCY_PER_VERTEX;
    /// The number of instances to draw using the same per-instance data before advancing
    /// in the buffer by one element.
    uint32_t instance_data_step_rate = 1;

    LayoutElement() noexcept {}
    /// Initializes the structure
    LayoutElement(uint32_t _input_index, uint32_t _buffer_slot, uint32_t _num_components,
                  VALUE_TYPE _value_type, bool _is_normalized = LayoutElement{}.is_normalized,
                  uint32_t _relative_offset = LayoutElement{}.relative_offset,
                  uint32_t _stride = LayoutElement{}.stride, FREQUENCY _Frequency = LayoutElement{}.frequency,
                  uint32_t _instance_data_step_rate = LayoutElement{}.instance_data_step_rate) noexcept :
        input_index(_input_index),
        buffer_slot(_buffer_slot),
        num_components(_num_components),
        value_type(_value_type),
        is_normalized(_is_normalized),
        relative_offset(_relative_offset),
        stride(_stride),
        frequency(_Frequency),
        instance_data_step_rate(_instance_data_step_rate)
    {}
};

/// Layout description
/// This structure is used by IRenderDevice::CreatePipelineState().
struct InputLayoutDesc
{
    /// Array of layout elements
    const LayoutElement * layout_elements = nullptr;
    /// Number of layout elements
    uint32_t num_elements = 0;

    InputLayoutDesc() noexcept {}
    InputLayoutDesc(const LayoutElement * _layout_elements, uint32_t _num_elements) noexcept :
        layout_elements(_layout_elements), num_elements(_num_elements)
    {}
};
}   // namespace evnt

#endif   // INPUTLAYOUT_H
