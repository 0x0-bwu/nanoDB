#pragma once

#include "NSParasitic.h"
#include "generic/graph/model/DirectedGraph.hpp"

namespace nano::parasitic {

using ResId = generic::graph::model::EdgeId;
using CapId = generic::graph::model::NodeId;

template<typename Key, typename Value>
using LinearMap = generic::utils::LinearMap<Key, Value>;

using DirectedGraph = generic::graph::model::DirectedGraph;

struct CouplingCap
{
    NetId net;
    CapId cap;
    Float value{0};
    CouplingCap() = default;
    CouplingCap(NetId net, CapId cap, Float value)
     : net(net), cap(cap), value(value) {}
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
};

using CouplingCaps = std::unordered_map<CapId, std::vector<CouplingCap>>;
struct ConnectedPin
{
    bool isPort;
    IOType ioType;
    std::string name;
    Optional<FCoord2D> coord;
    ConnectedPin() = default;
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
};

using ConnectedPins = std::vector<ConnectedPin>;

ParasiticId ReadSpef(std::string_view filename);
class Net : public NamedObj, public DirectedGraph, public Entity<Net>
{
public:
    friend ParasiticId ReadSpef(std::string_view filename);
    Net(std::string name, size_t nodes);
    Net() = default;

    // overrides
    CapId AddNode() override;
    ResId AddEdge(CapId source, CapId target) override;

    void SetRes(CapId c1, CapId c2, Float res);
    Float GetRes(CapId c1, CapId c2) const;
    Float GetRes(ResId id) const;

    void SetCap(CapId c, Float cap);
    Float GetCap(CapId c) const;

    void AddCouplingCap(CapId c1, NetId net, CapId c2, Float cap);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (LinearMap<ResId, Float>, res),
    (LinearMap<CapId, Float>, cap),
    (CouplingCaps, ccap),
    (ConnectedPins, pins))
};

} // namespace nano::parasitic
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::parasitic::Net)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::parasitic::CouplingCap)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::parasitic::ConnectedPin)