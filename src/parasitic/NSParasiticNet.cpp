#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-compare"
#include "NSParasiticNet.h"
#pragma GCC diagnostic pop
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::parasitic::Net)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::parasitic::CouplingCap)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::parasitic::ConnectedPin)
namespace nano::parasitic {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Net::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Net);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DirectedGraph);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Net)

template <typename Archive>
void CouplingCap::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    NS_SERIALIZATION_HANA_STRUCT(ar, *this);
}
NS_SERIALIZATION_FUNCTIONS_IMP(CouplingCap)

template <typename Archive>
void ConnectedPin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    NS_SERIALIZATION_HANA_STRUCT(ar, *this);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ConnectedPin)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Net::Net(std::string name, size_t nodes)
 : NamedObj(std::move(name)), DirectedGraph(nodes)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.cap.Resize(nodes);
}

Net::Net() : Net("", 0)
{
}

CapId Net::AddNode()
{
    auto capId = DirectedGraph::AddNode();
    if (size_t(capId) >= m_.cap.Size())
        m_.cap.Append(0);
    else m_.cap[capId] = 0;
    return capId;
}

ResId Net::AddEdge(CapId source, CapId target)
{
    auto resId = DirectedGraph::AddEdge(source, target);
    if (size_t(resId) >= m_.res.size())
        m_.res.Append(0);
    else m_.res[resId] = 0;
    return resId;
}

void Net::SetRes(CapId c1, CapId c2, Float res)
{
    auto id = FindEdge(c1, c2);
    if (not id)
        id = AddEdge(c1, c2);
    m_.res[id] = res;
}

Float Net::GetRes(CapId c1, CapId c2) const
{
    auto id = FindEdge(c1, c2);
    NS_ASSERT(id);
    return m_.res[id];
}

Float Net::GetRes(ResId id) const
{
    return m_.res[id];
}

void Net::SetCap(CapId c, Float cap)
{
    m_.cap[c] = cap;
}

Float Net::GetCap(CapId c) const
{
    return m_.cap[c];
}

void Net::AddCouplingCap(CapId c1, Id<Net> net, CapId c2, Float cap)
{
    auto iter = m_.ccap.find(c1);
    if (iter == m_.ccap.end())
        iter = m_.ccap.emplace(c1, Vec<CouplingCap>{}).first;
    iter->second.emplace_back(net, c2, cap);
}


} // namespace nano::parasitic