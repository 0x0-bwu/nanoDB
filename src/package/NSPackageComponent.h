#pragma once
#include "common/NSTransform.hpp"
#include "basic/NSContainer.hpp"

namespace nano::package {

class Component : public NamedObj, public Transformable2D, public Cloneable<Component>, public Entity<Component>
{
public:
    friend class Layout;
    explicit Component(std::string name, CId<FootprintCell> footprint, CId<Layout> layout);
    Component();

    CId<FootprintCell> GetFootprint() const;
    CId<Layout> GetLayout() const;

    Id<ComponentLayer> AddComponentLayer(Id<ComponentLayer> componentLayer);
    CId<ComponentLayer> FindComponentLayer(std::string_view name) const;
    CId<ComponentPin> FindComponentPin(std::string_view layerName, std::string_view pinName) const;

    auto GetComponentLayerIter() { return m_.componentLayers.GetIter<ComponentLayer>(); }
    auto GetComponentLayerIter() const { return m_.componentLayers.GetCIter<ComponentLayer>(); }

    void Transform(const Transform2D & transform);

    UPtr<Shape> GetBoundary() const;

private:
    void SetLayout(CId<Layout> layout);

private:
    NS_CLONE_FUNCTIONS_DECLARATION(Component)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<FootprintCell>, footprint),
    (CId<Layout>, layout),
    (IdVec<ComponentLayer, NameLut>, componentLayers)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Component)