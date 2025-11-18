#pragma once
#include <nano/common>

namespace nano::chip {

class Chip : public NamedObj, public Entity<Chip>
{
public:
    explicit Chip(const std::string& name);
    Chip() = default;

    void AddBlock(Id<Block> block);

    void SetTop(Id<Block> block);

    Id<Block> GetTop() const;

    Id<Block> FindBlock(std::string_view name) const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Id<Block>, top),
    (IdVec<Block, NameLut>, blocks))
};

} // namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::Chip)