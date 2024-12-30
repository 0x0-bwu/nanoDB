#define NANO_UNIT_TEST
#include <boost/test/included/unit_test.hpp>
#include <boost/stacktrace.hpp>
#include "TestParasitic.hpp"
#include "TestPackage.hpp"
#include "TestLiberty.hpp"
#include "TestVerilog.hpp"
#include "TestChip.hpp"
#include "TestBasic.hpp"

using namespace boost::unit_test;
extern test_suite * create_nano_parasitic_test_suite();
extern test_suite * create_nano_package_test_suite();
extern test_suite * create_nano_verilog_test_suite();
extern test_suite * create_nano_liberty_test_suite();
extern test_suite * create_nano_chip_test_suite();
extern test_suite * create_nano_basic_test_suite();

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
#include "TestArchive.hpp"
extern test_suite * create_nano_archive_test_suite();
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

void t_additional()
{
    //add additional test here
    BOOST_CHECK(true);
}

void SignalHandler(int signum)
{
    ::signal(signum, SIG_DFL);
    std::cout << boost::stacktrace::stacktrace();
    ::raise(SIGABRT);
}

test_suite * init_unit_test_suite(int argc, char* argv[])
{
    ::signal(SIGSEGV, &SignalHandler);
    ::signal(SIGABRT, &SignalHandler);

    framework::master_test_suite().add(create_nano_basic_test_suite());
    framework::master_test_suite().add(create_nano_chip_test_suite());
    framework::master_test_suite().add(create_nano_liberty_test_suite());
    framework::master_test_suite().add(create_nano_verilog_test_suite());
    framework::master_test_suite().add(create_nano_package_test_suite());
    framework::master_test_suite().add(create_nano_parasitic_test_suite());
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    framework::master_test_suite().add(create_nano_archive_test_suite());
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
    framework::master_test_suite().add(BOOST_TEST_CASE(&t_additional));
    return 0;
}