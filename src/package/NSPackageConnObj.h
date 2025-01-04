#pragma once
#include "NSPackage.h"

namespace nano::package {

class ConnObj : public Entity<ConnObj>
{
public:
    explicit ConnObj(Id<Net> net);
    ConnObj() = default;

    Id<Net> GetNet() const { return m_.net; }

    Id<BondingWire> GetBondingWire();
    CId<BondingWire> GetBondingWire() const;
    
    Id<RoutingWire> GetRoutingWire();
    CId<RoutingWire> GetRoutingWire() const;

    Id<PadstackInst> GetPadstackInst();
    CId<PadstackInst> GetPadstackInst() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Net>, net)
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
    BondingWire(std::string name, Id<Net> net, Id<Layer> start, Id<Layer> end, Float radius);
    BondingWire(std::string name, Id<Net> net, Float radius);
    BondingWire() = default;

    void SetRadius(Float radius);
    Float GetRadius() const;

    void SetHeight(Float height);
    Float GetHeight() const;

    void SetStartLayer(Id<Layer> layer, const NCoord2D & loc, bool flipped);
    void SetStartLayer(Id<Layer> layer);
    Id<Layer> GetStartLayer() const;

    void SetEndLayer(Id<Layer> layer, const NCoord2D & loc, bool flipped);
    void SetEndLayer(Id<Layer> layer);
    Id<Layer> GetEndLayer() const;

    void SetMaterial(Id<Material> material);
    Id<Material> GetMaterial() const;

    void SetBondingWireType(BondingWireType type);
    BondingWireType GetBondingWireType() const;

    void SetSolderJoints(Id<Padstack> solderJoints);
    Id<Padstack> GetSolderJoints() const;

    void Transform(const Transform2D & transform);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Material>, material),
    (Float, radius),
    (Float, height),
    (Arr2<Id<Layer>>, layers),
    (Arr2<NCoord2D>, locations),
    (Arr2<bool>, flipped),
    (Id<Padstack>, solderJoints),
    (BondingWireType, type)
    )
};

class RoutingWire : public ConnObj
{
public:
    RoutingWire(Id<Net> net, Id<StackupLayer> layer, Id<Shape> shape);
    RoutingWire() = default;

    Id<StackupLayer> GetStackupLayer() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Id<StackupLayer>, layer),
    (Id<Shape>, shape)
    )
};

class PadstackInst : public ConnObj, public Transformable2D
{
public:
    PadstackInst(Id<Padstack> padstack, Id<Net> net);
    PadstackInst() = default;

    void SetLayerRange(Id<StackupLayer> top, Id<StackupLayer> bot);
    void GetLayerRange(Id<StackupLayer> & top, Id<StackupLayer> & bot) const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Padstack>, padstack),
    (Arr2<Id<StackupLayer>>, layerRange)
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
