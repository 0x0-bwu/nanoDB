#pragma once
#include "TestCommon.hpp"
#include <nano/core/parasitic>

using namespace boost::unit_test;

void t_read_spef_simple()
{
    using namespace nano;
    using namespace nano::parasitic;
    auto filename = generic::fs::DirName(__FILE__).string() + "/data/parasitic/simple.spef";
    Database::Create("parse_spef_test");
    auto parasitic = ReadSpef(filename.c_str());
    BOOST_CHECK(parasitic);
    Database::Shutdown();
}

void t_read_spef_aes()
{
    using namespace nano;
    using namespace nano::parasitic;
    auto filename = generic::fs::DirName(__FILE__).string() + "/data/parasitic/aes.spef";
    Database::Create("parse_spef_test");
    auto parasitic = ReadSpef(filename.c_str());
    BOOST_CHECK(parasitic);
    Database::Shutdown();
}

test_suite * create_nano_parasitic_test_suite()
{
    test_suite * parasitic_suite = BOOST_TEST_SUITE("s_parasitic_test");
    //
    parasitic_suite->add(BOOST_TEST_CASE(&t_read_spef_simple));
    parasitic_suite->add(BOOST_TEST_CASE(&t_read_spef_aes));
    //
    return parasitic_suite;
}