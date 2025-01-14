#include <nano/core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::HierObj)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::CellInst)

#include "package/utils/NSPackageFlatten.h"
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
    NS_CLASS_MEMBERS_INITIALIZE
    m_.parent = parent;
}

HierObj::HierObj() : HierObj(CId<HierObj>())
{
}

void HierObj::Flatten()
{
    for (auto & child : m_.children)
        child->Flatten();
    FlattenImpl();
    m_.children.Destroy();
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
    NS_CLASS_MEMBERS_INITIALIZE
    m_.cell = cell;
}

CellInst::CellInst() : CellInst("", CId<CircuitCell>())
{
}

CId<CircuitCell> CellInst::GetCell() const
{
    return m_.cell;
}

Id<Layout> CellInst::GetFlattenedLayout()
{
    //should call Flatten() first
    return m_.flattenedLayout;
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

auto CellInst::GetCellInstIter()
{
    return HierObj::m_.children.GetIter<CellInst>();
}

auto CellInst::GetCellInstIter() const
{
    return HierObj::m_.children.GetCIter<CellInst>();
}

void CellInst::FlattenImpl()
{
    if (m_.flattenedLayout)
        m_.flattenedLayout.Destroy();
    m_.flattenedLayout = m_.cell->GetLayout()->Clone();
    NS_ASSERT(m_.flattenedLayout);
    auto iter = GetCellInstIter();
    while (auto cellInst = iter.Next()) {
        NS_ASSERT(cellInst->m_.flattenedLayout);
        [[maybe_unused]] auto res =
        utils::FlattenUtility::Merge(m_.flattenedLayout,
            cellInst->m_.flattenedLayout, 
            cellInst->GetName());
        NS_ASSERT(res);
        cellInst->m_.flattenedLayout.Destroy();
    }
    m_.flattenedLayout->Transform(GetTransform());
}

} // namespace nano::package