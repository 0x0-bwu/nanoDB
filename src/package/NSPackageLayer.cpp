#include <core/package>
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
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(StackupLayer);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Layer);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(StackupLayer)

template <typename Archive>
void ComponentLayer::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(ComponentLayer);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Layer);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Layer::Layer(std::string name, LayerType type)
 : NamedObj(std::move(name))
{
    m_.type = type;
}

StackupLayer::StackupLayer(std::string name,  LayerType type, 
    Float elevation, Float thickness, MaterialId conductingMat, MaterialId dielectricMat)
 : Layer(std::move(name), type)
{
    m_.elevation = elevation;
    m_.thickness = thickness;
    m_.conductingMat = conductingMat;
    m_.dielectricMat = dielectricMat;
}

StackupLayer::StackupLayer()
 : StackupLayer("", LayerType::INVALID, Float(0), Float(0), MaterialId(), MaterialId())
{
}

void StackupLayer::SetConductingMaterial(MaterialId conductingMat)
{
    m_.conductingMat = conductingMat;
}

MaterialId StackupLayer::GetConductingMaterial() const
{
    return m_.conductingMat;
}

void StackupLayer::SetDielectricMaterial(MaterialId dielectricMat)
{
    m_.dielectricMat = dielectricMat;
}

MaterialId StackupLayer::GetDielectricMaterial() const
{
    return m_.dielectricMat;
}

ComponentLayer::ComponentLayer(std::string name)
 : Layer(std::move(name), LayerType::COMPONENT)
{
}
    
} // namespace nano::package
