#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::InternalPower)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void InternalPower::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(InternalPower);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(InternalPower)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

InternalPower::InternalPower()
{
    NS_CLASS_MEMBERS_INITIALIZE
}

} // namespace nano::liberty