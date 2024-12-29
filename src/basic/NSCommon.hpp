#pragma once
#include "NSConfig.hpp"
#include "NSAlias.hpp"
#include "NSVersion.hpp"
#include "NSUnit.hpp"
#include <iostream>

namespace nano {

enum class ArchiveFormat { TXT = 0, XML = 1, BIN = 2};

enum class IOType : int8_t
{
    UNKNOWN = -1,
    INPUT = 0,
    INOUT = 1,
    OUTPUT = 2,
};

} // namespace nano