#include <nano/core/chip>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::chip::Block)

namespace nano::chip {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Block::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Block);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Block)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Block::Block(const std::string& name, char hierSep)
 : NamedObj(name)
{
    m_.hierSep = hierSep;
}

void Block::AddBTerm(Id<BTerm> bterm)
{
    NS_ASSERT(Identical(bterm->GetBlock()));
    m_.bterms.Add(bterm);
}

void Block::AddInst(Id<Inst> inst)
{
    NS_ASSERT(Identical(inst->GetBlock()));
    m_.insts.Add(inst);
}

void Block::AddNet(Id<Net> net)
{
    NS_ASSERT(Identical(net->GetBlock()));
    m_.nets.Add(net);
}

Id<Net> Block::GetNet(size_t i) const
{
    return m_.nets[i];
}

Id<Net> Block::FindNet(std::string_view name) const
{
    return m_.nets.Lookup<lut::Name>(name);
}

Id<Inst> Block::FindInst(std::string_view name) const
{
    return m_.insts.Lookup<lut::Name>(name);
}

Id<BTerm> Block::FindBTerm(std::string_view name) const
{
    return m_.bterms.Lookup<lut::Name>(name);
}

size_t Block::NumOfBTerms() const
{
    return m_.bterms.size();
}

size_t Block::NumOfInsts() const
{
    return m_.insts.size();
}

size_t Block::NumOfNets() const
{
    return m_.nets.size();
}

} // namespace nano::chip