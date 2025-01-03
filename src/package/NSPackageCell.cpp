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

Id<Layout> CircuitCell::SetLayout(Id<Layout> layout)
{
    NS_ASSERT(Entity<Cell>::Identical(layout->GetCell()));
    m_.layout = layout;
    return m_.layout;
}

CId<Layout> CircuitCell::GetLayout() const
{
    return m_.layout;
}

Id<Layout> CircuitCell::GetLayout()
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
 : FootprintCell("", Id<Package>())
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

void FootprintCell::SetSolderFillingMaterial(Id<Material> material)
{
    m_.solderFillingMaterial = material;
}

void FootprintCell::SetBoundary(Id<Shape> boundary)
{
    m_.boundary = boundary;
}

void FootprintCell::SetMaterial(Id<Material> material)
{
    m_.material = material;
}

void FootprintCell::SetHeight(Float height)
{
    m_.height = height;
}

void FootprintCell::AddPin(Id<FootprintPin> pin)
{
    m_.pins.Add(pin);
}

} // namespace package