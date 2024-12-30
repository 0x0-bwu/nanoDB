#include "NSLibertyVoltage.h"
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::Voltage)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Voltage::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Voltage);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar); 
}
NS_SERIALIZATION_FUNCTIONS_IMP(Voltage)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Voltage::Voltage(std::string name)
 : NamedObj(std::move(name))
{
}

} // namespace nano::liberty