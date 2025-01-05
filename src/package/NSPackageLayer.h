#pragma once
#include "NSPackage.h"

namespace nano::package {

enum class LayerType : int8_t
{
    INVALID = -1,
    DIELECTRIC = 0,
    CONDUCTING = 1,
    COMPONENT = 10,
};

class Layer : public NamedObj, public Entity<Layer>
{
public:
    friend class Layout;
    friend class BondingWire;
    Layer(std::string name, LayerType type);
    Layer() = default;

    LayerType GetLayerType() const { return m_.type; }
private:
    void AddStartBondingWire(Id<BondingWire> bw) { m_.startBondingWires.Add(bw); }
    void AddEndBondingWire(Id<BondingWire> bw) { m_.endBondingWires.Add(bw); }
    void RemoveStartBondingWire(Id<BondingWire> bw) { m_.startBondingWires.Remove(bw); }
    void RemoveEndBondingWire(Id<BondingWire> bw) { m_.endBondingWires.Remove(bw); }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (LayerType, type),
    (IdVec<BondingWire>, startBondingWires),
    (IdVec<BondingWire>, endBondingWires)
    )
};

class StackupLayer : public Layer
{
public:
    friend class Layout;
    StackupLayer(std::string name, LayerType type, 
        Float elevation, Float thickness, Id<Material> conductingMat, Id<Material> dielectricMat);
    StackupLayer();

    void SetElevation(Float elevation) { m_.elevation = elevation; }
    Float GetElevation() const { return m_.elevation; }

    void SetThickness(Float thickness) { m_.thickness = thickness; }
    Float GetThickness() const { return m_.thickness; }

    void SetConductingMaterial(Id<Material> material);
    Id<Material> GetConductingMaterial() const;

    void SetDielectricMaterial(Id<Material> material);
    Id<Material> GetDielectricMaterial() const;

private:
    void AddRoutingWire(Id<RoutingWire> wire) { m_.routingWires.Add(wire); }

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Float, elevation),
    (Float, thickness),
    (Id<Material>, conductingMat),
    (Id<Material>, dielectricMat),
    (IdVec<RoutingWire>, routingWires)
    )
};

class ComponentLayer : public Layer
{
public:
    ComponentLayer(std::string name, CId<Component> component);
    ComponentLayer() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Component>, component)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Layer);
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::StackupLayer);
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::ComponentLayer);
NS_INHERIT_FROM_BASE(nano::package::StackupLayer, nano::package::Layer);
NS_INHERIT_FROM_BASE(nano::package::ComponentLayer, nano::package::Layer);