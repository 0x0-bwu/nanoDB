#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class OperatingConditionsParser;
class OperatingConditions : public NamedObj, public Entity<OperatingConditions>
{
public:
    friend class OperatingConditionsParser;
    explicit OperatingConditions(std::string name);
    OperatingConditions() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Int, process),
    (Float, temperature),
    (Float, voltage))
};
} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::OperatingConditions)