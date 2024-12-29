#pragma once
#include "NSChip.h"

namespace nano::chip {

class Net : public NamedObj, public Entity<Net>
{
public:
    Net(std::string name, BlockId block);
    Net() = default;

    BlockId GetBlock() const;
    
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (BlockId, block))
};

inline BlockId Net::GetBlock() const
{
    return m_.block;
}

} //  namespace nano::design
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::Net)
