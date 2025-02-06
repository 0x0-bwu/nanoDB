#pragma once
#include "NSKiCadParser.h"
#include "NSKiCadObject.h"
#include <nano/fwd>

namespace nano::package::extension::kicad {

namespace pkg = package;
class KiCadExtension
{
public:
    KiCadExtension();
    Id<Package> Load(std::string_view filename);

private:
    bool Parse(std::string_view filename);

    void ExtractNode(const Tree & node);
    void ExtractLayer(const Tree & node);
    void ExtractSetup(const Tree & node);
    void ExtractStackup(const Tree & node);
    void ExtractNet(const Tree & node);
    void ExtractFootprint(const Tree & node);
    void ExtractSegment(const Tree & node);
    void ExtractZone(const Tree & node);
    void ExtractVia(const Tree & node);

    void ExtractCircle(const Tree & node);
    void ExtractArc(const Tree & node);
    void ExtractPoly(const Tree & node);
    void ExtractLine(const Tree & node);
    void ExtractPad(const Tree & node);

    void ExtractPoints(const Tree & node, Points & points);
    void ExtractStroke(const Tree & node, Stroke & stroke);

    //
    bool CreatePackage();
    void CreateStackup();
    void CreateNets(Id<pkg::Layout> layout);
    void CreateBoundary(const Component & comp, Id<pkg::Cell> cell);
    void CreateRoutingWires(const Component & comp, Id<pkg::Layout> layout);
    void CreateComponent(const Component & comp, Id<pkg::Layout> layout);
    void CreatePadstackInst(const Via & via, Id<pkg::Layout> layout);
    
    CId<pkg::Material> GetOrCreateMaterial(std::string_view name);
    CId<pkg::Padstack> GetOrCreatePadstack(NCoord padSize, NCoord viaSize);
    
    template <typename Lut>
    static std::string NextName(const std::string & name, const Lut & lut)
    {
        if (not lut.count(name)) return name;
        size_t index = 1;
        while (true) {
            std::string next = name + std::to_string(index);
            if (not lut.count(next)) return next;
            index++;
        }
        return "";
    }

    template <typename... Args>
    static void GetValue(const std::string & s, Args & ...args)
    {
        static std::stringstream ss;
        ss.str(s); ss.clear();
        (ss >> ... >> args);
    }

    template <typename... Args>
    static void GetValue(Vec<Tree>::const_iterator iter, Args & ...args)
    {
        ([&]{
            GetValue(iter->value, args);
            std::advance(iter, 1);
        }(), ...);
    }

    template <typename... Args>
    static void TryGetValue(Vec<Tree>::const_iterator iter, Vec<Tree>::const_iterator end, Args & ...args)
    {
        ([&]{
            if (iter != end) {
                GetValue(iter->value, args);
                std::advance(iter, 1);
            }
        }(), ...);
    }

    template <typename... Args>
    static void GetValue(const Vec<Tree> & branches, Args & ...args)
    {
        GetValue(branches.begin(), args...);
    }

    template <typename... Args>
    static void TryGetValue(const Vec<Tree> & branches, Args & ...args)
    {
        TryGetValue(branches.begin(), branches.end(), args...);
    }

    Id<pkg::Package> m_package;
    UPtr<Database> m_kicad{nullptr};

    // func lut
    HashMap<std::string, std::function<void(const Tree &)>> m_funcs;
    
    // kicad-nano lut
    struct Lut
    {
        CId<pkg::StackupLayer> FindStackupLayer(Index id) const;
        CId<pkg::Net> FindNet(Index id) const;
        HashMap<Index, CId<pkg::Net>> nets;
        HashMap<Index, CId<pkg::StackupLayer>> layers;
        HashMap<Arr2<NCoord>, CId<pkg::Padstack>> padstacks;
        HashMap<std::string_view, CId<pkg::Material>> materials;
    };
    Lut m_lut;
    
    //current state
    struct State
    {
        Ptr<Component> comp = nullptr;
        Index noNamePadId{INVALID_INDEX};
        void Reset() { *this = State{}; }
    };
    State m_current;
};

} // namespace nano::package::extension::kicad