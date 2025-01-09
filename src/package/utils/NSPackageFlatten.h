#pragma once
#include "basic/NSForward.hpp"
#include "common/NSTransform.hpp"
namespace nano::package::utils {

class FlattenUtility
{
public:
    static bool Merge(Id<Layout> layout, CId<Layout> other, const Transform2D & trans, std::string_view prefix);
};


} // namespace nano::package::utils