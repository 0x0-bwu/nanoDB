#pragma once
#include "generic/tools/Units.hpp"

namespace nano {

template <typename U>
struct NSUnit
{
public:
    using Unit = U;
    NSUnit() = default;
    NSUnit(Float scale, Unit u) : m_unit(u), m_scale(scale) {}

    Float toSI(Float value = 1) const
    {
        return value * m_scale * generic::unit::Scale2SI(m_unit);
    }

    Float toUnit(Float value, Unit u) const
    {
        return value * m_scale * std::pow(Float(10), (int(m_unit) - int(u)) * 3);
    }

    bool operator== (const NSUnit<U> & other) const
    {
        return std::abs(toSI() - other.toSI()) < 10 * std::numeric_limits<Float>::epsilon();
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
    Float m_scale{1};
};

using PwrUnit = NSUnit<generic::unit::Power>;
using ResUnit = NSUnit<generic::unit::Resistance>;
using CapUnit = NSUnit<generic::unit::Capacitance>;
using TimeUnit = NSUnit<generic::unit::Time>;
using VoltUnit = NSUnit<generic::unit::Voltage>;
using CurrUnit = NSUnit<generic::unit::Current>;

inline static constexpr CapUnit::Unit DEFAULT_CAP_UNIT = CapUnit::Unit::PF;
inline static constexpr ResUnit::Unit DEFAULT_RES_UNIT = ResUnit::Unit::KOHM;


struct CoordUnit
{
public:
    using Unit = generic::unit::Length;
    CoordUnit() : CoordUnit(Unit::Micrometer, Unit::Nanometer) {}
    explicit CoordUnit(Unit user) : CoordUnit(user, Unit::Nanometer) {}
    /**
     * @brief Construct a new CoordUnit object
     * 
     * @param user user unit, defualt is um
     * @param data database unit, default is nm
     */
    explicit CoordUnit(Unit user, Unit data)
    {
        m_precision = generic::unit::Scale2Meter(data);
        m_unit = m_precision / generic::unit::Scale2Meter(user);
    }

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        NS_UNUSED(version);
        ar & boost::serialization::make_nvp("unit", m_unit);
        ar & boost::serialization::make_nvp("precision", m_precision);
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

    Float Scale2Coord() const
    {
        return Float(1) / m_unit;
    }

    NCoord toCoord(Float value) const
    {
        return value * Scale2Coord();
    }

    NCoord2D toCoord(const FCoord2D & coord) const
    {
        return NCoord2D(toCoord(coord[0]), toCoord(coord[1]));
    }

    FCoord2D toCoordF(const FCoord2D & coord) const
    {
        return FCoord2D(toCoord(coord[0]), toCoord(coord[1]));
    }

    std::vector<NCoord2D> toCoord(const std::vector<FCoord2D> & coords) const
    {
        std::vector<NCoord2D> res; res.reserve(coords.size());
        std::transform(coords.begin(), coords.end(), 
            std::back_inserter(res), [&](const auto & c) { return toCoord(c); });
        return res;
    }

    Float Scale2Unit() const
    {
        return m_unit;
    }

    template <typename Coord>
    Float toUnit(Coord coord) const
    {
        return coord * Scale2Unit();
    }

private:
    Float m_unit{1e-3};
    Float m_precision{1e-9};
};

} // namespace nano
