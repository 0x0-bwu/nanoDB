#pragma once
#include "NSPackage.h"

namespace nano::package {

class Cell : public NamedObj, public Entity<Cell>
{
protected:
    Cell(std::string name);
    Cell() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
};

class CircuitCell : public Cell, public Entity<CircuitCell>
{
public:
    CircuitCell(std::string name);
    CircuitCell() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
};

class FootprintCell : public Cell, public Entity<FootprintCell>
{
public:
    FootprintCell(std::string name);
    FootprintCell() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
};


} // namespace nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Cell)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::CircuitCell)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::FootprintCell)
NS_INHERIT_FROM_BASE(nano::package::CircuitCell, nano::package::Cell)
NS_INHERIT_FROM_BASE(nano::package::FootprintCell, nano::package::Cell)
