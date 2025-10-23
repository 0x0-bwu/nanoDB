#pragma once
#include "TestCommon.hpp"
#include <nano/core/liberty>

using namespace boost::unit_test;

// Test liberty library creation
void t_liberty_library_creation()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_lib_create_test");
    
    auto lib = Create<Library>("test_lib");
    BOOST_CHECK(lib);
    BOOST_CHECK(lib->GetName() == "test_lib");
    BOOST_CHECK(lib->NumOfCells() == 0);
    
    // Set library attributes
    lib->delayModel = DelayModel::TABLE_LOOKUP;
    lib->timeUnit = TimeUnit(1.0, TimeUnit::Unit::NANOSECOND);
    lib->voltUnit = VoltUnit(1.0, VoltUnit::Unit::V);
    lib->currentUnit = CurrUnit(1.0, CurrUnit::Unit::MA);
    lib->capLoadUnit = CapUnit(1.0, CapUnit::Unit::PF);
    
    BOOST_CHECK(lib->delayModel == DelayModel::TABLE_LOOKUP);
    BOOST_CHECK(lib->timeUnit == TimeUnit(1.0, TimeUnit::Unit::NANOSECOND));
    
    Database::Shutdown();
}

// Test liberty cell and pin creation
void t_liberty_cell_pin_creation()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_cell_pin_test");
    
    auto lib = Create<Library>("test_lib");
    
    // Create a simple cell (NAND2)
    auto nand2 = Create<Cell>("NAND2", lib);
    lib->AddCell(nand2);
    
    BOOST_CHECK(lib->NumOfCells() == 1);
    BOOST_CHECK(nand2->GetName() == "NAND2");
    
    // Create input pins
    auto pinA = Create<InputPin>("A", nand2);
    auto pinB = Create<InputPin>("B", nand2);
    
    // Create output pin
    auto pinY = Create<OutputPin>("Y", nand2);
    
    // Add pins to cell
    nand2->AddSignalPins(Vec<Id<SignalPin>>{
        Id<SignalPin>(pinA),
        Id<SignalPin>(pinB),
        Id<SignalPin>(pinY)
    });
    
    BOOST_CHECK(nand2->NumOfSignalPins() == 3);
    
    // Find pins by name
    auto foundPinA = nand2->FindSignalPin("A");
    BOOST_CHECK(foundPinA);
    BOOST_CHECK(foundPinA->GetName() == "A");
    
    auto foundPinY = nand2->FindSignalPin("Y");
    BOOST_CHECK(foundPinY);
    BOOST_CHECK(foundPinY->GetName() == "Y");
    
    Database::Shutdown();
}

// Test liberty cell with power pins
void t_liberty_power_pins()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_power_pin_test");
    
    auto lib = Create<Library>("test_lib");
    auto cell = Create<Cell>("INV", lib);
    lib->AddCell(cell);
    
    // Create power and ground pins
    auto vdd = Create<PgPin>("VDD", cell);
    vdd->pgType = PgType::VDD;
    
    auto vss = Create<PgPin>("VSS", cell);
    vss->pgType = PgType::VSS;
    
    cell->AddPgPins(Vec<Id<PgPin>>{vdd, vss});
    
    BOOST_CHECK(cell->NumOfPgPins() == 2);
    
    // Find power pins
    auto foundVDD = cell->FindPgPin("VDD");
    BOOST_CHECK(foundVDD);
    BOOST_CHECK(foundVDD->pgType == PgType::VDD);
    
    auto foundVSS = cell->FindPgPin("VSS");
    BOOST_CHECK(foundVSS);
    BOOST_CHECK(foundVSS->pgType == PgType::VSS);
    
    Database::Shutdown();
}

// Test liberty lookup table templates
void t_liberty_lut_template()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_lut_template_test");
    
    auto lib = Create<Library>("test_lib");
    
    // Create 2D lookup table template
    auto lutTemplate = Create<LutTemplate>("delay_template_5x5");
    
    // Add index vectors (load and slew)
    Vec<Float> index1{0.01, 0.02, 0.04, 0.08, 0.16};
    Vec<Float> index2{0.1, 0.2, 0.4, 0.8, 1.6};
    
    lutTemplate->indices.push_back(index1);
    lutTemplate->indices.push_back(index2);
    
    // Add variable names
    lutTemplate->variables.push_back("input_net_transition");
    lutTemplate->variables.push_back("total_output_net_capacitance");
    
    lib->lutTemplates.push_back(lutTemplate);
    
    BOOST_CHECK(lib->lutTemplates.size() == 1);
    BOOST_CHECK(lib->lutTemplates.front()->GetName() == "delay_template_5x5");
    BOOST_CHECK(lib->lutTemplates.front()->indices.size() == 2);
    BOOST_CHECK(lib->lutTemplates.front()->indices[0].size() == 5);
    BOOST_CHECK(lib->lutTemplates.front()->variables.size() == 2);
    
    Database::Shutdown();
}

// Test liberty timing arcs
void t_liberty_timing_arcs()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_timing_arc_test");
    
    auto lib = Create<Library>("test_lib");
    auto cell = Create<Cell>("NAND2", lib);
    
    auto pinA = Create<InputPin>("A", cell);
    auto pinY = Create<OutputPin>("Y", cell);
    
    cell->AddSignalPins(Vec<Id<SignalPin>>{
        Id<SignalPin>(pinA),
        Id<SignalPin>(pinY)
    });
    
    // Create timing arc
    auto timing = Create<Timing>(pinY);
    timing->relatedPin = "A";
    timing->timingSense = TimingSense::NEGATIVE_UNATE;
    timing->timingType = TimingType::COMBINATIONAL;
    
    pinY->AddTimings(Vec<Id<Timing>>{timing});
    
    BOOST_CHECK(pinY->NumOfTimings() == 1);
    auto foundTiming = pinY->GetTiming(0);
    BOOST_CHECK(foundTiming);
    BOOST_CHECK(foundTiming->relatedPin == "A");
    BOOST_CHECK(foundTiming->timingSense == TimingSense::NEGATIVE_UNATE);
    BOOST_CHECK(foundTiming->timingType == TimingType::COMBINATIONAL);
    
    Database::Shutdown();
}

