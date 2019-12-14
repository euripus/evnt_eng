#ifndef IBUFFERVIEW_H
#define IBUFFERVIEW_H

#include "ibuffer.h"

namespace evnt
{
/// Buffer format description
struct BufferFormat
{
    /// Type of components. For a formatted buffer views, this value cannot be VT_UNDEFINED
    VALUE_TYPE value_type = VT_UNDEFINED;
    /// Number of components. Allowed values: 1, 2, 3, 4.
    /// For a formatted buffer, this value cannot be 0
    uint8_t num_components = 0;
    /// For signed and unsigned integer value types
    /// (VT_INT8, VT_INT16, VT_INT32, VT_UINT8, VT_UINT16, VT_uint32_t)
    /// indicates if the value should be normalized to [-1,+1] or
    /// [0, 1] range respectively. For floating point types
    /// (VT_FLOAT16 and VT_FLOAT32), this member is ignored.
    bool is_normalized = false;

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     BufferFormat{VT_FLOAT32, 4}
    BufferFormat() noexcept {}
    BufferFormat(VALUE_TYPE _value_type, uint8_t _num_components,
                 bool _is_normalized = BufferFormat{}.is_normalized) noexcept :
        value_type(_value_type), num_components(_num_components), is_normalized(_is_normalized)
    {}

    /// Tests if two structures are equivalent
    bool operator==(const BufferFormat & rhs) const
    {
        return value_type == rhs.value_type && num_components == rhs.num_components
               && is_normalized == rhs.is_normalized;
    }
};

/// Buffer view description
struct BufferViewDesc : DeviceObjectAttribs
{
    /// View type. See evnt::BUFFER_VIEW_TYPE for details.
    BUFFER_VIEW_TYPE view_type = BUFFER_VIEW_UNDEFINED;
    /// Format of the view. This member is only used for formatted and raw buffers.
    /// To create raw view of a raw buffer, set Format.ValueType member to VT_UNDEFINED
    /// (default value).
    BufferFormat format;
    /// Offset in bytes from the beginnig of the buffer to the start of the
    /// buffer region referenced by the view
    uint32_t byte_offset = 0;
    /// Size in bytes of the referenced buffer region
    uint32_t byte_width = 0;

    BufferViewDesc() noexcept {}
    explicit BufferViewDesc(BUFFER_VIEW_TYPE _view_type, BufferFormat _format = BufferViewDesc{}.format,
                            uint32_t _byte_offset = BufferViewDesc{}.byte_offset,
                            uint32_t _byte_width  = BufferViewDesc{}.byte_width) noexcept :
        view_type(_view_type), format(_format), byte_offset(_byte_offset), byte_width(_byte_width)
    {}

    /// Comparison operator tests if two structures are equivalent
    /// \param [in] RHS - reference to the structure to perform comparison with
    /// \return
    /// - True if all members of the two structures are equal.
    /// - False otherwise
    /// \remarks
    /// The operator ignores DeviceObjectAttribs::Name field.
    bool operator==(const BufferViewDesc & rhs) const
    {
        // Name is primarily used for debug purposes and does not affect the view.
        // It is ignored in comparison operation.
        return   // strcmp(Name, RHS.Name) == 0 &&
            view_type == rhs.view_type && byte_offset == rhs.byte_offset && byte_width == rhs.byte_width
            && format == rhs.format;
    }
};

/// Buffer view interface
/// To create a buffer view, call IBuffer::CreateView().
/// \remarks
/// Buffer view holds strong references to the buffer. The buffer
/// will not be destroyed until all views are released.
class IBufferView : public IDeviceObject
{
public:
    /// Returns the buffer view description used to create the object
    virtual const BufferViewDesc & getDesc() const override = 0;
    /// Returns pointer to the referenced buffer object.
    /// The method does *NOT* call AddRef() on the returned interface,
    /// so Release() must not be called.
    virtual IBuffer * getBuffer() = 0;
};

}   // namespace evnt

#endif   // IBUFFERVIEW_H
