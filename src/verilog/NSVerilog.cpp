#include "NSVerilog.h"
#include "parser/NSVerilogParser.hpp"
#include <core/chip>
namespace nano::verilog {

using namespace ast;
using namespace parser;
using namespace grammar;

chip::BlockId BuildBlock(const Netlist & netlist)
{
    auto block = nano::Create<chip::Block>(netlist.name);


    return block;
}

chip::ChipId ReadVerilog(std::string_view filename)
{
    VerilogDescription ast;
    VerilogParser verilogParser(ast);
    auto res = verilogParser(filename);
    if (not res) {
        NS_ERROR(verilogParser.error);
        return chip::ChipId();
    }
    auto chip = nano::Create<chip::Chip>("nano");
    return chip;
}

} // nano::verilog