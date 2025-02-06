#pragma once
#include <nano/common>

namespace nano::package {

class Layout : public Cloneable<Layout>, public Entity<Layout>
{
public:
    explicit Layout(CId<CircuitCell> cell);

    const CoordUnit & GetCoordUnit() const;

    void SetBoundary(Id<Shape> boundary);
    Id<Shape> GetBoundary() const { return m_.boundary; }

    CId<Package> GetPackage() const;
    CId<CircuitCell> GetCell() const { return m_.cell; }

    Id<Net> AddNet(Id<Net> net);
    Id<ConnObj> AddConnObj(Id<ConnObj> connObj);
    Id<Component> AddComponent(Id<Component> component);

    CId<Component> FindComponent(std::string_view name) const;

    auto GetNetIter() { return m_.nets.GetIter<Net>(); }
    auto GetNetIter() const { return m_.nets.GetCIter<Net>(); }
    auto GetConnObjIter() { return m_.connObjs.GetIter<ConnObj>(); }
    auto GetConnObjIter() const { return m_.connObjs.GetCIter<ConnObj>(); }
    auto GetComponentIter() { return m_.components.GetIter<Component>(); }
    auto GetComponentIter() const { return m_.components.GetCIter<Component>(); }
    auto GetBondingWireIter() { return m_.connObjs.GetIter<BondingWire>(); }
    auto GetBondingWireIter() const { return m_.connObjs.GetCIter<BondingWire>(); }
    auto GetRoutingWireIter() { return m_.connObjs.GetIter<RoutingWire>(); }
    auto GetRoutingWireIter() const { return m_.connObjs.GetCIter<RoutingWire>(); }
    auto GetPadstackInstIter() { return m_.connObjs.GetIter<PadstackInst>(); }
    auto GetPadstackInstIter() const { return m_.connObjs.GetCIter<PadstackInst>(); }

    auto GetMaterialIter() const { return GetPackage()->GetMaterialIter(); }
    auto GetStackupLayerIter() const { return GetPackage()->GetStackupLayerIter(); }

    void Transform(const Transform2D & transform);

private:
    Layout();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLONE_FUNCTIONS_DECLARATION(Layout)
    NS_CLASS_MEMBERS_DEFINE(
    (CId<CircuitCell>, cell),
    (Id<Shape>, boundary),
    (IdVec<Net, NameLut>, nets),
    (IdVec<ConnObj>, connObjs),
    (IdVec<Component, NameLut>, components)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Layout)