#pragma once
#include "NSPackage.h"

namespace nano::package {

class Cell : public NamedObj, public Entity<Cell>
{
protected:
    Cell(std::string name, CId<Package> package);
    Cell() = default;
public:
    CId<Package> GetPackage() const { return m_.package; }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Package>, package)
    )
};

class CircuitCell : public Cell, public Entity<CircuitCell>
{
public:
    CircuitCell(std::string name, CId<Package> package);
    CircuitCell() = default;

    Id<Layout> SetLayout(Id<Layout> layout);
    CId<Layout> GetLayout() const;
    Id<Layout> GetLayout();
    

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Layout>, layout))
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
    FootprintCell(std::string name, CId<Package> package);
    FootprintCell();

    void SetComponentType(ComponentType type);
    ComponentType GetComponentType() const;

    void SetSolderBallBumpHeight(Float height);
    void SetSolderFillingMaterial(Id<Material> material);
    void SetBoundary(Id<Shape> boundary);
    void SetMaterial(Id<Material> material);
    void SetHeight(Float height);

    void AddPin(Id<FootprintPin> pin);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (ComponentType, compType),
    (Id<Shape>, boundary),
    (Float, height),
    (Float, solderHeight),
    (Id<Material>, material),
    (Id<Material>, solderFillingMaterial),
    (IdVec<FootprintPin, NameLut>, pins)
    )
};


} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Cell)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::CircuitCell)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::FootprintCell)
NS_INHERIT_FROM_BASE(nano::package::CircuitCell, nano::package::Cell)
NS_INHERIT_FROM_BASE(nano::package::FootprintCell, nano::package::Cell)
