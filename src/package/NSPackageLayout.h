#pragma once
#include "basic/NSContainer.hpp"

namespace nano::package {

class Layout : public Entity<Layout>
{
public:
    explicit Layout(CellId cell);
    Layout() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (CellId, cell))
};

} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Layout)