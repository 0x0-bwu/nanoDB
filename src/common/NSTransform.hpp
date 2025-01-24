#pragma once
#include "generic/geometry/Transform.hpp"
#include "generic/geometry/Point.hpp"
#include "generic/math/MathUtility.hpp"
#include "NSUnit.hpp"
#include <list>

namespace nano  {

using Axis = generic::geometry::Axis;
enum class Mirror2D : int8_t { NO = 0, X = 1, Y = 2, XY = 3 };
inline Axis GetAxis(Mirror2D mirror)
{
    NS_ASSERT(mirror != Mirror2D::NO);
    return static_cast<Axis>(static_cast<int8_t>(mirror) - 1);
}

struct TransformData2D
{
    using Transform = ::generic::geometry::Transform2D<Float>;
    //Transform done in following order.
    Float scale = 1.0;
    Float rotation = 0;//Rotation (in radians) about point (0,0) in CCW direction
    Mirror2D mirror = Mirror2D::NO;//Mirror about X or Y axis
    FCoord2D offset = FCoord2D(0, 0);

    bool isScaled() const { return generic::math::NE<Float>(scale, 1); }
    bool isRotated() const { return generic::math::NE<Float>(rotation, 0); }
    bool isMirrored() const { return mirror != Mirror2D::NO; }
    bool isOffsetted() const { return offset != FCoord2D(0, 0); }
    bool isTransformed() const { return isScaled() || isRotated() || isMirrored() || isOffsetted(); }

    Transform GetTransform() const
    {
        using namespace ::generic::geometry;
        Transform transform;
        if(isScaled()) transform = makeScaleTransform2D<Float>(scale) * transform;
        if(isRotated()) transform = makeRotateTransform2D<Float>(rotation) * transform;
        if(isMirrored()) transform = makeMirroredTransform2D<Float>(GetAxis(mirror)) * transform;
        if(isOffsetted()) transform = makeShiftTransform2D<Float>(offset) * transform;
        return transform;
    }
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        NS_UNUSED(version)
        ar & boost::serialization::make_nvp("scale", scale);
        ar & boost::serialization::make_nvp("rotation", rotation);
        ar & boost::serialization::make_nvp("mirror", mirror);
        ar & boost::serialization::make_nvp("offset", offset);
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
};

class Transform2D
{
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        NS_UNUSED(version)
        if constexpr (Archive::is_loading::value) m_sequence.clear();
        ar & boost::serialization::make_nvp("sequence", m_sequence);
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
public:
    using Transform = TransformData2D::Transform;
    Transform2D() { m_sequence.push_back(TransformData2D{}); }
    ~Transform2D() = default;

    ///Copy
    Transform2D(const Transform2D & other) { *this = other; }
    Transform2D & operator= (const Transform2D & other)
    {
        m_transform.reset();
        m_sequence = other.m_sequence;
        if(other.m_transform)
            m_transform.reset(new Transform(*other.m_transform));  
        return *this;
    }
    ///Move
    Transform2D(Transform2D && other) { *this = std::move(other); }
    Transform2D & operator= (Transform2D && other)
    {
        m_transform = std::move(other.m_transform);
        m_sequence = std::move(other.m_sequence);
        other.m_transform.reset();
        other.m_sequence.clear();
        return *this;
    }
    
    Float & Scale() { m_transform.reset(); return m_sequence.back().scale; }
    const Float & Scale() const { return m_sequence.back().scale; }

    Float & Rotation() { m_transform.reset(); return m_sequence.back().rotation; }
    const Float & Rotation() const { return m_sequence.back().rotation; }

    Mirror2D & Mirror() { m_transform.reset(); return m_sequence.back().mirror; }
    const Mirror2D & Mirror() const { return m_sequence.back().mirror; }

    FCoord2D & Offset() { m_transform.reset(); return m_sequence.back().offset; }
    const FCoord2D & Offset() const { return m_sequence.back().offset; }

    void Append(const Transform2D & transform)
    {
        if (m_transform) *m_transform = transform.GetTransform() * (*m_transform);
        else {
            for (const auto & sequence : transform.m_sequence) {
                if (sequence.isTransformed())
                    m_sequence.push_back(sequence);
            }
        }
    }

    const Transform & GetTransform() const
    {
        using namespace ::generic::geometry;
        if (nullptr == m_transform) {
            Transform transform;
            for (const auto & sequence : m_sequence) {
                if (not sequence.isTransformed()) continue;
                transform = sequence.GetTransform() * transform;
            }
            m_transform.reset(new Transform(std::move(transform)));
        }
        return *m_transform;
    }
private:
    mutable UPtr<Transform> m_transform = nullptr;
    std::list<TransformData2D> m_sequence;//FIFO
};

inline Transform2D makeTransform2D(Float scale, Float rotation, Float x, Float y, Mirror2D mirror = Mirror2D::NO)
{
    Transform2D transform;
    transform.Scale() = scale;
    transform.Rotation() = rotation;
    transform.Offset() = FCoord2D(x, y);
    transform.Mirror() = mirror;
    return transform;
}

class Transformable2D
{
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        NS_UNUSED(version)
        ar & boost::serialization::make_nvp("transform", m_transform);
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
public:
    explicit Transformable2D(Transform2D transform) : m_transform(std::move(transform)) {}
    Transformable2D() = default;
    Transform2D & GetTransform() { return m_transform; }
    const Transform2D & GetTransform() const { return m_transform; }
    void SetTransform(const Transform2D & transform) { m_transform = transform; }
    void AddTransform(const Transform2D & transform) { m_transform.Append(transform); }
protected:
    Ptr<Transformable2D> CloneFrom(const Transformable2D & src) { m_transform = src.m_transform; return this; }
private:
    Transform2D m_transform;
};

inline Transform2D CreateTransform2D(const CoordUnit & coordUnit, Float scale, Float rotation, const FCoord2D & offset, Mirror2D mirror = Mirror2D::NO)
{
    Transform2D transform;
    transform.Scale() = scale;
    transform.Rotation() = rotation;
    transform.Offset() = coordUnit.toCoordF(offset);
    transform.Mirror() = mirror;
    return transform;
}

} // namespace nano

namespace std {

template <>
struct hash<::nano::Transform2D>
{
    size_t operator() (const ::nano::Transform2D & t) const
    {
        return hash<::nano::Transform2D::Transform>{}(t.GetTransform());
    }
};

}