// Test liberty operating conditions
void t_liberty_operating_conditions()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_op_cond_test");
    
    auto lib = Create<Library>("test_lib");
    
    // Create operating conditions
    auto opCond = Create<OperatingConditions>("typical");
    opCond->process = 1.0;
    opCond->temperature = 25.0;
    opCond->voltage = 1.0;
    
    lib->operatingConditions.push_back(opCond);
    lib->defaultOperatingConditions = "typical";
    
    BOOST_CHECK(lib->operatingConditions.size() == 1);
    BOOST_CHECK(lib->operatingConditions.front()->GetName() == "typical");
    BOOST_CHECK_CLOSE(lib->operatingConditions.front()->voltage, 1.0, 1e-6);
    BOOST_CHECK_CLOSE(lib->operatingConditions.front()->temperature, 25.0, 1e-6);
    BOOST_CHECK(lib->defaultOperatingConditions == "typical");
    
    Database::Shutdown();
}

// Test liberty voltage mapping
void t_liberty_voltage_map()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_voltage_map_test");
    
    auto lib = Create<Library>("test_lib");
    
    // Set voltage mappings
    lib->voltageMap["VDD"] = 1.2;
    lib->voltageMap["VSS"] = 0.0;
    lib->voltageMap["VDDIO"] = 1.8;
    
    BOOST_CHECK(lib->voltageMap.size() == 3);
    BOOST_CHECK_CLOSE(lib->voltageMap.at("VDD"), 1.2, 1e-6);
    BOOST_CHECK_CLOSE(lib->voltageMap.at("VSS"), 0.0, 1e-6);
    BOOST_CHECK_CLOSE(lib->voltageMap.at("VDDIO"), 1.8, 1e-6);
    
    Database::Shutdown();
}

// Test liberty cell iteration
void t_liberty_cell_iteration()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_cell_iter_test");
    
    auto lib = Create<Library>("test_lib");
    
    // Create multiple cells
    for (int i = 0; i < 10; ++i) {
        auto cell = Create<Cell>("cell_" + std::to_string(i), lib);
        lib->AddCell(cell);
    }
    
    BOOST_CHECK(lib->NumOfCells() == 10);
    
    // Iterate through cells
    size_t count = 0;
    auto iter = lib->GetCellIter();
    while (iter.Next()) {
        BOOST_CHECK(iter.Current());
        count++;
    }
    BOOST_CHECK(count == 10);
    
    Database::Shutdown();
}

// Test liberty pin capacitance
void t_liberty_pin_capacitance()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_pin_cap_test");
    
    auto lib = Create<Library>("test_lib");
    auto cell = Create<Cell>("BUF", lib);
    
    auto pinA = Create<InputPin>("A", cell);
    auto pinY = Create<OutputPin>("Y", cell);
    
    // Set pin capacitances
    pinA->capacitance = 0.01;  // 0.01 pF
    pinY->maxCapacitance = 0.5; // 0.5 pF
    
    cell->AddSignalPins(Vec<Id<SignalPin>>{
        Id<SignalPin>(pinA),
        Id<SignalPin>(pinY)
    });
    
    BOOST_CHECK_CLOSE(pinA->capacitance, 0.01, 1e-6);
    BOOST_CHECK_CLOSE(pinY->maxCapacitance, 0.5, 1e-6);
    
    Database::Shutdown();
}

// Test liberty default values
void t_liberty_default_values()
{
    using namespace nano;
    using namespace liberty;
    Database::Create("liberty_defaults_test");
    
    auto lib = Create<Library>("test_lib");
    
    // Set default values
    lib->defaultFanoutLoad = 1.0;
    lib->defaultMaxTransition = 0.5;
    lib->defaultOutputPinCap = 0.0;
    lib->defaultCellLeakagePwr = 0.0;
    
    lib->inputThresholdPctRise = 50.0;
    lib->inputThresholdPctFall = 50.0;
    lib->outputThresholdPctRise = 50.0;
    lib->outputThresholdPctFall = 50.0;
    
    lib->slewLowerThresholdPctRise = 10.0;
    lib->slewLowerThresholdPctFall = 10.0;
    lib->slewUpperThresholdPctRise = 90.0;
    lib->slewUpperThresholdPctFall = 90.0;
    
    BOOST_CHECK_CLOSE(lib->defaultFanoutLoad, 1.0, 1e-6);
    BOOST_CHECK_CLOSE(lib->defaultMaxTransition, 0.5, 1e-6);
    BOOST_CHECK_CLOSE(lib->inputThresholdPctRise, 50.0, 1e-6);
    BOOST_CHECK_CLOSE(lib->slewUpperThresholdPctRise, 90.0, 1e-6);
    
    Database::Shutdown();
}

test_suite * create_nano_liberty_enhanced_test_suite()
{
    test_suite * liberty_enhanced_suite = BOOST_TEST_SUITE("s_liberty_enhanced_test");
    
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_library_creation));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_cell_pin_creation));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_power_pins));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_lut_template));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_timing_arcs));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_operating_conditions));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_voltage_map));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_cell_iteration));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_pin_capacitance));
    liberty_enhanced_suite->add(BOOST_TEST_CASE(&t_liberty_default_values));
    
    return liberty_enhanced_suite;
}
