#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::StackupLayer)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void StackupLayer::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(StackupLayer);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(StackupLayer)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

StackupLayer::StackupLayer(std::string name,  LayerType type)
 : NamedObj(std::move(name))
{
    m_.type = type;
    m_.elevation = 0;
    m_.thickness = 0;
}

StackupLayer::StackupLayer()
 : StackupLayer("", LayerType::INVALID)
{
}

void StackupLayer::SetConductingMaterial(std::string conductingMat)
{
    m_.conductingMat = std::move(conductingMat);
}

std::string_view StackupLayer::GetConductingMaterial() const
{
    return m_.conductingMat.c_str();
}

void StackupLayer::SetDielectricMaterial(std::string dielectricMat)
{
    m_.dielectricMat = std::move(dielectricMat);    
}

std::string_view StackupLayer::GetDielectricMaterial() const
{
    return m_.dielectricMat.c_str();
}
    
} // namespace nano::package