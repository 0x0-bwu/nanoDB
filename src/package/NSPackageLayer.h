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
    void AddStartBondingWire(BondingWireId bw) { m_.startBondingWires.Add(bw); }
    void AddEndBondingWire(BondingWireId bw) { m_.endBondingWires.Add(bw); }
    void RemoveStartBondingWire(BondingWireId bw) { m_.startBondingWires.Remove(bw); }
    void RemoveEndBondingWire(BondingWireId bw) { m_.endBondingWires.Remove(bw); }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (LayerType, type),
    (IdVec<BondingWire>, startBondingWires),
    (IdVec<BondingWire>, endBondingWires)
    )
};

class StackupLayer : public Layer, public Entity<StackupLayer>
{
public:
    friend class Layout;
    StackupLayer(std::string name, LayerType type, 
        Float elevation, Float thickness, MaterialId conductingMat, MaterialId dielectricMat);
    StackupLayer();

    void SetElevation(Float elevation) { m_.elevation = elevation; }
    Float GetElevation() const { return m_.elevation; }

    void SetThickness(Float thickness) { m_.thickness = thickness; }
    Float GetThickness() const { return m_.thickness; }

    void SetConductingMaterial(MaterialId material);
    MaterialId GetConductingMaterial() const;

    void SetDielectricMaterial(MaterialId material);
    MaterialId GetDielectricMaterial() const;

private:
    void AddRoutingWire(RoutingWireId wire) { m_.routingWires.Add(wire); }

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Float, elevation),
    (Float, thickness),
    (MaterialId, conductingMat),
    (MaterialId, dielectricMat),
    (IdVec<RoutingWire>, routingWires)
    )
};

class ComponentLayer : public Layer, public Entity<ComponentLayer>
{
public:
    ComponentLayer(std::string name);
    ComponentLayer() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (size_t, dummy)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Layer);
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::StackupLayer);
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::ComponentLayer);
NS_INHERIT_FROM_BASE(nano::package::StackupLayer, nano::package::Layer);
NS_INHERIT_FROM_BASE(nano::package::ComponentLayer, nano::package::Layer);