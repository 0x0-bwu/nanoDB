#pragma once
#include <nano/common>

namespace nano::package::utils {


class LayoutRetriever
{
public:
    explicit LayoutRetriever(CId<Layout> layout);
    virtual ~LayoutRetriever() = default;

    bool GetLayerHeightThickness(CId<Layer> layer, Float & elevation, Float & thickness) const;
    bool GetStackupLayerHeightThickness(CId<StackupLayer> stackupLayer, Float & elevation, Float & thickness) const;
    bool GetComponentLayerHeightThickness(CId<ComponentLayer> compLayer, Float & elevation, Float & thickness) const;
    bool GetComponentHeightThickness(CId<Component> component, Float & elevation, Float & thickness) const;
    bool GetBondingWireHeight(CId<BondingWire> bw, Float & start, Float & end, bool & startFlipped, bool & endFlipped) const;
    bool GetBondingWireSegments(CId<BondingWire> bw, Vec<NCoord2D> & pt2ds, Vec<Float> & heights) const; 
    bool GetBondingWireSegmentsWithMinSeg(CId<BondingWire> bw, Vec<NCoord2D> & pt2ds, Vec<Float> & heights, size_t minSeg) const;
    UPtr<Shape> GetBondingWireStartSolderJointParameters(CId<BondingWire> bw, CId<Material> & mat, Float & elevation, Float & thickness) const;
    UPtr<Shape> GetBondingWireEndSolderJointParameters(CId<BondingWire> bw, CId<Material> & mat, Float & elevation, Float & thickness) const;

    ///brief: Get the polygons of the specified layer, solid shapes are ccw, holes are cw, start shape is the layout boundary
    bool GetLayerPolygons(CId<StackupLayer> layer, Vec<NPolygon> & polygons, Vec<CId<Net>> & nets) const;
    bool GetLayerComponents(CId<StackupLayer> layer, Vec<CId<Component>> & components) const;

private:    
    UPtr<Shape> GetBondingWireStartSolderJointShape(CId<BondingWire> bw, Float & thickness) const;
    UPtr<Shape> GetBondingWireEndSolderJointShape(CId<BondingWire> bw, Float & thickness) const;
    bool GetSimpleBondingWireSegments(CId<BondingWire> bw, Vec<NCoord2D> & pt2ds, Vec<Float> & heights) const;
    bool GetJedec4BondingWireSegments(CId<BondingWire> bw, Vec<NCoord2D> & pt2ds, Vec<Float> & heights) const;
private:  
    CId<Layout> m_layout;
    mutable HashMap<CId<Layer>, Arr2<Float>> m_lyrHeightsMap;//[elevation, thickness], todo: mt-safe
};

} // namespace nano::package::utils