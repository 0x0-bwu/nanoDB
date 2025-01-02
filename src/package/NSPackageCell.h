#pragma once
#include "NSPackage.h"

namespace nano::package {

class Cell : public NamedObj, public Entity<Cell>
{
protected:
    Cell(std::string name, PackageId package);
    Cell() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (PackageId, package)
    )
};

class CircuitCell : public Cell, public Entity<CircuitCell>
{
public:
    CircuitCell(std::string name, PackageId package);
    CircuitCell() = default;

    LayoutId SetLayout(LayoutId layout);
    LayoutId GetLayout() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (LayoutId, layout))
};

enum class ComponentType
{
    INVALID = -1,
    OTHER = 0,
    RESISTOR,
    CAPACITOR,
    INDUCTOR,
    IC,
    IO,
    MOLDING
};

class FootprintCell : public Cell, public Entity<FootprintCell>
{
public:
    FootprintCell(std::string name, PackageId package);
    FootprintCell();

    void SetComponentType(ComponentType type);
    ComponentType GetComponentType() const;

    void SetSolderBallBumpHeight(Float height);
    void SetSolderFillingMaterial(MaterialId material);
    void SetBoundary(ShapeId boundary);
    void SetMaterial(MaterialId material);
    void SetHeight(Float height);

    void AddPin(FootprintPinId pin);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (ComponentType, compType),
    (ShapeId, boundary),
    (Float, height),
    (Float, solderHeight),
    (MaterialId, material),
    (MaterialId, solderFillingMaterial),
    (IdVec<FootprintPin, NameLut>, pins)
    )
};


} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Cell)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::CircuitCell)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::FootprintCell)
NS_INHERIT_FROM_BASE(nano::package::CircuitCell, nano::package::Cell)
NS_INHERIT_FROM_BASE(nano::package::FootprintCell, nano::package::Cell)
