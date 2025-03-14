#include <nano/core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Component)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Component::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Component);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Transformable2D);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Component)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT    

Component::Component(std::string name, CId<FootprintCell> footprint, CId<Layout> layout)    
 : NamedObj(std::move(name))
{
    NS_CLASS_MEMBERS_INITIALIZE;
    m_.footprint = footprint;
    m_.layout = layout;
}

Component::Component()
 : Component("", CId<FootprintCell>(), CId<Layout>())
{
}

CId<Material> Component::GetMaterial() const
{
    return m_.footprint->GetMaterial();
}

CId<FootprintCell> Component::GetFootprint() const
{
    return m_.footprint;
}

CId<Layout> Component::GetLayout() const
{
    return m_.layout;
}

Id<ComponentLayer> Component::AddComponentLayer(Id<ComponentLayer> componentLayer)
{
    return m_.componentLayers.Add(componentLayer);
}

CId<ComponentLayer> Component::FindComponentLayer(std::string_view name) const
{
    return m_.componentLayers.Lookup<lut::Name>(name);
}

CId<ComponentPin> Component::FindComponentPin(std::string_view layerName, std::string_view pinName) const
{
    if (auto layer = FindComponentLayer(layerName); layer)
        return layer->FindPin(pinName);
    return CId<ComponentPin>();
}

CId<ComponentLayer> Component::GetAssemblyLayer() const
{
    for (auto & layer : m_.componentLayers)
        if (auto connectedLayer = layer->GetConnectedLayer(); connectedLayer)
            return layer;
    return CId<ComponentLayer>();
}

void Component::Transform(const Transform2D & transform)
{
    Transformable2D::AddTransform(transform);
}

UPtr<Shape> Component::GetBoundary() const
{
    if (auto boundary = m_.footprint->GetBoundary(); boundary) {
        auto shape = boundary->UniqueClone();
        shape->Transform(GetTransform());
        return shape;
    }
    return nullptr;
}

Float Component::GetHeight() const
{
    return m_.footprint->GetHeight();
}

Ptr<Component> Component::CloneFrom(const Component & src)
{
    NamedObj::CloneFrom(src);
    Transformable2D::CloneFrom(src);
    m_.footprint = src.m_.footprint;
    m_.layout = src.m_.layout;
    m_.componentLayers = src.m_.componentLayers.Clone();
    for (auto & layer : m_.componentLayers) {
        layer->SetComponent(CId<Component>(GetId()));
    }
    m_.flipped = src.m_.flipped;
    return this;
}

void Component::SetLayout(CId<Layout> layout)
{
    m_.layout = layout;
}

} // namespace nano::package 