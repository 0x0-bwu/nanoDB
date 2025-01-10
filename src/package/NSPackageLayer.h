#pragma once
#include <core/basic>

namespace nano::package {

enum class LayerType : int8_t
{
    INVALID = -1,
    DIELECTRIC = 0,
    CONDUCTING = 1,
    COMPONENT = 10,
};

class Layer : public NamedObj, public Cloneable<Layer>, public Entity<Layer>
{
public:
    friend class Layout;
    friend class BondingWire;
    Layer(std::string name, LayerType type);
    Layer() = default;
    LayerType GetLayerType() const { return m_.type; }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLONE_FUNCTIONS_DECLARATION(Layer)
    NS_DEFINE_CLASS_MEMBERS(
    (LayerType, type)
    )
};

class StackupLayer : public Layer
{
public:
    friend class Layout;
    StackupLayer(std::string name, LayerType type, 
        Float elevation, Float thickness, CId<Material> conductingMat, CId<Material> dielectricMat);
    StackupLayer();

    void SetElevation(Float elevation) { m_.elevation = elevation; }
    Float GetElevation() const { return m_.elevation; }

    void SetThickness(Float thickness) { m_.thickness = thickness; }
    Float GetThickness() const { return m_.thickness; }

    void SetConductingMaterial(CId<Material> material);
    CId<Material> GetConductingMaterial() const;

    void SetDielectricMaterial(CId<Material> material);
    CId<Material> GetDielectricMaterial() const;

private:
    NS_CLONE_FUNCTIONS_DECLARATION(StackupLayer)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Float, elevation),
    (Float, thickness),
    (CId<Material>, conductingMat),
    (CId<Material>, dielectricMat)
    )
};

class ComponentLayer : public Layer
{
public:
    friend class Component;
    ComponentLayer(std::string name, CId<Component> component, CId<Footprint> footprint);
    ComponentLayer() = default;

    CId<Footprint> GetFootprint() const;
    CId<Component> GetComponent() const;

    Id<ComponentPin> AddPin(Id<ComponentPin> pin);
    CId<ComponentPin> FindPin(std::string_view name) const;

    void SetConnectedLayer(CId<Layer> layer);
    CId<Layer> GetConnectedLayer() const;

    auto GetComponentPinIter() { return m_.pins.GetIter<ComponentPin>(); }
    auto GetComponentPinIter() const { return m_.pins.GetCIter<ComponentPin>(); }

private:
    void SetComponent(CId<Component> component);
private:
    NS_CLONE_FUNCTIONS_DECLARATION(ComponentLayer)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Component>, component),
    (CId<Footprint>, footprint),
    (IdVec<ComponentPin, NameLut>, pins),
    (CId<Layer>, connectedLayer)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Layer);
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::StackupLayer);
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::ComponentLayer);
NS_INHERIT_FROM_BASE(nano::package::StackupLayer, nano::package::Layer);
NS_INHERIT_FROM_BASE(nano::package::ComponentLayer, nano::package::Layer);