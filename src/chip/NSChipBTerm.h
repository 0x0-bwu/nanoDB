#pragma once
#include <nano/core/common>
#include "NSChip.h"

namespace nano::chip {

class BTerm : public NamedObj, public Entity<BTerm>
{
public:
    BTerm(const std::string& name, Id<Net> net, IOType type);
    BTerm() = default;

    Id<Block> GetBlock() const;

    Id<Net> GetNet() const;
    IOType GetIOType() const;

private:  
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Id<Net>, net),
    (IOType, ioType))
};

inline Id<Net> BTerm::GetNet() const
{
    return m_.net;
}

inline IOType BTerm::GetIOType() const
{
    return m_.ioType;
}

} //  namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::BTerm)
