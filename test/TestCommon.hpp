#pragma once
#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/hana.hpp>

#include "generic/tools/FileSystem.hpp"
#include "generic/math/MathUtility.hpp"

namespace nano::test {
namespace hana = boost::hana;
auto variables = hana::make_map(
    hana::make_pair(BOOST_HANA_STRING("package_checksum"), std::string{})
);

}; // namespace nano::test