#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Interface)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Interface::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Interface);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Interface)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Interface::Interface(std::string name, CId<FootprintCell> footprint, InterfaceLocation location)
 : NamedObj(std::move(name))
{
    m_.footprint = footprint;
    m_.location = location;
    m_.solderHeight = 0;
}

void Interface::SetSolderFillingMaterial(CId<Material> material)
{
    m_.solderFillingMaterial = material;
}

void Interface::SetSolderMaterial(CId<Material> material)
{
    m_.solderMaterial = material;
}

void Interface::SetSolderBallBumpHeight(Float height)
{
    m_.solderHeight = height;
}

void Interface::SetBoundary(CId<Shape> boundary)
{
    m_.boundary = boundary;
}

Id<FootprintPin> Interface::AddPin(Id<FootprintPin> pin)
{
    return m_.pins.Add(pin);
}


} // namespace nano::package