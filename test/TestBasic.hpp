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
    // id
    {
        auto id = Create<Material>("test1");
        static_assert(std::is_same_v<decltype(id), Id<Material>>);
        BOOST_CHECK(size_t(id) == 0);
        auto id2 = Create<Material>("test2");
        auto cid2 = CId<Material>(id2);
        static_assert(std::is_same_v<decltype(cid2), CId<Material>>);
        BOOST_CHECK(size_t(id2) == size_t(cid2));
    
    }
    // toString
    {
        auto & n1 = toString<chip::Chip>();
        auto & n2 = toString<chip::Chip>();
        BOOST_CHECK(n1 == "Chip");
        BOOST_CHECK(&n1 == &n2);
    }

    // inheritance
    {
        Id<liberty::CcsLut> ccsLut = Create<liberty::CcsLut>();
        Id<liberty::Lut> lut = ccsLut;
        BOOST_CHECK(ccsLut->Identical(lut));
        BOOST_CHECK(not Id<liberty::CcsLut>(lut).isNull());
        BOOST_CHECK(Id<liberty::NormalizedDriverWaveform>(lut).isNull());

        static_assert(std::is_same_v<decltype(ccsLut), Id<liberty::CcsLut>>);
        static_assert(std::is_same_v<decltype(*ccsLut),liberty::CcsLut&>);
        static_assert(std::is_same_v<decltype(--(*ccsLut)),liberty::Lut&>);
        BOOST_CHECK((*ccsLut)--.Dimension() == 0);
        BOOST_CHECK(&((*ccsLut)--) == dynamic_cast<liberty::Lut *>(&(*ccsLut)));
    }

    //clone
    {
        auto circle = Create<ShapeCircle>(NCoord2D(0, 0), 10);
        Id<Shape> shape = circle;
        Id<Shape> clone = nano::Clone(shape);
        BOOST_CHECK(not shape->Identical(clone));
        BOOST_CHECK(circle->GetRadius() == Id<ShapeCircle>(clone)->GetRadius());
        BOOST_CHECK(circle->GetCenter() == Id<ShapeCircle>(clone)->GetCenter());
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