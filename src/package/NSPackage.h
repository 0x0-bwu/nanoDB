#pragma once
#include <nano/common>
#include "NSPackageCell.h"

namespace nano::package {

class Package : public FootprintCell
{
public:
    Package(std::string name);

    void SetCoordUnit(const CoordUnit & unit) { m_.coordUnit = unit; }
    const CoordUnit & GetCoordUnit() const { return m_.coordUnit; }

    void SetMaterialLib(Id<MaterialLib> matLib) { m_.matLib = matLib; }
    CId<MaterialLib> GetMaterialLib() const { return m_.matLib; }
    Id<MaterialLib> GetMaterialLib() { return m_.matLib; }

    Id<Cell> AddCell(Id<Cell> cell);
    Id<Padstack> AddPadstack(Id<Padstack> padstack);
    Id<StackupLayer> AddStackupLayer(Id<StackupLayer> layer);
    
    void SortStackupLayers();
    CId<StackupLayer> GetTopStackupLayer() const;
    CId<StackupLayer> GetBotStackupLayer() const;

    CId<StackupLayer> FindStackupLayer(std::string_view name) const;
    Id<StackupLayer> FindStackupLayer(std::string_view name);

    CId<Cell> FindCell(std::string_view name) const;
    Id<Cell> FindCell(std::string_view name);

    Id<CellInst> SetTop(Id<CellInst> top);
    Id<CellInst> GetTop() { return m_.top; }
    CId<CellInst> GetTop() const { return m_.top; }

    auto GetCellIter();
    auto GetCellIter() const;
    size_t NumOfCells() const { return m_.cells.size(); }

    auto GetPadstackIter();
    auto GetPadstackIter() const;
    size_t NumOfPadstacks() const { return m_.padstacks.size(); }

    auto GetMaterialIter() { return m_.matLib->GetMaterialIter(); }
    auto GetMaterialIter() const { return m_.matLib->GetMaterialIter(); }
    
    auto GetStackupLayerIter() { return m_.stackupLayers.GetIter<StackupLayer>(); }
    auto GetStackupLayerIter() const { return m_.stackupLayers.GetCIter<StackupLayer>(); }
    size_t NumOfStackupLayers() const { return m_.stackupLayers.size(); }

    Float GetHeight() const override;
    bool isBlackBox() const override;

private:
    Package();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (CoordUnit, coordUnit),
    (Id<CellInst>, top),
    (Id<MaterialLib>, matLib),
    (IdVec<Cell, NameLut>, cells),
    (IdVec<Padstack, NameLut>, padstacks),
    (IdVec<StackupLayer, NameLut>, stackupLayers)
    )
};
} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Package)
NS_INHERITANCE(nano::package::Package, nano::package::FootprintCell, nano::package::Cell)