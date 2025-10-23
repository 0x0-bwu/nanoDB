#pragma once
#include "TestCommon.hpp"
#include <nano/db>

using namespace boost::unit_test;

// Test LossPower creation and basic operations
void t_loss_power_basic()
{
    using namespace nano;
    using namespace power;
    Database::Create("loss_power_basic_test");
    
    // Create a 1D lookup table for temperature vs power
    Vec<Float> temps{TempUnit(25).inKelvins(), TempUnit(50).inKelvins(), TempUnit(75).inKelvins()};
    Vec<Float> powers{10.0, 15.0, 20.0}; // Watts
    auto lut = Create<LookupTable1D>(temps, powers);
    
    // Create loss power
    auto lossPower = Create<LossPower>("TestLossPower", ScenarioId(0), lut);
    
    BOOST_CHECK(lossPower);
    BOOST_CHECK(lossPower->GetName() == "TestLossPower");
    BOOST_CHECK(lossPower->isValid());
    BOOST_CHECK(lossPower->GetScenario() == ScenarioId(0));
    
    // Check lookup table
    auto retrievedLut = lossPower->GetLookupTable();
    BOOST_CHECK(retrievedLut);
    
    Database::Shutdown();
}

// Test LossPower with 1D lookup
void t_loss_power_lookup_1d()
{
    using namespace nano;
    using namespace power;
    Database::Create("loss_power_lookup_1d_test");
    
    // Create a 1D lookup table
    Vec<Float> index{0.0, 25.0, 50.0, 75.0, 100.0};
    Vec<Float> power{5.0, 10.0, 20.0, 35.0, 50.0};
    auto lut = Create<LookupTable1D>(index, power);
    
    auto lossPower = Create<LossPower>("TempPower", ScenarioId(1), lut);
    
    // Test exact lookups
    BOOST_CHECK_CLOSE(lossPower->GetLossPower(0.0), 5.0, 1e-6);
    BOOST_CHECK_CLOSE(lossPower->GetLossPower(25.0), 10.0, 1e-6);
    BOOST_CHECK_CLOSE(lossPower->GetLossPower(100.0), 50.0, 1e-6);
    
    // Test interpolated lookup
    BOOST_CHECK_CLOSE(lossPower->GetLossPower(12.5), 7.5, 1e-6);
    BOOST_CHECK_CLOSE(lossPower->GetLossPower(37.5), 15.0, 1e-6);
    
    Database::Shutdown();
}

// Test multiple LossPower objects with different scenarios
void t_loss_power_multiple_scenarios()
{
    using namespace nano;
    using namespace power;
    Database::Create("loss_power_scenarios_test");
    
    // Scenario 0: Low power mode
    Vec<Float> temps1{TempUnit(25).inKelvins(), TempUnit(75).inKelvins()};
    Vec<Float> powers1{5.0, 8.0};
    auto lut1 = Create<LookupTable1D>(temps1, powers1);
    auto lowPower = Create<LossPower>("LowPowerMode", ScenarioId(0), lut1);
    
    // Scenario 1: Normal power mode
    Vec<Float> temps2{TempUnit(25).inKelvins(), TempUnit(75).inKelvins()};
    Vec<Float> powers2{15.0, 25.0};
    auto lut2 = Create<LookupTable1D>(temps2, powers2);
    auto normalPower = Create<LossPower>("NormalPowerMode", ScenarioId(1), lut2);
    
    // Scenario 2: High power mode
    Vec<Float> temps3{TempUnit(25).inKelvins(), TempUnit(75).inKelvins()};
    Vec<Float> powers3{30.0, 50.0};
    auto lut3 = Create<LookupTable1D>(temps3, powers3);
    auto highPower = Create<LossPower>("HighPowerMode", ScenarioId(2), lut3);
    
    // Verify scenarios
    BOOST_CHECK(lowPower->GetScenario() == ScenarioId(0));
    BOOST_CHECK(normalPower->GetScenario() == ScenarioId(1));
    BOOST_CHECK(highPower->GetScenario() == ScenarioId(2));
    
    // Verify different power levels at same temperature
    Float temp = TempUnit(25).inKelvins();
    BOOST_CHECK_CLOSE(lowPower->GetLossPower(temp), 5.0, 1e-6);
    BOOST_CHECK_CLOSE(normalPower->GetLossPower(temp), 15.0, 1e-6);
    BOOST_CHECK_CLOSE(highPower->GetLossPower(temp), 30.0, 1e-6);
    
    Database::Shutdown();
}

