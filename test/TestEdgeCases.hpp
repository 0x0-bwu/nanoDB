#pragma once
#include "TestCommon.hpp"
#include <nano/db>

using namespace boost::unit_test;

// Test edge case: Empty collections
void t_edge_empty_collections()
{
    using namespace nano;
    using namespace chip;
    Database::Create("edge_empty_collections");
    
    auto chip = Create<Chip>("empty_chip");
    auto block = Create<Block>("empty_block");
    chip->AddBlock(block);
    chip->SetTop(block);
    
    // Check empty collections
    BOOST_CHECK(block->NumOfNets() == 0);
    BOOST_CHECK(block->NumOfInsts() == 0);
    BOOST_CHECK(block->NumOfBTerms() == 0);
    
    // Try to find non-existent items
    BOOST_CHECK(block->FindNet("non_existent").isNull());
    BOOST_CHECK(block->FindInst("non_existent").isNull());
    BOOST_CHECK(block->FindBTerm("non_existent").isNull());
    
    // Iterate over empty collections
    auto netIter = block->GetNetIter();
    size_t count = 0;
    while (netIter.Next()) count++;
    BOOST_CHECK(count == 0);
    
    auto instIter = block->GetInstIter();
    count = 0;
    while (instIter.Next()) count++;
    BOOST_CHECK(count == 0);
    
    Database::Shutdown();
}

// Test edge case: Null IDs and invalid references
void t_edge_null_ids()
{
    using namespace nano;
    Database::Create("edge_null_ids");
    
    // Create null IDs
    Id<Material> nullMat;
    BOOST_CHECK(nullMat.isNull());
    BOOST_CHECK(!nullMat);
    
    CId<Material> cnullMat;
    BOOST_CHECK(cnullMat.isNull());
    BOOST_CHECK(!cnullMat);
    
    // Test null ID operations
    BOOST_CHECK(nullMat.operator->() == nullptr);
    BOOST_CHECK(cnullMat.operator->() == nullptr);
    
    Database::Shutdown();
}

// Test edge case: Duplicate names
void t_edge_duplicate_names()
{
    using namespace nano;
    using namespace chip;
    Database::Create("edge_duplicate_names");
    
    auto chip = Create<Chip>("test");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    
    // Create nets with same name
    auto net1 = Create<Net>("signal", block);
    auto net2 = Create<Net>("signal", block);
    auto net3 = Create<Net>("signal", block);
    
    block->AddNets(Vec<Id<Net>>{net1, net2, net3});
    BOOST_CHECK(block->NumOfNets() == 3);
    
    // All should exist even with duplicate names
    BOOST_CHECK(net1->GetName() == "signal");
    BOOST_CHECK(net2->GetName() == "signal");
    BOOST_CHECK(net3->GetName() == "signal");
    
    // But they should be different objects
    BOOST_CHECK(!net1->Identical(net2));
    BOOST_CHECK(!net2->Identical(net3));
    
    Database::Shutdown();
}

// Test edge case: Large collections
void t_edge_large_collections()
{
    using namespace nano;
    using namespace chip;
    Database::Create("edge_large_collections");
    
    auto chip = Create<Chip>("large_chip");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    
    // Create many nets
    const size_t numNets = 1000;
    for (size_t i = 0; i < numNets; ++i) {
        auto net = Create<Net>("net_" + std::to_string(i), block);
        block->AddNets(Vec<Id<Net>>{net});
    }
    
    BOOST_CHECK(block->NumOfNets() == numNets);
    
    // Verify we can iterate through all
    auto iter = block->GetNetIter();
    size_t count = 0;
    while (iter.Next()) {
        BOOST_CHECK(iter.Current());
        count++;
    }
    BOOST_CHECK(count == numNets);
    
    // Verify we can find by name
    auto found = block->FindNet("net_500");
    BOOST_CHECK(found);
    BOOST_CHECK(found->GetName() == "net_500");
    
    Database::Shutdown();
}

