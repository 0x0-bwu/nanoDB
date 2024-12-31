#pragma once
#include "basic/NSContainer.hpp"
#include "common/NSUnit.hpp"

namespace nano::package {

class Package : public NamedObj, public Entity<Package>
{
public:
    Package(std::string name);
    Package() = default;

    void SetMaterialLib(MaterialLibId matLib) { m_.matLib = matLib; }
    MaterialLibId GetMaterialLib() const { return m_.matLib; }

    void AddCell(CellId cell);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (CoordUnit, coordUnit),
    (MaterialLibId, matLib),
    (IdVec<Cell, NameLut>, cells)
    )
};
} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Package)