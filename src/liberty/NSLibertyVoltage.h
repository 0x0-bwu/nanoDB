#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class VoltageParser;
class Voltage : public NamedObj, public Entity<Voltage>
{
public:
    friend class VoltageParser;
    explicit Voltage(std::string name);
    Voltage() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Float, low),
    (Float, high),
    (Float, min),
    (Float, max))
};
} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::Voltage)