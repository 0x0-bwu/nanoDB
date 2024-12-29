#pragma once
#include "generic/tools/Units.hpp"

namespace nano {

template <typename U>
struct NSUnit
{
public:
    using Unit = U;
    NSUnit() = default;
    NSUnit(NSFloat scale, Unit u) : m_unit(u), m_scale(scale) {}

    NSFloat toSI(NSFloat value = 1) const
    {
        return value * m_scale * generic::unit::Scale2SI(m_unit);
    }

    NSFloat toUnit(NSFloat value, Unit u) const
    {
        return value * m_scale * std::pow(NSFloat(10), (int(m_unit) - int(u)) * 3);
    }

    bool operator== (const NSUnit<U> & other) const
    {
        return std::abs(toSI() - other.toSI()) < 10 * std::numeric_limits<NSFloat>::epsilon();
    }

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        NS_UNUSED(version);
        ar & boost::serialization::make_nvp("scale", m_scale);
        ar & boost::serialization::make_nvp("unit", m_unit);
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
private:
    Unit m_unit{static_cast<U>(0)};
    NSFloat m_scale{1};
};

using PwrUnit = NSUnit<generic::unit::Power>;
using ResUnit = NSUnit<generic::unit::Resistance>;
using CapUnit = NSUnit<generic::unit::Capacitance>;
using TimeUnit = NSUnit<generic::unit::Time>;
using VoltUnit = NSUnit<generic::unit::Voltage>;
using CurrUnit = NSUnit<generic::unit::Current>;

inline static constexpr CapUnit::Unit DEFAULT_CAP_UNIT = CapUnit::Unit::PF;
inline static constexpr ResUnit::Unit DEFAULT_RES_UNIT = ResUnit::Unit::KOHM;

} // namespace nano
