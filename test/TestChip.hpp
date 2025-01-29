#pragma once
#include "TestCommon.hpp"
#include <nano/db>

using namespace nano;
using namespace boost::unit_test;

void t_create_chip_design()
{
    //     (n1)   +-----
    // IN1--------|a    \    (n5)
    //     (n2)   | (i1)o|-----------+
    // IN2--------|b    /            |       +-------
    //            +-----             +--------\a     \    (n7)
    //                                         ) (i3)o|---------------OUT
    //     (n3)   +-----             +--------/b     /
    // IN3--------|a    \    (n6)    |       +-------
    //     (n4)   | (i2)o|-----------+
    // IN4--------|b    /
    //            +-----

    using namespace chip;
    Database::Create("nano");

    //chip
    auto chip = Create<Chip>("and_or_circuit");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    chip->SetTop(block);
    BOOST_CHECK(chip->GetTop() == block);

    auto inst1 = Create<Inst>("inst1", block);
    auto inst2 = Create<Inst>("inst2", block);
    auto inst3 = Create<Inst>("inst3", block);
    block->AddInsts(Vec<Id<Inst>>{inst1, inst2, inst3});
    BOOST_CHECK(block->NumOfInsts() == 3);

    auto n1 = Create<Net>("n1", block);
    auto n2 = Create<Net>("n2", block);
    auto n3 = Create<Net>("n3", block);
    auto n4 = Create<Net>("n4", block);
    auto n5 = Create<Net>("n5", block);
    auto n6 = Create<Net>("n6", block);
    auto n7 = Create<Net>("n7", block);
    block->AddNets(Vec<Id<Net>>{n1, n2, n3, n4, n5, n6, n7});

    auto bterm1 = Create<BTerm>("IN1", n1, IOType::INPUT);
    auto bterm2 = Create<BTerm>("IN2", n2, IOType::INPUT);
    auto bterm3 = Create<BTerm>("IN3", n3, IOType::INPUT);
    auto bterm4 = Create<BTerm>("IN4", n4, IOType::INPUT);
    auto bterm5 = Create<BTerm>("OUT", n5, IOType::OUTPUT);
    block->AddBTerms(Vec<Id<BTerm>>{bterm1, bterm2, bterm3, bterm4, bterm5});
    auto inst1a = Create<ITerm>("a", inst1, n1, IOType::INPUT);
    auto inst1b = Create<ITerm>("b", inst1, n2, IOType::INPUT);
    auto inst1o = Create<ITerm>("o", inst1, n5, IOType::OUTPUT);
    Database::Shutdown();
}

test_suite * create_nano_chip_test_suite()
{
    test_suite * design_suite = BOOST_TEST_SUITE("s_chip_test");
    //
    design_suite->add(BOOST_TEST_CASE(&t_create_chip_design));
    //
    return design_suite;
}