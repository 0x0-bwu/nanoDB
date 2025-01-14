#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class LutTemplateParser;
class LutTemplate : public NamedObj, public Entity<LutTemplate>
{
public:
    friend class LutTemplateParser;
    explicit LutTemplate(std::string name);

private:
    LutTemplate();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (std::vector<std::string>, variables),
    (LutIndices, indices))
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::LutTemplate)