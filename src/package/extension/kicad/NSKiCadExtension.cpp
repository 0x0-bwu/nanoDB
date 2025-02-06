#include "NSKiCadExtension.h"
#include <nano/core/package>

#include "generic/tools/StringHelper.hpp"
#include "generic/geometry/Utility.hpp"
namespace nano::package::extension::kicad {

KiCadExtension::KiCadExtension()
{
    m_funcs.emplace("layers"   , std::bind(&KiCadExtension::ExtractLayer    , this, std::placeholders::_1));
    m_funcs.emplace("setup"    , std::bind(&KiCadExtension::ExtractSetup    , this, std::placeholders::_1));
    m_funcs.emplace("stackup"  , std::bind(&KiCadExtension::ExtractStackup  , this, std::placeholders::_1));
    m_funcs.emplace("net"      , std::bind(&KiCadExtension::ExtractNet      , this, std::placeholders::_1));
    m_funcs.emplace("footprint", std::bind(&KiCadExtension::ExtractFootprint, this, std::placeholders::_1));
    m_funcs.emplace("segment"  , std::bind(&KiCadExtension::ExtractSegment  , this, std::placeholders::_1));
    m_funcs.emplace("zone"     , std::bind(&KiCadExtension::ExtractZone     , this, std::placeholders::_1));
    m_funcs.emplace("via"      , std::bind(&KiCadExtension::ExtractVia      , this, std::placeholders::_1));
    m_funcs.emplace("pad"      , std::bind(&KiCadExtension::ExtractPad      , this, std::placeholders::_1));
    m_funcs.emplace("fp_arc"   , std::bind(&KiCadExtension::ExtractArc      , this, std::placeholders::_1));
    m_funcs.emplace("gr_arc"   , std::bind(&KiCadExtension::ExtractArc      , this, std::placeholders::_1));
    m_funcs.emplace("fp_line"  , std::bind(&KiCadExtension::ExtractLine     , this, std::placeholders::_1));
    m_funcs.emplace("gr_line"  , std::bind(&KiCadExtension::ExtractLine     , this, std::placeholders::_1));
    m_funcs.emplace("fp_poly"  , std::bind(&KiCadExtension::ExtractPoly     , this, std::placeholders::_1));
    m_funcs.emplace("fp_circle", std::bind(&KiCadExtension::ExtractCircle   , this, std::placeholders::_1));
    m_funcs.emplace("gr_circle", std::bind(&KiCadExtension::ExtractCircle   , this, std::placeholders::_1));
}

Id<Package> KiCadExtension::Load(std::string_view filename)
{
    if (not Parse(filename)) {
        NS_ERROR("failed to load file %1%.", filename);
        return Id<Package>();
    }
    if (not CreatePackage()) {
        NS_ERROR("failed to create package.");
        return Id<Package>();
    }
    return m_package;
}

bool KiCadExtension::Parse(std::string_view filename)
{
    Tree tree;
    KiCadParser parser;
    if (not parser(filename, tree)) {
        NS_ERROR("Error: failed to parse %1%.", filename);
        return false; 
    }
    
    m_kicad.reset(new Database);
    m_current.comp = m_kicad.get();
    for (auto & node : tree.branches)
        ExtractNode(node);
    return true;
}

void KiCadExtension::ExtractNode(const Tree & node)
{
    auto iter = m_funcs.find(node.value);
    if (iter != m_funcs.cend())
        iter->second(node);
}

void KiCadExtension::ExtractLayer(const Tree & node)
{
    for (const auto & sub : node.branches) {   
        Index id = std::stoi(sub.value);         
        auto name = sub.branches.at(0).value;
        auto & layer = m_kicad->AddLayer(id, name);
        layer.SetGroup(sub.branches.at(1).value);
        if (sub.branches.size() > 2)
            layer.attr = sub.branches.at(2).value;
    }
}

void KiCadExtension::ExtractSetup(const Tree & node)
{
    for (const auto & sub : node.branches)
        ExtractNode(sub);
}

void KiCadExtension::ExtractStackup(const Tree & node)
{
    UPtr<Layer> dielectricLayer;
    for (const auto & sub : node.branches) {
        if ("layer" == sub.value) {
            auto iter = sub.branches.begin();
            auto layer = m_kicad->FindLayer(iter->value);
            if (not layer) {
                dielectricLayer.reset(new Layer(INVALID_INDEX, iter->value));
                layer = dielectricLayer.get();
            }
            for (iter = std::next(iter); iter != sub.branches.end(); ++iter) {
                if ("type" == iter->value)
                    layer->SetType(iter->branches.front().value);
                else if ("thickness" == iter->value)
                    GetValue(iter->branches, layer->thickness);
                else if ("material" == iter->value)
                    GetValue(iter->branches, layer->material);
                else if ("epsilon_r" == iter->value)
                    GetValue(iter->branches, layer->epsilonR);
                else if ("loss_tangent" == iter->value)
                    GetValue(iter->branches, layer->lossTangent);
            }
            if (Layer::Type::CONDUCTING == layer->type or
                Layer::Type::DIELECTRIC == layer->type or
                Layer::Type::MIXED == layer->type)
                m_kicad->stackupLayers.emplace_back(*layer);
        }
    }
}

void KiCadExtension::ExtractNet(const Tree & node)
{
    Index netId{INVALID_INDEX};
    auto & netName = node.branches.at(1).value;
    GetValue(node.branches, netId);
    m_kicad->AddNet(netId, netName);
}

void KiCadExtension::ExtractFootprint(const Tree & node)
{
    auto iter = node.branches.begin();
    auto name = NextName(iter->value, m_kicad->components);
    auto & comp = m_kicad->AddComponent(name);
    m_current.comp = &comp;
    for (iter = std::next(iter); iter != node.branches.end(); ++iter) {
        const auto & branches = iter->branches;
        if ("layer" == iter->value) {
            auto layer = m_kicad->FindLayer(branches.front().value);
            comp.layer = layer ? layer->id : INVALID_INDEX;
            comp.flipped = (0 != comp.layer);
        }
        else if ("at" == iter->value)
            TryGetValue(branches, comp.location[0], comp.location[1], comp.angle);
        else if ("pad" == iter->value)
            ExtractPad(*iter);
        else
            ExtractNode(*iter);         
    }
    m_current.comp = m_kicad.get();
}

void KiCadExtension::ExtractSegment(const Tree & node)
{
    Segment segment;
    for (const auto & sub : node.branches) {
        if ("start" == sub.value)
            GetValue(sub.branches, segment.start[0], segment.start[1]);
        else if ("end" == sub.value)
            GetValue(sub.branches, segment.end[0], segment.end[1]);
        else if ("width" == sub.value)
            GetValue(sub.branches, segment.width);
        else if ("net" == sub.value)
            GetValue(sub.branches, segment.net);
        else if ("layer" == sub.value) {
            if (auto layer = m_kicad->FindLayer(sub.branches.front().value); layer)
                segment.layer = layer->id;
        }
    }
    m_current.comp->segments.emplace_back(std::move(segment));
}

void KiCadExtension::ExtractZone(const Tree & node)
{
    Zone zone;
    for (const auto & sub : node.branches) {
        if ("net" == sub.value)
            GetValue(sub.branches, zone.net);
        else if ("layer" == sub.value) {
            if (auto layer = m_kicad->FindLayer(sub.branches.front().value); layer)
                zone.layer = layer->id;
        }
        else if ("polygon" == sub.value) {
            for (const auto & polygon : sub.branches) {
                if ("pts" == polygon.value)
                    ExtractPoints(polygon, zone.polygon);
            }
        }
        else if ("filled_polygon" == sub.value) {
           for (const auto & polygon : sub.branches) {
                if ("pts" == polygon.value)
                    ExtractPoints(polygon, zone.filledPolygons.emplace_back(Points{}));
            }
        }
    }
    m_current.comp->zones.emplace_back(std::move(zone));
}

void KiCadExtension::ExtractVia(const Tree & node)
{
    Via via;
    via.type = Via::Type::THROUGH;
    for (const auto & sub : node.branches) {
        if ("at" == sub.value)
            GetValue(sub.branches, via.pos[0], via.pos[1]);
        else if ("size" == sub.value)
            GetValue(sub.branches, via.size);
        else if ("drill" == sub.value)
            GetValue(sub.branches, via.drillSize);
        else if ("net" == sub.value)
            GetValue(sub.branches, via.net);
        else if ("layers" == sub.value) {
            for (size_t i = 0; i < via.layers.size(); ++i) {
                if (auto layer = m_kicad->FindLayer(sub.branches.at(i).value); layer)
                    via.layers[i] = layer->id;
            }
        }
        else if ("micro" == sub.value)
            via.type = Via::Type::MICRO;
        else if ("bline" == sub.value)
            via.type = Via::Type::BLIND_BURIED;
    }
    m_current.comp->vias.emplace_back(std::move(via));
}

void KiCadExtension::ExtractCircle(const Tree & node)
{
    Circle circle;
    for (const auto & sub : node.branches) {
        if ("center" == sub.value)
            GetValue(sub.branches, circle.center[0], circle.center[1]);
        else if ("end" == sub.value)
            GetValue(sub.branches, circle.end[0], circle.end[1]);
        else if ("stroke" == sub.value)
            ExtractStroke(sub, circle);
    }
    m_current.comp->circles.emplace_back(std::move(circle));
}

void KiCadExtension::ExtractPoly(const Tree & node)
{
    Poly poly;
    for (const auto & sub : node.branches) {
        if ("pts" == sub.value)
            ExtractPoints(sub, poly.shape);
        else if ("stroke" == sub.value)
            ExtractStroke(sub, poly);
    }
    m_current.comp->polys.emplace_back(std::move(poly));
}

void KiCadExtension::ExtractArc(const Tree & node)
{
    Arc arc;
    GetValue(node.branches.at(0).branches, arc.start[0], arc.start[1]);
    GetValue(node.branches.at(1).branches, arc.end[0], arc.end[1]);
    GetValue(node.branches.at(2).branches, arc.angle);
    GetValue(node.branches.at(4).branches, arc.width);
    m_current.comp->arcs.emplace_back(std::move(arc));
}

void KiCadExtension::ExtractLine(const Tree & node)
{
    Line line;
    for (const auto & sub : node.branches) {
        if ("start" == sub.value)
            GetValue(sub.branches, line.start[0], line.start[1]);
        else if ("end" == sub.value)
            GetValue(sub.branches, line.end[0], line.end[1]);
        else if ("stroke" == sub.value)
            ExtractStroke(sub, line);
        else if ("layer" == sub.value) {
            auto layer = m_kicad->FindLayer(sub.branches.front().value);
            if (layer) line.layer = layer->id;
        }
    }
    m_current.comp->lines.emplace_back(std::move(line));
}

void KiCadExtension::ExtractPad(const Tree & node)
{
    Pad pad;
    auto iter = node.branches.begin();
    pad.name = iter->value; iter++;
    if (pad.name.empty())
        pad.name = "Unnamed" + std::to_string(m_current.noNamePadId++);
    pad.SetType(iter->value); iter++;
    pad.SetShape(iter->value); iter++;
    for (; iter != node.branches.end(); ++iter) {
        if ("at" == iter->value)
            TryGetValue(iter->branches, pad.pos[0], pad.pos[1], pad.angle);
        else if ("size" == iter->value)
            GetValue(iter->branches, pad.size[0], pad.size[1]);
        else if ("drill" == iter->value)
            GetValue(iter->branches, pad.drill);
        else if ("layers" == iter->value) {
            for (const auto & lyrNode : iter->branches)
                pad.layers.emplace_back(lyrNode.value);
        }
        else if ("roundrect_rratio" == iter->value)
            GetValue(iter->branches, pad.roundrectRatio);
        else if ("net" == iter->value)
            GetValue(iter->branches, pad.net);
        else if ("primitives" == iter->value) {
            for (const auto & primNode : iter->branches) {
                if ("gr_poly" == primNode.value) {
                    ExtractPoints(primNode.branches.front(), pad.shapePolygon);
                }
            }
        }
    }
    m_current.comp->pads.emplace_back(std::move(pad));
}

void KiCadExtension::ExtractPoints(const Tree & node, Vec<FCoord2D> & points)
{
    points.clear();
    points.reserve(node.branches.size());
    for (const auto & sub : node.branches) {
        Float x{0}, y{0};
        GetValue(sub.branches, x, y);
        points.emplace_back(x, y);
    }
    if (points.back() == points.front())
        points.pop_back();
}

void KiCadExtension::ExtractStroke(const Tree & node, Stroke & stroke)
{
    for (const auto & sub : node.branches) {
        if ("width" == sub.value)
            GetValue(sub.branches, stroke.width);
        else if ("type" == sub.value)
            stroke.SetType(sub.branches.front().value);
        else if ("fill" == sub.value)
            stroke.SetFill(sub.branches.front().value);
        else if ("layer" == sub.value) {
            if (auto layer = m_kicad->FindLayer(sub.branches.front().value); layer)
                stroke.layer = layer->id;
        }
    }
}

bool KiCadExtension::CreatePackage()
{
    m_package = nano::Create<pkg::Package>(m_kicad->name);
    CoordUnit coordUnit(CoordUnit::Unit::Millimeter);
    m_package->SetCoordUnit(coordUnit);
    CreateStackup();

    // top cell
    auto cell = nano::Create<pkg::CircuitCell>(m_kicad->name, m_package);
    m_package->AddCell(cell);
    
    auto layout = nano::Create<pkg::Layout>(cell);
    cell->SetLayout(layout);

    CreateNets(layout);
    CreateBoundary(*m_kicad, cell);
    CreateRoutingWires(*m_kicad, layout);

    for (const auto & via : m_kicad->vias)
        CreatePadstackInst(via, layout);

    for (const auto & [name, comp] : m_kicad->components)
        CreateComponent(comp, layout);

    auto top = nano::Create<pkg::CellInst>(m_kicad->name, cell);
    m_package->SetTop(top);
    return true;
}

void KiCadExtension::CreateStackup()
{
    Float elevation{0};
    for (const auto & kLayer : m_kicad->stackupLayers) {
        auto type = kLayer.type == Layer::Type::CONDUCTING ?
            LayerType::CONDUCTING : LayerType::DIELECTRIC;
        auto layer = nano::Create<pkg::StackupLayer>(kLayer.name, type, elevation, kLayer.thickness);
        auto mat = GetOrCreateMaterial(kLayer.material);
        if (type == LayerType::DIELECTRIC)
            layer->SetDielectricMaterial(mat);
        else layer->SetConductingMaterial(mat);
        elevation -= kLayer.thickness;
        m_package->AddStackupLayer(layer);
        if (INVALID_INDEX != kLayer.id)
            m_lut.layers.emplace(kLayer.id, layer);
    }
}

void KiCadExtension::CreateNets(Id<pkg::Layout> layout)
{
    for (const auto & [kNetId, kNet] : m_kicad->nets) {
        auto net = nano::Create<pkg::Net>(kNet.name, layout);
        m_lut.nets.emplace(kNetId, net);
        layout->AddNet(net);
    }
}

void KiCadExtension::CreateBoundary(const Component & comp, Id<pkg::Cell> cell)
{
    Vec<NPolygon> polygons;
    const auto & coordUnit = cell->GetCoordUnit();
    for (const auto & line : comp.lines) {
        if (NANO_KICAD_PCB_LAYER_EDGE_CUT_ID == line.layer or
            NANO_KICAD_PCB_LAYER_FRONT_CRTYD_ID == line.layer or
            NANO_KICAD_PCB_LAYER_BOTTOM_CRTYD_ID == line.layer) {
            auto s = coordUnit.toCoord(line.start);
            auto e = coordUnit.toCoord(line.end);
            auto & polygon = polygons.emplace_back();
            polygon << s << e;
        }
    }
    auto polygon = generic::geometry::ConvexHull(polygons);
    auto shape = nano::Create<ShapePolygon>(std::move(polygon));
    cell->SetBoundary(shape);
}

void KiCadExtension::CreateRoutingWires(const Component & comp, Id<pkg::Layout> layout)
{
    const auto & coordUnit = layout->GetCoordUnit();
    for (const auto & segment : comp.segments) {
        if (auto layer = m_lut.FindStackupLayer(segment.layer); layer) {
            auto shape = nano::Create<pkg::ShapePath>(coordUnit, Vec<FCoord2D>{segment.start, segment.end}, segment.width);
            auto net = m_lut.FindNet(segment.net);
            auto wire = nano::Create<pkg::RoutingWire>(net, layer, shape);
            layout->AddConnObj(wire);
        }
    }

    Vec<NPolygon> holes;
    for (const auto & zone : comp.zones) {
        auto layer = m_lut.FindStackupLayer(zone.layer);
        if (not layer) continue;
        
        Id<Shape> shape;
        auto net = m_lut.FindNet(zone.net);
        // auto shape = nano::Create<pkg::ShapePolygon>(coordUnit, zone.polygon);
        // auto wire = nano::Create<pkg::RoutingWire>(net, layer, shape);
        // layout->AddConnObj(wire);
        for (const auto & fp : zone.filledPolygons) {
            auto polygon = NPolygon(coordUnit.toCoord(fp));
            generic::geometry::Simplify(polygon, holes);
            if (not holes.empty()) {
                NPolygonWithHoles pwh;
                pwh.outline = std::move(polygon);
                pwh.holes = std::move(holes);
                shape = nano::Create<pkg::ShapePolygonWithHoles>(std::move(pwh));
            }
            else
                shape = nano::Create<pkg::ShapePolygon>(std::move(polygon));
            
            auto wire = nano::Create<pkg::RoutingWire>(net, layer, shape);
            layout->AddConnObj(wire);
        }
    }
}

void KiCadExtension::CreateComponent(const Component & comp, Id<pkg::Layout> layout)
{
    auto getPadStackupLayers = [&](const Pad & pad, Vec<CId<StackupLayer>> & layers) {
        for (const auto & layer : pad.layers) {
            auto iter = m_package->GetStackupLayerIter();
            while (auto stackupLayer = iter.Next()) {
                if (generic::str::WildcardMatch(stackupLayer->GetName(), layer.c_str()))
                    layers.emplace_back(stackupLayer);
            }
        }
    };
    const auto & coordUnit = layout->GetCoordUnit();
    auto footprintCell = nano::Create<pkg::FootprintCell>(comp.name, m_package);
    CreateBoundary(comp, footprintCell);
    m_package->AddCell(footprintCell);

    auto footprint = nano::Create<pkg::Footprint>(comp.name, footprintCell, FootprintLocation::BOT);
    footprintCell->AddFootprint(footprint);

    auto component = nano::Create<pkg::Component>(comp.name, footprintCell, layout);
    auto location = layout->GetCoordUnit().toCoord(comp.location);
    auto transform = makeTransform2D(1.0, -generic::math::Rad(comp.angle), location[0], location[1]);
    component->SetTransform(transform);
    auto stackupLayer = m_lut.FindStackupLayer(comp.layer); { NS_ASSERT(stackupLayer); }
    auto componentLayer = nano::Create<pkg::ComponentLayer>(comp.name, component, footprint);
    componentLayer->SetConnectedLayer(stackupLayer);
    component->AddComponentLayer(componentLayer);

    for (const auto & pad : comp.pads) {
        auto footprintPin = nano::Create<pkg::FootprintPin>(pad.name, footprint);
        footprintPin->SetLocation(coordUnit.toCoord(pad.pos));
        footprint->AddPin(footprintPin);
        auto componentPin = nano::Create<pkg::ComponentPin>(pad.name, componentLayer, footprintPin);
        componentLayer->AddPin(componentPin);
        
        auto net = m_lut.FindNet(pad.net);
        auto padstack = nano::Create<pkg::Padstack>(pad.name, m_package);
        auto padstackInst = nano::Create<pkg::PadstackInst>(padstack, net);
        padstackInst->SetTransform(transform);
        layout->AddConnObj(padstackInst);

        auto viaShape = nano::Create<pkg::ShapeCircle>(NCoord2D{0, 0}, coordUnit.toCoord(pad.drill / 2));
        padstack->SetViaShape(viaShape, coordUnit.toCoord(pad.pos), 0);
        Id<pkg::Shape> padShape;
        if (not pad.shapePolygon.empty()) {
            padShape = nano::Create<pkg::ShapePolygon>(coordUnit.toCoord(pad.shapePolygon));
        }
        else if (Pad::Shape::RECT == pad.shape) {
            auto x = coordUnit.toCoord(pad.size[0] / 2);
            auto y = coordUnit.toCoord(pad.size[1] / 2);
            padShape = nano::Create<pkg::ShapeRect>(NCoord2D(-x, -y), NCoord2D(x, y));
        }
        else if (Pad::Shape::ROUNDRECT == pad.shape) {
            auto x = coordUnit.toCoord(pad.size[0] / 2);
            auto y = coordUnit.toCoord(pad.size[1] / 2);
            NCoord cornerR = std::min(x, y) * pad.roundrectRatio;
            Vec<NCoord2D> outline{NCoord2D(-x, -y), NCoord2D(x, -y), NCoord2D(x, y), NCoord2D(-x, y)};
            padShape = nano::Create<pkg::ShapePolygon>(std::move(outline), cornerR);
        }
        else if (Pad::Shape::CIRCLE == pad.shape) {
            padShape = nano::Create<pkg::ShapeCircle>(NCoord2D(0, 0), coordUnit.toCoord(pad.size[0] / 2));
        }
        else if (Pad::Shape::OVAL == pad.shape) {
            auto a = coordUnit.toCoord(pad.size[0] / 2);
            auto b = coordUnit.toCoord(pad.size[1] / 2);
            padShape = nano::Create<pkg::ShapeOval>(NCoord2D(0, 0), a, b);
        }
        else {
            NS_ASSERT_MSG(false, "todo, implement other pad shapes.");
        }
        Vec<CId<StackupLayer>> layers;
        getPadStackupLayers(pad, layers);
        for (auto layer : layers)
            padstack->SetPadShape(layer, padShape, coordUnit.toCoord(pad.pos), 0);
        
        if (Pad::Type::THRU_HOLE == pad.type) {
            padstackInst->SetLayerRange(m_package->GetTopStackupLayer(), m_package->GetBotStackupLayer());
        }
        else padstackInst->SetLayerRange(stackupLayer, stackupLayer);
    }
    layout->AddComponent(component);
}

void KiCadExtension::CreatePadstackInst(const Via & via, Id<pkg::Layout> layout)
{
    const auto & coordUnit = layout->GetCoordUnit();
    auto padstack = GetOrCreatePadstack(coordUnit.toCoord(via.size), coordUnit.toCoord(via.drillSize));

    auto net = m_lut.FindNet(via.net);
    auto topLayer = m_lut.FindStackupLayer(via.layers[0]);
    auto botLayer = m_lut.FindStackupLayer(via.layers[1]);
    auto padstackInst = nano::Create<pkg::PadstackInst>(padstack, net);
    padstackInst->SetLayerRange(topLayer, botLayer);

    auto loc = coordUnit.toCoord(via.pos);
    auto trans = makeTransform2D(1.0, 0, loc[0], loc[1]);
    padstackInst->SetTransform(trans);

    layout->AddConnObj(padstackInst);
}

CId<pkg::Material> KiCadExtension::GetOrCreateMaterial(std::string_view name)
{
    auto iter = m_lut.materials.find(name);
    if (iter != m_lut.materials.cend())
        return iter->second;
    auto mat = nano::Create<pkg::Material>(name.data());
    return m_lut.materials.emplace(name, mat).first->second;
}

CId<pkg::Padstack> KiCadExtension::GetOrCreatePadstack(NCoord padSize, NCoord viaSize)
{
    auto key = Arr2<NCoord>{padSize, viaSize};
    auto iter = m_lut.padstacks.find(key);
    if (iter == m_lut.padstacks.cend()) {
        auto name = "via" + std::to_string(m_package->NumOfPadstacks());
        auto padstack = nano::Create<pkg::Padstack>(std::move(name), m_package);
        auto viaShape = nano::Create<pkg::ShapeCircle>(NCoord2D(0, 0), viaSize / 2);
        padstack->SetViaShape(viaShape);
        auto padShape = nano::Create<pkg::ShapeCircle>(NCoord2D(0, 0), padSize / 2);
        auto layerIter = m_package->GetStackupLayerIter();
        while (auto stackupLayer = layerIter.Next()) {
            if (pkg::LayerType::CONDUCTING == stackupLayer->GetLayerType())
                padstack->SetPadShape(stackupLayer, padShape);
        }
        m_package->AddPadstack(padstack);
        iter = m_lut.padstacks.emplace(key, padstack).first;
    }
    return iter->second;
}

CId<pkg::StackupLayer> KiCadExtension::Lut::FindStackupLayer(Index id) const
{
    auto iter = layers.find(id);
    return iter == layers.cend() ? CId<pkg::StackupLayer>() : iter->second;
}

CId<pkg::Net> KiCadExtension::Lut::FindNet(Index id) const
{
    auto iter = nets.find(id);
    return iter == nets.cend() ? CId<pkg::Net>() : iter->second;
}

} // namespace nano::package::extension::kicad