#pragma once
#include "NSChip.h"

namespace nano::chip {

class ITerm : public NamedObj, public Entity<ITerm>
{
public:
    ITerm(std::string name, Id<Inst> inst, Id<Net> net, IOType ioType);
    ITerm() = default;

    Id<Inst> GetInst() const;
    
    Id<Net> GetNet() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Inst>, inst),
    (Id<Net>, net),
    (IOType, ioType))
};

inline Id<Inst> ITerm::GetInst() const
{
    return m_.inst;
}

inline Id<Net> ITerm::GetNet() const
{
    return m_.net;
}

} //  namespace nano::design
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::ITerm)
