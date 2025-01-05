#pragma once
#include "basic/NSContainer.hpp"

namespace nano::package {

enum class FootprintLocation
{
    INVALID = -1,
    TOP = 0,
    BOTTOM = 1,
    OTHER = 2,
};
class Footprint : public NamedObj, public Entity<Footprint>
{
public:
    Footprint(std::string name, CId<FootprintCell> footprint, FootprintLocation location);
    Footprint() = default;

    void SetSolderFillingMaterial(CId<Material> material);
    void SetSolderMaterial(CId<Material> material);
    void SetSolderBallBumpHeight(Float height);
    void SetBoundary(CId<Shape> boundary);

    Id<FootprintPin> AddPin(Id<FootprintPin> pin);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<FootprintCell>, footprint),
    (FootprintLocation, location),
    (IdVec<FootprintPin, NameLut>, pins),
    (CId<Shape>, boundary),
    (CId<Material>, solderFillingMaterial),
    (CId<Material>, solderMaterial),
    (Float, solderHeight)
    )
};

} // nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Footprint);