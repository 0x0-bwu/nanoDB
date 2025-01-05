#pragma once
#include "basic/NSContainer.hpp"

namespace nano::package {

class Net : public NamedObj, public Entity<Net>
{
public:
    friend class Layout;
    Net(std::string name, Id<Layout> layout);
    Net() = default;

    Id<Layout> GetLayout() const { return m_.layout; }
    
private:
    Id<ConnObj> AddConnObj(Id<ConnObj> connObj);
    
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Layout>, layout),
    (IdVec<ConnObj>, connObjs)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Net)