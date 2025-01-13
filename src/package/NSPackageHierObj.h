#pragma once
#include <nano/common>

namespace nano::package {
class HierObj : public Entity<HierObj>
{
protected:
    HierObj(CId<HierObj> parent);
    HierObj() = default;

    void SetParent(CId<HierObj> parent);
    CId<HierObj> GetParent() const;    

    Id<HierObj> AddChild(Id<HierObj> child);

public:
    static std::string_view GetHierSep();
    static void SetHierSep(std::string );
    virtual void Flatten();

protected:
    virtual void FlattenImpl() = 0;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (CId<HierObj>, parent),
    (IdVec<HierObj>, children)
    )
    static std::string m_hierSep;
};

class CellInst : public NamedObj, public Transformable2D, public HierObj
{
public:
    CellInst(std::string name, CId<CircuitCell> cell, CId<CellInst> parent = CId<CellInst>());
    CellInst() = default;

    CId<CircuitCell> GetCell() const;

    Id<Layout> GetFlattenedLayout();
    CId<Layout> GetFlattenedLayout() const;

    Id<CellInst> AddCellInst(Id<CellInst> cellInst);

    auto GetCellInstIter();
    auto GetCellInstIter() const;

protected:
    void FlattenImpl() override;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (CId<CircuitCell>, cell),
    (Id<Layout>, flattenedLayout))
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::HierObj)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::CellInst)
NS_INHERIT_FROM_BASE(nano::package::CellInst, nano::package::HierObj)