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

Ptr<Layout> Layout::CloneImpl() const
{
    auto clone = new Layout(m_.cell);
    clone->m_.boundary = nano::Clone(m_.boundary);
    //todo
    return clone;
}


} // namespace nano::package