#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Footprint)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Footprint::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Footprint);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Footprint)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Footprint::Footprint(std::string name, CId<FootprintCell> footprint, FootprintLocation location)
 : NamedObj(std::move(name))
{
    m_.footprint = footprint;
    m_.location = location;
    m_.solderHeight = 0;
}

void Footprint::SetSolderFillingMaterial(CId<Material> material)
{
    m_.solderFillingMaterial = material;
}

void Footprint::SetSolderMaterial(CId<Material> material)
{
    m_.solderMaterial = material;
}

void Footprint::SetSolderBallBumpHeight(Float height)
{
    m_.solderHeight = height;
}

void Footprint::SetBoundary(CId<Shape> boundary)
{
    m_.boundary = boundary;
}

Id<FootprintPin> Footprint::AddPin(Id<FootprintPin> pin)
{
    return m_.pins.Add(pin);
}


} // namespace nano::package