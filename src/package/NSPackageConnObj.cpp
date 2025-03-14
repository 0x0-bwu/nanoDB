#include <nano/core/package>
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
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ConnObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(BondingWire)

template <typename Archive>
void RoutingWire::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ConnObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(RoutingWire)

template <typename Archive>
void PadstackInst::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ConnObj);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Transformable2D);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

ConnObj::ConnObj(CId<Net> net)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.net = net;
}

ConnObj::ConnObj() : ConnObj(CId<Net>())
{
}

Id<BondingWire> ConnObj::GetBondingWire()
{
    return Cast<BondingWire>();
}

CId<BondingWire> ConnObj::GetBondingWire() const
{
    return Cast<BondingWire>();
}

Id<RoutingWire> ConnObj::GetRoutingWire()
{
    return Cast<RoutingWire>();
}

CId<RoutingWire> ConnObj::GetRoutingWire() const
{
    return Cast<RoutingWire>();
}

Id<PadstackInst> ConnObj::GetPadstackInst()
{
    return Cast<PadstackInst>();
}

CId<PadstackInst> ConnObj::GetPadstackInst() const
{
    return Cast<PadstackInst>();
}

Ptr<ConnObj> ConnObj::CloneFrom(const ConnObj & src)
{
    m_ = src.m_;
    return this;
}

BondingWire::BondingWire(std::string name, CId<Net> net, Float radius)
 : NamedObj(std::move(name)), ConnObj(net)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.type = BondingWireType::SIMPLE;
    m_.radius = radius;
}

BondingWire::BondingWire()
 : BondingWire("", CId<Net>(), 0)
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

void BondingWire::SetStartPin(CId<ComponentPin> connectedPin)
{
    NS_ASSERT(connectedPin);
    SetStartLayer(connectedPin->GetComponentLayer());
    m_.connectedPins[0] = connectedPin;
}

CId<ComponentPin> BondingWire::GetStartPin() const
{
    return m_.connectedPins[0];
}

void BondingWire::SetStartLayer(CId<Layer> layer, const NCoord2D & loc, bool flipped)
{
    NS_ASSERT(CId<StackupLayer>(layer));
    SetStartLayer(layer);
    m_.locations[0] = loc;
    m_.flipped[0] = flipped;
}

void BondingWire::SetStartLayer(CId<Layer> layer)
{
    m_.layers[0] = layer;
}

CId<Layer> BondingWire::GetStartLayer() const
{
    return m_.layers[0];   
}

NCoord2D BondingWire::GetStartLocation() const
{
    if (m_.connectedPins[0])
        return m_.connectedPins[0]->GetLocation();
    return m_.locations[0];
}

NCoord2D BondingWire::GetEndLocation() const
{
    if (m_.connectedPins[1])
        return m_.connectedPins[1]->GetLocation();
    return m_.locations[1];
}

void BondingWire::SetEndPin(CId<ComponentPin> connectedPin)
{
    NS_ASSERT(connectedPin);
    SetEndLayer(connectedPin->GetComponentLayer());
    m_.connectedPins[1] = connectedPin;
}

CId<ComponentPin> BondingWire::GetEndPin() const
{
    return m_.connectedPins[1];
}

void BondingWire::SetEndLayer(CId<Layer> layer, const NCoord2D & loc, bool flipped)
{
    NS_ASSERT(CId<StackupLayer>(layer));
    SetEndLayer(layer);
    m_.locations[1] = loc;
    m_.flipped[1] = flipped;
}

void BondingWire::SetEndLayer(CId<Layer> layer)
{
    m_.layers[1] = layer;
}

CId<Layer> BondingWire::GetEndLayer() const
{
    return m_.layers[1];
}

void BondingWire::SetMaterial(CId<Material> material)
{
    m_.material = material;
}

CId<Material> BondingWire::GetMaterial() const
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

void BondingWire::SetSolderJoints(CId<Padstack> solderJoints)
{
    m_.solderJoints = solderJoints;
}

CId<Padstack> BondingWire::GetSolderJoints() const
{
    return m_.solderJoints;
}

void BondingWire::Transform(const Transform2D & transform)
{
    generic::geometry::Transform(m_.locations[0], transform.GetTransform());
    generic::geometry::Transform(m_.locations[1], transform.GetTransform());
}

Ptr<BondingWire> BondingWire::CloneFrom(const BondingWire & src)
{
    NamedObj::CloneFrom(src);
    ConnObj::CloneFrom(src);
    m_ = src.m_;
    return this;
}

RoutingWire::RoutingWire(CId<Net> net, CId<StackupLayer> layer, Id<Shape> shape)
 : ConnObj(net)
{
    m_.layer = layer;
    m_.shape = shape;
}

RoutingWire::RoutingWire()
 : RoutingWire(CId<Net>(), CId<StackupLayer>(), Id<Shape>())
{
}

CId<StackupLayer> RoutingWire::GetStackupLayer() const
{
    return m_.layer;
}

CId<Shape> RoutingWire::GetShape() const
{
    return m_.shape;
}

void RoutingWire::Transform(const Transform2D & transform)
{
    m_.shape->Transform(transform);
}

Ptr<RoutingWire> RoutingWire::CloneFrom(const RoutingWire & src)
{
    ConnObj::CloneFrom(src);
    m_.layer = src.m_.layer;
    m_.shape = src.m_.shape->Clone();
    return this;
}

PadstackInst::PadstackInst(CId<Padstack> padstack, CId<Net> net)
 : ConnObj(net)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.padstack = padstack;
}

PadstackInst::PadstackInst()
 : PadstackInst(CId<Padstack>(), CId<Net>())
{
}

bool PadstackInst::isLayerInRange(CId<StackupLayer> layer) const
{
    Vec<CId<StackupLayer>> range;
    GetLayerRange(range);
    return std::find(range.begin(), range.end(), layer) != range.end();
}

void PadstackInst::SetLayerRange(CId<StackupLayer> top, CId<StackupLayer> bot)
{
    m_.layerRange[0] = top;
    m_.layerRange[1] = bot;
}
    
void PadstackInst::GetLayerRange(CId<StackupLayer> & top, CId<StackupLayer> & bot) const
{
    top = m_.layerRange[0];
    bot = m_.layerRange[1];
}

void PadstackInst::GetLayerRange(Vec<CId<StackupLayer>> & range) const
{
    return GetPadstack()->GetPackage()->GetStackupLayerRange(m_.layerRange[0], m_.layerRange[1], range);
}

UPtr<Shape> PadstackInst::GetPadShape(CId<StackupLayer> layer) const
{
    auto shape = m_.padstack->GetPadShape(layer);
    if (shape) shape->Transform(GetTransform());
    return shape;
}

UPtr<Shape> PadstackInst::GetViaShape() const
{
    auto shape = m_.padstack->GetViaShape();
    if (shape) shape->Transform(GetTransform());
    return shape;
}

void PadstackInst::Transform(const Transform2D & transform)
{
    Transformable2D::AddTransform(transform);
}

Ptr<PadstackInst> PadstackInst::CloneFrom(const PadstackInst & src)
{
    ConnObj::CloneFrom(src);
    Transformable2D::CloneFrom(src);
    m_ = src.m_;
    return this;
}

} // namespace nano::package
