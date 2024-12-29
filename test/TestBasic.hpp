#pragma once
#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <nano>
using namespace nano;
using namespace boost::unit_test;

void t_basic()
{
    Database::Create("basic_test");
    // toString
    {
        auto & n1 = toString<chip::Chip>();
        auto & n2 = toString<chip::Chip>();
        BOOST_CHECK(n1 == "Chip");
        BOOST_CHECK(&n1 == &n2);
    }

    // inheritance
    {
        auto ccsLut = Create<liberty::CcsLut>();
        BOOST_CHECK(ccsLut->Identical(Id<liberty::Lut>(ccsLut)));
        static_assert(std::is_same_v<decltype(ccsLut), Id<liberty::CcsLut>>);
        static_assert(std::is_same_v<decltype(*ccsLut),liberty::CcsLut&>);
        static_assert(std::is_same_v<decltype(--(*ccsLut)),liberty::Lut&>);
        BOOST_CHECK((*ccsLut)--.Dimension() == 0);
        BOOST_CHECK(&((*ccsLut)--) == dynamic_cast<liberty::Lut *>(&(*ccsLut)));

    }
    Database::Shutdown();
}

test_suite * create_nano_basic_test_suite()
{
    test_suite * basic_suite = BOOST_TEST_SUITE("s_basic_test");
    //
    basic_suite->add(BOOST_TEST_CASE(&t_basic));
    //
    return basic_suite;
}