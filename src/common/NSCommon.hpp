#pragma once
#include "NSUnit.hpp"
#include <iostream>

namespace nano {

enum class IOType : int8_t
{
    UNKNOWN = -1,
    INPUT = 0,
    INOUT = 1,
    OUTPUT = 2,
};

} // namespace nano