#pragma once
#include "NSChip.h"

namespace nano::chip {

class ITerm : public NamedObj, public Entity<ITerm>
{
public:
    ITerm(std::string name, InstId inst, NetId net, IOType ioType);
    ITerm() = default;

    InstId GetInst() const;
    
    NetId GetNet() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (InstId, inst),
    (NetId, net),
    (IOType, ioType))
};

inline InstId ITerm::GetInst() const
{
    return m_.inst;
}

inline NetId ITerm::GetNet() const
{
    return m_.net;
}

} //  namespace nano::design
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::ITerm)
