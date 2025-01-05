#pragma once
#include "basic/NSContainer.hpp"
#include "common/NSCommon.hpp"

namespace nano::package {

class Pin : public NamedObj, public Entity<Pin>
{
protected:
    Pin(std::string name, IOType ioType);
    Pin() = default;
public:
    IOType GetIOType() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (IOType, ioType))
};

class FootprintPin : public Pin
{
public:
    FootprintPin(std::string name, CId<Footprint> footprint, NCoord2D location, IOType ioType);
    FootprintPin() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Footprint>, footprint),
    (CId<Padstack>, padstack),
    (NCoord2D, location))
};

class ComponentPin : public Pin
{
public:
    ComponentPin(std::string name, CId<ComponentLayer> componentLayer, CId<FootprintPin> footprintPin);
    ComponentPin() = default;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<ComponentLayer>, componentLayer),
    (CId<FootprintPin>, footprintPin))
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Pin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::FootprintPin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::ComponentPin)
NS_INHERIT_FROM_BASE(nano::package::FootprintPin, nano::package::Pin)
NS_INHERIT_FROM_BASE(nano::package::ComponentPin, nano::package::Pin)
