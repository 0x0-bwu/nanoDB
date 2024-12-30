#pragma once
#include "basic/NSDatabase.h"
#include "basic/NSContainer.hpp"
#include "common/NSCommon.hpp"
namespace nano::chip {

class Chip : public NamedObj, public Entity<Chip>
{
public:
    Chip(std::string name);
    Chip() = default;

    void AddBlock(BlockId block);

    void SetTop(BlockId block);

    BlockId GetTop() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (BlockId, top),
    (IdVec<Block>, blocks))
};

} // namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::Chip)