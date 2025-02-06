#include <nano/core/common>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::LookupTable)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::LookupTable1D)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::LookupTable2D)

namespace nano {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void LookupTable::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(LookupTable);
}
NS_SERIALIZATION_FUNCTIONS_IMP(LookupTable)

template <typename Archive>
void LookupTable1D::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LookupTable);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(LookupTable1D)

template <typename Archive>
void LookupTable2D::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LookupTable);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(LookupTable2D)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

LookupTable1D::LookupTable1D(Vec<Float> x, Vec<Float> y)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.lut[0] = std::move(x);
    *(m_.lut) = std::move(y);
}

Float LookupTable1D::Lookup(Float x) const
{
    return m_.lut.Lookup(x);
}

Float LookupTable1D::Lookup(Float x, Float y) const
{
    NS_UNUSED(y);
    return m_.lut.Lookup(x);
}

LookupTable2D::LookupTable2D(Vec<Float> x, Vec<Float> y, Vec<Float> z)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.lut[0] = std::move(x);
    m_.lut[1] = std::move(y);
    *(m_.lut) = std::move(z);
}

Float LookupTable2D::Lookup(Float x) const
{
    return m_.lut.Lookup(x, m_.lut[1].front());
}

Float LookupTable2D::Lookup(Float x, Float y) const
{
    return m_.lut.Lookup(x, y);
}

} // namespace nano