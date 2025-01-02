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

BondingWire::BondingWire(NetId net)
 : ConnObj(net)
{
}

RoutingWire::RoutingWire(NetId net, ShapeId shape)
 : ConnObj(net)
{
    m_.shape = shape;
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
