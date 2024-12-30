#pragma once
#include "NSChip.h"

namespace nano::chip {

class BTerm : public NamedObj, public Entity<BTerm>
{
public:
    BTerm(std::string name, NetId net, IOType type);
    BTerm() = default;

    BlockId GetBlock() const;

    NetId GetNet() const;

private:  
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (NetId, net),
    (IOType, ioType))
};

inline NetId BTerm::GetNet() const
{
    return m_.net;
}

} //  namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::BTerm)
