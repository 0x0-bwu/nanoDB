#pragma once
#include <nano/common>
namespace nano::package {

class Net : public NamedObj, public Cloneable<Net>, public Entity<Net>
{
public:
    friend class Layout;
    Net(std::string name, CId<Layout> layout);

    CId<Layout> GetLayout() const { return m_.layout; }

private:
    Net();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLONE_FUNCTIONS_DECLARATION(Net)
    NS_CLASS_MEMBERS_DEFINE(
    (CId<Layout>, layout)
    )
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Net)