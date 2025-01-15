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

UPtr<Shape> Padstack::Via::GetShape() const
{
    if (not shape) return nullptr;
    auto res = shape->UniqueClone();
    res->Transform(makeTransform2D(1.0, rotation, offset[0], offset[1]));
    return res;
}

UPtr<Shape> Padstack::Pad::GetShape() const
{
    if (not shape) return nullptr;
    auto res = shape->UniqueClone();
    res->Transform(makeTransform2D(1.0, rotation, offset[0], offset[1]));
    return res;
}

Padstack::Padstack(std::string name, CId<Package> package)
 : NamedObj(std::move(name))
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.package = package;
}

Padstack::Padstack() : Padstack("", CId<Package>())
{
}

void Padstack::SetTopSolderBumpMaterial(CId<Material> material)
{
    m_.solderBump.material = material;
}

void Padstack::SetBotSolderBallMaterial(CId<Material> material)
{
    m_.solderBall.material = material;
}

CId<Material> Padstack::GetTopSolderBumpMaterial() const
{
    return m_.solderBump.material;
}

CId<Material> Padstack::GetBotSolderBallMaterial() const
{
    return m_.solderBall.material;
}

void Padstack::SetTopSolderBumpParameters(CId<Shape> shape, Float thickness)
{
    m_.solderBump.shape = shape;
    m_.solderBump.thickness = thickness;
}

void Padstack::SetBotSolderBallParameters(CId<Shape> shape, Float thickness)
{
    m_.solderBall.shape = shape;
    m_.solderBall.thickness = thickness;
}

bool Padstack::GetTopSolderBumpParameters(CId<Shape> & shape, Float & thickness) const
{
    shape = m_.solderBump.shape;
    thickness = m_.solderBump.thickness;
    return shape && thickness > 0;
}

bool Padstack::GetBotSolderBallParameters(CId<Shape> & shape, Float & thickness) const
{
    shape = m_.solderBall.shape;
    thickness = m_.solderBall.thickness;
    return shape && thickness > 0;
}

bool Padstack::hasTopSolderBump() const
{
    return m_.solderBump.shape && m_.solderBump.thickness > 0;
}

bool Padstack::hasBotSolderBall() const
{
    return m_.solderBall.shape && m_.solderBall.thickness > 0;
}

CId<Material> Padstack::GetMaterial() const
{
    return m_.material;
}

UPtr<Shape> Padstack::GetPadShape(CId<StackupLayer> layer) const
{
    auto iter = m_.pads.find(layer);
    if (iter == m_.pads.cend()) return nullptr;
    return iter->second.GetShape();
}

UPtr<Shape> Padstack::GetViaShape() const
{
    return m_.via.GetShape();
}
} // namespace nano::package

