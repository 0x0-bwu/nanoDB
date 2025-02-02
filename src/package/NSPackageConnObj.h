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
    ConnObj();

    Ptr<ConnObj> CloneFrom(const ConnObj & src);
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (CId<Net>, net)
    )
};

enum class BondingWireType
{
    SIMPLE = 0,
    JEDEC4,
    // JEDEC5
};

class BondingWire : public NamedObj, public ConnObj
{
public:
    BondingWire(std::string name, CId<Net> net, Float radius);

    void SetRadius(Float radius);
    Float GetRadius() const;

    void SetHeight(Float height);
    Float GetHeight() const;

    NCoord2D GetStartLocation() const;
    NCoord2D GetEndLocation() const;

    void SetStartPin(CId<ComponentPin> connectedPin);
    CId<ComponentPin> GetStartPin() const;

    void SetStartLayer(CId<Layer> layer, const NCoord2D & loc, bool flipped = false);
    void SetStartLayer(CId<Layer> layer);
    CId<Layer> GetStartLayer() const;

    void SetEndPin(CId<ComponentPin> connectedPin);
    CId<ComponentPin> GetEndPin() const;

    void SetEndLayer(CId<Layer> layer, const NCoord2D & loc, bool flipped = false);
    void SetEndLayer(CId<Layer> layer);
    CId<Layer> GetEndLayer() const;

    bool isStartFlipped() const { return m_.flipped[0]; }
    bool isEndFlipped() const { return m_.flipped[1]; }

    void SetMaterial(CId<Material> material);
    CId<Material> GetMaterial() const;

    void SetBondingWireType(BondingWireType type);
    BondingWireType GetBondingWireType() const;

    void SetSolderJoints(CId<Padstack> solderJoints);
    CId<Padstack> GetSolderJoints() const;

    void Transform(const Transform2D & transform) override;

private:
    BondingWire();
    NS_CLONE_FUNCTIONS_DECLARATION(BondingWire)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (CId<Material>, material),
    (Float, radius),
    (Float, height),
    (CIdArr2<Layer>, layers),
    (CIdArr2<ComponentPin>, connectedPins),
    (Arr2<NCoord2D>, locations),
    (CId<Padstack>, solderJoints),
    (BondingWireType, type),
    (Arr2<bool>, flipped)
    )
};

class RoutingWire : public ConnObj
{
public:
    RoutingWire(CId<Net> net, CId<StackupLayer> layer, Id<Shape> shape);

    CId<StackupLayer> GetStackupLayer() const;
    CId<Shape> GetShape() const;

    void Transform(const Transform2D & transform) override;

private:
    RoutingWire();
    NS_CLONE_FUNCTIONS_DECLARATION(RoutingWire)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (CId<StackupLayer>, layer),
    (Id<Shape>, shape)
    )
};

class PadstackInst : public ConnObj, public Transformable2D
{
public:
    PadstackInst(CId<Padstack> padstack, CId<Net> net);

    bool isLayerInRange(CId<StackupLayer> layer) const;
    void SetLayerRange(CId<StackupLayer> top, CId<StackupLayer> bot);
    void GetLayerRange(CId<StackupLayer> & top, CId<StackupLayer> & bot) const;

    CId<Padstack> GetPadstack() const { return m_.padstack; }
    UPtr<Shape> GetPadShape(CId<StackupLayer> layer) const;
    UPtr<Shape> GetViaShape() const;

    void Transform(const Transform2D & transform) override;
private:
    PadstackInst();
    NS_CLONE_FUNCTIONS_DECLARATION(PadstackInst)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
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
