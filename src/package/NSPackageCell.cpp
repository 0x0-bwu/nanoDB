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

Cell::Cell(std::string name)
 : NamedObj(std::move(name))
{
}

CircuitCell::CircuitCell(std::string name)
 : Cell(std::move(name))
{
}

FootprintCell::FootprintCell(std::string name)
 : Cell(std::move(name))
{
}

} // namespace package