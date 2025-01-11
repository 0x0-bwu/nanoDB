#pragma once
#include <core/basic>
#include <core/common>
namespace nano::package::utils {


class LayoutRetriever
{
public:
    explicit LayoutRetriever(CId<Layout> layout);
    virtual ~LayoutRetriever() = default;

    bool GetLayerHeightThickness(CId<Layer> layer, Float & elevation, Float & thickness) const;
    bool GetStackupLayerHeightThickness(CId<StackupLayer> stackupLayer, Float & elevation, Float & thickness) const;
    bool GetComoponentLayerHeightThickness(CId<ComponentLayer> compLayer, Float & elevation, Float & thickness) const;
    bool GetComponentBallBumpThickness(CId<ComponentLayer> compLayer, Float & thickness) const;
private:    

private:  
    CId<Layout> m_layout;
    mutable HashMap<CId<Layer>, Arr2<Float>> m_lyrHeightsMap;//[elevation, thickness]
};

} // namespace nano::package::utils