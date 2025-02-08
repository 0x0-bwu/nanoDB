#pragma once
#include <nano/common>

namespace nano::package {

enum class LayerType : int8_t
{
    INVALID = 0,
    DIELECTRIC = 1,
    CONDUCTING = 2,
    COMPONENT = 10,
};

class Layer : public NamedObj, public Cloneable<Layer>, public Entity<Layer>
{
protected:
    Layer(std::string name, LayerType type);
    Layer();
public:
    friend class Layout;
    friend class BondingWire;
    Id<ComponentLayer> GetComponentLayer();
    CId<ComponentLayer> GetComponentLayer() const;

    Id<StackupLayer> GetStackupLayer();
    CId<StackupLayer> GetStackupLayer() const;

    LayerType GetLayerType() const { return m_.type; }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLONE_FUNCTIONS_DECLARATION(Layer)
    NS_CLASS_MEMBERS_DEFINE(
    (LayerType, type)
    )
};

class StackupLayer : public Layer
{
public:
    friend class Layout;
    StackupLayer(std::string name, LayerType type, Float elevation, Float thickness);
    StackupLayer(std::string name, LayerType type, 
                 Float elevation, Float thickness, 
                 CId<Material> conductingMat, CId<Material> dielectricMat);

    void SetElevation(Float elevation) { m_.elevation = elevation; }
    Float GetElevation() const { return m_.elevation; }

    void SetThickness(Float thickness) { m_.thickness = thickness; }
    Float GetThickness() const { return m_.thickness; }

    void SetConductingMaterial(CId<Material> material);
    CId<Material> GetConductingMaterial() const;

    void SetDielectricMaterial(CId<Material> material);
    CId<Material> GetDielectricMaterial() const;

private:
    StackupLayer();
    NS_CLONE_FUNCTIONS_DECLARATION(StackupLayer)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
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
    ComponentLayer();

    CId<Footprint> GetFootprint() const;
    CId<Component> GetComponent() const;

    FootprintLocation GetLocation() const;

    Id<ComponentPin> AddPin(Id<ComponentPin> pin);
    CId<ComponentPin> FindPin(std::string_view name) const;

    void SetConnectedLayer(CId<Layer> layer);
    CId<Layer> GetConnectedLayer() const;

    auto GetComponentPinIter() { return m_.pins.GetIter<ComponentPin>(); }
    auto GetComponentPinIter() const { return m_.pins.GetCIter<ComponentPin>(); }

    Float GetSolderBallBumpThickness() const;

    CId<Material> GetSolderFillingMaterial() const;
    CId<Material> GetSolderMaterial() const;

    UPtr<Shape> GetBoundary() const;
    bool isFlipped() const;

private:
    void SetComponent(CId<Component> component);
private:
    NS_CLONE_FUNCTIONS_DECLARATION(ComponentLayer)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
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