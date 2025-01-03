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

HierObj::HierObj(CId<HierObj> parent)
{
    m_.parent = parent;
}

Id<HierObj> HierObj::AddChild(Id<HierObj> child)
{
    return m_.children.Add(child);
}

CellInst::CellInst(std::string name, CId<Cell> cell, CId<CellInst> parent)
 : NamedObj(std::move(name)), HierObj(parent)
{
    m_.cell = cell;
}

CId<Cell> CellInst::GetCell() const
{
    return m_.cell;
}

Id<CellInst> CellInst::AddCellInst(Id<CellInst> cellInst)
{
    return Id<CellInst>(AddChild(cellInst));
}

} // namespace nano::package