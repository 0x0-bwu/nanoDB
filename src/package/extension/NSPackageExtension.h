#pragma once
#include <nano/fwd>

namespace nano::package::extension {

Id<Package> CreateFromKiCad(std::string_view filename);

} // namespace nano::package::extension