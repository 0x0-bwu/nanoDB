#pragma once
#include "NSChip.h"

namespace nano::chip {

class Net : public NamedObj, public Entity<Net>
{
public:
    Net(const std::string& name, Id<Block> block);
    Net() = default;

    Id<Block> GetBlock() const;
    
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Id<Block>, block))
};

inline Id<Block> Net::GetBlock() const
{
    return m_.block;
}

} //  namespace nano::design
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::Net)
