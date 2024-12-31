#pragma once
#include "basic/NSDatabase.h"
#include "basic/NSContainer.hpp"
#include "common/NSCommon.hpp"
namespace nano::liberty {

inline static constexpr IdType FALL = 0;
inline static constexpr IdType RISE = 1; 

enum class DelayModel : int8_t
{
    TABLE_LOOKUP = 0,
};

enum class InPlaceSwapMode : int8_t
{
    MATCH_FOOTPRINT = 0,
};

enum class PGType : int8_t
{
    PRIMARY_POWER,
    PRIMARY_GROUND,
    BACKUP_POWER,
    BACKUP_GROUND,
    INTERNAL_POWER,
    INTERNAL_GROUND,
    PWELL,
    NWELL,
    DEEPNWELL,
    DEEPPWELL,
};

enum class TimingSense : int8_t
{
    NON_UNATE = -1,
    NEGATIVE_UNATE = 0,
    POSITIVE_UNATE = 1,
};

enum class TimingType : int8_t
{
    // combinational
    COMBINATIONAL,
    COMBINATIONAL_RISE,
    COMBINATIONAL_FALL,
    THREE_STATE_DISABLE,
    THREE_STATE_ENABLE,
    THREE_STATE_DISABLE_RISE,
    THREE_STATE_DISABLE_FALL,
    THREE_STATE_ENABLE_RISE,
    THREE_STATE_ENABLE_FALL,
    //todo, other timing arcs
};

using VoltageMap = std::unordered_map<std::string, Float>;

Id<Library> LoadLibrary(std::string_view filename);

} // namespace nano::liberty