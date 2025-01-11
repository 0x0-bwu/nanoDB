#include "NSPackageRetriever.h"
#include <core/package>

namespace nano::package::utils {


LayoutRetriever::LayoutRetriever(CId<Layout> layout)
 : m_layout(layout)
{
}

bool LayoutRetriever::GetLayerHeightThickness(CId<Layer> layer, Float & elevation, Float & thickness) const
{
    NS_ASSERT(layer)
    if (auto stackupLayer = layer->GetStackupLayer(); stackupLayer)
        return GetStackupLayerHeightThickness(stackupLayer, elevation, thickness);
    else if (auto componentLayer = layer->GetComponentLayer(); componentLayer)
        return GetComoponentLayerHeightThickness(componentLayer, elevation, thickness);
    else {
        NS_ASSERT_MSG(false, "invalid layer type");
        return false;
    }
}

bool LayoutRetriever::GetStackupLayerHeightThickness(CId<StackupLayer> stackupLayer, Float & elevation, Float & thickness) const
{
    NS_ASSERT(stackupLayer);
    auto iter = m_lyrHeightsMap.find(CId<Layer>(stackupLayer));
    if (iter == m_lyrHeightsMap.cend())
        iter = m_lyrHeightsMap.emplace(stackupLayer, Arr2<Float>{stackupLayer->GetElevation(), stackupLayer->GetThickness()}).first;
    
    elevation = iter->second[0];
    thickness = iter->second[1];
    return true;
}

bool LayoutRetriever::GetComoponentLayerHeightThickness(CId<ComponentLayer> compLayer, Float & elevation, Float & thickness) const
{
    auto iter = m_lyrHeightsMap.find(CId<Layer>(compLayer));
    if (iter != m_lyrHeightsMap.cend()) {
        elevation = iter->second[0];
        thickness = iter->second[1];
        return true;
    }

    auto connectedLayer = compLayer->GetConnectedLayer();
    if (not connectedLayer) return false;

    Float refElevation{0}, refThickness{0};
    if (not GetLayerHeightThickness(connectedLayer, refElevation, refThickness)) return false;
    if (not GetComponentBallBumpThickness(compLayer, thickness)) return false;

    if (compLayer->isFlipped()) {
        elevation = refElevation - refThickness;
    }
    else elevation = refElevation + thickness;
    m_lyrHeightsMap.emplace(compLayer, Arr2<Float>{elevation, thickness});
    return true;
}

bool LayoutRetriever::GetComponentBallBumpThickness(CId<ComponentLayer> compLayer, Float & thickness) const
{
    NS_ASSERT(compLayer);
    thickness = compLayer->GetSolderBallBumpThickness();
    return true;
}

} // namespace nano::package::utils