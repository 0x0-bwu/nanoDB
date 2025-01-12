#pragma once
#include <core/basic>
#include <core/common>
namespace nano::power {

class LossPower : public NamedObj, public Entity<LossPower>
{
public:
    LossPower(std::string name, ScenarioId scenario, CId<LookupTable> lut);
    LossPower() = default;

    bool isValid() const;
    ScenarioId GetScenario() const;
    Float GetLossPower(Float x) const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (ScenarioId, scenario),
    (CId<LookupTable>, lut))
};

} // namespace nano::power
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::power::LossPower)
