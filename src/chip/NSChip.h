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

    void AddBlock(Id<Block> block);

    void SetTop(Id<Block> block);

    Id<Block> GetTop() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Block>, top),
    (IdVec<Block>, blocks))
};

} // namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::Chip)