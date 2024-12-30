#pragma once
#include "basic/NSContainer.hpp"

namespace nano::common {


class Material : public NamedObj, public Entity<Material>
{
public:
    Material(std::string name);
    Material() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
};

} // namespace nane::common
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::common::Material)