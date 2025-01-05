#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Pin)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::FootprintPin)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::ComponentPin)

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
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Pin);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(FootprintPin)

template <typename Archive>
void ComponentPin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Pin);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Pin::Pin(std::string name, IOType ioType)
 : NamedObj(std::move(name))
{
    m_.ioType = ioType;
}

IOType Pin::GetIOType() const
{
    return m_.ioType;
}

FootprintPin::FootprintPin(std::string name, CId<Footprint> footprint, NCoord2D location, IOType ioType)
 : Pin(std::move(name), ioType)
{
    m_.footprint = footprint;
    m_.location = location;
}

ComponentPin::ComponentPin(std::string name, CId<ComponentLayer> componentLayer, CId<FootprintPin> footprintPin)
 : Pin(std::move(name), footprintPin->GetIOType())
{
    m_.componentLayer = componentLayer;
    m_.footprintPin = footprintPin;
}

} // namespace nano::package
