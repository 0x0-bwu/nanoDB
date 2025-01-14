#include <nano/core/package>
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
    NS_CLASS_MEMBERS_INITIALIZE
    m_.ioType = ioType;
}

Pin::Pin() : Pin("", IOType::UNKNOWN)
{
}

IOType Pin::GetIOType() const
{
    return m_.ioType;
}

Ptr<Pin> Pin::CloneFrom(const Pin & src)
{
    NamedObj::CloneFrom(src);
    m_ = src.m_;
    return this;
}

FootprintPin::FootprintPin(std::string name, CId<Footprint> footprint, NCoord2D location, IOType ioType)
 : Pin(std::move(name), ioType)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.footprint = footprint;
    m_.location = location;
}

FootprintPin::FootprintPin() : FootprintPin("", CId<Footprint>(), NCoord2D(), IOType::UNKNOWN)
{
}

Ptr<FootprintPin> FootprintPin::CloneFrom(const FootprintPin & src)
{
    Pin::CloneFrom(src);
    m_ = src.m_;
    return this;
}

ComponentPin::ComponentPin(std::string name, CId<ComponentLayer> componentLayer, CId<FootprintPin> footprintPin)
 : Pin(std::move(name), footprintPin ? footprintPin->GetIOType() : IOType::UNKNOWN)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.componentLayer = componentLayer;
    m_.footprintPin = footprintPin;
}

ComponentPin::ComponentPin() : ComponentPin("", CId<ComponentLayer>(), CId<FootprintPin>())
{
}

NCoord2D ComponentPin::GetLocation() const
{
    auto location = m_.footprintPin->GetLocation();
    auto & transform = m_.componentLayer->GetComponent()->GetTransform();
    generic::geometry::Transform(location, transform.GetTransform());
    return location;
}

void ComponentPin::SetComponentLayer(CId<ComponentLayer> layer)
{
    m_.componentLayer = layer;
}

Ptr<ComponentPin> ComponentPin::CloneFrom(const ComponentPin & src)
{
    Pin::CloneFrom(src);
    m_ = src.m_;
    return this;
}

} // namespace nano::package
