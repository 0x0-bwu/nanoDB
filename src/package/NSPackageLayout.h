#pragma once
#include "NSPackage.h"

namespace nano::package {

class Layout : public Entity<Layout>
{
public:
    explicit Layout(CId<Cell> cell);
    Layout() = default;

    void SetBoundary(Id<Shape> boundary);
    Id<Shape> GetBoundary() const { return m_.boundary; }

    CId<Cell> GetCell() const { return m_.cell; }

    Id<Net> AddNet(Id<Net> net);
    Id<ConnObj> AddConnObj(Id<ConnObj> connObj);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Cell>, cell),
    (Id<Shape>, boundary),
    (IdVec<Net, NameLut>, nets),
    (IdVec<ConnObj>, connObjs)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Layout)