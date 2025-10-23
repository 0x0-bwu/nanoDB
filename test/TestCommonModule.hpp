#pragma once
#include "TestCommon.hpp"
#include <nano/db>

using namespace boost::unit_test;

// Test LookupTable1D functionality
void t_lookup_table_1d()
{
    using namespace nano;
    Database::Create("lookup_table_1d_test");
    
    // Create a simple 1D lookup table
    Vec<Float> x{0.0, 1.0, 2.0, 3.0};
    Vec<Float> y{0.0, 10.0, 20.0, 30.0};
    auto lut1d = Create<LookupTable1D>(x, y);
    
    // Test valid lookup table
    BOOST_CHECK(lut1d->isValid());
    
    // Test exact value lookups
    BOOST_CHECK_CLOSE(lut1d->Lookup(0.0, false), 0.0, 1e-6);
    BOOST_CHECK_CLOSE(lut1d->Lookup(1.0, false), 10.0, 1e-6);
    BOOST_CHECK_CLOSE(lut1d->Lookup(2.0, false), 20.0, 1e-6);
    BOOST_CHECK_CLOSE(lut1d->Lookup(3.0, false), 30.0, 1e-6);
    
    // Test interpolated values
    BOOST_CHECK_CLOSE(lut1d->Lookup(0.5, false), 5.0, 1e-6);
    BOOST_CHECK_CLOSE(lut1d->Lookup(1.5, false), 15.0, 1e-6);
    BOOST_CHECK_CLOSE(lut1d->Lookup(2.5, false), 25.0, 1e-6);
    
    // Test with extrapolation
    BOOST_CHECK_CLOSE(lut1d->Lookup(-1.0, true), -10.0, 1e-6);
    BOOST_CHECK_CLOSE(lut1d->Lookup(4.0, true), 40.0, 1e-6);
    
    Database::Shutdown();
}

// Test LookupTable2D functionality
void t_lookup_table_2d()
{
    using namespace nano;
    Database::Create("lookup_table_2d_test");
    
    // Create a 2D lookup table
    Vec<Float> x{0.0, 1.0, 2.0};
    Vec<Float> y{0.0, 10.0, 20.0};
    Vec<Float> z{0.0, 5.0, 10.0,   // x=0
                 10.0, 15.0, 20.0, // x=1
                 20.0, 25.0, 30.0}; // x=2
    auto lut2d = Create<LookupTable2D>(x, y, z);
    
    // Test valid lookup table
    BOOST_CHECK(lut2d->isValid());
    
    // Test exact value lookups
    BOOST_CHECK_CLOSE(lut2d->Lookup(0.0, 0.0, false), 0.0, 1e-6);
    BOOST_CHECK_CLOSE(lut2d->Lookup(1.0, 0.0, false), 10.0, 1e-6);
    BOOST_CHECK_CLOSE(lut2d->Lookup(2.0, 20.0, false), 30.0, 1e-6);
    
    // Test interpolated values
    BOOST_CHECK_CLOSE(lut2d->Lookup(0.5, 0.0, false), 5.0, 1e-6);
    BOOST_CHECK_CLOSE(lut2d->Lookup(1.0, 5.0, false), 12.5, 1e-6);
    
    Database::Shutdown();
}

