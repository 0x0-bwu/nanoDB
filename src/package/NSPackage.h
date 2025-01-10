#pragma once
#include <core/basic>
#include <core/common>
#include "NSPackageCell.h"

namespace nano::package {

class Package : public FootprintCell
{
public:
    Package(std::string name);
    Package() = default;

    void SetCoordUnit(const CoordUnit & unit) { m_.coordUnit = unit; }
    const CoordUnit & GetCoordUnit() const { return m_.coordUnit; }

    void SetMaterialLib(Id<MaterialLib> matLib) { m_.matLib = matLib; }
    CId<MaterialLib> GetMaterialLib() const { return m_.matLib; }
    Id<MaterialLib> GetMaterialLib() { return m_.matLib; }

    Id<Cell> AddCell(Id<Cell> cell);
    Id<Padstack> AddPadstack(Id<Padstack> padstack);
    Id<StackupLayer> AddStackupLayer(Id<StackupLayer> layer);
    void SortStackupLayers(bool ascending = false);

    CId<StackupLayer> FindStackupLayer(std::string_view name) const;
    Id<StackupLayer> FindStackupLayer(std::string_view name);

    CId<Cell> FindCell(std::string_view name) const;
    Id<Cell> FindCell(std::string_view name);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (CoordUnit, coordUnit),
    (Id<MaterialLib>, matLib),
    (IdVec<Cell, NameLut>, cells),
    (IdVec<Padstack, NameLut>, padstacks),
    (IdVec<StackupLayer, NameLut>, stackupLayers)
    )
};
} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Package)
NS_INHERITANCE(nano::package::Package, nano::package::FootprintCell, nano::package::Cell)