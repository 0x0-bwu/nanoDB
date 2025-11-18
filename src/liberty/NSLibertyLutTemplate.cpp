#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::LutTemplate)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void LutTemplate::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(LutTemplate);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(LutTemplate)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

LutTemplate::LutTemplate(const std::string& name)
 : NamedObj(name)
{
    NS_CLASS_MEMBERS_INITIALIZE
}

LutTemplate::LutTemplate()
 : LutTemplate("")
{
}

} // namespace nano::liberty


