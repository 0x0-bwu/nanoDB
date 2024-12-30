#include "NSPackageLayout.h"
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Layout)

#include <core/package>
namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Layout::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Layout);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Layout)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Layout::Layout(CellId cell)
{
    m_.cell = cell;
}

} // namespace package