// Test edge case: Boundary values for material properties
void t_edge_material_boundary_values()
{
    using namespace nano;
    Database::Create("edge_material_boundaries");
    
    auto mat = Create<Material>("BoundaryTest");
    
    // Test with very small values
    auto smallProp = Create<MaterialPropValue>(1e-10);
    mat->SetProperty(Material::THERMAL_CONDUCTIVITY, smallProp);
    
    Float value;
    BOOST_CHECK(Id<MaterialPropValue>(mat->GetProperty(Material::THERMAL_CONDUCTIVITY))->GetSimpleProperty(value));
    BOOST_CHECK_CLOSE(value, 1e-10, 1e-6);
    
    // Test with very large values
    auto largeProp = Create<MaterialPropValue>(1e10);
    mat->SetProperty(Material::MASS_DENSITY, largeProp);
    
    BOOST_CHECK(Id<MaterialPropValue>(mat->GetProperty(Material::MASS_DENSITY))->GetSimpleProperty(value));
    BOOST_CHECK_CLOSE(value, 1e10, 1e-6);
    
    // Test with zero
    auto zeroProp = Create<MaterialPropValue>(0.0);
    mat->SetProperty(Material::RESISTIVITY, zeroProp);
    
    BOOST_CHECK(Id<MaterialPropValue>(mat->GetProperty(Material::RESISTIVITY))->GetSimpleProperty(value));
    BOOST_CHECK_CLOSE(value, 0.0, 1e-10);
    
    // Test with negative values (should work for some properties)
    auto negProp = Create<MaterialPropValue>(-273.15); // Could represent temperature offset
    mat->SetProperty(Material::THERMAL_EXPANSION_COEFFICIENT, negProp);
    
    BOOST_CHECK(Id<MaterialPropValue>(mat->GetProperty(Material::THERMAL_EXPANSION_COEFFICIENT))->GetSimpleProperty(value));
    BOOST_CHECK_CLOSE(value, -273.15, 1e-6);
    
    Database::Shutdown();
}

// Test edge case: LookupTable with single point
void t_edge_lookup_single_point()
{
    using namespace nano;
    Database::Create("edge_lookup_single");
    
    // Single point 1D LUT
    Vec<Float> x{5.0};
    Vec<Float> y{10.0};
    auto lut = Create<LookupTable1D>(x, y);
    
    BOOST_CHECK(lut->isValid());
    
    // All lookups should return the same value
    BOOST_CHECK_CLOSE(lut->Lookup(0.0, false), 10.0, 1e-6);
    BOOST_CHECK_CLOSE(lut->Lookup(5.0, false), 10.0, 1e-6);
    BOOST_CHECK_CLOSE(lut->Lookup(100.0, false), 10.0, 1e-6);
    
    Database::Shutdown();
}

// Test edge case: LookupTable with two points
void t_edge_lookup_two_points()
{
    using namespace nano;
    Database::Create("edge_lookup_two");
    
    // Two point 1D LUT
    Vec<Float> x{0.0, 10.0};
    Vec<Float> y{0.0, 100.0};
    auto lut = Create<LookupTable1D>(x, y);
    
    BOOST_CHECK(lut->isValid());
    
    // Test interpolation
    BOOST_CHECK_CLOSE(lut->Lookup(0.0, false), 0.0, 1e-6);
    BOOST_CHECK_CLOSE(lut->Lookup(5.0, false), 50.0, 1e-6);
    BOOST_CHECK_CLOSE(lut->Lookup(10.0, false), 100.0, 1e-6);
    
    // Test extrapolation
    BOOST_CHECK_CLOSE(lut->Lookup(-10.0, true), -100.0, 1e-6);
    BOOST_CHECK_CLOSE(lut->Lookup(20.0, true), 200.0, 1e-6);
    
    Database::Shutdown();
}

// Test edge case: Shape with zero dimensions
void t_edge_shape_zero_dimensions()
{
    using namespace nano;
    Database::Create("edge_shape_zero");
    
    CoordUnit coordUnit(CoordUnit::Unit::MILLIMETER);
    
    // Rectangle with zero area (line)
    auto lineRect = Create<ShapeRect>(coordUnit, FCoord2D{0.0, 0.0}, FCoord2D{10.0, 0.0});
    // This might be invalid depending on implementation
    
    // Circle with zero radius (point)
    auto pointCircle = Create<ShapeCircle>(coordUnit, FCoord2D{5.0, 5.0}, 0.0);
    BOOST_CHECK(pointCircle->GetRadius() == 0);
    
    Database::Shutdown();
}

