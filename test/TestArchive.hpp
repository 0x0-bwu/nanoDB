#pragma once
#define BOOST_TEST_INCLUDED
#include "generic/tools/FileSystem.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <nano/db>
using namespace nano;
using namespace boost::unit_test;

void t_save_design()
{
    Database::Create("nano");
    //liberty
    auto lib = Create<liberty::Library>("lib");
    auto and2 = Create<liberty::Cell>("and2", lib);
    auto and2a = Create<liberty::InputPin>("a", and2);
    auto and2b = Create<liberty::InputPin>("b", and2);
    auto and2o = Create<liberty::OutputPin>("o", and2);
    and2->AddSignalPins(std::vector<Id<liberty::SignalPin>>{Id<liberty::SignalPin>(and2a), Id<liberty::SignalPin>(and2b), Id<liberty::SignalPin>(and2o)});
    lib->AddCell(and2);

    auto or2 = Create<liberty::Cell>("or2", lib);
    auto or2a = Create<liberty::InputPin>("a", or2);
    auto or2b = Create<liberty::InputPin>("b", or2);
    auto or2o = Create<liberty::OutputPin>("o", or2);
    or2->AddSignalPins(std::vector<Id<liberty::SignalPin>>{Id<liberty::SignalPin>(or2a), Id<liberty::SignalPin>(or2b), Id<liberty::SignalPin>(or2o)});
    lib->AddCell(or2);

    //design
    auto chip = Create<chip::Chip>("and_or_circuit");
    auto block = Create<chip::Block>("top");
    chip->AddBlock(block);
    chip->SetTop(block);

    auto inst1 = Create<chip::Inst>("inst1", block);
    auto inst2 = Create<chip::Inst>("inst2", block);
    auto inst3 = Create<chip::Inst>("inst3", block);
    block->AddInsts(std::vector<Id<chip::Inst>>{inst1, inst2, inst3});

    auto n1 = Create<chip::Net>("n1", block);
    auto n2 = Create<chip::Net>("n2", block);
    auto n3 = Create<chip::Net>("n3", block);
    auto n4 = Create<chip::Net>("n4", block);
    auto n5 = Create<chip::Net>("n5", block);
    auto n6 = Create<chip::Net>("n6", block);
    auto n7 = Create<chip::Net>("n7", block);
    block->AddNets(std::vector<Id<chip::Net>>{n1, n2, n3, n4, n5, n6, n7});

    auto bterm1 = Create<chip::BTerm>("IN1", n1, IOType::INPUT);
    auto bterm2 = Create<chip::BTerm>("IN2", n2, IOType::INPUT);
    auto bterm3 = Create<chip::BTerm>("IN3", n3, IOType::INPUT);
    auto bterm4 = Create<chip::BTerm>("IN4", n4, IOType::INPUT);
    auto bterm5 = Create<chip::BTerm>("OUT", n5, IOType::OUTPUT);
    block->AddBTerms(std::vector<Id<chip::BTerm>>{bterm1, bterm2, bterm3, bterm4, bterm5});
    auto inst1a = Create<chip::ITerm>("a", inst1, n1, IOType::INPUT);
    inst1a->Bind(and2a);
    
    //remove
    Remove<chip::Net>(n2);
    BOOST_CHECK(block->GetNet(1).isNull());

    auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/nano.xml";
    auto saveState = Database::SaveCurrent(filename.c_str(), ArchiveFormat::XML);
    BOOST_CHECK(saveState);
    Database::Shutdown();
}

void t_load_design()
{
    using namespace nano;
    auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/nano.xml";
    auto loadState = Database::Load(filename.c_str(), ArchiveFormat::XML);
    BOOST_CHECK(loadState);
    BOOST_CHECK(Database::Current().GetName() == "nano");

    auto inst1a = Id<chip::ITerm>(0);
    BOOST_CHECK(inst1a and inst1a->GetName() == "a");
    
    auto libPin = inst1a->GetBind<liberty::InputPin>();
    BOOST_CHECK(libPin and libPin->GetName() == "a");
    Database::Shutdown();
}

test_suite * create_nano_archive_test_suite()
{
    test_suite * archive_suite = BOOST_TEST_SUITE("s_archive_test");
    //
    archive_suite->add(BOOST_TEST_CASE(&t_save_design));
    archive_suite->add(BOOST_TEST_CASE(&t_load_design));
    //
    return archive_suite;
}