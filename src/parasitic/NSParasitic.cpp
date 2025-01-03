#include <core/parasitic>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::parasitic::Parasitic)

#include "parasitic/parser/NSParasiticParser.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/spirit/home/x3.hpp>

namespace nano::parasitic {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Parasitic::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Parasitic);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Parasitic)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Parasitic::Parasitic(std::string filename)
{
    m_.filename = std::move(filename);
}

Id<Net> Parasitic::FindNet(std::string_view name) const
{
    return m_.nets.Lookup<lut::Name>(name);
}

void Parasitic::ClearNets(bool remove)
{
    if (remove) {
        for (auto id : m_.nets)
            nano::Remove<Net>(id);
    }
    m_.nets.clear();
}

using namespace parser;
namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;
using ascii::space;

bool ParseResUnit(std::string_view s, ResUnit & ru)
{
    struct Symbols : x3::symbols<ResUnit::Unit>
    {
        Symbols() 
        { 
            add("KOHM", ResUnit::Unit::KOHM)
            ("OHM", ResUnit::Unit::OHM);
        }
    } const symbols;
    Float scale;
    ResUnit::Unit unit;
    auto f1 = [&](auto & ctx){ scale = x3::_attr(ctx); };
    auto f2 = [&](auto & ctx){ unit = x3::_attr(ctx); };
    auto r = x3::phrase_parse(s.begin(), s.end(), (x3::float_[f1] > x3::no_case[symbols[f2]]), space);
    ru = ResUnit(scale, unit);
    return r;
}

bool ParseCapUnit(std::string_view s, CapUnit & cu)
{
    struct Symbols : x3::symbols<CapUnit::Unit>
    {
        Symbols() 
        { 
            add("FF", CapUnit::Unit::FF)
            ("PF", CapUnit::Unit::PF);
        }
    } const symbols;
    Float scale;
    CapUnit::Unit unit;
    auto f1 = [&](auto & ctx){ scale = x3::_attr(ctx); };
    auto f2 = [&](auto & ctx){ unit = x3::_attr(ctx); };
    auto r = x3::phrase_parse(s.begin(), s.end(), (x3::float_[f1] > x3::no_case[symbols[f2]]), space);
    cu = CapUnit(scale, unit);
    return r;
}

// using namespace grammar;
Id<Parasitic> ReadSpef(std::string_view filename)
{
    using namespace ast;
    SpefDescription ast;
    SpefParser spefParser(ast);
    auto res = spefParser(filename.data());
    if (not res) {
        NS_ERROR(spefParser.error);
        return Id<Parasitic>();
    }

    auto parasitic = nano::Create<Parasitic>(filename.data());
    CapUnit capUnit;
    ResUnit resUnit;
    NS_CHECK_TRUE(ParseCapUnit(ast.header.capacitanceUnit, capUnit));
    NS_CHECK_TRUE(ParseResUnit(ast.header.resistanceUnit, resUnit));
    auto divider = ast.header.divider; boost::trim(divider);
    const auto & nameMap = ast.nameMap;
    std::unordered_map<std::string_view, size_t> pin2id;
    std::unordered_map<std::string_view, Id<Net>> pin2net;
    auto getName = [&](const SpefName & spefName) -> std::string {
        if (auto * index = boost::get<int>(&spefName); index) {
            auto iter = nameMap.find(*index);
            if (iter == nameMap.cend()) {
                NS_ASSERT(false);
                return "";
            }
            return iter->second;
        }
        else if (auto * name = boost::get<std::string>(&spefName); name) {
            return *name;
        }
        NS_ASSERT(false);
        return "";
    };
    auto getNodeName = [&](const SpefNode & spefNode) -> std::string {
        NS_ASSERT(not spefNode.isInternal());
        auto name = getName(spefNode.name);
        if (spefNode.subName.has_value()) {
            name.append(divider);
            name.append(boost::get<std::string>(spefNode.subName.value()));
        }
        return name;
    };
    auto getNodeId = [&](const SpefNode & node) -> size_t {
        if (node.isInternal()) return boost::get<int>(node.subName.value());
        auto nodeName = getNodeName(node);
        auto iter = pin2id.find(nodeName.c_str());
        NS_ASSERT(iter != pin2id.cend());
        return iter->second;
    };
    auto totalNodes = [&](const SpefNet & net) {
        size_t total = 0;
        for (const auto & cap : net.capacitances) {
            if (not cap.n2.has_value()) total++;
        }
        for (const auto & cap : net.capacitances) {
            if (cap.n1.subName.has_value()) {
                if (auto * n = boost::get<int>(&cap.n1.subName.value()); n) 
                    total = std::max<size_t>(*n, total);
            }
        }
        return total + 1;
    };
    for (const auto & snet : ast.nets) {
        auto nodes = totalNodes(snet);
        NS_ASSERT(nodes >= 2);
        auto name = getName(snet.name);
        auto netId = nano::Create<Net>(std::move(name), nodes);
        auto & net = *netId;
        for (const auto & sconn : snet.connections) {
            auto & conn = net->pins.emplace_back();
            conn.isPort = sconn.type == ConnectType::EXTERNAL ? true : false;
            conn.ioType = sconn.ioType;
            conn.name = getNodeName(sconn.node);
            if (sconn.coord.has_value()) {
                auto & c = sconn.coord.value();
                conn.coord = FCoord2D(c.x, c.y);
            }
        }
        std::sort(net->pins.begin(), net->pins.end(), [](const auto & a, const auto & b) {
            return static_cast<int>(a.ioType) > static_cast<int>(b.ioType);
        });
        for (size_t i = 0; i < net->pins.size(); ++i) {
            pin2id.emplace(net->pins.at(i).name.c_str(), i);
            pin2net.emplace(net->pins.at(i).name.c_str(), netId);
        }
        for (const auto & scap : snet.capacitances) {
            if (scap.n2.has_value()) continue; // handle coupling cap later
            auto id = getNodeId(scap.n1);
            net.SetCap(CapId(id), capUnit.toUnit(scap.value, DEFAULT_CAP_UNIT));
        }
        for (const auto & sres : snet.resistances) {
            NS_ASSERT(sres.n2.has_value());
            auto n1 = getNodeId(sres.n1);
            auto n2 = getNodeId(sres.n2.value());
            net.SetRes(CapId(n1), CapId(n2), resUnit.toUnit(sres.value, DEFAULT_RES_UNIT));
        }
        (*parasitic)->nets.emplace_back(netId);
    }
    NS_ASSERT((*parasitic)->nets.size() == ast.nets.size());
    for (size_t i = 0; i < ast.nets.size(); ++i) {
        auto & net = *((*parasitic)->nets.at(i));
        const auto & snet = ast.nets.at(i);
        for (const auto & scap : snet.capacitances) {
            if (not scap.n2.has_value()) continue;
            const auto & node2 = scap.n2.value();
            Id<Net> net2Id;
            if (node2.isInternal()) {
                auto netName = getName(node2.name);
                net2Id = parasitic->FindNet(netName.c_str());
            }
            else {
                auto pinName = getNodeName(node2);
                auto iter = pin2net.find(pinName.c_str());
                if (iter != pin2net.cend()) net2Id = iter->second;
            }
            NS_ASSERT(net2Id);
            auto id1 = getNodeId(scap.n1);
            auto id2 = getNodeId(node2);
            net.AddCouplingCap(CapId(id1), net2Id, CapId(id2), capUnit.toUnit(scap.value, DEFAULT_CAP_UNIT));
        }
    }
    return parasitic;
}

} // namespace nano::parasitic