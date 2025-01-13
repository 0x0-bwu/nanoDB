#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class OutputCurrentParser;
class OutputCurrent : public Entity<OutputCurrent>
{
public:
    friend class OutputCurrentParser;
    OutputCurrent() = default;

    void AddCcsLut(Id<CcsLut> ccsLut);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (IdVec<CcsLut>, vectors))
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::OutputCurrent)