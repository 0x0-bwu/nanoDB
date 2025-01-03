#include <core/chip>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::chip::Inst)

namespace nano::chip {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Inst::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Inst);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar)
}
NS_SERIALIZATION_FUNCTIONS_IMP(Inst)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Inst::Inst(std::string name, Id<Block> block)
 : NamedObj(std::move(name))
{
    m_.block = block;
}

void Inst::AddITerm(Id<ITerm> iterm)
{
    m_.iterms.Add(iterm);
}

} // namespace nano::design