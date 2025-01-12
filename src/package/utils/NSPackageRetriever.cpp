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

    Float refElevation, refThickness;
    thickness = compLayer->GetSolderBallBumpThickness();
    auto location = compLayer->GetLocation();
    if (location != FootprintLocation::TOP and
        location != FootprintLocation::BOT) {
        NS_ASSERT_MSG(false, "invalid footprint location");
        return false;
    }
    if (auto connectedLayer = compLayer->GetConnectedLayer(); connectedLayer) {
        if (not GetLayerHeightThickness(connectedLayer, refElevation, refThickness)) {
            NS_ASSERT_MSG(false, "no connected layer height/thickness found");
            return false;
        }
        if (compLayer->isFlipped()) {
            elevation = FootprintLocation::TOP == location ? refElevation + thickness : refElevation;
        }   
        else {
            elevation = FootprintLocation::TOP == location ? refElevation : refElevation + thickness;
        }
    }
    else {
        if (not GetComponentHeightThickness(compLayer->GetComponent(), refElevation, refThickness)) {
            NS_ASSERT_MSG(false, "no component height/thickness found");
            return false;
        }
        if (compLayer->isFlipped()) {
            elevation = FootprintLocation::TOP == location ? refElevation - refThickness : refElevation;
        }
        else {
            elevation = FootprintLocation::TOP == location ? refElevation : refElevation - refThickness;
        }
    }
    m_lyrHeightsMap.emplace(compLayer, Arr2<Float>{elevation, thickness});
    return true;
}

bool LayoutRetriever::GetComponentHeightThickness(CId<Component> component, Float & elevation, Float & thickness) const
{
    auto assemblyLayer = component->GetAssemblyLayer();
    if (not assemblyLayer) {
        NS_ASSERT_MSG(false, "no assembly layer found");
        return false;
    }
    
    Float refElevation{0}, refThickness{0};
    if (not GetComponentLayerHeightThickness(assemblyLayer, refElevation, refThickness)) {
        NS_ASSERT_MSG(false, "no assembly layer height/thickness found");
        return false;
    }

    thickness = component->GetHeight();
    auto location = assemblyLayer->GetLocation();
    if (location != FootprintLocation::TOP and
        location != FootprintLocation::BOT) {
        NS_ASSERT_MSG(false, "invalid footprint location");
        return false;
    }
    if (component->isFlipped()) {
        if (FootprintLocation::TOP == location) {
            elevation = refElevation + thickness;
        }
        else if (FootprintLocation::BOT == location) {
            elevation = refElevation - refThickness;
        }
    }
    else {
        if (FootprintLocation::TOP == location) {
            elevation = refElevation - refThickness;
        }
        else if (FootprintLocation::BOT == location) {
            elevation = refElevation + thickness;
        }
    }
    return true;
}



} // namespace nano::package::utils