// Test LossPower with 2D lookup table
void t_loss_power_lookup_2d()
{
    using namespace nano;
    using namespace power;
    Database::Create("loss_power_lookup_2d_test");
    
    // Create a 2D lookup table (temperature vs voltage)
    Vec<Float> temps{TempUnit(25).inKelvins(), TempUnit(75).inKelvins()};
    Vec<Float> voltages{0.9, 1.0, 1.1};
    Vec<Float> powers{
        8.0, 10.0, 12.0,   // at 25°C
        14.0, 18.0, 22.0   // at 75°C
    };
    auto lut2d = Create<LookupTable2D>(temps, voltages, powers);
    
    auto lossPower = Create<LossPower>("TempVoltagePower", ScenarioId(0), lut2d);
    
    BOOST_CHECK(lossPower->isValid());
    
    // Note: GetLossPower only takes one parameter (x), 
    // so we need to access the lookup table directly for 2D
    auto lut = lossPower->GetLookupTable();
    BOOST_CHECK(lut);
    
    // Test 2D lookups through the LUT
    BOOST_CHECK_CLOSE(lut->Lookup(TempUnit(25).inKelvins(), 0.9, false), 8.0, 1e-6);
    BOOST_CHECK_CLOSE(lut->Lookup(TempUnit(25).inKelvins(), 1.0, false), 10.0, 1e-6);
    BOOST_CHECK_CLOSE(lut->Lookup(TempUnit(75).inKelvins(), 1.1, false), 22.0, 1e-6);
    
    Database::Shutdown();
}

// Test LossPower collection management
void t_loss_power_collection()
{
    using namespace nano;
    using namespace power;
    Database::Create("loss_power_collection_test");
    
    // Create multiple loss power objects
    IdVec<LossPower, NameLut> lossPowers;
    
    for (int i = 0; i < 5; ++i) {
        Vec<Float> x{0.0, 100.0};
        Vec<Float> y{static_cast<Float>(i * 10), static_cast<Float>(i * 20)};
        auto lut = Create<LookupTable1D>(x, y);
        auto lp = Create<LossPower>("Component_" + std::to_string(i), ScenarioId(i), lut);
        lossPowers.emplace_back(lp);
    }
    
    BOOST_CHECK(lossPowers.size() == 5);
    
    // Test lookup by name
    auto found = lossPowers.Lookup<lut::Name>("Component_2");
    BOOST_CHECK(found);
    BOOST_CHECK(found->GetName() == "Component_2");
    BOOST_CHECK(found->GetScenario() == ScenarioId(2));
    
    // Test iteration
    size_t count = 0;
    auto iter = lossPowers.GetIter();
    while (iter.Next()) {
        BOOST_CHECK(iter.Current());
        count++;
    }
    BOOST_CHECK(count == 5);
    
    Database::Shutdown();
}

// Test LossPower validity checking
void t_loss_power_validity()
{
    using namespace nano;
    using namespace power;
    Database::Create("loss_power_validity_test");
    
    // Valid loss power
    Vec<Float> x{0.0, 100.0};
    Vec<Float> y{10.0, 20.0};
    auto validLut = Create<LookupTable1D>(x, y);
    auto validLossPower = Create<LossPower>("Valid", ScenarioId(0), validLut);
    
    BOOST_CHECK(validLut->isValid());
    BOOST_CHECK(validLossPower->isValid());
    
    // Empty lookup table
    Vec<Float> emptyX;
    Vec<Float> emptyY;
    auto emptyLut = Create<LookupTable1D>(emptyX, emptyY);
    auto invalidLossPower = Create<LossPower>("Invalid", ScenarioId(0), emptyLut);
    
    BOOST_CHECK(!emptyLut->isValid());
    BOOST_CHECK(!invalidLossPower->isValid());
    
    Database::Shutdown();
}

test_suite * create_nano_power_test_suite()
{
    test_suite * power_suite = BOOST_TEST_SUITE("s_power_test");
    
    power_suite->add(BOOST_TEST_CASE(&t_loss_power_basic));
    power_suite->add(BOOST_TEST_CASE(&t_loss_power_lookup_1d));
    power_suite->add(BOOST_TEST_CASE(&t_loss_power_multiple_scenarios));
    power_suite->add(BOOST_TEST_CASE(&t_loss_power_lookup_2d));
    power_suite->add(BOOST_TEST_CASE(&t_loss_power_collection));
    power_suite->add(BOOST_TEST_CASE(&t_loss_power_validity));
    
    return power_suite;
}
