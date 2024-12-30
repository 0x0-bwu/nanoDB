#pragma once
#include "basic/NSForward.hpp"
#include "common/NSCommon.hpp"
namespace nano::verilog {

chip::ChipId ReadVerilog(std::string_view filename);


} // namespace nano::verilog