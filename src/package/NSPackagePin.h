#pragma once
#include "basic/NSContainer.hpp"
#include "common/NSCommon.hpp"

namespace nano::package {

class Pin : public NamedObj, public Entity<Pin>
{
protected:
    Pin(std::string name, NCoord2D location, IOType ioType);
    Pin() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (NCoord2D, location),
    (IOType, ioType))
};

class FootprintPin : public Pin
{
public:
    FootprintPin(std::string name, Id<FootprintCell> cell, NCoord2D location, IOType ioType);
    FootprintPin() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Id<FootprintCell>, cell),
    (Id<StackupLayer>, layer),
    (Id<Padstack>, padstack))
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Pin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::FootprintPin)
NS_INHERIT_FROM_BASE(nano::package::FootprintPin, nano::package::Pin)
