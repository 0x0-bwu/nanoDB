#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Component)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Component::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Component);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Component)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT    

Component::Component(std::string name, CId<FootprintCell> footprint, CId<Layout> layout)    
 : NamedObj(std::move(name))
{
    m_.footprint = footprint;
    m_.layout = layout;
}

CId<FootprintCell> Component::GetFootprint() const
{
    return m_.footprint;
}

CId<Layout> Component::GetLayout() const
{
    return m_.layout;
}

void Component::SetLayer(CId<Layer> layer)
{
    m_.layer = layer;
}

CId<Layer> Component::GetLayer() const
{
    return m_.layer;
}

void Component::SetFlipped(bool flipped)
{
    m_.flipped = flipped;
}

bool Component::isFlipped() const
{
    return m_.flipped;
}

UPtr<Shape> Component::GetBoundary() const
{
    auto shape = m_.footprint->GetBoundary()->UniqueClone();
    shape->Transform(GetTransform());
    return shape;
}

} // namespace nano::package 