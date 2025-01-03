#pragma once
#include "NSPackage.h"

namespace nano::package {

class ConnObj : public Entity<ConnObj>
{
public:
    explicit ConnObj(Id<Net> net);
    ConnObj() = default;

    NetId GetNet() const { return m_.net; }

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

class BondingWire : public NamedObj, public ConnObj, public Entity<BondingWire>
{
public:
    BondingWire(std::string name, NetId net, LayerId start, LayerId end, Float radius);
    BondingWire(std::string name, NetId net, Float radius);
    BondingWire() = default;

    void SetRadius(Float radius);
    Float GetRadius() const;

    void SetHeight(Float height);
    Float GetHeight() const;

    void SetStartLayer(LayerId layer, const NCoord2D & loc, bool flipped);
    void SetStartLayer(LayerId layer);
    LayerId GetStartLayer() const;

    void SetEndLayer(LayerId layer, const NCoord2D & loc, bool flipped);
    void SetEndLayer(LayerId layer);
    LayerId GetEndLayer() const;

    void SetMaterial(MaterialId material);
    MaterialId GetMaterial() const;

    void SetBondingWireType(BondingWireType type);
    BondingWireType GetBondingWireType() const;

    void SetSolderJoints(PadstackId solderJoints);
    PadstackId GetSolderJoints() const;

    void Transform(const Transform2D & transform);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (MaterialId, material),
    (Float, radius),
    (Float, height),
    (Arr2<LayerId>, layers),
    (Arr2<NCoord2D>, locations),
    (Arr2<bool>, flipped),
    (PadstackId, solderJoints),
    (BondingWireType, type)
    )
};

class RoutingWire : public ConnObj, public Entity<RoutingWire>
{
public:
    RoutingWire(NetId net, StackupLayerId layer, ShapeId shape);
    RoutingWire() = default;

    StackupLayerId GetStackupLayer() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (StackupLayerId, layer),
    (ShapeId, shape)
    )
};

class PadstackInst : public ConnObj, public Transformable2D, public Entity<PadstackInst>
{
public:
    PadstackInst(PadstackId padstack, NetId net);
    PadstackInst() = default;

    void SetLayerRange(StackupLayerId top, StackupLayerId bot);
    void GetLayerRange(StackupLayerId & top, StackupLayerId & bot) const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (PadstackId, padstack),
    (Arr2<StackupLayerId>, layerRange)
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
