#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Layout)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Layout::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Layout);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Layout)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Layout::Layout(CId<Cell> cell)
{
    m_.cell = cell;
}

void Layout::SetBoundary(ShapeId boundary)
{
    m_.boundary = boundary;
}

NetId Layout::AddNet(NetId net)
{
    return m_.nets.Add(net);
}

ConnObjId Layout::AddConnObj(ConnObjId connObj)
{
    m_.connObjs.Add(connObj);
    if (auto net = connObj->GetNet(); net)
        net->AddConnObj(connObj);
    
    if (auto bw = connObj->GetBondingWire(); bw) {
        if (auto sLayer = bw->GetStartLayer(); sLayer)
            sLayer->AddStartBondingWire(bw);
        if (auto eLayer = bw->GetEndLayer(); eLayer)
            eLayer->AddEndBondingWire(bw);
    }
    else if (auto rw = connObj->GetRoutingWire(); rw) {
        if (auto layer = rw->GetStackupLayer(); layer)
            layer->AddRoutingWire(rw);
    }

    return connObj;
}

} // namespace nano::package