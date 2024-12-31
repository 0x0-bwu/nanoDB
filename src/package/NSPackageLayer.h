#pragma once
#include "NSPackage.h"

namespace nano::package {

enum class LayerType : int8_t
{
    INVALID = -1,
    DIELECTRIC = 0,
    CONDUCTING = 1,
};

class StackupLayer : public NamedObj, public Entity<StackupLayer>
{
public:
    StackupLayer(std::string name, LayerType type);
    StackupLayer();

    LayerType GetLayerType() const { return m_.type; }

    void SetElevation(Float elevation) { m_.elevation = elevation; }
    Float GetElevation() const { return m_.elevation; }

    void SetThickness(Float thickness) { m_.thickness = thickness; }
    Float GetThickness() const { return m_.thickness; }

    void SetConductingMaterial(std::string conductingMat);
    std::string_view GetConductingMaterial() const;

    void SetDielectricMaterial(std::string dielectricMat);
    std::string_view GetDielectricMaterial() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (LayerType, type),
    (Float, elevation),
    (Float, thickness),
    (std::string, conductingMat),
    (std::string, dielectricMat))
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::StackupLayer);