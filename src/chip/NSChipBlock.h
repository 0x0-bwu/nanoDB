#pragma once
#include "NSChip.h"

namespace nano::chip {

class Block : public NamedObj, public Entity<Block>
{
public:
    Block(std::string name, char hierSep = '/');
    Block() = default;

    template <typename Range>
    void AddBTerms(const Range & range)
    {
        for (auto term : range) AddBTerm(term);
    }

    void AddBTerm(Id<BTerm> bterm);

    template <typename Range>
    void AddInsts(const Range & range)
    {
        for (auto inst : range) AddInst(inst);
    }

    void AddInst(Id<Inst> inst);

    template <typename Range>
    void AddNets(const Range & range)
    {
        for (auto net : range) AddNet(net);
    }

    void AddNet(Id<Net> net);

    Id<Net> GetNet(size_t i) const;

    size_t NumOfInsts() const;

    char HierSep() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (IdVec<BTerm>, bterms),
    (IdVec<Inst>, insts),
    (IdVec<Net>, nets),
    (char, hierSep));
};

inline char Block::HierSep() const
{
    return m_.hierSep;
}

} //  namespace nano::chip
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::chip::Block)
