#pragma once
#include "basic/NSContainer.hpp"

namespace nano::package {

class ConnObj : public Entity<ConnObj>
{
public:
    explicit ConnObj(CId<Net> net);
    ConnObj() = default;

    CId<Net> GetNet() const { return m_.net; }

    Id<BondingWire> GetBondingWire();
    CId<BondingWire> GetBondingWire() const;
    
    Id<RoutingWire> GetRoutingWire();
    CId<RoutingWire> GetRoutingWire() const;

    Id<PadstackInst> GetPadstackInst();
    CId<PadstackInst> GetPadstackInst() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Net>, net)
    )
};

enum class BondingWireType
{
    INVALID = -1,
    SIMPLE = 0,
    JEDEC4,
    // JEDEC5
};

class BondingWire : public NamedObj, public ConnObj
{
public:
    BondingWire(std::string name, CId<Net> net, CId<Layer> start, CId<Layer> end, Float radius);
    BondingWire(std::string name, CId<Net> net, Float radius);
    BondingWire() = default;

    void SetRadius(Float radius);
    Float GetRadius() const;

    void SetHeight(Float height);
    Float GetHeight() const;

    void SetStartPin(CId<ComponentPin> connectedPin, bool flipped);
    void SetStartLayer(CId<Layer> layer, const NCoord2D & loc, bool flipped);
    void SetStartLayer(CId<Layer> layer);
    CId<Layer> GetStartLayer() const;

    void SetEndPin(CId<ComponentPin> connectedPin, bool flipped);
    void SetEndLayer(CId<Layer> layer, const NCoord2D & loc, bool flipped);
    void SetEndLayer(CId<Layer> layer);
    CId<Layer> GetEndLayer() const;

    void SetMaterial(CId<Material> material);
    CId<Material> GetMaterial() const;

    void SetBondingWireType(BondingWireType type);
    BondingWireType GetBondingWireType() const;

    void SetSolderJoints(CId<Padstack> solderJoints);
    CId<Padstack> GetSolderJoints() const;

    void Transform(const Transform2D & transform);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Material>, material),
    (Float, radius),
    (Float, height),
    (CIdArr2<Layer>, layers),
    (CIdArr2<ComponentPin>, connectedPins),
    (Arr2<NCoord2D>, locations),
    (Arr2<bool>, flipped),
    (CId<Padstack>, solderJoints),
    (BondingWireType, type)
    )
};

class RoutingWire : public ConnObj
{
public:
    RoutingWire(CId<Net> net, CId<StackupLayer> layer, CId<Shape> shape);
    RoutingWire() = default;

    CId<StackupLayer> GetStackupLayer() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<StackupLayer>, layer),
    (CId<Shape>, shape)
    )
};

class PadstackInst : public ConnObj, public Transformable2D
{
public:
    PadstackInst(CId<Padstack> padstack, CId<Net> net);
    PadstackInst() = default;

    void SetLayerRange(CId<StackupLayer> top, CId<StackupLayer> bot);
    void GetLayerRange(CId<StackupLayer> & top, CId<StackupLayer> & bot) const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Padstack>, padstack),
    (CIdArr2<StackupLayer>, layerRange)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::ConnObj)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::BondingWire)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::RoutingWire)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::PadstackInst)
NS_INHERIT_FROM_BASE(nano::package::BondingWire, nano::package::ConnObj)
NS_INHERIT_FROM_BASE(nano::package::RoutingWire, nano::package::ConnObj)
NS_INHERIT_FROM_BASE(nano::package::PadstackInst, nano::package::ConnObj)
