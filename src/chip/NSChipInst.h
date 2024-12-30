#pragma once
#include "NSChip.h"

namespace nano::chip {

class Inst : public NamedObj, public Entity<Inst>
{
public:
    Inst(std::string name, BlockId block);
    Inst() = default;

    BlockId GetBlock() const;

    template <typename Range>
    void AddITerms(const Range & range)
    {
        for (auto iterm : range) AddITerm(iterm);
    }

    void AddITerm(ITermId iterm);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (BlockId, block),
    (IdVec<ITerm>, iterms))
};

inline BlockId Inst::GetBlock() const
{
    return m_.block;
}

} //  namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::Inst)
