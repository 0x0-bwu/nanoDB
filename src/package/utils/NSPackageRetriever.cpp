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
        return GetComponentLayerHeightThickness(componentLayer, elevation, thickness);
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

bool LayoutRetriever::GetComponentLayerHeightThickness(CId<ComponentLayer> compLayer, Float & elevation, Float & thickness) const
{
    auto iter = m_lyrHeightsMap.find(CId<Layer>(compLayer));
    if (iter != m_lyrHeightsMap.cend()) {
        elevation = iter->second[0];
        thickness = iter->second[1];
        return true;
    }
    return false;
}

bool LayoutRetriever::GetComponentHeightThickness(CId<Component> component, Float & elevation, Float & thickness) const
{
    return false;
}



} // namespace nano::package::utils