#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::HierObj)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::CellInst)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void HierObj::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(HierObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(HierObj)

template <typename Archive>
void CellInst::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(CellInst);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Transformable2D);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(HierObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(CellInst)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

CellInst::CellInst(std::string name, CellId cell)
 : NamedObj(std::move(name))
{
    m_.cell = cell;
}

CellId CellInst::GetCell() const
{
    return m_.cell;
}

} // namespace nano::package