#include <nano/core/common>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::Shape)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::ShapeRect)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::ShapePath)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::ShapeOval)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::ShapeCircle)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::ShapePolygon)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::ShapePolygonWithHoles)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::ShapeFromTemplate)

#include "generic/geometry/Utility.hpp"

namespace nano {

using namespace generic::geometry;

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Shape::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Shape);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Shape)

template <typename Archive>
void ShapeRect::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Shape);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ShapeRect)

template <typename Archive>
void ShapePath::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Shape);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ShapePath)

template <typename Archive>
void ShapeOval::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Shape);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ShapeOval)

template <typename Archive>
void ShapeCircle::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Shape);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ShapeCircle)

template <typename Archive>
void ShapePolygon::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Shape);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ShapePolygon)

template <typename Archive>
void ShapePolygonWithHoles::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Shape);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ShapePolygonWithHoles)

template <typename Archive>
void ShapeFromTemplate::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Shape);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ShapeFromTemplate)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

ShapeRect::ShapeRect(const CoordUnit & coordUnit, FCoord2D ll, FCoord2D ur)
 : ShapeRect(coordUnit.toCoord(ll), coordUnit.toCoord(ur))
{
}

ShapeRect::ShapeRect(NCoord2D ll, NCoord2D ur)
 : ShapeRect(NBox2D(ll, ur))
{
}

ShapeRect::ShapeRect(NBox2D box)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.shape = std::move(box);
}

NBox2D ShapeRect::GetBBox() const
{
    return m_.shape;
}

NPolygon ShapeRect::GetOutline() const
{
    return toPolygon(m_.shape);
}

NPolygonWithHoles ShapeRect::GetContour() const
{
    NPolygonWithHoles pwh;
    pwh.outline = GetOutline();
    return pwh;
}
    
void ShapeRect::Transform(const Transform2D & trans)
{
    m_.shape = Extent(trans.GetTransform() * m_.shape);
}

bool ShapeRect::isValid() const
{
    return m_.shape.isValid();
}

Ptr<ShapeRect> ShapeRect::CloneFrom(const ShapeRect & src)
{
    Entity<Shape>::CloneFrom(src);
    m_ = src.m_;
    return this;
}

ShapePath::ShapePath(const CoordUnit & coordUnit, Vec<FCoord2D> points, FCoord width)
 : ShapePath(coordUnit.toCoord(points), coordUnit.toCoord(width))
{
}

ShapePath::ShapePath(Vec<NCoord2D> points, NCoord width)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.shape = std::move(points);
    m_.width = width;
}

NBox2D ShapePath::GetBBox() const
{
    return Extent(GetContour());
}

NPolygon ShapePath::GetOutline() const
{
    return toPolygon(m_.shape, m_.width, NANO_SHAPE_CIRCLE_DIV);
}

NPolygonWithHoles ShapePath::GetContour() const
{
    NPolygonWithHoles pwh;
    pwh.outline = GetOutline();
    return pwh;
}

void ShapePath::Transform(const Transform2D & trans)
{
    generic::geometry::Transform(m_.shape, trans.GetTransform());
}

bool ShapePath::isValid() const
{
    return not m_.shape.empty() and generic::math::NE<NCoord>(m_.width, 0);
}

void ShapePath::SetPoints(Vec<NCoord2D> points)
{
    m_.shape = std::move(points);
}

Ptr<ShapePath> ShapePath::CloneFrom(const ShapePath & src)
{
    Entity<Shape>::CloneFrom(src);
    m_ = src.m_;
    return this;
}

ShapeOval::ShapeOval(const CoordUnit & coordUnit, FCoord2D o, FCoord a, FCoord b)
 : ShapeOval(coordUnit.toCoord(o), coordUnit.toCoord(a), coordUnit.toCoord(b))
{
}

ShapeOval::ShapeOval(NCoord2D o, NCoord a, NCoord b)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.center = o;
    m_.a = a;
    m_.b = b;
}

ShapeOval::ShapeOval()
 : ShapeOval(NCoord2D(0, 0), 0, 0)
{
}

NBox2D ShapeOval::GetBBox() const
{
    NCoord2D r(m_.a, m_.b);
    return NBox2D(m_.center - r, m_.center + r);
}

NPolygon ShapeOval::GetOutline() const
{
    return toPolygon(Ellipse(m_.center, m_.a, m_.b), NANO_SHAPE_CIRCLE_DIV);
}

NPolygonWithHoles ShapeOval::GetContour() const
{
    NPolygonWithHoles pwh;
    pwh.outline = GetOutline();
    return pwh;
}

void ShapeOval::Transform(const Transform2D & trans)
{
    generic::geometry::Transform(m_.center, trans.GetTransform());
}

bool ShapeOval::isValid() const
{
    return generic::math::GT<NCoord>(m_.a, 0) and generic::math::GT<NCoord>(m_.b, 0);
}

Ptr<ShapeOval> ShapeOval::CloneFrom(const ShapeOval & src)
{
    Entity<Shape>::CloneFrom(src);
    m_ = src.m_;
    return this;
}

ShapeCircle::ShapeCircle(const CoordUnit & coordUnit, FCoord2D o, FCoord r)
 : ShapeCircle(coordUnit.toCoord(o), coordUnit.toCoord(r))
{
}

ShapeCircle::ShapeCircle(NCoord2D o, NCoord r)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.center = o;
    m_.radius = r;
}

