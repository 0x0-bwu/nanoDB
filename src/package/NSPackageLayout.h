#pragma once
#include "NSPackage.h"

namespace nano::package {

class Layout : public Entity<Layout>
{
public:
    explicit Layout(CId<Cell> cell);
    Layout() = default;

    void SetBoundary(ShapeId boundary);
    ShapeId GetBoundary() const { return m_.boundary; }

    CId<Cell> GetCell() const { return m_.cell; }

    NetId AddNet(NetId net);
    ConnObjId AddConnObj(ConnObjId connObj);

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