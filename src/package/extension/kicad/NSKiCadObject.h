#pragma once
#include <nano/common>

namespace nano::package::extension::kicad {

inline static constexpr IdType NANO_KICAD_PCB_LAYER_BOTTOM_ADHES_ID = 32;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_FRONT_ADHES_ID  = 33;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_BOTTOM_PASTE_ID = 34;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_FRONT_PASTE_ID  = 35;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_BOTTOM_SILKS_ID = 36;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_FRONT_SILKS_ID  = 37;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_BOTTOM_MASK_ID  = 38;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_FRONT_MASK_ID   = 39;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_EDGE_CUT_ID     = 44;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_BOTTOM_CRTYD_ID = 46;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_FRONT_CRTYD_ID  = 47;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_BOTTOM_FAB_ID   = 48;
inline static constexpr IdType NANO_KICAD_PCB_LAYER_FRONT_FAB_ID    = 49;
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_BOTTOM_ADHES_STR = "B.Adhes";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_FRONT_ADHES_STR  = "F.Adhes";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_BOTTOM_PASTE_STR = "B.Paste";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_FRONT_PASTE_STR  = "F.Paste";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_BOTTOM_SILKS_STR = "B.SilkS";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_FRONT_SILKS_STR  = "F.SilkS";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_BOTTOM_MASK_STR  = "B.Mask";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_FRONT_MASK_STR   = "F.Mask";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_EDGE_CUT_STR     = "Edge.Cuts";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_BOTTOM_CRTYD_STR = "B.CrtYd";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_FRONT_CRTYD_STR  = "F.CrtYd";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_BOTTOM_FAB_STR   = "B.Fab";
inline static constexpr std::string_view NANO_KICAD_PCB_LAYER_FRONT_FAB_STR    = "F.Fab";

using Points = Vec<FCoord2D>;
struct Stroke
{
    enum class Type { UNKNOWN, SOLID };
    enum class Fill { UNKNOWN, SOLID };
    Type type{Type::UNKNOWN};
    Fill fill{Fill::UNKNOWN}; 
    IdType layer{INVALID_ID};
    Float width{0};
    virtual ~Stroke() = default;
    virtual void SetType(const std::string & str);
    virtual void SetFill(const std::string & str);
};

struct Arc : public Stroke
{
    Float angle{0};
    FCoord2D start{0, 0}, end{0, 0};
};

struct Line : public Stroke
{ 
    Float angle{0};
    FCoord2D start{0, 0}, end{0, 0};
};

struct Circle : public Stroke
{
    //width = 2 * radius
    FCoord2D center{0, 0}, end{0, 0};
};

struct Poly : public Stroke
{   
    Points shape;
};

struct Text
{
    bool refOrValue{true};//ref=true
    bool hide{false};
    IdType layer{INVALID_ID};
    FCoord2D loc{0, 0};
    std::string text{};
};

struct Layer
{
    enum class Group { UNKNOWN, POWER, SIGNAL, USER };
    enum class Type { UNKNOWN, SILK_SCREEN, SOLDER_PASTE, SOLDER_MASK, CONDUCTING, DIELECTRIC, MIXED };
    IdType id{INVALID_ID};
    Group group{Group::UNKNOWN};
    Type type{Type::UNKNOWN};
    Float thickness{0};
    Float epsilonR{0};
    Float lossTangent{0};
    std::string attr;
    std::string name;
    std::string material;

    Layer(IdType id, std::string name) : id(id), name(std::move(name)) {}

    void SetGroup(const std::string & str);
    void SetType(const std::string & str);
};

struct Via
{
    enum class Type { UNKNOWN, THROUGH, MICRO, BLIND_BURIED };
    Type type{Type::UNKNOWN};    
    IdType net{INVALID_ID};
    Float size{.0};
    Float drillSize{.0};
    FCoord2D pos{0, 0};
    std::array<IdType, 2> layers{INVALID_ID, INVALID_ID};
};
 
struct Segment
{
    IdType net{INVALID_ID};
    IdType layer{INVALID_ID};
    Float width{0};
    FCoord2D start{0, 0};
    FCoord2D end{0, 0};
};

struct Zone
{
    IdType net{INVALID_ID};
    IdType layer{INVALID_ID};
    Points polygon;
    Vec<Points> filledPolygons;
};

struct Pad
{
    enum class Type { UNKNOWN, SMD, THRU_HOLE, CONNECT, NP_THRU_HOLE };
    enum class Shape { UNKNOWN, RECT, ROUNDRECT, CIRCLE, OVAL, TRAPEZOID }; 
    Type type{Type::UNKNOWN};
    Shape shape{Shape::UNKNOWN};
    IdType net;
    Float angle{0};
    Float roundrectRatio{0};
    FCoord2D pos;
    FCoord2D size;
    std::string name{};
    Points shapePolygon;
    Vec<IdType> layers;
    void SetType(const std::string & str);
    void SetShape(const std::string & str);
};

struct Net
{
    IdType id{INVALID_ID};
    IdType netClass{INVALID_ID};
    std::string name;
    Vec<Via> vias;
    Vec<Segment> segments;
    Pair<IdType, IdType> diffPair;
    Net(IdType id, std::string name) : id(id), name(std::move(name)) {}
};

struct Component
{
    bool flipped{false};
    IdType layer{INVALID_ID};
    FCoord2D location{0, 0};
    Float angle{0};
    Float width{0};
    Float height{0};
    std::string name;
    Vec<Via> vias;
    Vec<Arc> arcs;
    Vec<Pad> pads;
    Vec<Line> lines;
    Vec<Poly> polys;
    Vec<Zone> zones;
    Vec<Circle> circles;
    Vec<Segment> segments;

    Component() = default;
    Component(std::string name) : name(std::move(name)) {}
    virtual ~Component() = default;
};

struct Database : public Component
{
    HashMap<std::string, Component> components;
    HashMap<std::string, Layer> layers;
    HashMap<IdType, Net> nets;

    // lut
    HashMap<std::string_view, Ptr<Net>> netLut;

    // add
    Layer & AddLayer(IdType id, std::string name);
    Net & AddNet(IdType id, std::string name);
    Component & AddComponent(const std::string & name);

    // find
    Ptr<Net> FindNet(IdType id);
    Ptr<Net> FindNet(const std::string & name);
    Ptr<Layer> FindLayer(const std::string & name);   
};

} // namespace nano::package::extension::kicad