#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::OutputCurrent)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void OutputCurrent::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(OutputCurrent);
    NS_SERIALIZATION_CLASS_MEMBERS(ar); 
}
NS_SERIALIZATION_FUNCTIONS_IMP(OutputCurrent)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

void OutputCurrent::AddCcsLut(Id<CcsLut> ccsLut)
{
    m_.vectors.emplace_back(ccsLut);
}

OutputCurrent::OutputCurrent()
{
    NS_CLASS_MEMBERS_INITIALIZE
}

} // namespace nano::liberty