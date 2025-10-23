#pragma once
#include "TestCommon.hpp"
#include <nano/db>
using namespace boost::unit_test;

// Test archive material serialization
void t_archive_material_serialization()
{
    using namespace nano;
    std::string checksum1, checksum2;
    
    // Save database with materials
    {
        Database::Create("material_archive_test");
        
        // Create materials with various properties
        auto copper = Create<Material>("Copper");
        copper->SetMaterialType(MaterialType::RIGID);
        
        auto thermalCond = Create<MaterialPropValue>(400.0);
        copper->SetProperty(Material::THERMAL_CONDUCTIVITY, thermalCond);
        
        auto density = Create<MaterialPropValue>(8960.0);
        copper->SetProperty(Material::MASS_DENSITY, density);
        
        // Create anisotropic material
        auto composite = Create<Material>("Composite");
        Arr3<Float> anisoThermal{100.0, 200.0, 150.0};
        auto anisoThermalProp = Create<MaterialPropValue>(anisoThermal);
        composite->SetProperty(Material::THERMAL_CONDUCTIVITY, anisoThermalProp);
        
        // Create material with polynomial property
        auto silicon = Create<Material>("Silicon");
        Vec<Vec<Float>> polyCoeffs{{148.0, -0.23, 0.00015}};
        auto polyProp = Create<MaterialPropPolynomial>(polyCoeffs);
        silicon->SetProperty(Material::THERMAL_CONDUCTIVITY, polyProp);
        
        // Create material library
        auto matLib = Create<MaterialLib>("TestMaterialLib");
        matLib->AddMaterial(copper);
        matLib->AddMaterial(composite);
        matLib->AddMaterial(silicon);
        
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/material_test.nano/database.bin";
        checksum1 = Database::Current().Checksum();
        Database::SaveCurrent(filename, ArchiveFormat::BIN);
        Database::Shutdown();
    }
    
    // Load and verify
    {
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/material_test.nano/database.bin";
        Database::Load(filename, ArchiveFormat::BIN);
        
        checksum2 = Database::Current().Checksum();
        BOOST_CHECK(checksum1 == checksum2);
        
        // Verify copper material
        auto copper = Id<Material>(0);
        BOOST_CHECK(copper && copper->GetName() == "Copper");
        BOOST_CHECK(copper->GetMaterialType() == MaterialType::RIGID);
        BOOST_CHECK(copper->hasProperty(Material::THERMAL_CONDUCTIVITY));
        BOOST_CHECK(copper->hasProperty(Material::MASS_DENSITY));
        
        Float value;
        auto thermalProp = copper->GetProperty(Material::THERMAL_CONDUCTIVITY);
        BOOST_CHECK(Id<MaterialPropValue>(thermalProp)->GetSimpleProperty(value));
        BOOST_CHECK_CLOSE(value, 400.0, 1e-6);
        
        // Verify composite (anisotropic)
        auto composite = Id<Material>(1);
        BOOST_CHECK(composite && composite->GetName() == "Composite");
        auto anisoThermal = composite->GetProperty(Material::THERMAL_CONDUCTIVITY);
        BOOST_CHECK(Id<MaterialPropValue>(anisoThermal)->GetAnisotropicProperty(0, value));
        BOOST_CHECK_CLOSE(value, 100.0, 1e-6);
        
        // Verify silicon (polynomial)
        auto silicon = Id<Material>(2);
        BOOST_CHECK(silicon && silicon->GetName() == "Silicon");
        auto polyProp = silicon->GetProperty(Material::THERMAL_CONDUCTIVITY);
        BOOST_CHECK(polyProp->isPropPolynomial());
        
        Database::Shutdown();
    }
}

