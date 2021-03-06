#ifndef CMPMSGS_H
#define CMPMSGS_H

#include "classids.h"
#include <any>
#include <cstdint>
#include <functional>

namespace evnt
{
class Component;

class CmpMsgsTable
{
public:
    enum msg_id : int32_t
    {
        mUndefined,
        mDidAddComponent,
        mTransformChanged,

        mLargestMsgsId
    };

    // https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    using call_ptr = std::function<void(Component * rec, msg_id id, std::any msg_data)>;
    struct key_type
    {
        msg_id      first;
        ClassIDType second;

        bool operator==(key_type const & other) const
        {
            return (first == other.first && second == other.second);
        }
    };

    struct key_hasher
    {
        std::size_t operator()(key_type const & k) const
        {
            return ((std::hash<std::int32_t>()(k.first) ^ (std::hash<std::int32_t>()(k.second) << 1)) >> 1);
        }
    };

    CmpMsgsTable() = default;

    void registerMessageCallback(msg_id mid, ClassIDType classid, call_ptr fnct);
    bool hasMessageCallback(msg_id mid, ClassIDType classid) const;
    void handleMessage(Component * rec, msg_id id, std::any msg_data) const;

private:
    std::unordered_map<key_type, call_ptr, key_hasher> m_callback_map;
};
}   // namespace evnt

#endif   // CMPMSGS_H
