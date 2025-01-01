#pragma once
#include "NSPackage.h"

namespace nano::package {

class Layout : public Entity<Layout>
{
public:
    explicit Layout(CellId cell);
    Layout() = default;

    void SetBoundary(ShapeId boundary);
    ShapeId GetBoundary() const { return m_.boundary; }

    CellId GetCell() const { return m_.cell; }

    void AddNet(NetId net);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (CellId, cell),
    (ShapeId, boundary),
    (IdVec<Net, NameLut>, nets)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Layout)