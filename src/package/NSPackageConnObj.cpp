#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::ConnObj)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::BondingWire)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::RoutingWire)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::PadstackInst)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void ConnObj::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(ConnObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ConnObj)

template <typename Archive>
void BondingWire::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(BondingWire);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ConnObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(BondingWire)

template <typename Archive>
void RoutingWire::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(RoutingWire);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ConnObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(RoutingWire)

template <typename Archive>
void PadstackInst::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(PadstackInst);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ConnObj);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Transformable2D);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

ConnObj::ConnObj(NetId net)
{
    m_.net = net;
}

BondingWireId ConnObj::GetBondingWire() const
{
    return BondingWireId(GetId());
}

RoutingWireId ConnObj::GetRoutingWire() const
{
    return RoutingWireId(GetId());
}

PadstackInstId ConnObj::GetPadstackInst() const
{
    return PadstackInstId(GetId());
}

BondingWire::BondingWire(std::string name, NetId net, LayerId start, LayerId end, Float radius)
 : NamedObj(std::move(name)), ConnObj(net)
{
    m_.layers[0] = start;
    m_.layers[1] = end;
    m_.radius = radius;
}

BondingWire::BondingWire(std::string name, NetId net, Float radius)
 : BondingWire(std::move(name), net, LayerId(), LayerId(), radius)
{
}

void BondingWire::SetRadius(Float radius)
{
    m_.radius = radius;
}

Float BondingWire::GetRadius() const
{
    return m_.radius;
} 

void BondingWire::SetHeight(Float height)
{
    m_.height = height;
}

Float BondingWire::GetHeight() const
{
    return m_.height;
}

void BondingWire::SetStartLayer(LayerId layer, const NCoord2D & loc, bool flipped)
{
    SetStartLayer(layer);
    m_.locations[0] = loc;
    m_.flipped[0] = flipped;
}

void BondingWire::SetStartLayer(LayerId layer)
{
    if (m_.layers[0])
        m_.layers[0]->RemoveStartBondingWire(Entity<BondingWire>::GetId());
    m_.layers[0] = layer;
}

LayerId BondingWire::GetStartLayer() const
{
    return m_.layers[0];   
}

void BondingWire::SetEndLayer(LayerId layer, const NCoord2D & loc, bool flipped)
{
    SetEndLayer(layer);
    m_.locations[1] = loc;
    m_.flipped[1] = flipped;
}

void BondingWire::SetEndLayer(LayerId layer)
{
    if (m_.layers[1])
        m_.layers[1]->RemoveEndBondingWire(Entity<BondingWire>::GetId());
    m_.layers[1] = layer;
}

LayerId BondingWire::GetEndLayer() const
{
    return m_.layers[1];
}

void BondingWire::SetMaterial(MaterialId material)
{
    m_.material = material;
}

MaterialId BondingWire::GetMaterial() const
{
    return m_.material;
}

void BondingWire::SetBondingWireType(BondingWireType type)
{
    m_.type = type;
}

BondingWireType BondingWire::GetBondingWireType() const
{
    return m_.type;
}

void BondingWire::SetSolderJoints(PadstackId solderJoints)
{
    m_.solderJoints = solderJoints;
}

PadstackId BondingWire::GetSolderJoints() const
{
    return m_.solderJoints;
}

void BondingWire::Transform(const Transform2D & transform)
{
    generic::geometry::Transform(m_.locations[0], transform.GetTransform());
    generic::geometry::Transform(m_.locations[1], transform.GetTransform());
}

RoutingWire::RoutingWire(NetId net, StackupLayerId layer, ShapeId shape)
 : ConnObj(net)
{
    m_.layer = layer;
    m_.shape = shape;
}

StackupLayerId RoutingWire::GetStackupLayer() const
{
    return m_.layer;
}

PadstackInst::PadstackInst(PadstackId padstack, NetId net)
 : ConnObj(net)
{
    m_.padstack = padstack;
}

void PadstackInst::SetLayerRange(StackupLayerId top, StackupLayerId bot)
{
    m_.layerRange[0] = top;
    m_.layerRange[1] = bot;
}
    
void PadstackInst::GetLayerRange(StackupLayerId & top, StackupLayerId & bot) const
{
    top = m_.layerRange[0];
    bot = m_.layerRange[1];
}

} // namespace nano::package