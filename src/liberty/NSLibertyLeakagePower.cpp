#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::LeakagePower)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void LeakagePower::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(LeakagePower);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(LeakagePower)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

} // namespace nano::liberty