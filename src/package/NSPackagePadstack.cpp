#include <nano/core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Padstack)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Padstack::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Padstack);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Padstack)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Padstack::Padstack(std::string name, Id<Package> package)
 : NamedObj(std::move(name))
{
    m_.package = package;
}

void Padstack::SetTopSolderBumpMaterial(Id<Material> material)
{
    m_.solderBump.material = material;
}

void Padstack::SetBotSolderBallMaterial(Id<Material> material)
{
    m_.solderBall.material = material;
}

void Padstack::SetTopSolderBumpParameters(Id<Shape> shape, Float thickness)
{
    m_.solderBump.shape = shape;
    m_.solderBump.thickness = thickness;
}

void Padstack::SetBotSolderBallParameters(Id<Shape> shape, Float thickness)
{
    m_.solderBall.shape = shape;
    m_.solderBall.thickness = thickness;
}

} // nano::package

