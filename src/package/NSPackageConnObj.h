#pragma once
#include <nano/common>

namespace nano::package {

class ConnObj : public Cloneable<ConnObj>, public Entity<ConnObj>
{
public:
    void SetNet(CId<Net> net) { m_.net = net; }
    CId<Net> GetNet() const { return m_.net; }

    Id<BondingWire> GetBondingWire();
    CId<BondingWire> GetBondingWire() const;
    
    Id<RoutingWire> GetRoutingWire();
    CId<RoutingWire> GetRoutingWire() const;

    Id<PadstackInst> GetPadstackInst();
    CId<PadstackInst> GetPadstackInst() const;

    virtual void Transform(const Transform2D & transform) = 0;

protected:
    explicit ConnObj(CId<Net> net);
    ConnObj() = default;

    Ptr<ConnObj> CloneFrom(const ConnObj & src);
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
    BondingWire(std::string name, CId<Net> net, Float radius);
    BondingWire();

    void SetRadius(Float radius);
    Float GetRadius() const;

    void SetHeight(Float height);
    Float GetHeight() const;

    void SetStartPin(CId<ComponentPin> connectedPin);
    CId<ComponentPin> GetStartPin() const;

    void SetStartLayer(CId<Layer> layer, const NCoord2D & loc);
    void SetStartLayer(CId<Layer> layer);
    CId<Layer> GetStartLayer() const;

    void SetEndPin(CId<ComponentPin> connectedPin);
    CId<ComponentPin> GetEndPin() const;

    void SetEndLayer(CId<Layer> layer, const NCoord2D & loc);
    void SetEndLayer(CId<Layer> layer);
    CId<Layer> GetEndLayer() const;

    void SetMaterial(CId<Material> material);
    CId<Material> GetMaterial() const;

    void SetBondingWireType(BondingWireType type);
    BondingWireType GetBondingWireType() const;

    void SetSolderJoints(CId<Padstack> solderJoints);
    CId<Padstack> GetSolderJoints() const;

    void Transform(const Transform2D & transform) override;

private:
    NS_CLONE_FUNCTIONS_DECLARATION(BondingWire)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Material>, material),
    (Float, radius),
    (Float, height),
    (CIdArr2<Layer>, layers),
    (CIdArr2<ComponentPin>, connectedPins),
    (Arr2<NCoord2D>, locations),
    (CId<Padstack>, solderJoints),
    (BondingWireType, type)
    )
};

class RoutingWire : public ConnObj
{
public:
    RoutingWire(CId<Net> net, CId<StackupLayer> layer, Id<Shape> shape);
    RoutingWire() = default;

    CId<StackupLayer> GetStackupLayer() const;

    void Transform(const Transform2D & transform) override;

private:
    NS_CLONE_FUNCTIONS_DECLARATION(RoutingWire)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<StackupLayer>, layer),
    (Id<Shape>, shape)
    )
};

class PadstackInst : public ConnObj, public Transformable2D
{
public:
    PadstackInst(CId<Padstack> padstack, CId<Net> net);
    PadstackInst() = default;

    void SetLayerRange(CId<StackupLayer> top, CId<StackupLayer> bot);
    void GetLayerRange(CId<StackupLayer> & top, CId<StackupLayer> & bot) const;

    void Transform(const Transform2D & transform) override;
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
