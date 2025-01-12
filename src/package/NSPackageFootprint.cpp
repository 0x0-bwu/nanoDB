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
    m_.solderThickness = 0;
}

FootprintLocation Footprint::GetLocation() const
{
    return m_.location;
}

void Footprint::SetSolderFillingMaterial(CId<Material> material)
{
    m_.solderFillingMaterial = material;
}

CId<Material> Footprint::GetSolderFillingMaterial() const
{
    return m_.solderFillingMaterial;
}

CId<Material> Footprint::GetSolderMaterial() const
{
    return m_.solderMaterial;
}

void Footprint::SetSolderMaterial(CId<Material> material)
{
    m_.solderMaterial = material;
}

void Footprint::SetSolderBallBumpThickenss(Float thickness)
{
    m_.solderThickness = thickness;
}

Float Footprint::GetSolderBallBumpThickenss() const
{
    return m_.solderThickness;
}

void Footprint::SetBoundary(CId<Shape> boundary)
{
    m_.boundary = boundary;
}

Id<FootprintPin> Footprint::AddPin(Id<FootprintPin> pin)
{
    return m_.pins.Add(pin);
}

CId<FootprintPin> Footprint::FindPin(std::string_view name) const
{
    return m_.pins.Lookup<lut::Name>(name);
}


} // namespace nano::package