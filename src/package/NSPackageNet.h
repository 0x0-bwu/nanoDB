#pragma once
#include <core/basic>

namespace nano::package {

class Net : public NamedObj, public Cloneable<Net>, public Entity<Net>
{
public:
    friend class Layout;
    Net(std::string name, CId<Layout> layout);
    Net() = default;

    CId<Layout> GetLayout() const { return m_.layout; }

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLONE_FUNCTIONS_DECLARATION(Net)
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Layout>, layout)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Net)