#pragma once
#include "NSPackage.h"
#include "common/NSTransform.hpp"

namespace nano::package {

class HierObj : public Entity<HierObj>
{
public:
    HierObj(CId<HierObj> parent);
    HierObj() = default;

    HierObjId AddChild(HierObjId child);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<HierObj>, parent),
    (IdVec<HierObj>, children)
    )
};

class CellInst : public Transformable2D, public NamedObj, public HierObj, public Entity<CellInst>
{
public:
    CellInst(std::string name, CId<Cell> cell, CId<CellInst> parent = CId<CellInst>());
    CellInst() = default;
    CId<Cell> GetCell() const;
    CellInstId AddCellInst(CellInstId cellInst);
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Cell>, cell)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::HierObj)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::CellInst)
NS_INHERIT_FROM_BASE(nano::package::CellInst, nano::package::HierObj)