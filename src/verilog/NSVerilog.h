#pragma once
#include <nano/common>
namespace nano::verilog {

Id<chip::Chip> ReadVerilog(std::string_view filename);


} // namespace nano::verilog