#pragma once
#include <nano/core/basic>
#include "NSTransform.hpp"
#include "NSUnit.hpp"

#include "generic/geometry/Geometries.hpp"

namespace nano {

using NBox2D   = generic::geometry::Box2D<NCoord>;
using FBox2D   = generic::geometry::Box2D<FCoord>;
using NBox3D   = generic::geometry::Box3D<NCoord>;
using FBox3D   = generic::geometry::Box3D<FCoord>;
using NPolygon = generic::geometry::Polygon2D<NCoord>;
using FPolygon = generic::geometry::Polygon2D<FCoord>;
using NPolyline = generic::geometry::Polyline2D<NCoord>;
using FPolyline = generic::geometry::Polyline2D<FCoord>;
using NPolygonWithHoles = generic::geometry::PolygonWithHoles2D<NCoord>;
using FPolygonWithHoles = generic::geometry::PolygonWithHoles2D<FCoord>;

inline static constexpr size_t NANO_SHAPE_CIRCLE_DIV = 32;
enum class ShapeType
{
    INVALID = -1,
    RECTANGLE,
    PATH,
    CIRCLE,
    POLYGON,
    POLYGON_WITH_HOLES,
    FROM_TEMPLATE
};

class Shape : public Cloneable<Shape>, public Entity<Shape>
{
public:
    virtual ~Shape() = default;
    virtual bool hasHole() const = 0;
    virtual NBox2D GetBBox() const = 0;
    virtual NPolygon GetOutline() const = 0;
    virtual NPolygonWithHoles GetContour() const = 0;
    virtual void Transform(const Transform2D & trans) = 0;
    virtual ShapeType GetType() const { return ShapeType::INVALID; }
    virtual bool isValid() const { return false; }
protected:
    Shape() = default;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
};

class ShapeRect : public Shape
{
public:
    ShapeRect(const CoordUnit & coordUnit, FCoord2D ll, FCoord2D ur);
    ShapeRect(NCoord2D ll, NCoord2D ur);
    explicit ShapeRect(NBox2D box);
    ShapeRect() = default;

    bool hasHole() const override { return false; }
    NBox2D GetBBox() const override;
    NPolygon GetOutline() const override;
    NPolygonWithHoles GetContour() const override;
    void Transform(const Transform2D & trans) override;
    ShapeType GetType() const override { return ShapeType::RECTANGLE; }
    bool isValid() const override;

private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapeRect)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (NBox2D, shape))
};

class ShapePath : public Shape
{
public:
    ShapePath() = default;
    
    bool hasHole() const override { return false; }
    NBox2D GetBBox() const override;
    NPolygon GetOutline() const override;
    NPolygonWithHoles GetContour() const override;
    void Transform(const Transform2D & trans) override;
    ShapeType GetType() const override { return ShapeType::PATH; }
    bool isValid() const override;

    void SetPoints(std::vector<NCoord2D> points);
    void SetWidth(NCoord width) { m_.width = width;}
    NCoord GetWidth() const { return m_.width; }

private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapePath)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (NPolyline, shape),
    (NCoord, width))
};

class ShapeCircle : public Shape
{
public:
    ShapeCircle(const CoordUnit & coordUnit, FCoord2D o, FCoord r);
    ShapeCircle(NCoord2D o, NCoord r);
    ShapeCircle();
    
    bool hasHole() const override { return false; }
    NBox2D GetBBox() const override;
    NPolygon GetOutline() const override;
    NPolygonWithHoles GetContour() const override;
    void Transform(const Transform2D & trans) override;
    ShapeType GetType() const override { return ShapeType::CIRCLE; }
    bool isValid() const override;

    NCoord2D GetCenter() const { return m_.center; }
    NCoord GetRadius() const { return m_.radius; }

private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapeCircle)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (NCoord2D, center),
    (NCoord, radius))
};

class ShapePolygon : public Shape
{
public:
    ShapePolygon(const CoordUnit & coordUnit, std::vector<FCoord2D> outline, FCoord cornerR = 0);
    explicit ShapePolygon(std::vector<NCoord2D> outline, NCoord cornerR = 0);
    ShapePolygon() = default;

    bool hasHole() const override { return false; }
    NBox2D GetBBox() const override;
    NPolygon GetOutline() const override;
    NPolygonWithHoles GetContour() const override;
    void Transform(const Transform2D & trans) override;
    ShapeType GetType() const override { return ShapeType::POLYGON; }
    bool isValid() const override;

private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapePolygon)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (NPolygon, shape))
};

class ShapePolygonWithHoles : public Shape
{
public:
    ShapePolygonWithHoles() = default;

    bool hasHole() const override;
    NBox2D GetBBox() const override;
    NPolygon GetOutline() const override;
    NPolygonWithHoles GetContour() const override;
    void Transform(const Transform2D & trans) override;
    ShapeType GetType() const override { return ShapeType::POLYGON_WITH_HOLES; }
    bool isValid() const override;

    void SetOutline(NPolygon outline);
    void AddHole(NPolygon hole);

private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapePolygonWithHoles)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (NPolygonWithHoles, shape))
};

class ShapeFromTemplate : public Shape
{
public:
    explicit ShapeFromTemplate(Id<Shape> shape);
    ShapeFromTemplate() = default;

    bool hasHole() const override;
    NBox2D GetBBox() const override;
    NPolygon GetOutline() const override;
    NPolygonWithHoles GetContour() const override;
    void Transform(const Transform2D & trans) override;
    ShapeType GetType() const override { return ShapeType::FROM_TEMPLATE; }
    bool isValid() const override;

    void SetTemplate(CId<Shape> shape);

private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapeFromTemplate)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (CId<Shape>, shape),
    (Transform2D, transform))
};

} // namespace nano
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::Shape)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::ShapeRect)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::ShapePath)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::ShapeCircle)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::ShapePolygon)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::ShapePolygonWithHoles)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::ShapeFromTemplate)
NS_INHERIT_FROM_BASE(nano::ShapeRect, nano::Shape)
NS_INHERIT_FROM_BASE(nano::ShapePath, nano::Shape)
NS_INHERIT_FROM_BASE(nano::ShapeCircle, nano::Shape)
NS_INHERIT_FROM_BASE(nano::ShapePolygon, nano::Shape)
NS_INHERIT_FROM_BASE(nano::ShapePolygonWithHoles, nano::Shape)
NS_INHERIT_FROM_BASE(nano::ShapeFromTemplate, nano::Shape)