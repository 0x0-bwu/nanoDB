#pragma once
#include "NSPackage.h"
#include "common/NSTransform.hpp"

namespace nano::package {

class HierObj : public Entity<HierObj>
{
public:
    HierObj() = default;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (HierObjId, parent),
    (IdVec<HierObj>, children)
    )
};

class CellInst : public Transformable2D, public NamedObj, public HierObj, public Entity<CellInst>
{
public:
    CellInst(std::string name, CellId cell);
    CellInst() = default;
    CellId GetCell() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CellId, cell)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::HierObj)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::CellInst)
NS_INHERIT_FROM_BASE(nano::package::CellInst, nano::package::HierObj)