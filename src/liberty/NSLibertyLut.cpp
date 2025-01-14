#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::Lut)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::CcsLut)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::NormalizedDriverWaveform)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Lut::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Lut);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}   
NS_SERIALIZATION_FUNCTIONS_IMP(Lut)

template <typename Archive>
void CcsLut::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Lut);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}   
NS_SERIALIZATION_FUNCTIONS_IMP(CcsLut)

template <typename Archive>
void NormalizedDriverWaveform::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Lut);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}   
NS_SERIALIZATION_FUNCTIONS_IMP(NormalizedDriverWaveform)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Lut::Lut(Id<LutTemplate> lutTemplate)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.lutTemplate = lutTemplate;
}

Lut::Lut() : Lut(Id<LutTemplate>())
{
}

size_t Lut::Dimension() const
{
    return m_.indices.size();
}

CcsLut::CcsLut(Id<LutTemplate> lutTemplate)
 : Lut(lutTemplate)
{
    m_.referenceTime = 0;
}

CcsLut::CcsLut()
 : CcsLut(Id<LutTemplate>())
{
}

NormalizedDriverWaveform::NormalizedDriverWaveform(Id<LutTemplate> lutTemplate)
 : Lut(lutTemplate)
{
}

} // namespace nano::liberty