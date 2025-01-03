#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Pin)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::FootprintPin)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Pin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Pin);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Pin)

template <typename Archive>
void FootprintPin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(FootprintPin);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Pin);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(FootprintPin)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Pin::Pin(std::string name, NCoord2D location, IOType ioType)
 : NamedObj(std::move(name))
{
    m_.location = location;
    m_.ioType = ioType;
}

FootprintPin::FootprintPin(std::string name, Id<FootprintCell> cell, NCoord2D location, IOType ioType)
 : Pin(std::move(name), location, ioType)
{
    m_.cell = cell;
}

} // namespace nano::package