// Test Material properties - simple value
void t_material_prop_value()
{
    using namespace nano;
    Database::Create("material_prop_value_test");
    
    // Test simple property value
    auto simpleProp = Create<MaterialPropValue>(100.0);
    BOOST_CHECK(simpleProp->isPropValue());
    BOOST_CHECK(!simpleProp->isPropTable());
    BOOST_CHECK(!simpleProp->isPropPolynomial());
    
    Float value;
    BOOST_CHECK(simpleProp->GetSimpleProperty(value));
    BOOST_CHECK_CLOSE(value, 100.0, 1e-6);
    
    size_t row, col;
    simpleProp->GetDimensions(row, col);
    BOOST_CHECK(row == 1 && col == 1);
    
    // Test anisotropic property value
    Arr3<Float> anisoValues{100.0, 200.0, 300.0};
    auto anisoProp = Create<MaterialPropValue>(anisoValues);
    
    anisoProp->GetDimensions(row, col);
    BOOST_CHECK(row == 3 && col == 1);
    
    BOOST_CHECK(anisoProp->GetAnisotropicProperty(0, value));
    BOOST_CHECK_CLOSE(value, 100.0, 1e-6);
    BOOST_CHECK(anisoProp->GetAnisotropicProperty(1, value));
    BOOST_CHECK_CLOSE(value, 200.0, 1e-6);
    BOOST_CHECK(anisoProp->GetAnisotropicProperty(2, value));
    BOOST_CHECK_CLOSE(value, 300.0, 1e-6);
    
    // Test tensor property value
    Arr9<Float> tensorValues{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    auto tensorProp = Create<MaterialPropValue>(tensorValues);
    
    tensorProp->GetDimensions(row, col);
    BOOST_CHECK(row == 3 && col == 3);
    
    BOOST_CHECK(tensorProp->GetTensorProperty(0, 0, value));
    BOOST_CHECK_CLOSE(value, 1.0, 1e-6);
    BOOST_CHECK(tensorProp->GetTensorProperty(1, 1, value));
    BOOST_CHECK_CLOSE(value, 5.0, 1e-6);
    BOOST_CHECK(tensorProp->GetTensorProperty(2, 2, value));
    BOOST_CHECK_CLOSE(value, 9.0, 1e-6);
    
    Database::Shutdown();
}

// Test Material properties - polynomial
void t_material_prop_polynomial()
{
    using namespace nano;
    Database::Create("material_prop_polynomial_test");
    
    // Create polynomial: 2 + 3x + 4x^2
    Vec<Vec<Float>> coeffs{{2.0, 3.0, 4.0}};
    auto polyProp = Create<MaterialPropPolynomial>(coeffs);
    
    BOOST_CHECK(polyProp->isPropPolynomial());
    
    Float value;
    // At x=0: 2 + 3*0 + 4*0 = 2
    BOOST_CHECK(polyProp->GetSimpleProperty(0.0, value));
    BOOST_CHECK_CLOSE(value, 2.0, 1e-6);
    
    // At x=1: 2 + 3*1 + 4*1 = 9
    BOOST_CHECK(polyProp->GetSimpleProperty(1.0, value));
    BOOST_CHECK_CLOSE(value, 9.0, 1e-6);
    
    // At x=2: 2 + 3*2 + 4*4 = 24
    BOOST_CHECK(polyProp->GetSimpleProperty(2.0, value));
    BOOST_CHECK_CLOSE(value, 24.0, 1e-6);
    
    size_t row, col;
    polyProp->GetDimensions(row, col);
    BOOST_CHECK(row == 1 && col == 1);
    
    Database::Shutdown();
}

// Test Material and MaterialLib functionality
void t_material_and_lib()
{
    using namespace nano;
    Database::Create("material_lib_test");
    
    // Create materials
    auto copper = Create<Material>("Copper");
    auto aluminum = Create<Material>("Aluminum");
    auto silicon = Create<Material>("Silicon");
    
    // Set material types
    copper->SetMaterialType(MaterialType::RIGID);
    BOOST_CHECK(copper->GetMaterialType() == MaterialType::RIGID);
    
    // Set material properties
    auto thermalCond = Create<MaterialPropValue>(400.0); // W/(m·K)
    copper->SetProperty(Material::THERMAL_CONDUCTIVITY, thermalCond);
    
    auto density = Create<MaterialPropValue>(8960.0); // kg/m^3
    copper->SetProperty(Material::MASS_DENSITY, density);
    
    // Check properties
    BOOST_CHECK(copper->hasProperty(Material::THERMAL_CONDUCTIVITY));
    BOOST_CHECK(copper->hasProperty(Material::MASS_DENSITY));
    BOOST_CHECK(!copper->hasProperty(Material::RESISTIVITY));
    
    auto retrievedProp = copper->GetProperty(Material::THERMAL_CONDUCTIVITY);
    BOOST_CHECK(retrievedProp);
    Float value;
    BOOST_CHECK(Id<MaterialPropValue>(retrievedProp)->GetSimpleProperty(value));
    BOOST_CHECK_CLOSE(value, 400.0, 1e-6);
    
    // Create material library
    auto matLib = Create<MaterialLib>("TestLib");
    matLib->AddMaterial(copper);
    matLib->AddMaterial(aluminum);
    matLib->AddMaterial(silicon);
    
    // Find materials
    auto foundCopper = matLib->FindMaterial("Copper");
    BOOST_CHECK(foundCopper);
    BOOST_CHECK(foundCopper->GetName() == "Copper");
    
    auto notFound = matLib->FindMaterial("Gold");
    BOOST_CHECK(notFound.isNull());
    
    // Iterate materials
    size_t count = 0;
    auto iter = matLib->GetMaterialIter();
    while(iter.Next()) {
        count++;
        BOOST_CHECK(iter.Current());
    }
    BOOST_CHECK(count == 3);
    
    // Remove property
    copper->RemoveProperty(Material::THERMAL_CONDUCTIVITY);
    BOOST_CHECK(!copper->hasProperty(Material::THERMAL_CONDUCTIVITY));
    BOOST_CHECK(copper->hasProperty(Material::MASS_DENSITY));
    
    Database::Shutdown();
}

// Test Shape - Rectangle
void t_shape_rect()
{
    using namespace nano;
    Database::Create("shape_rect_test");
    
    CoordUnit coordUnit(CoordUnit::Unit::MILLIMETER);
    
    // Create rectangle
    auto rect = Create<ShapeRect>(coordUnit, FCoord2D{0.0, 0.0}, FCoord2D{10.0, 5.0});
    
    BOOST_CHECK(rect->isValid());
    BOOST_CHECK(!rect->hasHole());
    BOOST_CHECK(rect->GetType() == ShapeType::RECTANGLE);
    
    // Check bounding box
    auto bbox = rect->GetBBox();
    BOOST_CHECK(bbox[0][0] == 0);
    BOOST_CHECK(bbox[0][1] == 0);
    BOOST_CHECK(bbox[1][0] == coordUnit.toCoord(10.0));
    BOOST_CHECK(bbox[1][1] == coordUnit.toCoord(5.0));
    
    // Check outline (should be 4 points for rectangle)
    auto outline = rect->GetOutline();
    BOOST_CHECK(outline.size() == 5); // 4 corners + closed
    
    // Clone test
    auto clone = rect->Clone();
    BOOST_CHECK(clone);
    BOOST_CHECK(!rect->Identical(clone));
    auto cloneRect = Id<ShapeRect>(clone);
    BOOST_CHECK(cloneRect->GetBBox() == rect->GetBBox());
    
    Database::Shutdown();
}

// Test Shape - Circle
void t_shape_circle()
{
    using namespace nano;
    Database::Create("shape_circle_test");
    
    CoordUnit coordUnit(CoordUnit::Unit::MILLIMETER);
    
    // Create circle
    auto circle = Create<ShapeCircle>(coordUnit, FCoord2D{5.0, 5.0}, 3.0);
    
    BOOST_CHECK(circle->isValid());
    BOOST_CHECK(!circle->hasHole());
    BOOST_CHECK(circle->GetType() == ShapeType::CIRCLE);
    
    // Check center and radius
    BOOST_CHECK(circle->GetCenter()[0] == coordUnit.toCoord(5.0));
    BOOST_CHECK(circle->GetCenter()[1] == coordUnit.toCoord(5.0));
    BOOST_CHECK(circle->GetRadius() == coordUnit.toCoord(3.0));
    
    // Check bounding box
    auto bbox = circle->GetBBox();
    BOOST_CHECK(bbox[0][0] == coordUnit.toCoord(2.0));
    BOOST_CHECK(bbox[0][1] == coordUnit.toCoord(2.0));
    BOOST_CHECK(bbox[1][0] == coordUnit.toCoord(8.0));
    BOOST_CHECK(bbox[1][1] == coordUnit.toCoord(8.0));
    
    // Check outline (approximated by polygon)
    auto outline = circle->GetOutline();
    BOOST_CHECK(outline.size() > 0);
    
    Database::Shutdown();
}

// Test Shape - Polygon
void t_shape_polygon()
{
    using namespace nano;
    Database::Create("shape_polygon_test");
    
    CoordUnit coordUnit(CoordUnit::Unit::MILLIMETER);
    
    // Create triangle
    Vec<FCoord2D> points{{0.0, 0.0}, {10.0, 0.0}, {5.0, 8.66}};
    auto polygon = Create<ShapePolygon>(coordUnit, points);
    
    BOOST_CHECK(polygon->isValid());
    BOOST_CHECK(!polygon->hasHole());
    BOOST_CHECK(polygon->GetType() == ShapeType::POLYGON);
    
    // Check outline
    auto outline = polygon->GetOutline();
    BOOST_CHECK(outline.size() == 4); // 3 points + closed
    
    // Check bounding box
    auto bbox = polygon->GetBBox();
    BOOST_CHECK(bbox[0][0] == coordUnit.toCoord(0.0));
    BOOST_CHECK(bbox[0][1] == coordUnit.toCoord(0.0));
    BOOST_CHECK(bbox[1][0] == coordUnit.toCoord(10.0));
    BOOST_CHECK(bbox[1][1] >= coordUnit.toCoord(8.0)); // Allow for rounding
    
    Database::Shutdown();
}

// Test Shape - Polygon with Holes
void t_shape_polygon_with_holes()
{
    using namespace nano;
    Database::Create("shape_polygon_holes_test");
    
    CoordUnit coordUnit(CoordUnit::Unit::MILLIMETER);
    
    // Create polygon with hole
    auto polygon = Create<ShapePolygonWithHoles>();
    
    // Set outer boundary (square)
    Vec<FCoord2D> outer{{0.0, 0.0}, {10.0, 0.0}, {10.0, 10.0}, {0.0, 10.0}};
    auto outerPoly = ShapePolygon(coordUnit, outer);
    polygon->SetOutline(outerPoly.GetOutline());
    
    // Add hole (smaller square)
    Vec<FCoord2D> hole{{2.0, 2.0}, {8.0, 2.0}, {8.0, 8.0}, {2.0, 8.0}};
    auto holePoly = ShapePolygon(coordUnit, hole);
    polygon->AddHole(holePoly.GetOutline());
    
    BOOST_CHECK(polygon->isValid());
    BOOST_CHECK(polygon->hasHole());
    BOOST_CHECK(polygon->GetType() == ShapeType::POLYGON_WITH_HOLES);
    
    // Check contour
    auto contour = polygon->GetContour();
    BOOST_CHECK(contour.outer().size() > 0);
    BOOST_CHECK(contour.holes().size() == 1);
    
    Database::Shutdown();
}

// Test Shape transformations
void t_shape_transform()
{
    using namespace nano;
    Database::Create("shape_transform_test");
    
    CoordUnit coordUnit(CoordUnit::Unit::MILLIMETER);
    
    // Create rectangle
    auto rect = Create<ShapeRect>(coordUnit, FCoord2D{0.0, 0.0}, FCoord2D{10.0, 5.0});
    auto origBBox = rect->GetBBox();
    
    // Create translation transform
    auto trans = CreateTransform2D(coordUnit, 1.0, 0.0, {5.0, 5.0});
    
    // Apply transform
    rect->Transform(*trans);
    auto newBBox = rect->GetBBox();
    
    // Check that rectangle moved
    BOOST_CHECK(newBBox[0][0] == coordUnit.toCoord(5.0));
    BOOST_CHECK(newBBox[0][1] == coordUnit.toCoord(5.0));
    BOOST_CHECK(newBBox[1][0] == coordUnit.toCoord(15.0));
    BOOST_CHECK(newBBox[1][1] == coordUnit.toCoord(10.0));
    
    Database::Shutdown();
}

test_suite * create_nano_common_module_test_suite()
{
    test_suite * common_module_suite = BOOST_TEST_SUITE("s_common_module_test");
    
    // Lookup table tests
    common_module_suite->add(BOOST_TEST_CASE(&t_lookup_table_1d));
    common_module_suite->add(BOOST_TEST_CASE(&t_lookup_table_2d));
    
    // Material property tests
    common_module_suite->add(BOOST_TEST_CASE(&t_material_prop_value));
    common_module_suite->add(BOOST_TEST_CASE(&t_material_prop_polynomial));
    common_module_suite->add(BOOST_TEST_CASE(&t_material_and_lib));
    
    // Shape tests
    common_module_suite->add(BOOST_TEST_CASE(&t_shape_rect));
    common_module_suite->add(BOOST_TEST_CASE(&t_shape_circle));
    common_module_suite->add(BOOST_TEST_CASE(&t_shape_polygon));
    common_module_suite->add(BOOST_TEST_CASE(&t_shape_polygon_with_holes));
    common_module_suite->add(BOOST_TEST_CASE(&t_shape_transform));
    
    return common_module_suite;
}
