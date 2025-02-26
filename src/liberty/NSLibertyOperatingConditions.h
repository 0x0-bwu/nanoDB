#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class OperatingConditionsParser;
class OperatingConditions : public NamedObj, public Entity<OperatingConditions>
{
public:
    friend class OperatingConditionsParser;
    explicit OperatingConditions(std::string name);

private:
    OperatingConditions();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Int, process),
    (Float, temperature),
    (Float, voltage))
};
} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::OperatingConditions)