#pragma once
#include "basic/NSContainer.hpp"

namespace nano::package {

class Layout : public Clonable<Layout>, public Entity<Layout>
{
public:
    explicit Layout(CId<CircuitCell> cell);
    Layout() = default;

    void SetBoundary(Id<Shape> boundary);
    Id<Shape> GetBoundary() const { return m_.boundary; }

    CId<CircuitCell> GetCell() const { return m_.cell; }

    Id<Net> AddNet(Id<Net> net);
    Id<ConnObj> AddConnObj(Id<ConnObj> connObj);
    Id<Component> AddComponent(Id<Component> component);

    auto GetConnObjIter() { return m_.connObjs.GetIter<ConnObj>(); }
    auto GetConnObjIter() const { return m_.connObjs.GetCIter<ConnObj>(); }

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLONE_FUNCTIONS_DECLARATION(Layout)
    NS_DEFINE_CLASS_MEMBERS(
    (CId<CircuitCell>, cell),
    (Id<Shape>, boundary),
    (IdVec<Net, NameLut>, nets),
    (IdVec<ConnObj>, connObjs),
    (IdVec<Component>, components)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Layout)