#pragma once
#include "basic/NSForward.hpp"
#include "common/NSCommon.hpp"
namespace nano::verilog {

Id<chip::Chip> ReadVerilog(std::string_view filename);


} // namespace nano::verilog