#include "NSPackage.h"
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Package)

#include <core/package>
namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Package::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Package);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Package)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Package::Package(std::string name)
 : NamedObj(std::move(name))
{
}

void Package::AddCell(CircuitCellId cell)
{
    m_.cells.emplace_back(CellId(cell));
}

} // namespace package