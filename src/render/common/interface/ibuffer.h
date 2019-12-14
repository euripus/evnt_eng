#ifndef IBUFFER_H
#define IBUFFER_H

#include "ideviceobject.h"

namespace evnt
{
/// Describes the buffer access mode.
/// This enumeration is used by BufferDesc structure.
enum BUFFER_MODE : uint8_t
{
    /// Undefined mode.
    BUFFER_MODE_UNDEFINED = 0,
    /// Formated buffer. Access to the buffer will use format conversion operations.
    /// In this mode, ElementByteStride member of BufferDesc defines the buffer element size.
    /// Buffer views can use different formats, but the format size must match ElementByteStride.
    BUFFER_MODE_FORMATTED,
    /// Structured buffer.
    /// In this mode, ElementByteStride member of BufferDesc defines the structure stride.
    BUFFER_MODE_STRUCTURED,
    /// Raw buffer.
    /// In this mode, the buffer is accessed as raw bytes. Formatted views of a raw
    /// buffer can also be created similar to formatted buffer. If formatted views
    /// are to be created, the ElementByteStride member of BufferDesc must specify the
    /// size of the format.
    BUFFER_MODE_RAW,
    /// Helper value storing the total number of modes in the enumeration.
    BUFFER_MODE_NUM_MODES
};

/// Buffer description
struct BufferDesc : public DeviceObjectAttribs
{
    /// Size of the buffer, in bytes. For a uniform buffer, this must be multiple of 16.
    uint32_t size_in_bytes = 0;
    /// Buffer bind flags, see evnt::BIND_FLAGS for details
    /// The following bind flags are allowed:
    /// evnt::BIND_VERTEX_BUFFER, evnt::BIND_INDEX_BUFFER, evnt::BIND_UNIFORM_BUFFER,
    /// evnt::BIND_SHADER_RESOURCE, evnt::BIND_STREAM_OUTPUT, evnt::BIND_UNORDERED_ACCESS,
    /// evnt::BIND_INDIRECT_DRAW_ARGS
    BIND_FLAGS bind_flags = BIND_NONE;
    /// Buffer usage, see evnt::USAGE for details
    USAGE usage = USAGE_DEFAULT;
    /// CPU access flags or 0 if no CPU access is allowed,
    /// see evnt::CPU_ACCESS_FLAGS for details.
    CPU_ACCESS_FLAGS cpu_access_flags = CPU_ACCESS_NONE;
    /// Buffer mode, see evnt::BUFFER_MODE
    BUFFER_MODE mode = BUFFER_MODE_UNDEFINED;
    /// Buffer element stride, in bytes.
    /// For a structured buffer (BufferDesc::Mode equals evnt::BUFFER_MODE_STRUCTURED) this member
    /// defines the size of each buffer element. For a formatted buffer
    /// (BufferDesc::Mode equals evnt::BUFFER_MODE_FORMATTED) and optionally for a raw buffer
    /// (evnt::BUFFER_MODE_RAW), this member defines the size of the format that will be used for views
    /// created for this buffer.
    uint32_t element_byte_stride = 0;
    /// Defines which command queues this buffer can be used with
    uint64_t command_queue_mask = 1;

    // We have to explicitly define constructors because otherwise the following initialization fails on
    // Apple's clang:
    //      BufferDesc{1024, BIND_UNIFORM_BUFFER, USAGE_DEFAULT}
    BufferDesc() noexcept {}
    explicit BufferDesc(uint32_t _size_in_bytes, BIND_FLAGS _bind_flags, USAGE _usage = BufferDesc{}.usage,
                        CPU_ACCESS_FLAGS _cpu_access_flags    = BufferDesc{}.cpu_access_flags,
                        BUFFER_MODE      _mode                = BufferDesc{}.mode,
                        uint32_t         _element_byte_stride = BufferDesc{}.element_byte_stride,
                        uint64_t         _command_queue_mask  = BufferDesc{}.command_queue_mask) noexcept :
        size_in_bytes(_size_in_bytes),
        bind_flags(_bind_flags),
        usage(_usage),
        cpu_access_flags(_cpu_access_flags),
        mode(_mode),
        element_byte_stride(_element_byte_stride),
        command_queue_mask(_command_queue_mask)
    {}

