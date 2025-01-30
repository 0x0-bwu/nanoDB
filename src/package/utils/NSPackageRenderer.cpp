#include "NSPackageRenderer.h"
#include <nano/core/package>

#include "generic/geometry/GeometryIO.hpp"
#include "generic/tools/FileSystem.hpp"
namespace nano::package::utils {

LayoutRenderer::LayoutRenderer(CId<Layout> layout, size_t width)
 : m_layout(layout), m_width(width)
{
}

bool LayoutRenderer::WritePNG(std::string_view directory) const
{
    if (not generic::fs::CreateDir(directory)) return false;

    auto iter = m_layout->GetStackupLayerIter();
    while (auto layer = iter.Next()) {
        auto filename = std::string(directory) + "/" + layer->GetName().data() + ".png";
        if (not WritePNG(filename, layer)) return false;
    }
    return true;
}

bool LayoutRenderer::WritePNG(std::string_view filename, CId<StackupLayer> layer) const
{
    Vec<CId<Net>> net;
    Vec<NPolygon> polygons;
    LayoutRetriever retriever(m_layout);
    retriever.GetLayerPolygons(layer, polygons, net);

    Vec<CId<Component>> components;
    retriever.GetLayerComponents(layer, components);
    for (auto comp : components) {
        auto boundary = comp->GetBoundary();
        NS_ASSERT(boundary);
        auto contour = boundary->GetContour();
        polygons.emplace_back(std::move(contour.outline));
        for (auto & hole : contour.holes)
            polygons.emplace_back(std::move(hole));
    }
    return generic::geometry::GeometryIO::WritePNG(filename, polygons.begin(), polygons.end(), m_width);
}

} // namespace nano::package::utils