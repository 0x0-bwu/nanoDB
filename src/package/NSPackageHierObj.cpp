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
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Transformable2D);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(HierObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(CellInst)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

std::string HierObj::m_hierSep = "/";
std::string_view HierObj::GetHierSep()
{
    return m_hierSep.c_str();
}

void HierObj::SetHierSep(std::string sep)
{
    m_hierSep = std::move(sep);
}

HierObj::HierObj(CId<HierObj> parent)
{
    m_.parent = parent;
}

void HierObj::Flatten()
{
    for (auto & child : m_.children)
        child->Flatten();
    FlattenImpl();
}

void HierObj::SetParent(CId<HierObj> parent)
{
    m_.parent = parent;
}

CId<HierObj> HierObj::GetParent() const
{
    return m_.parent;
}

Id<HierObj> HierObj::AddChild(Id<HierObj> child)
{
    child->SetParent(Entity<HierObj>::GetCId());
    return m_.children.Add(child);
}

CellInst::CellInst(std::string name, CId<CircuitCell> cell, CId<CellInst> parent)
 : NamedObj(std::move(name)), HierObj(parent)
{
    m_.cell = cell;
}

CId<CircuitCell> CellInst::GetCell() const
{
    return m_.cell;
}

CId<Layout> CellInst::GetFlattenedLayout() const
{
    //should call Flatten() first
    return m_.flattenedLayout;
} 

Id<CellInst> CellInst::AddCellInst(Id<CellInst> cellInst)
{
    AddChild(cellInst);
    return cellInst;
}

auto CellInst::GetCellInstIter() const
{
    return (*this)--->children.GetCIter<CellInst>();
}

void CellInst::FlattenImpl()
{
    if (m_.flattenedLayout) {
        nano::Remove(m_.flattenedLayout);
        m_.flattenedLayout = nano::Clone<Layout>(m_.cell->GetLayout());
    }
    auto iter = GetCellInstIter();
    while (auto cellInst = iter.Next()) {
        //todo
    }
}

} // namespace nano::package