#pragma once
#include "basic/NSContainer.hpp"
#include "common/NSUnit.hpp"

namespace nano::package {

class Package : public NamedObj, public Entity<Package>
{
public:
    Package(std::string name);
    Package() = default;

    void SetCoordUnit(const CoordUnit & unit) { m_.coordUnit = unit; }
    const CoordUnit & GetCoordUnit() const { return m_.coordUnit; }

    void SetMaterialLib(MaterialLibId matLib) { m_.matLib = matLib; }
    MaterialLibId GetMaterialLib() const { return m_.matLib; }

    CellId AddCell(CellId cell);
    PadstackId AddPadstack(PadstackId padstack);
    StackupLayerId AddStackupLayer(StackupLayerId layer);
    ComponentLayerId AddComponentLayer(ComponentLayerId layer);
    void SortStackupLayers(bool ascending = false);

    StackupLayerId FindStackupLayer(const std::string & name) const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (CoordUnit, coordUnit),
    (MaterialLibId, matLib),
    (IdVec<Cell, NameLut>, cells),
    (IdVec<Padstack, NameLut>, padstacks),
    (IdVec<ComponentLayer>, componentLayers),
    (IdVec<StackupLayer, NameLut>, stackupLayers)
    )
};
} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Package)