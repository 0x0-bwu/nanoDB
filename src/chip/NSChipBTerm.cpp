#include "NSChipBTerm.h"
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::chip::BTerm)

#include <core/chip>
namespace nano::chip {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void BTerm::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(BTerm);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(BTerm)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

BTerm::BTerm(std::string name, Id<Net> net, IOType type)
 : NamedObj(std::move(name))
{
    m_.net = net;
    m_.ioType = type;
}

Id<Block> BTerm::GetBlock() const
{
    if (m_.net.isNull()) return Id<Block>();
    return m_.net->GetBlock();
}

} // namespace nano::chip