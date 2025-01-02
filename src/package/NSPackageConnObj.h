#pragma once
#include "NSPackage.h"

namespace nano::package {

class ConnObj : public Entity<ConnObj>
{
public:
    explicit ConnObj(NetId net);
    ConnObj() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (NetId, net)
    )
};

class BondingWire : public ConnObj, public Entity<BondingWire>
{
public:
    BondingWire(NetId net);
    BondingWire() = default;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Float, diameter)
    )
};

class RoutingWire : public ConnObj, public Entity<RoutingWire>
{
public:
    RoutingWire(NetId net, ShapeId shape);
    RoutingWire() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
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
    (IdArr2<StackupLayer>, layerRange)
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
