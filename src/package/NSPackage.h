#pragma once
#include "basic/NSContainer.hpp"

namespace nano::package {

class Package : public NamedObj, public Entity<Package>
{
public:
    Package(std::string name);
    Package() = default;

    void AddCell(CircuitCellId cell);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (IdVec<Cell, lut::Name>, cells)
    )
};
} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Package)