#pragma once
#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <core/package>

#include "generic/tools/FileSystem.hpp"

using namespace nano;
using namespace boost::unit_test;

void t_create_package()
{
    using namespace nano::package;
    Database::Create("CAS300M12BM2");
    auto pkg = nano::Create<Package>("CAS300M12BM2");
    BOOST_CHECK(pkg);

    auto baseCell = nano::Create<CircuitCell>("base");
    pkg->AddCell(baseCell);
    Database::Shutdown();
}

test_suite * create_nano_package_test_suite()
{
    test_suite * package_suite = BOOST_TEST_SUITE("s_package_test");
    //
    package_suite->add(BOOST_TEST_CASE(&t_create_package));
    //
    return package_suite;
}
