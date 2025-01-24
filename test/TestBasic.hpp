#pragma once
#include "TestCommon.hpp"
#include <nano/db>
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
        id2.Destroy();
        BOOST_CHECK(not id2);
        BOOST_CHECK(cid2.operator->() == nullptr);
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

    // clone
    {
        auto circle = Create<ShapeCircle>(NCoord2D(0, 0), 10);
        Id<Shape> shape = circle;
        Id<Shape> clone = shape->Clone();
        BOOST_CHECK(not shape->Identical(clone));
        BOOST_CHECK(circle->GetRadius() == Id<ShapeCircle>(clone)->GetRadius());
        BOOST_CHECK(circle->GetCenter() == Id<ShapeCircle>(clone)->GetCenter());
    }

    // iterator
    {
        IdVec<package::Package> pkgs;
        pkgs.emplace_back(nano::Create<package::Package>("pkg1"));
        pkgs.emplace_back(nano::Create<package::Package>("pkg2"));
        pkgs.emplace_back(nano::Create<package::Package>("pkg3"));

        auto iter = pkgs.GetIter();
        BOOST_CHECK(iter.Current() && iter->GetName() == "pkg1");
        static_assert(std::is_same_v<decltype(iter.Current()), Id<package::Package>>);
        size_t count = 0;
        while(iter.Next()) { count++; }
        BOOST_CHECK(count == 3);
        BOOST_CHECK(not (*iter));
        auto citer = pkgs.GetCIter();
        static_assert(std::is_same_v<decltype(citer.Current()), CId<package::Package>>);
        count = 0;
        while(citer.Next()) { count++; }
        BOOST_CHECK(count == 3);
        BOOST_CHECK(not (*citer));

        IdVec<package::Shape> shapes;
        shapes.emplace_back(nano::Create<package::ShapeCircle>(NCoord2D(0, 0), 10));
        shapes.emplace_back(nano::Create<package::ShapePolygon>());
        shapes.emplace_back(nano::Create<package::ShapeCircle>(NCoord2D(0, 0), 20));
        auto iter2 = shapes.GetIter<ShapeCircle>();
        count = 0;
        while(iter2.Next()) { count++; }
        BOOST_CHECK(count == 2);
        BOOST_CHECK(not (*iter2));

        auto iter3 = shapes.GetCIter<ShapePolygon>();
        BOOST_CHECK(iter3.Current());
        BOOST_CHECK(iter3.Next());
        BOOST_CHECK(not iter3.Current());
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