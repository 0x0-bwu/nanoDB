#pragma once
#include <nano/common>

namespace nano::package {

enum class FootprintLocation
{
    INVALID = 0,
    BOT,
    TOP,
    OTHER
};
class Footprint : public NamedObj, public Entity<Footprint>
{
public:
    Footprint(std::string name, CId<FootprintCell> footprint, FootprintLocation location);

    FootprintLocation GetLocation() const;
    void SetSolderFillingMaterial(CId<Material> material);
    CId<Material> GetSolderFillingMaterial() const;

    void SetSolderMaterial(CId<Material> material);
    CId<Material> GetSolderMaterial() const;

    void SetSolderBallBumpThickness(Float thickness);
    Float GetSolderBallBumpThickness() const;

    void SetBoundary(CId<Shape> boundary);
    CId<Shape> GetBoundary() const;

    Id<FootprintPin> AddPin(Id<FootprintPin> pin);
    CId<FootprintPin> FindPin(std::string_view name) const;

private:
    Footprint();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
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