#include <nano/core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Layer)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::StackupLayer)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::ComponentLayer)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Layer::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Layer);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Layer)

template <typename Archive>
void StackupLayer::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Layer);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(StackupLayer)

template <typename Archive>
void ComponentLayer::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Layer);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Layer::Layer(std::string name, LayerType type)
 : NamedObj(std::move(name))
{
    m_.type = type;
}

Id<ComponentLayer> Layer::GetComponentLayer()
{
    return Id<ComponentLayer>(GetId());
}

CId<ComponentLayer> Layer::GetComponentLayer() const
{
    return CId<ComponentLayer>(GetCId());
}

Id<StackupLayer> Layer::GetStackupLayer()
{
    return Id<StackupLayer>(GetId());
}

CId<StackupLayer> Layer::GetStackupLayer() const
{
    return CId<StackupLayer>(GetCId());
}


Ptr<Layer> Layer::CloneFrom(const Layer & src)
{
    NamedObj::CloneFrom(src);
    m_ = src.m_;
    return this;
}

StackupLayer::StackupLayer(std::string name,  LayerType type, 
    Float elevation, Float thickness, CId<Material> conductingMat, CId<Material> dielectricMat)
 : Layer(std::move(name), type)
{
    m_.elevation = elevation;
    m_.thickness = thickness;
    m_.conductingMat = conductingMat;
    m_.dielectricMat = dielectricMat;
}

StackupLayer::StackupLayer()
 : StackupLayer("", LayerType::INVALID, Float(0), Float(0), CId<Material>(), CId<Material>())
{
}

void StackupLayer::SetConductingMaterial(CId<Material> conductingMat)
{
    m_.conductingMat = conductingMat;
}

CId<Material> StackupLayer::GetConductingMaterial() const
{
    return m_.conductingMat;
}

void StackupLayer::SetDielectricMaterial(CId<Material> dielectricMat)
{
    m_.dielectricMat = dielectricMat;
}

CId<Material> StackupLayer::GetDielectricMaterial() const
{
    return m_.dielectricMat;
}

Ptr<StackupLayer> StackupLayer::CloneFrom(const StackupLayer & src)
{
    Layer::CloneFrom(src);
    m_ = src.m_;
    return this;
}

ComponentLayer::ComponentLayer(std::string name, CId<Component> component, CId<Footprint> footprint)
 : Layer(std::move(name), LayerType::COMPONENT)
{
    m_.component = component;
    m_.footprint = footprint;
}

ComponentLayer::ComponentLayer()
 : ComponentLayer("", CId<Component>(), CId<Footprint>())
{
}

CId<Footprint> ComponentLayer::GetFootprint() const
{
    return m_.footprint;
}

CId<Component> ComponentLayer::GetComponent() const
{
    return m_.component;
}

FootprintLocation ComponentLayer::GetLocation() const
{
    return m_.footprint->GetLocation();
}

Id<ComponentPin> ComponentLayer::AddPin(Id<ComponentPin> pin)
{
    return m_.pins.Add(pin);
}

CId<ComponentPin> ComponentLayer::FindPin(std::string_view name) const
{
    return m_.pins.Lookup<lut::Name>(name);
}

void ComponentLayer::SetConnectedLayer(CId<Layer> layer)
{
    m_.connectedLayer = layer;
}

CId<Layer> ComponentLayer::GetConnectedLayer() const
{
    return m_.connectedLayer;
}

Float ComponentLayer::GetSolderBallBumpThickness() const
{
    return m_.connectedLayer ? m_.footprint->GetSolderBallBumpThickenss() : 0;
}

CId<Material> ComponentLayer::GetSolderFillingMaterial() const
{
    return m_.footprint->GetSolderFillingMaterial();
}

CId<Material> ComponentLayer::GetSolderMaterial() const
{
    return m_.footprint->GetSolderMaterial();
}

bool ComponentLayer::isFlipped() const
{
    return m_.component->isFlipped();
}

void ComponentLayer::SetComponent(CId<Component> component)
{
    m_.component = component;
}

Ptr<ComponentLayer> ComponentLayer::CloneFrom(const ComponentLayer & src)
{
    Layer::CloneFrom(src);
    m_.connectedLayer = src.m_.connectedLayer;
    m_.component = src.m_.component;
    m_.footprint = src.m_.footprint;
    m_.pins = src.m_.pins.Clone();
    for (auto & pin : m_.pins)
        pin->SetComponentLayer(CId<ComponentLayer>(GetCId()));
    return this;
}
    
} // namespace nano::package