    /// Tests if two structures are equivalent

    /// \param [in] RHS - reference to the structure to perform comparison with
    /// \return
    /// - True if all members of the two structures except for the Name are equal.
    /// - False otherwise.
    /// The operator ignores DeviceObjectAttribs::Name field as it does not affect
    /// the buffer description.
    bool operator==(const BufferDesc & rhs) const
    {
        return size_in_bytes == rhs.size_in_bytes && bind_flags == rhs.bind_flags && usage == rhs.usage
               && cpu_access_flags == rhs.cpu_access_flags && mode == rhs.mode
               && element_byte_stride == rhs.element_byte_stride
               && command_queue_mask == rhs.command_queue_mask;
    }
};

/// Describes the buffer initial data
struct BufferData
{
    /// Pointer to the data
    const void * p_data = nullptr;
    /// Data size, in bytes
    uint32_t data_size = 0;

    // We have to explicitly define constructors because otherwise Apple's clang fails to compile the
    // following legitimate code:
    //     BufferData{nullptr, 0}
    BufferData() noexcept {}
    BufferData(const void * _p_data, uint32_t _data_size) : p_data(_p_data), data_size(_data_size) {}
};

/// Buffer interface
/// Defines the methods to manipulate a buffer object
class IBuffer : public IDeviceObject
{
public:
    virtual ~IBuffer() {}
    /// Returns the buffer description used to create the object
    const BufferDesc & getDesc() const override = 0;

    /// Creates a new buffer view
    /// \param [in] ViewDesc - View description. See evnt::BufferViewDesc for details.
    /// \param [out] ppView - Address of the memory location where the pointer to the view interface will be
    /// written to.
    ///
    /// \remarks To create a view addressing the entire buffer, set only BufferViewDesc::ViewType member
    ///          of the ViewDesc structure and leave all other members in their default values.
    ///          Buffer view will contain strong reference to the buffer, so the buffer will not be destroyed
    ///          until all views are released.
    ///          The function calls AddRef() for the created interface, so it must be released by
    ///          a call to Release() when it is no longer needed.
    virtual void createView(const struct BufferViewDesc & view_desc, class IBufferView ** pp_view) = 0;

    /// Returns the pointer to the default view.
    /// \param [in] ViewType - Type of the requested view. See evnt::BUFFER_VIEW_TYPE.
    /// \return Pointer to the interface
    ///
    /// \remarks Default views are only created for structured and raw buffers. As for formatted buffers
    ///          the view format is unknown at buffer initialization time, no default views are created.
    ///
    /// \note The function does not increase the reference counter for the returned interface, so
    ///       Release() must *NOT* be called.
    virtual IBufferView * getDefaultView(BUFFER_VIEW_TYPE view_type) = 0;

    /// Returns native buffer handle specific to the underlying graphics API
    /// \return pointer to ID3D11Resource interface, for D3D11 implementation
    ///         pointer to ID3D12Resource interface, for D3D12 implementation
    ///         GL buffer handle, for GL implementation
    virtual void * getNativeHandle() = 0;

    /// Sets the buffer usage state.
    /// \note This method does not perform state transition, but
    ///       resets the internal buffer state to the given value.
    ///       This method should be used after the application finished
    ///       manually managing the buffer state and wants to hand over
    ///       state management back to the engine.
    virtual void setState(RESOURCE_STATE state) = 0;

    /// Returns the internal buffer state
    virtual RESOURCE_STATE getState() const = 0;
};

}   // namespace evnt
#endif   // IBUFFER_H