// Test archive shape serialization
void t_archive_shape_serialization()
{
    using namespace nano;
    std::string checksum1, checksum2;
    
    // Save database with shapes
    {
        Database::Create("shape_archive_test");
        
        CoordUnit coordUnit(CoordUnit::Unit::MILLIMETER);
        
        // Create various shapes
        auto rect = Create<ShapeRect>(coordUnit, FCoord2D{0.0, 0.0}, FCoord2D{10.0, 5.0});
        auto circle = Create<ShapeCircle>(coordUnit, FCoord2D{5.0, 5.0}, 3.0);
        
        Vec<FCoord2D> points{{0.0, 0.0}, {10.0, 0.0}, {5.0, 8.66}};
        auto polygon = Create<ShapePolygon>(coordUnit, points);
        
        auto polygonWithHoles = Create<ShapePolygonWithHoles>();
        Vec<FCoord2D> outer{{0.0, 0.0}, {10.0, 0.0}, {10.0, 10.0}, {0.0, 10.0}};
        polygonWithHoles->SetOutline(ShapePolygon(coordUnit, outer).GetOutline());
        Vec<FCoord2D> hole{{2.0, 2.0}, {8.0, 2.0}, {8.0, 8.0}, {2.0, 8.0}};
        polygonWithHoles->AddHole(ShapePolygon(coordUnit, hole).GetOutline());
        
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/shape_test.nano/database.bin";
        checksum1 = Database::Current().Checksum();
        Database::SaveCurrent(filename, ArchiveFormat::BIN);
        Database::Shutdown();
    }
    
    // Load and verify
    {
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/shape_test.nano/database.bin";
        Database::Load(filename, ArchiveFormat::BIN);
        
        checksum2 = Database::Current().Checksum();
        BOOST_CHECK(checksum1 == checksum2);
        
        // Verify shapes
        auto rect = Id<ShapeRect>(0);
        BOOST_CHECK(rect && rect->isValid());
        BOOST_CHECK(rect->GetType() == ShapeType::RECTANGLE);
        
        auto circle = Id<ShapeCircle>(1);
        BOOST_CHECK(circle && circle->isValid());
        BOOST_CHECK(circle->GetType() == ShapeType::CIRCLE);
        
        auto polygon = Id<ShapePolygon>(2);
        BOOST_CHECK(polygon && polygon->isValid());
        BOOST_CHECK(polygon->GetType() == ShapeType::POLYGON);
        
        auto polygonWithHoles = Id<ShapePolygonWithHoles>(3);
        BOOST_CHECK(polygonWithHoles && polygonWithHoles->isValid());
        BOOST_CHECK(polygonWithHoles->hasHole());
        
        Database::Shutdown();
    }
}

// Test archive lookup table serialization
void t_archive_lookup_table_serialization()
{
    using namespace nano;
    std::string checksum1, checksum2;
    
    // Save database with lookup tables
    {
        Database::Create("lut_archive_test");
        
        // Create 1D LUT
        Vec<Float> x1d{0.0, 1.0, 2.0, 3.0};
        Vec<Float> y1d{0.0, 10.0, 20.0, 30.0};
        auto lut1d = Create<LookupTable1D>(x1d, y1d);
        
        // Create 2D LUT
        Vec<Float> x2d{0.0, 1.0, 2.0};
        Vec<Float> y2d{0.0, 10.0, 20.0};
        Vec<Float> z2d{0.0, 5.0, 10.0, 10.0, 15.0, 20.0, 20.0, 25.0, 30.0};
        auto lut2d = Create<LookupTable2D>(x2d, y2d, z2d);
        
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/lut_test.nano/database.bin";
        checksum1 = Database::Current().Checksum();
        Database::SaveCurrent(filename, ArchiveFormat::BIN);
        Database::Shutdown();
    }
    
    // Load and verify
    {
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/lut_test.nano/database.bin";
        Database::Load(filename, ArchiveFormat::BIN);
        
        checksum2 = Database::Current().Checksum();
        BOOST_CHECK(checksum1 == checksum2);
        
        // Verify 1D LUT
        auto lut1d = Id<LookupTable1D>(0);
        BOOST_CHECK(lut1d && lut1d->isValid());
        BOOST_CHECK_CLOSE(lut1d->Lookup(1.0, false), 10.0, 1e-6);
        BOOST_CHECK_CLOSE(lut1d->Lookup(1.5, false), 15.0, 1e-6);
        
        // Verify 2D LUT
        auto lut2d = Id<LookupTable2D>(1);
        BOOST_CHECK(lut2d && lut2d->isValid());
        BOOST_CHECK_CLOSE(lut2d->Lookup(1.0, 0.0, false), 10.0, 1e-6);
        
        Database::Shutdown();
    }
}

