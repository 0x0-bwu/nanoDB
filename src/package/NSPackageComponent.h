#pragma once
#include "NSPackage.h"
#include "common/NSTransform.hpp"

namespace nano::package {

class Component : public NamedObj, public Transformable2D, public Entity<Component>
{
public:
    explicit Component(std::string name, CId<FootprintCell> footprint, CId<Layout> layout);
    Component() = default;

    CId<FootprintCell> GetFootprint() const;
    CId<Layout> GetLayout() const;

    void SetLayer(CId<Layer> layer);
    CId<Layer> GetLayer() const;

    void SetFlipped(bool flipped);
    bool isFlipped() const;

    CId<Shape> GetBoundary() const;





private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<FootprintCell>, footprint),
    (CId<Layout>, layout),
    (CId<Layer>, layer),
    (Id<Shape>, boundary),
    (bool, flipped)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Component)