ShapeCircle::ShapeCircle()
 : ShapeCircle(NCoord2D(0, 0), 0)
{
}

NBox2D ShapeCircle::GetBBox() const
{
    NCoord2D r(m_.radius, m_.radius);
    return NBox2D(m_.center - r, m_.center + r);
}

NPolygon ShapeCircle::GetOutline() const
{
    return InscribedPolygon(Circle(m_.center, m_.radius), NANO_SHAPE_CIRCLE_DIV);
}

NPolygonWithHoles ShapeCircle::GetContour() const
{
    NPolygonWithHoles pwh;
    pwh.outline = GetOutline();
    return pwh;
}

void ShapeCircle::Transform(const Transform2D & trans)
{
    generic::geometry::Transform(m_.center, trans.GetTransform());
}

bool ShapeCircle::isValid() const
{
    return generic::math::GT<NCoord>(m_.radius, 0);
}

Ptr<ShapeCircle> ShapeCircle::CloneFrom(const ShapeCircle & src)
{
    Entity<Shape>::CloneFrom(src);
    m_ = src.m_;
    return this;
}

ShapePolygon::ShapePolygon(const CoordUnit & coordUnit, const Vec<FCoord2D> & outline, FCoord cornerR)
 : ShapePolygon(coordUnit.toCoord(outline), coordUnit.toCoord(cornerR))
{
}

ShapePolygon::ShapePolygon(Vec<NCoord2D> points, NCoord cornerR)
{
    NS_CLASS_MEMBERS_INITIALIZE
    NPolygon polygon;
    polygon.Set(std::move(points));
    if (cornerR > 0)
        polygon = RoundCorners(polygon, cornerR, NANO_SHAPE_CIRCLE_DIV);
    m_.shape = std::move(polygon);
}

ShapePolygon::ShapePolygon(NPolygon outline)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.shape = std::move(outline);
}

NBox2D ShapePolygon::GetBBox() const
{
    return Extent(m_.shape);
}

NPolygon ShapePolygon::GetOutline() const
{
    return m_.shape;
}

NPolygonWithHoles ShapePolygon::GetContour() const
{
    NPolygonWithHoles pwh;
    pwh.outline = m_.shape;
    return pwh;
}

void ShapePolygon::Transform(const Transform2D & trans)
{
    generic::geometry::Transform(m_.shape, trans.GetTransform());
}

bool ShapePolygon::isValid() const
{
    return m_.shape.Size() >= 3;
}

Ptr<ShapePolygon> ShapePolygon::CloneFrom(const ShapePolygon & src)
{
    Entity<Shape>::CloneFrom(src);
    m_ = src.m_;
    return this;
}

ShapePolygonWithHoles::ShapePolygonWithHoles(NPolygonWithHoles pwh)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.shape = std::move(pwh);
}

bool ShapePolygonWithHoles::hasHole() const
{
    return m_.shape.hasHole();
}

NBox2D ShapePolygonWithHoles::GetBBox() const
{
    return Extent(m_.shape);
}

NPolygon ShapePolygonWithHoles::GetOutline() const
{
    return m_.shape.outline;
}

NPolygonWithHoles ShapePolygonWithHoles::GetContour() const
{
    return m_.shape;
}

void ShapePolygonWithHoles::Transform(const Transform2D & trans)
{
    generic::geometry::Transform(m_.shape, trans.GetTransform());
}

bool ShapePolygonWithHoles::isValid() const
{
    return m_.shape.outline.Size() >= 3;
}

void ShapePolygonWithHoles::SetOutline(NPolygon outline)
{
    m_.shape.outline = std::move(outline);
}

void ShapePolygonWithHoles::AddHole(NPolygon hole)
{
    m_.shape.holes.emplace_back(std::move(hole));
}

Ptr<ShapePolygonWithHoles> ShapePolygonWithHoles::CloneFrom(const ShapePolygonWithHoles & src)
{
    Entity<Shape>::CloneFrom(src);
    m_ = src.m_;
    return this;
}

ShapeFromTemplate::ShapeFromTemplate(Id<Shape> shape)
{
    SetTemplate(shape);
}

bool ShapeFromTemplate::hasHole() const
{
    return m_.shape->hasHole();
}

NBox2D ShapeFromTemplate::GetBBox() const
{
    auto box = m_.shape->GetBBox();
    return Extent(m_.transform.GetTransform() * box);
}

NPolygon ShapeFromTemplate::GetOutline() const
{
    auto outline = m_.shape->GetOutline();
    generic::geometry::Transform(outline, m_.transform.GetTransform());
    return outline;
}

NPolygonWithHoles ShapeFromTemplate::GetContour() const
{
    auto contour = m_.shape->GetContour();
    generic::geometry::Transform(contour, m_.transform.GetTransform());
    return contour;
}

void ShapeFromTemplate::Transform(const Transform2D & trans)
{
    m_.transform.Append(trans);
}

bool ShapeFromTemplate::isValid() const
{
    return m_.shape and m_.shape->isValid();
}

void ShapeFromTemplate::SetTemplate(CId<Shape> shape)
{
    NS_ASSERT(shape and ShapeType::FROM_TEMPLATE != shape->GetType());
    m_.shape = shape;
}

Ptr<ShapeFromTemplate> ShapeFromTemplate::CloneFrom(const ShapeFromTemplate & src)
{
    Entity<Shape>::CloneFrom(src);
    m_ = src.m_;
    return this;
}
} // namespace nano