#pragma once
#include "NSChip.h"

namespace nano::chip {

class BTerm : public NamedObj, public Entity<BTerm>
{
public:
    BTerm(std::string name, Id<Net> net, IOType type);
    BTerm() = default;

    Id<Block> GetBlock() const;

    Id<Net> GetNet() const;

private:  
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Net>, net),
    (IOType, ioType))
};

inline Id<Net> BTerm::GetNet() const
{
    return m_.net;
}

} //  namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::BTerm)
