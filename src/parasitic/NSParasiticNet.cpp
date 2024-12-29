#include "NSParasiticNet.h"
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
    ar & boost::serialization::make_nvp("net", net);
    ar & boost::serialization::make_nvp("cap", cap);
    ar & boost::serialization::make_nvp("value", value);
}
NS_SERIALIZATION_FUNCTIONS_IMP(CouplingCap)

template <typename Archive>
void ConnectedPin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & boost::serialization::make_nvp("p", isPort);
    ar & boost::serialization::make_nvp("io", ioType);
    ar & boost::serialization::make_nvp("name", name);
    ar & boost::serialization::make_nvp("coord", coord);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ConnectedPin)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Net::Net(std::string name, size_t nodes)
 : NamedObj(std::move(name)), DirectedGraph(nodes)
{
    m_.cap.Resize(nodes);
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

void Net::SetRes(CapId c1, CapId c2, NSFloat res)
{
    auto id = FindEdge(c1, c2);
    if (not id)
        id = AddEdge(c1, c2);
    m_.res[id] = res;
}

NSFloat Net::GetRes(CapId c1, CapId c2) const
{
    auto id = FindEdge(c1, c2);
    NS_ASSERT(id);
    return m_.res[id];
}

NSFloat Net::GetRes(ResId id) const
{
    return m_.res[id];
}

void Net::SetCap(CapId c, NSFloat cap)
{
    m_.cap[c] = cap;
}

NSFloat Net::GetCap(CapId c) const
{
    return m_.cap[c];
}

void Net::AddCouplingCap(CapId c1, NetId net, CapId c2, NSFloat cap)
{
    auto iter = m_.ccap.find(c1);
    if (iter == m_.ccap.end())
        iter = m_.ccap.emplace(c1, std::vector<CouplingCap>{}).first;
    iter->second.emplace_back(net, c2, cap);
}


} // namespace nano::parasitic