// Test archive liberty serialization
void t_archive_liberty_serialization()
{
    using namespace nano;
    using namespace liberty;
    std::string checksum1, checksum2;
    
    // Save database with liberty library
    {
        Database::Create("liberty_archive_test");
        
        // Create library
        auto lib = Create<Library>("test_lib");
        lib->delayModel = DelayModel::TABLE_LOOKUP;
        lib->timeUnit = TimeUnit(1.0, TimeUnit::Unit::NANOSECOND);
        lib->voltageMap["VDD"] = 1.0;
        lib->voltageMap["VSS"] = 0.0;
        
        // Create cell
        auto inv = Create<Cell>("INV", lib);
        auto pinA = Create<InputPin>("A", inv);
        auto pinY = Create<OutputPin>("Y", inv);
        
        pinA->capacitance = 0.01;
        pinY->maxCapacitance = 0.5;
        
        inv->AddSignalPins(Vec<Id<SignalPin>>{
            Id<SignalPin>(pinA),
            Id<SignalPin>(pinY)
        });
        
        lib->AddCell(inv);
        
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/liberty_test.nano/database.bin";
        checksum1 = Database::Current().Checksum();
        Database::SaveCurrent(filename, ArchiveFormat::BIN);
        Database::Shutdown();
    }
    
    // Load and verify
    {
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/liberty_test.nano/database.bin";
        Database::Load(filename, ArchiveFormat::BIN);
        
        checksum2 = Database::Current().Checksum();
        BOOST_CHECK(checksum1 == checksum2);
        
        // Verify library
        auto lib = Id<Library>(0);
        BOOST_CHECK(lib && lib->GetName() == "test_lib");
        BOOST_CHECK(lib->delayModel == DelayModel::TABLE_LOOKUP);
        BOOST_CHECK_CLOSE(lib->voltageMap.at("VDD"), 1.0, 1e-6);
        
        // Verify cell
        BOOST_CHECK(lib->NumOfCells() == 1);
        auto inv = lib->GetCell(0);
        BOOST_CHECK(inv && inv->GetName() == "INV");
        BOOST_CHECK(inv->NumOfSignalPins() == 2);
        
        // Verify pins
        auto pinA = inv->FindSignalPin("A");
        BOOST_CHECK(pinA);
        auto inputPin = Id<InputPin>(pinA);
        BOOST_CHECK(inputPin);
        BOOST_CHECK_CLOSE(inputPin->capacitance, 0.01, 1e-6);
        
        Database::Shutdown();
    }
}

