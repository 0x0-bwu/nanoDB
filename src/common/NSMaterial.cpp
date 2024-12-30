#include "NSMaterial.h"
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::common::Material)

#include <core/common>
namespace nano::common {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Material::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Material);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    // NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Material)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Material::Material(std::string name)
 : NamedObj(std::move(name))
{
}

} // namespace nano::common