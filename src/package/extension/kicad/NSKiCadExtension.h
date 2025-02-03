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
    Id<pkg::Package> CreatePackage();
    void CreateStackup(Id<pkg::Package> package);
    void CreateNets(Id<pkg::Layout> layout);
    void CreateBoundary(const Component & comp, Id<pkg::Cell> cell);
    void CreateRoutingWires(const Component & comp, Id<pkg::Layout> layout);
    void CreateComponent(const Component & comp, Id<pkg::Layout> layout);
    void CreatePadstackInst(const Via & via, Id<pkg::Layout> layout);
    
    CId<pkg::Material> GetOrCreateMaterial(std::string_view name);
    
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

    UPtr<Database> m_kicad{nullptr};

    // func lut
    HashMap<std::string, std::function<void(const Tree &)>> m_funcs;
    
    // kicad-nano lut
    struct Lut
    {
        CId<pkg::StackupLayer> FindStackupLayer(IdType id) const;
        CId<pkg::Net> FindNet(IdType id) const;
        HashMap<IdType, CId<pkg::Net>> nets;
        HashMap<IdType, CId<pkg::StackupLayer>> layers;
        HashMap<std::string_view, CId<pkg::Material>> material;
    };
    Lut m_lut;
    
    //current state
    struct State
    {
        IdType noNamePadId{INVALID_ID};
        Ptr<Component> comp = nullptr;
        void Reset() { *this = State{}; }
    };
    State m_current;
};

} // namespace nano::package::extension::kicad