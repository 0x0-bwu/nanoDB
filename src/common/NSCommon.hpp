#pragma once
#include "NSUnit.hpp"
#include <iostream>

namespace nano {

enum class IOType : int8_t
{
    UNKNOWN = 0,
    INPUT = 1,
    INOUT = 2,
    OUTPUT = 3
};

} // namespace nano