// Test archive chip design serialization
void t_archive_chip_serialization()
{
    using namespace nano;
    using namespace chip;
    std::string checksum1, checksum2;
    
    // Save database with chip design
    {
        Database::Create("chip_archive_test");
        
        auto chip = Create<Chip>("test_chip");
        auto block = Create<Block>("top");
        chip->AddBlock(block);
        chip->SetTop(block);
        
        // Add nets
        auto net1 = Create<Net>("clk", block);
        auto net2 = Create<Net>("data", block);
        block->AddNets(Vec<Id<Net>>{net1, net2});
        
        // Add instances
        auto inst1 = Create<Inst>("u1", block);
        block->AddInsts(Vec<Id<Inst>>{inst1});
        
        // Add BTerms
        auto bterm1 = Create<BTerm>("CLK", net1, IOType::INPUT);
        block->AddBTerms(Vec<Id<BTerm>>{bterm1});
        
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/chip_test.nano/database.bin";
        checksum1 = Database::Current().Checksum();
        Database::SaveCurrent(filename, ArchiveFormat::BIN);
        Database::Shutdown();
    }
    
    // Load and verify
    {
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/chip_test.nano/database.bin";
        Database::Load(filename, ArchiveFormat::BIN);
        
        checksum2 = Database::Current().Checksum();
        BOOST_CHECK(checksum1 == checksum2);
        
        // Verify chip
        auto chip = Id<Chip>(0);
        BOOST_CHECK(chip && chip->GetName() == "test_chip");
        
        auto block = chip->GetTop();
        BOOST_CHECK(block && block->GetName() == "top");
        
        // Verify nets
        BOOST_CHECK(block->NumOfNets() == 2);
        auto net1 = block->FindNet("clk");
        BOOST_CHECK(net1 && net1->GetName() == "clk");
        
        // Verify instances
        BOOST_CHECK(block->NumOfInsts() == 1);
        auto inst1 = block->FindInst("u1");
        BOOST_CHECK(inst1 && inst1->GetName() == "u1");
        
        // Verify BTerms
        BOOST_CHECK(block->NumOfBTerms() == 1);
        auto bterm1 = block->FindBTerm("CLK");
        BOOST_CHECK(bterm1 && bterm1->GetIOType() == IOType::INPUT);
        
        Database::Shutdown();
    }
}

// Test multiple save/load cycles
void t_archive_multiple_cycles()
{
    using namespace nano;
    std::string checksum1, checksum2, checksum3;
    
    // First save
    {
        Database::Create("multi_cycle_test");
        auto mat = Create<Material>("TestMat");
        auto prop = Create<MaterialPropValue>(100.0);
        mat->SetProperty(Material::THERMAL_CONDUCTIVITY, prop);
        
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/multi_cycle.nano/database.bin";
        checksum1 = Database::Current().Checksum();
        Database::SaveCurrent(filename, ArchiveFormat::BIN);
        Database::Shutdown();
    }
    
    // Load and modify
    {
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/multi_cycle.nano/database.bin";
        Database::Load(filename, ArchiveFormat::BIN);
        
        checksum2 = Database::Current().Checksum();
        BOOST_CHECK(checksum1 == checksum2);
        
        // Add another material
        auto mat2 = Create<Material>("TestMat2");
        auto prop2 = Create<MaterialPropValue>(200.0);
        mat2->SetProperty(Material::MASS_DENSITY, prop2);
        
        checksum3 = Database::Current().Checksum();
        BOOST_CHECK(checksum1 != checksum3); // Should be different after modification
        
        Database::SaveCurrent(filename, ArchiveFormat::BIN);
        Database::Shutdown();
    }
    
    // Load again and verify both materials
    {
        auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/multi_cycle.nano/database.bin";
        Database::Load(filename, ArchiveFormat::BIN);
        
        auto mat1 = Id<Material>(0);
        BOOST_CHECK(mat1 && mat1->GetName() == "TestMat");
        
        auto mat2 = Id<Material>(1);
        BOOST_CHECK(mat2 && mat2->GetName() == "TestMat2");
        
        Database::Shutdown();
    }
}

test_suite * create_nano_archive_enhanced_test_suite()
{
    test_suite * archive_enhanced_suite = BOOST_TEST_SUITE("s_archive_enhanced_test");
    
    archive_enhanced_suite->add(BOOST_TEST_CASE(&t_archive_material_serialization));
    archive_enhanced_suite->add(BOOST_TEST_CASE(&t_archive_shape_serialization));
    archive_enhanced_suite->add(BOOST_TEST_CASE(&t_archive_lookup_table_serialization));
    archive_enhanced_suite->add(BOOST_TEST_CASE(&t_archive_liberty_serialization));
    archive_enhanced_suite->add(BOOST_TEST_CASE(&t_archive_chip_serialization));
    archive_enhanced_suite->add(BOOST_TEST_CASE(&t_archive_multiple_cycles));
    
    return archive_enhanced_suite;
}