// Test edge case: Liberty cell with no pins
void t_edge_liberty_no_pins()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("edge_liberty_no_pins");
    
    auto lib = Create<Library>("test_lib");
    auto cell = Create<Cell>("EmptyCell", lib);
    lib->AddCell(cell);
    
    BOOST_CHECK(cell->NumOfSignalPins() == 0);
    BOOST_CHECK(cell->NumOfPgPins() == 0);
    
    // Try to find non-existent pin
    BOOST_CHECK(cell->FindSignalPin("A").isNull());
    BOOST_CHECK(cell->FindPgPin("VDD").isNull());
    
    Database::Shutdown();
}

// Test edge case: Chip with no connectivity
void t_edge_chip_no_connectivity()
{
    using namespace nano;
    using namespace chip;
    Database::Create("edge_chip_no_connect");
    
    auto chip = Create<Chip>("floating");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    
    // Create nets but no connections
    auto net1 = Create<Net>("floating1", block);
    auto net2 = Create<Net>("floating2", block);
    block->AddNets(Vec<Id<Net>>{net1, net2});
    
    // Create instances but no terminals
    auto inst1 = Create<Inst>("u1", block);
    auto inst2 = Create<Inst>("u2", block);
    block->AddInsts(Vec<Id<Inst>>{inst1, inst2});
    
    // Check they exist but have no connections
    BOOST_CHECK(block->NumOfNets() == 2);
    BOOST_CHECK(block->NumOfInsts() == 2);
    BOOST_CHECK(inst1->NumOfITerms() == 0);
    BOOST_CHECK(inst2->NumOfITerms() == 0);
    
    Database::Shutdown();
}

// Test edge case: Material with all property types
void t_edge_material_all_properties()
{
    using namespace nano;
    Database::Create("edge_material_all_props");
    
    auto mat = Create<Material>("ComplexMaterial");
    
    // Add all types of properties
    auto simpleProp = Create<MaterialPropValue>(100.0);
    mat->SetProperty(Material::THERMAL_CONDUCTIVITY, simpleProp);
    
    Arr3<Float> anisoValues{100.0, 200.0, 300.0};
    auto anisoProp = Create<MaterialPropValue>(anisoValues);
    mat->SetProperty(Material::MASS_DENSITY, anisoProp);
    
    Vec<Vec<Float>> polyCoeffs{{1.0, 2.0, 3.0}};
    auto polyProp = Create<MaterialPropPolynomial>(polyCoeffs);
    mat->SetProperty(Material::SPECIFIC_HEAT, polyProp);
    
    Arr9<Float> tensorValues{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    auto tensorProp = Create<MaterialPropValue>(tensorValues);
    mat->SetProperty(Material::PERMITTIVITY, tensorProp);
    
    // Verify all properties exist
    BOOST_CHECK(mat->hasProperty(Material::THERMAL_CONDUCTIVITY));
    BOOST_CHECK(mat->hasProperty(Material::MASS_DENSITY));
    BOOST_CHECK(mat->hasProperty(Material::SPECIFIC_HEAT));
    BOOST_CHECK(mat->hasProperty(Material::PERMITTIVITY));
    
    Database::Shutdown();
}

// Test edge case: Multiple databases
void t_edge_multiple_databases()
{
    using namespace nano;
    
    // Create first database
    Database::Create("db1");
    auto mat1 = Create<Material>("Mat1");
    auto checksum1 = Database::Current().Checksum();
    Database::Shutdown();
    
    // Create second database
    Database::Create("db2");
    auto mat2 = Create<Material>("Mat2");
    auto checksum2 = Database::Current().Checksum();
    Database::Shutdown();
    
    // Checksums should be different
    BOOST_CHECK(checksum1 != checksum2);
    
    // Create third database with same content as first
    Database::Create("db1");
    auto mat3 = Create<Material>("Mat1");
    auto checksum3 = Database::Current().Checksum();
    Database::Shutdown();
    
    // Should have same checksum as first (same content)
    BOOST_CHECK(checksum1 == checksum3);
}

test_suite * create_nano_edge_cases_test_suite()
{
    test_suite * edge_cases_suite = BOOST_TEST_SUITE("s_edge_cases_test");
    
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_empty_collections));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_null_ids));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_duplicate_names));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_large_collections));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_material_boundary_values));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_lookup_single_point));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_lookup_two_points));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_shape_zero_dimensions));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_liberty_no_pins));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_chip_no_connectivity));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_material_all_properties));
    edge_cases_suite->add(BOOST_TEST_CASE(&t_edge_multiple_databases));
    
    return edge_cases_suite;
}
