#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class InternalPowerParser;
class InternalPower : public Entity<InternalPower>
{
public:
    friend class InternalPowerParser;
    InternalPower() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (std::string, relatedPin),
    (std::string, relatedPG),
    (IdArr2<Lut>, power))
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::InternalPower)
