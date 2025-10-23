#pragma once
#include "TestCommon.hpp"
#include <nano/db>

using namespace boost::unit_test;

// Test creating and manipulating nets
void t_chip_net_operations()
{
    using namespace nano;
    using namespace chip;
    Database::Create("chip_net_test");
    
    auto chip = Create<Chip>("net_test_chip");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    chip->SetTop(block);
    
    // Create nets
    auto net1 = Create<Net>("clk", block);
    auto net2 = Create<Net>("data", block);
    auto net3 = Create<Net>("reset", block);
    
    block->AddNets(Vec<Id<Net>>{net1, net2, net3});
    BOOST_CHECK(block->NumOfNets() == 3);
    
    // Find net by name
    auto foundNet = block->FindNet("clk");
    BOOST_CHECK(foundNet);
    BOOST_CHECK(foundNet->GetName() == "clk");
    
    // Test non-existent net
    auto notFound = block->FindNet("non_existent");
    BOOST_CHECK(notFound.isNull());
    
    // Get net by index
    auto netByIdx = block->GetNet(1);
    BOOST_CHECK(netByIdx);
    BOOST_CHECK(netByIdx->GetName() == "data");
    
    Database::Shutdown();
}

// Test creating instances with more details
void t_chip_inst_operations()
{
    using namespace nano;
    using namespace chip;
    Database::Create("chip_inst_test");
    
    auto chip = Create<Chip>("inst_test_chip");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    chip->SetTop(block);
    
    // Create instances
    auto inst1 = Create<Inst>("u_inv1", block);
    auto inst2 = Create<Inst>("u_inv2", block);
    auto inst3 = Create<Inst>("u_and1", block);
    
    block->AddInsts(Vec<Id<Inst>>{inst1, inst2, inst3});
    BOOST_CHECK(block->NumOfInsts() == 3);
    
    // Find instance by name
    auto foundInst = block->FindInst("u_inv1");
    BOOST_CHECK(foundInst);
    BOOST_CHECK(foundInst->GetName() == "u_inv1");
    
    // Get instance by index
    auto instByIdx = block->GetInst(2);
    BOOST_CHECK(instByIdx);
    BOOST_CHECK(instByIdx->GetName() == "u_and1");
    
    Database::Shutdown();
}

// Test boundary terminals (BTerms)
void t_chip_bterm_operations()
{
    using namespace nano;
    using namespace chip;
    Database::Create("chip_bterm_test");
    
    auto chip = Create<Chip>("bterm_test_chip");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    chip->SetTop(block);
    
    // Create nets
    auto clkNet = Create<Net>("clk", block);
    auto dataNet = Create<Net>("data", block);
    auto outNet = Create<Net>("out", block);
    block->AddNets(Vec<Id<Net>>{clkNet, dataNet, outNet});
    
    // Create boundary terminals
    auto clkBTerm = Create<BTerm>("CLK", clkNet, IOType::INPUT);
    auto dataBTerm = Create<BTerm>("DATA", dataNet, IOType::INPUT);
    auto outBTerm = Create<BTerm>("OUT", outNet, IOType::OUTPUT);
    
    block->AddBTerms(Vec<Id<BTerm>>{clkBTerm, dataBTerm, outBTerm});
    BOOST_CHECK(block->NumOfBTerms() == 3);
    
    // Check IO types
    BOOST_CHECK(clkBTerm->GetIOType() == IOType::INPUT);
    BOOST_CHECK(outBTerm->GetIOType() == IOType::OUTPUT);
    
    // Find BTermby name
    auto foundBTerm = block->FindBTerm("CLK");
    BOOST_CHECK(foundBTerm);
    BOOST_CHECK(foundBTerm->GetName() == "CLK");
    
    // Check net connectivity
    BOOST_CHECK(clkBTerm->GetNet() == clkNet);
    BOOST_CHECK(outBTerm->GetNet() == outNet);
    
    Database::Shutdown();
}

// Test instance terminals (ITerms) and connectivity
void t_chip_iterm_connectivity()
{
    using namespace nano;
    using namespace chip;
    Database::Create("chip_iterm_test");
    
    auto chip = Create<Chip>("iterm_test_chip");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    chip->SetTop(block);
    
    // Create instances
    auto inst1 = Create<Inst>("u_inv1", block);
    auto inst2 = Create<Inst>("u_inv2", block);
    block->AddInsts(Vec<Id<Inst>>{inst1, inst2});
    
    // Create nets
    auto inNet = Create<Net>("in", block);
    auto midNet = Create<Net>("mid", block);
    auto outNet = Create<Net>("out", block);
    block->AddNets(Vec<Id<Net>>{inNet, midNet, outNet});
    
    // Create iterms for first inverter
    auto inst1_in = Create<ITerm>("I", inst1, inNet, IOType::INPUT);
    auto inst1_out = Create<ITerm>("O", inst1, midNet, IOType::OUTPUT);
    inst1->AddITerms(Vec<Id<ITerm>>{inst1_in, inst1_out});
    
    // Create iterms for second inverter  
    auto inst2_in = Create<ITerm>("I", inst2, midNet, IOType::INPUT);
    auto inst2_out = Create<ITerm>("O", inst2, outNet, IOType::OUTPUT);
    inst2->AddITerms(Vec<Id<ITerm>>{inst2_in, inst2_out});
    
    // Check connectivity
    BOOST_CHECK(inst1->NumOfITerms() == 2);
    BOOST_CHECK(inst2->NumOfITerms() == 2);
    
    // Check instance terminal finds
    auto foundITerm = inst1->FindITerm("I");
    BOOST_CHECK(foundITerm);
    BOOST_CHECK(foundITerm->GetNet() == inNet);
    
    // Check output connects to next input via midNet
    BOOST_CHECK(inst1_out->GetNet() == midNet);
    BOOST_CHECK(inst2_in->GetNet() == midNet);
    
    Database::Shutdown();
}

