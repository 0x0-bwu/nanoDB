#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class LeakagePowerParser;
class LeakagePower : public Entity<LeakagePower>
{
public:
    friend class LeakagePowerParser;
    LeakagePower() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Float, value),
    (std::string, when),
    (std::string, relatedPG))
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::LeakagePower)
