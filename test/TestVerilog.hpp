#pragma once
#include "TestCommon.hpp"
#include <nano/core/verilog>

using namespace boost::unit_test;

void t_read_verilog()
{
    using namespace nano;
    using namespace verilog;
    auto filename = generic::fs::DirName(__FILE__).string() + "/data/verilog/test.v";
    Database::Create("read_verilog_test");
    auto chip = ReadVerilog(filename.c_str());
    BOOST_CHECK(chip);
    Database::Shutdown();
}

test_suite * create_nano_verilog_test_suite()
{
    test_suite * verilog_suite = BOOST_TEST_SUITE("s_verilog_test");
    //
    verilog_suite->add(BOOST_TEST_CASE(&t_read_verilog));
    //
    return verilog_suite;
}