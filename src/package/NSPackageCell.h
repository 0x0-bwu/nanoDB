#pragma once
#include <nano/common>
namespace nano::package {

class Cell : public NamedObj, public Entity<Cell>
{
protected:
    Cell(std::string name, CId<Package> package);
    Cell();
public:
    CId<Package> GetPackage() const { return m_.package; }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (CId<Package>, package)
    )
};

class CircuitCell : public Cell
{
public:
    CircuitCell(std::string name, CId<Package> package);

    Id<Layout> SetLayout(Id<Layout> layout);
    CId<Layout> GetLayout() const;
    Id<Layout> GetLayout();
    
private:
    CircuitCell();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
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

class FootprintCell : public Cell
{
public:
    FootprintCell(std::string name, CId<Package> package);

    void SetComponentType(ComponentType type);
    ComponentType GetComponentType() const;
    
    void SetBoundary(Id<Shape> boundary);
    CId<Shape> GetBoundary() const;

    void SetMaterial(Id<Material> material);
    CId<Material> GetMaterial() const;

    void SetHeight(Float height);
    virtual Float GetHeight() const;

    Id<Footprint> AddFootprint(Id<Footprint> interface);

    CId<Footprint> FindFootprint(std::string_view name) const;

    virtual bool isBlackBox() const { return true; }

private:
    FootprintCell();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (ComponentType, compType),
    (Id<Shape>, boundary),
    (Float, height),
    (Id<Material>, material),
    (IdVec<Footprint, NameLut>, footprints)
    )
};


} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Cell)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::CircuitCell)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::FootprintCell)
NS_INHERIT_FROM_BASE(nano::package::CircuitCell, nano::package::Cell)
NS_INHERIT_FROM_BASE(nano::package::FootprintCell, nano::package::Cell)
