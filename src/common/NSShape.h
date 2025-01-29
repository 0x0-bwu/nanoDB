#pragma once
#include <nano/core/basic>
#include "NSTransform.hpp"
#include "NSUnit.hpp"

#include "generic/geometry/HashFunction.hpp"

namespace nano {

inline static constexpr size_t NANO_SHAPE_CIRCLE_DIV = 16;
enum class ShapeType
{
    INVALID = 0,
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

    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapeRect)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (NBox2D, shape))
};

class ShapePath : public Shape
{
public:
    ShapePath(const CoordUnit & coordUnit, Vec<FCoord2D> points, FCoord width);
    ShapePath(Vec<NCoord2D> points, NCoord width);
    ShapePath() = default;
    
    bool hasHole() const override { return false; }
    NBox2D GetBBox() const override;
    NPolygon GetOutline() const override;
    NPolygonWithHoles GetContour() const override;
    void Transform(const Transform2D & trans) override;
    ShapeType GetType() const override { return ShapeType::PATH; }
    bool isValid() const override;

    void SetPoints(Vec<NCoord2D> points);
    void SetWidth(NCoord width) { m_.width = width;}
    NCoord GetWidth() const { return m_.width; }

    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapePath)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
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

    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapeCircle)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (NCoord2D, center),
    (NCoord, radius))
};

class ShapePolygon : public Shape
{
public:
    ShapePolygon(const CoordUnit & coordUnit, const Vec<FCoord2D> & outline, FCoord cornerR = 0);
    explicit ShapePolygon(Vec<NCoord2D> outline, NCoord cornerR = 0);
    explicit ShapePolygon(NPolygon outline);
    ShapePolygon() = default;

    bool hasHole() const override { return false; }
    NBox2D GetBBox() const override;
    NPolygon GetOutline() const override;
    NPolygonWithHoles GetContour() const override;
    void Transform(const Transform2D & trans) override;
    ShapeType GetType() const override { return ShapeType::POLYGON; }
    bool isValid() const override;

    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapePolygon)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (NPolygon, shape))
};

class ShapePolygonWithHoles : public Shape
{
public:
    explicit ShapePolygonWithHoles(NPolygonWithHoles pwh);
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

    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapePolygonWithHoles)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
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

    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_CLONE_FUNCTIONS_DECLARATION(ShapeFromTemplate)
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
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