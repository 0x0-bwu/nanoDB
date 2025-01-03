#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Cell)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::CircuitCell)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::FootprintCell)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Cell::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Cell);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    // NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Cell)

template <typename Archive>
void CircuitCell::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(CircuitCell);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Cell);
    // NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(CircuitCell)

template <typename Archive>
void FootprintCell::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(FootprintCell);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Cell);
    // NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(FootprintCell)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Cell::Cell(std::string name, CId<Package> package)
 : NamedObj(std::move(name))
{
    m_.package = package;
}

CircuitCell::CircuitCell(std::string name, CId<Package> package)
 : Cell(std::move(name), package)
{
}

LayoutId CircuitCell::SetLayout(LayoutId layout)
{
    NS_ASSERT(Entity<Cell>::Identical(layout->GetCell()));
    m_.layout = layout;
    return m_.layout;
}

CId<Layout> CircuitCell::GetLayout() const
{
    return m_.layout;
}

LayoutId CircuitCell::GetLayout()
{
    return m_.layout;
}

FootprintCell::FootprintCell(std::string name, CId<Package> package)
 : Cell(std::move(name), package)
{
    m_.compType = ComponentType::INVALID;
    m_.height = 0;
    m_.solderHeight = 0;
}

FootprintCell::FootprintCell()
 : FootprintCell("", PackageId())
{
}

void FootprintCell::SetComponentType(ComponentType type)
{
    m_.compType = type;
}

ComponentType FootprintCell::GetComponentType() const
{
    return m_.compType;
}

void FootprintCell::SetSolderBallBumpHeight(Float height)
{
    m_.solderHeight = height;
}

void FootprintCell::SetSolderFillingMaterial(MaterialId material)
{
    m_.solderFillingMaterial = material;
}

void FootprintCell::SetBoundary(ShapeId boundary)
{
    m_.boundary = boundary;
}

void FootprintCell::SetMaterial(MaterialId material)
{
    m_.material = material;
}

void FootprintCell::SetHeight(Float height)
{
    m_.height = height;
}

void FootprintCell::AddPin(FootprintPinId pin)
{
    m_.pins.Add(pin);
}

} // namespace package