#pragma once
#include "NSPackage.h"

namespace nano::package {

class Net : public NamedObj, public Entity<Net>
{
public:
    Net(std::string name, LayoutId layout);
    Net() = default;

    LayoutId GetLayout() const { return m_.layout; }
    
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (LayoutId, layout),
    (IdVec<ConnObj>, connObjs)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Net)