// Test hierarchical blocks
void t_chip_hierarchical_design()
{
    using namespace nano;
    using namespace chip;
    Database::Create("chip_hierarchical_test");
    
    auto chip = Create<Chip>("hierarchical_chip");
    
    // Create top block
    auto topBlock = Create<Block>("top");
    chip->AddBlock(topBlock);
    chip->SetTop(topBlock);
    
    // Create sub-block
    auto subBlock1 = Create<Block>("sub1");
    auto subBlock2 = Create<Block>("sub2");
    chip->AddBlock(subBlock1);
    chip->AddBlock(subBlock2);
    
    // Add content to sub-blocks
    auto sub1_inst = Create<Inst>("u1", subBlock1);
    subBlock1->AddInsts(Vec<Id<Inst>>{sub1_inst});
    
    auto sub2_inst = Create<Inst>("u2", subBlock2);
    subBlock2->AddInsts(Vec<Id<Inst>>{sub2_inst});
    
    // Verify structure
    BOOST_CHECK(chip->GetTop() == topBlock);
    BOOST_CHECK(chip->NumOfBlocks() == 3);
    BOOST_CHECK(subBlock1->NumOfInsts() == 1);
    BOOST_CHECK(subBlock2->NumOfInsts() == 1);
    
    Database::Shutdown();
}

// Test net iteration
void t_chip_net_iteration()
{
    using namespace nano;
    using namespace chip;
    Database::Create("chip_net_iter_test");
    
    auto chip = Create<Chip>("iter_test_chip");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    chip->SetTop(block);
    
    // Create multiple nets
    for (int i = 0; i < 10; ++i) {
        auto net = Create<Net>("net_" + std::to_string(i), block);
        block->AddNets(Vec<Id<Net>>{net});
    }
    
    BOOST_CHECK(block->NumOfNets() == 10);
    
    // Iterate through nets
    size_t count = 0;
    auto iter = block->GetNetIter();
    while (iter.Next()) {
        BOOST_CHECK(iter.Current());
        count++;
    }
    BOOST_CHECK(count == 10);
    
    Database::Shutdown();
}

// Test instance iteration
void t_chip_inst_iteration()
{
    using namespace nano;
    using namespace chip;
    Database::Create("chip_inst_iter_test");
    
    auto chip = Create<Chip>("iter_test_chip");
    auto block = Create<Block>("top");
    chip->AddBlock(block);
    chip->SetTop(block);
    
    // Create multiple instances
    for (int i = 0; i < 15; ++i) {
        auto inst = Create<Inst>("inst_" + std::to_string(i), block);
        block->AddInsts(Vec<Id<Inst>>{inst});
    }
    
    BOOST_CHECK(block->NumOfInsts() == 15);
    
    // Iterate through instances
    size_t count = 0;
    auto iter = block->GetInstIter();
    while (iter.Next()) {
        BOOST_CHECK(iter.Current());
        count++;
    }
    BOOST_CHECK(count == 15);
    
    Database::Shutdown();
}

// Test chip design with multiple blocks
void t_chip_multi_block_design()
{
    using namespace nano;
    using namespace chip;
    Database::Create("multi_block_chip");
    
    auto chip = Create<Chip>("multi_block_design");
    
    // Create multiple blocks
    auto block1 = Create<Block>("block1");
    auto block2 = Create<Block>("block2");
    auto block3 = Create<Block>("block3");
    
    chip->AddBlock(block1);
    chip->AddBlock(block2);
    chip->AddBlock(block3);
    chip->SetTop(block1);
    
    BOOST_CHECK(chip->NumOfBlocks() == 3);
    BOOST_CHECK(chip->GetTop() == block1);
    
    // Find blocks by name
    auto found = chip->FindBlock("block2");
    BOOST_CHECK(found);
    BOOST_CHECK(found->GetName() == "block2");
    
    Database::Shutdown();
}

test_suite * create_nano_chip_enhanced_test_suite()
{
    test_suite * chip_enhanced_suite = BOOST_TEST_SUITE("s_chip_enhanced_test");
    
    chip_enhanced_suite->add(BOOST_TEST_CASE(&t_chip_net_operations));
    chip_enhanced_suite->add(BOOST_TEST_CASE(&t_chip_inst_operations));
    chip_enhanced_suite->add(BOOST_TEST_CASE(&t_chip_bterm_operations));
    chip_enhanced_suite->add(BOOST_TEST_CASE(&t_chip_iterm_connectivity));
    chip_enhanced_suite->add(BOOST_TEST_CASE(&t_chip_hierarchical_design));
    chip_enhanced_suite->add(BOOST_TEST_CASE(&t_chip_net_iteration));
    chip_enhanced_suite->add(BOOST_TEST_CASE(&t_chip_inst_iteration));
    chip_enhanced_suite->add(BOOST_TEST_CASE(&t_chip_multi_block_design));
    
    return chip_enhanced_suite;
}
