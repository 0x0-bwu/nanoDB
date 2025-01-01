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
    StackupLayer(std::string name, LayerType type, 
        Float elevation, Float thickness, MaterialId conductingMat, MaterialId dielectricMat);
    StackupLayer();

    LayerType GetLayerType() const { return m_.type; }

    void SetElevation(Float elevation) { m_.elevation = elevation; }
    Float GetElevation() const { return m_.elevation; }

    void SetThickness(Float thickness) { m_.thickness = thickness; }
    Float GetThickness() const { return m_.thickness; }

    void SetConductingMaterial(MaterialId material);
    MaterialId GetConductingMaterial() const;

    void SetDielectricMaterial(MaterialId material);
    MaterialId GetDielectricMaterial() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (LayerType, type),
    (Float, elevation),
    (Float, thickness),
    (MaterialId, conductingMat),
    (MaterialId, dielectricMat))
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::StackupLayer);