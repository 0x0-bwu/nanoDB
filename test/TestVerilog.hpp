#pragma once
#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include "verilog/NSVerilog.h"

using namespace boost::unit_test;

void t_read_verilog()
{
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