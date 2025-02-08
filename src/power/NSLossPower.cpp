#include <nano/core/power>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::power::LossPower)

namespace nano::power {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void LossPower::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(LossPower);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(LossPower)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

LossPower::LossPower(std::string name, ScenarioId scenario, CId<LookupTable> lut)
 : NamedObj(std::move(name))
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.scenario = scenario;
    m_.lut = lut;
}

LossPower::LossPower() : LossPower("", ScenarioId(), CId<LookupTable>())
{
}

bool LossPower::isValid() const
{
    return m_.scenario and m_.lut->isValid();
}

ScenarioId LossPower::GetScenario() const
{
    return m_.scenario;
}

Float LossPower::GetLossPower(Float temperature) const
{
    return m_.lut->Lookup(temperature, /*extrapolation*/false);
}

} // namespace nano::power