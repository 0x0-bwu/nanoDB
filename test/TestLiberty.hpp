#pragma once
#include "TestCommon.hpp"
#include <nano/core/liberty>

using namespace boost::unit_test;

void liberty_member_check(const nano::liberty::Library & lib)
{
    using namespace nano;
    using namespace liberty;
    constexpr Float t = 1e-3;
    BOOST_CHECK(lib->comment == "");
    BOOST_CHECK(lib->revision == "1.0");
    BOOST_CHECK(lib->delayModel == DelayModel::TABLE_LOOKUP);
    BOOST_CHECK(lib->capLoadUnit == CapUnit(1, CapUnit::Unit::FF));
    BOOST_CHECK(lib->currentUnit == CurrUnit(1, CurrUnit::Unit::MA));
    BOOST_CHECK(lib->leakagePwrUnit == PwrUnit(1, PwrUnit::Unit::PW));
    BOOST_CHECK(lib->pullResUnit == ResUnit(1e3, ResUnit::Unit::OHM));
    BOOST_CHECK(lib->timeUnit == TimeUnit(1e-3, TimeUnit::Unit::NANOSECOND));
    BOOST_CHECK(lib->voltUnit == VoltUnit(1, VoltUnit::Unit::V));
    BOOST_CHECK(lib->voltageMap.size() == 3);
    BOOST_CHECK_CLOSE(lib->voltageMap.at("VDD"),0.7, t);
    BOOST_CHECK_CLOSE(lib->voltageMap.at("GND"), 0, t);
    BOOST_CHECK_CLOSE(lib->defaultCellLeakagePwr,0, t);
    BOOST_CHECK_CLOSE(lib->defaultFanoutLoad, 1, t);
    BOOST_CHECK_CLOSE(lib->defaultMaxTransition, 320, t);
    BOOST_CHECK_CLOSE(lib->defaultOutputPinCap, 0, t);
    BOOST_CHECK(lib->defaultInPlaceSwapMode == InPlaceSwapMode::MATCH_FOOTPRINT);
    BOOST_CHECK_CLOSE(lib->inputThresholdPctFall, 50, t);
    BOOST_CHECK_CLOSE(lib->inputThresholdPctRise, 50, t);
    BOOST_CHECK(lib->nomProcess == 1);
    BOOST_CHECK_CLOSE(lib->nomTemperature, 25, t);
    BOOST_CHECK_CLOSE(lib->nomVoltage, 0.7, t);
    BOOST_CHECK_CLOSE(lib->outputThresholdPctFall, 50, t);
    BOOST_CHECK_CLOSE(lib->outputThresholdPctRise, 50, t);
    BOOST_CHECK_CLOSE(lib->slewLowerThresholdPctFall, 10, t);
    BOOST_CHECK_CLOSE(lib->slewLowerThresholdPctRise, 10, t);
    BOOST_CHECK_CLOSE(lib->slewUpperThresholdPctFall, 90, t);
    BOOST_CHECK_CLOSE(lib->slewUpperThresholdPctRise, 90, t);
    BOOST_CHECK(lib->operatingConditions.size() == 1); 
    {
        const auto & oc = *lib->operatingConditions.front();
        BOOST_CHECK(oc->process == 1);
        BOOST_CHECK_CLOSE(oc->temperature, 25, t);
        BOOST_CHECK_CLOSE(oc->voltage, 0.7, t);
    }
    BOOST_CHECK(lib->defaultOperatingConditions == "PVT_0P7V_25C");
    BOOST_CHECK(lib->outputCurrentTemplate.size() == 1);
    {
        const auto & lut = *lib->outputCurrentTemplate.front();
        BOOST_CHECK(lut->indices.size() == 0);
        BOOST_CHECK(lut->variables.size() == 3);
        BOOST_CHECK(lut->variables[1] == "total_output_net_capacitance");
    }
    BOOST_CHECK(lib->lutTemplates.size() == 5);
    {
        const auto & lut = *lib->lutTemplates.back();
        BOOST_CHECK(lut->indices.size() == 2);
        BOOST_CHECK(lut->indices.back().back() == 16);
    }
    BOOST_CHECK(lib->inputVoltage.size() == 1);
    {
        const auto & lut = *lib->inputVoltage.front();
        BOOST_CHECK(lut->low == 0);
        BOOST_CHECK_CLOSE(lut->high, 0.7, t);
    }
    BOOST_CHECK(lib->outputVoltage.size() == 1);
}

void t_parse_liberty()
{
    using namespace nano;
    using namespace liberty;
    auto filename = generic::fs::DirName(__FILE__).string() + "/data/liberty/asap7sc7p5t_INVBUF_RVT_TT_ccs_201020.lib";
    Database::Create("parse_liberty_test");
    Id<Library> lib = nano::liberty::LoadLibrary(filename.c_str());
    BOOST_CHECK(lib);
    BOOST_CHECK(lib->GetName() == "asap7sc7p5t_INVBUF_RVT_TT_ccs_201020");
    BOOST_CHECK(lib->NumOfCells() == 37);
    BOOST_CHECK(lib->GetFilename() == filename);

    liberty_member_check(*lib);

    Database::Shutdown();
}

test_suite * create_nano_liberty_test_suite()
{
    test_suite * liberty_suite = BOOST_TEST_SUITE("s_liberty_test");
    //
    liberty_suite->add(BOOST_TEST_CASE(&t_parse_liberty));
    //
    return liberty_suite;
}