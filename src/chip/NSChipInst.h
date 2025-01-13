#pragma once
#include "NSChip.h"

namespace nano::chip {

class Inst : public NamedObj, public Entity<Inst>
{
public:
    Inst(std::string name, Id<Block> block);
    Inst() = default;

    Id<Block> GetBlock() const;

    template <typename Range>
    void AddITerms(const Range & range)
    {
        for (auto iterm : range) AddITerm(iterm);
    }

    void AddITerm(Id<ITerm> iterm);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Id<Block>, block),
    (IdVec<ITerm>, iterms))
};

inline Id<Block> Inst::GetBlock() const
{
    return m_.block;
}

} //  namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::Inst)
