#pragma once
#include <nano/common>

namespace nano::package::utils {

class FlattenUtility
{
public:
    static bool Merge(Id<Layout> layout, Id<Layout> other, std::string_view prefix);
};


} // namespace nano::package::utils