#pragma once
#include <nano/common>

namespace nano::package {

class Pin : public NamedObj, public Cloneable<Pin>, public Entity<Pin>
{
protected:
    Pin(std::string name, IOType ioType);
    Pin();
public:
    IOType GetIOType() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLONE_FUNCTIONS_DECLARATION(Pin)
    NS_CLASS_MEMBERS_DEFINE(
    (IOType, ioType))
};

class FootprintPin : public Pin
{
public:
    FootprintPin(std::string name, CId<Footprint> footprint, NCoord2D location, IOType ioType);

    void SetLocation(CRef<NCoord2D> location) { m_.location = location; }
    CRef<NCoord2D> GetLocation() const { return m_.location; }

private:
    FootprintPin();
    NS_CLONE_FUNCTIONS_DECLARATION(FootprintPin)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (CId<Footprint>, footprint),
    (CId<Padstack>, padstack),
    (NCoord2D, location))
};

class ComponentPin : public Pin
{
public:
    friend class ComponentLayer;
    ComponentPin(std::string name, CId<ComponentLayer> componentLayer, CId<FootprintPin> footprintPin);

    NCoord2D GetLocation() const;
    CId<ComponentLayer> GetComponentLayer() const { return m_.componentLayer; }

private:
    void SetComponentLayer(CId<ComponentLayer> layer);

private:
    ComponentPin();
    NS_CLONE_FUNCTIONS_DECLARATION(ComponentPin)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (CId<ComponentLayer>, componentLayer),
    (CId<FootprintPin>, footprintPin))
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Pin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::FootprintPin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::ComponentPin)
NS_INHERIT_FROM_BASE(nano::package::FootprintPin, nano::package::Pin)
NS_INHERIT_FROM_BASE(nano::package::ComponentPin, nano::package::Pin)
