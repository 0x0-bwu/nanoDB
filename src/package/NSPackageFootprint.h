#pragma once
#include <core/basic>

namespace nano::package {

enum class FootprintLocation
{
    INVALID = -1,
    BOT = 0,
    TOP = 1,
    OTHER = 2,
};
class Footprint : public NamedObj, public Entity<Footprint>
{
public:
    Footprint(std::string name, CId<FootprintCell> footprint, FootprintLocation location);
    Footprint() = default;

    FootprintLocation GetLocation() const;
    void SetSolderFillingMaterial(CId<Material> material);
    CId<Material> GetSolderFillingMaterial() const;

    void SetSolderMaterial(CId<Material> material);
    CId<Material> GetSolderMaterial() const;

    void SetSolderBallBumpThickenss(Float thickness);
    Float GetSolderBallBumpThickenss() const;

    void SetBoundary(CId<Shape> boundary);

    Id<FootprintPin> AddPin(Id<FootprintPin> pin);
    CId<FootprintPin> FindPin(std::string_view name) const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<FootprintCell>, footprint),
    (FootprintLocation, location),
    (IdVec<FootprintPin, NameLut>, pins),
    (CId<Shape>, boundary),
    (CId<Material>, solderFillingMaterial),
    (CId<Material>, solderMaterial),
    (Float, solderThickness)
    )
};

} // nano::package
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::package::Footprint);