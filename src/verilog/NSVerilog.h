#pragma once
#include <core/basic>
#include <core/common>
namespace nano::verilog {

Id<chip::Chip> ReadVerilog(std::string_view filename);


} // namespace nano::verilog