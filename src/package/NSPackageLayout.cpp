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

Layout::Layout(CId<CircuitCell> cell)
{
    m_.cell = cell;
}

void Layout::SetBoundary(Id<Shape> boundary)
{
    m_.boundary = boundary;
}

Id<Net> Layout::AddNet(Id<Net> net)
{
    return m_.nets.Add(net);
}

Id<ConnObj> Layout::AddConnObj(Id<ConnObj> connObj)
{
    return m_.connObjs.Add(connObj);
}

Id<Component> Layout::AddComponent(Id<Component> component)
{
    return m_.components.Add(component);
}

Ptr<Layout> Layout::CloneImpl(const Layout & src)
{
    m_.cell = src.m_.cell;
    m_.boundary = nano::Clone<Shape>(src.m_.boundary);
    
    std::unordered_map<CId<Net>, CId<Net>> netMap{{CId<Net>(), CId<Net>()}};
    for (const auto & net : src.m_.nets) {
        auto cloneNet = m_.nets.Add(nano::Clone<Net>(net, std::string(net->GetName())));
        netMap.emplace(net, cloneNet);
    }

    for (const auto & connObj : src.m_.connObjs) {
        auto cloneConnObj = m_.connObjs.Add(nano::Clone<ConnObj>(connObj));
        cloneConnObj->SetNet(netMap.at(connObj->GetNet()));
    }

    return this;
}

} // namespace nano::package