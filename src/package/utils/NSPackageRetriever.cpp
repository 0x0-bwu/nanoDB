#include "NSPackageRetriever.h"
#include <nano/core/package>

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

bool LayoutRetriever::GetLayoutHeightThickness(Float & elevation, Float & thickness) const
{
    Float minH = std::numeric_limits<Float>::max(), maxH = -minH;
    auto layerIter = m_layout->GetStackupLayerIter();
    while (auto stackupLayer = layerIter.Next()) {
        if (not GetStackupLayerHeightThickness(stackupLayer, elevation, thickness)) return false;
        maxH = std::max(maxH, elevation);
        minH = std::min(minH, elevation - thickness);
    }
    auto compIter = m_layout->GetComponentIter();
    while (auto comp = compIter.Next()) {
        if (not GetComponentHeightThickness(comp, elevation, thickness)) return false;
        maxH = std::max(maxH, elevation);
        minH = std::min(minH, elevation - thickness);
    }
    thickness = maxH - minH;
    elevation = maxH;
    return true;
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

bool LayoutRetriever::GetBondingWireHeight(CId<BondingWire> bw, Float & start, Float & end, bool & startFlipped, bool & endFlipped) const
{
    Float elevation, thickness;
    startFlipped = bw->isStartFlipped();
    endFlipped = bw->isEndFlipped();
    auto getHeight = [&](bool start, Float & height) {
        auto layer = start ? bw->GetStartLayer() : bw->GetEndLayer();
        if (not GetLayerHeightThickness(layer, elevation, thickness)) return false;

        Float solderJointThickness;
        [[maybe_unused]] auto solderJoint = start ? GetBondingWireStartSolderJointShape(bw, solderJointThickness) :
                                                    GetBondingWireEndSolderJointShape(bw, solderJointThickness);
        auto flipped = start ? startFlipped : endFlipped;
        height = flipped ? elevation - thickness : elevation;
        if (solderJoint) height = flipped ? height - solderJointThickness : height + solderJointThickness;
        return true;
    };
    if (not getHeight(true, start)) return false;
    if (not getHeight(false, end)) return false;
    return true;   
}

bool LayoutRetriever::GetBondingWireSegments(CId<BondingWire> bw, Vec<NCoord2D> & pt2ds, Vec<Float> & heights) const
{
    switch (bw->GetBondingWireType())
    {
        case BondingWireType::SIMPLE :
            return GetSimpleBondingWireSegments(bw, pt2ds, heights);
        case BondingWireType::JEDEC4 :
            return GetJedec4BondingWireSegments(bw, pt2ds, heights);
        default:
            NS_ASSERT_MSG(false, "unsupported bonding wire type");
            return false;
    }
}

bool LayoutRetriever::GetBondingWireSegmentsWithMinSeg(CId<BondingWire> bw, Vec<NCoord2D> & pt2ds, Vec<Float> & heights, size_t minSeg) const
{
    if (pt2ds.empty()) {
        auto res =  GetBondingWireSegments(bw, pt2ds, heights);
        if (not res) return false;
    }

    if (pt2ds.size() >= minSeg) return true;
    auto midPoint = [&](const  NCoord2D & p1, Float h1, const  NCoord2D & p2, Float h2) {
        return std::make_pair((p1 + p2) / 2, (h1 + h2) / 2);
    };

    Vec<Float> newHts{heights.front()};
    Vec<NCoord2D> newPts{pt2ds.front()};
    for (size_t i = 1; i < pt2ds.size(); ++i) {
        auto [mPt, mHt] = midPoint(newPts.back(), newHts.back(), pt2ds.at(i), heights.at(i));
        newPts.emplace_back(std::move(mPt));
        newPts.emplace_back(pt2ds.at(i));
        newHts.emplace_back(mHt);
        newHts.emplace_back(heights.at(i));
    }
    std::swap(newHts, heights);
    std::swap(newPts, pt2ds);
    return GetBondingWireSegmentsWithMinSeg(bw, pt2ds, heights, minSeg);
}

UPtr<Shape> LayoutRetriever::GetBondingWireStartSolderJointParameters(CId<BondingWire> bw, CId<Material> & mat, Float & elevation, Float & thickness) const
{
    auto shape = GetBondingWireStartSolderJointShape(bw, thickness);
    if (nullptr == shape) return nullptr;
    mat = bw->GetSolderJoints()->GetTopSolderBumpMaterial();
    Float start{0}, end{0};
    bool startFlipped{false}, endFlipped(false);
    if (not GetBondingWireHeight(bw, start, end, startFlipped, endFlipped)) return nullptr;
    elevation = startFlipped ? start + thickness : start;
    return shape;

}
UPtr<Shape> LayoutRetriever::GetBondingWireEndSolderJointParameters(CId<BondingWire> bw, CId<Material> & mat, Float & elevation, Float & thickness) const
{
    auto shape = GetBondingWireEndSolderJointShape(bw, thickness);
    if (nullptr == shape) return nullptr;
    mat = bw->GetSolderJoints()->GetBotSolderBallMaterial();
    Float start{0}, end{0};
    bool startFlipped{false}, endFlipped(false);
    if (not GetBondingWireHeight(bw, start, end, startFlipped, endFlipped)) return nullptr;
    elevation = endFlipped ? end + thickness : end;
    return shape;
}

UPtr<Shape> LayoutRetriever::GetBondingWireStartSolderJointShape(CId<BondingWire> bw, Float & thickness) const
{
    if (auto sj = bw->GetSolderJoints(); sj) {
        CId<Shape> shape;
        if (sj->GetTopSolderBumpParameters(shape, thickness)) {
            auto res = shape->UniqueClone();
            auto loc = bw->GetStartLocation();
            res->Transform(makeTransform2D(1.0, 0.0, loc[0], loc[1]));
            return res;
        }
    }
    return nullptr;
}

UPtr<Shape> LayoutRetriever::GetBondingWireEndSolderJointShape(CId<BondingWire> bw, Float & thickness) const
{
    if (auto sj = bw->GetSolderJoints(); sj) {
        CId<Shape> shape;
        if (sj->GetBotSolderBallParameters(shape, thickness)) {
            auto res = shape->UniqueClone();
            auto loc = bw->GetEndLocation();
            res->Transform(makeTransform2D(1.0, 0.0, loc[0], loc[1]));
            return res;
        }
    }
    return nullptr;
}

bool LayoutRetriever::GetSimpleBondingWireSegments(CId<BondingWire> bw, Vec<NCoord2D> & pt2ds, Vec<Float> & heights) const
{
   //simple
    pt2ds.resize(4);
    heights.resize(4);  
    pt2ds[0] = bw->GetStartLocation();
    pt2ds[3] = bw->GetEndLocation();
    pt2ds[1] = pt2ds.at(0) + (pt2ds.at(3) - pt2ds.at(0)) * 0.125;
    pt2ds[2] = pt2ds.at(0) + (pt2ds.at(3) - pt2ds.at(0)) * 0.875;
    bool startFlipped, endFlipped;
    if (not GetBondingWireHeight(bw, heights.front(), heights.back(), startFlipped, endFlipped)) return false;
    heights[1] = startFlipped ? heights[0] - bw->GetHeight() : heights[0] + bw->GetHeight();
    heights[2] = endFlipped ? heights[3] - bw->GetHeight() : heights[3] + bw->GetHeight();
    return true;
}

bool LayoutRetriever::GetJedec4BondingWireSegments(CId<BondingWire> bw, Vec<NCoord2D> & pt2ds, Vec<Float> & heights) const
{
   //JEDEC4
    pt2ds.resize(4);
    heights.resize(4);  
    pt2ds[0] = bw->GetStartLocation();
    pt2ds[1] = pt2ds.front();
    pt2ds[3] = bw->GetEndLocation();
    pt2ds[2] = pt2ds.at(0) + (pt2ds.at(3) - pt2ds.at(0)) * 0.125;
    bool startFlipped, endFlipped;
    if (not GetBondingWireHeight(bw, heights.front(), heights.back(), startFlipped, endFlipped)) return false;
    heights[2] = heights[1] = startFlipped ? heights[0] - bw->GetHeight() : heights[0] + bw->GetHeight();
    return true;
}

bool LayoutRetriever::GetLayerPolygons(CId<StackupLayer> layer, Vec<NPolygon> & polygons, Vec<CId<Net>> & nets) const
{
    nets.clear();
    polygons.clear();
    auto addPolygon = [&](NPolygon shape, const CId<Net> & net, bool isHole = false) {
        if (isHole == shape.isCCW()) shape.Reverse();
        polygons.emplace_back(std::move(shape));
        nets.emplace_back(net);
    };
    auto addPolygonWithHoles = [&](NPolygonWithHoles shape, const CId<Net> & net) {
        addPolygon(std::move(shape.outline), net);
        for (auto & hole : shape.holes) {
            addPolygon(std::move(hole), net, true);
        }
    };
    auto addShape = [&](const Shape & shape, const CId<Net> & net = CId<Net>()) {
        if (shape.hasHole())
            addPolygonWithHoles(shape.GetContour(), net);
        else addPolygon(shape.GetOutline(), net);
    };

    auto boundary = m_layout->GetBoundary();
    NS_ASSERT(boundary);
    addShape(*boundary);

    auto rwIter = m_layout->GetRoutingWireIter();
    while (auto rw = rwIter.Next()) {
        if (rw->GetStackupLayer() == layer) {
            if (auto shape = rw->GetShape(); shape)
                addShape(*shape, rw->GetNet());
        }
    }

    auto psIter = m_layout->GetPadstackInstIter();
    while (auto ps = psIter.Next()) {
        if (ps->isLayerInRange(layer)) {
            if (auto shape = ps->GetViaShape(); shape)
                addShape(*shape, ps->GetNet());
            if (auto shape = ps->GetPadShape(layer); shape)
                addShape(*shape, ps->GetNet());
        }
    }
    return true;
}

bool LayoutRetriever::GetLayerComponents(CId<StackupLayer> layer, Vec<CId<Component>> & components) const
{
    components.clear();
    auto compIter = m_layout->GetComponentIter();
    while (auto comp = compIter.Next()) {
        if (auto compLayer = comp->GetAssemblyLayer(); compLayer) {
            if (layer->Identical(compLayer->GetConnectedLayer()))
                components.emplace_back(comp);
        }
    }
    return true;
}


} // namespace nano::package::utils