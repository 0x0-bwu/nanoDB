#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::ConnObj)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void ConnObj::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(ConnObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ConnObj)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

ConnObj::ConnObj(NetId net)
{
    m_.net = net;
}

} // namespace nano::package
