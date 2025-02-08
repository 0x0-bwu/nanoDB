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
    Ptr<Shape> res;
    if (shape->hasHole())
        res = new ShapePolygonWithHoles(shape->GetContour());
    else res = new ShapePolygon(shape->GetOutline());
    res->Transform(makeTransform2D(1.0, rotation, offset[0], offset[1]));
    return UPtr<Shape>(res);
}

UPtr<Shape> Padstack::Pad::GetShape() const
{
    if (not shape) return nullptr;
    Ptr<Shape> res;
    if (shape->hasHole())
        res = new ShapePolygonWithHoles(shape->GetContour());
    else res = new ShapePolygon(shape->GetOutline());
    res->Transform(makeTransform2D(1.0, rotation, offset[0], offset[1]));
    return UPtr<Shape>(res);
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

void Padstack::SetPadShape(CId<StackupLayer> layer, CId<Shape> shape, const NCoord2D & offset, Float rotation)
{
    m_.pads[layer].shape = shape;
    m_.pads[layer].offset = offset;
    m_.pads[layer].rotation = rotation;
}

UPtr<Shape> Padstack::GetPadShape(CId<StackupLayer> layer) const
{
    auto iter = m_.pads.find(layer);
    if (iter == m_.pads.cend()) return nullptr;
    return iter->second.GetShape();
}

void Padstack::SetViaShape(CId<Shape> shape, const NCoord2D & offset, Float rotation)
{
    m_.via.shape = shape;
    m_.via.offset = offset;
    m_.via.rotation = rotation;
}

UPtr<Shape> Padstack::GetViaShape() const
{
    return m_.via.GetShape();
}

CId<Package> Padstack::GetPackage() const
{
    return m_.package;
}

} // namespace nano::package

