#pragma once
#include "NSConfig.hpp"

#include "generic/common/Exception.hpp"
#include "generic/geometry/Point.hpp"
#include "generic/tools/Log.hpp"

#include <boost/core/ignore_unused.hpp>
#include <unordered_set>
#include <memory>
#include <vector>
#include <array>

#define NS_UNUSED(ex) boost::ignore_unused(ex);

#define NS_ASSERT(ex) GENERIC_ASSERT(ex);
#define NS_ASSERT_MSG(ex, msg) GENERIC_ASSERT_MSG(ex, msg);
#ifdef ENABLE_ASSERTION
    #define NS_CHECK_TRUE(ex) NS_ASSERT(ex)
#else
    #define NS_CHECK_TRUE(ex) !!(ex)
#endif
namespace nano {

template <typename T>
using Ptr = T*;

template <typename T>
using CPtr = const T*;

template <typename T>
using SPtr = std::shared_ptr<T>;

template <typename T, typename Deleter = std::default_delete<T> >
using UPtr = std::unique_ptr<T, Deleter>;

using IdType = size_t;

using Int = int64_t;
using Float = float;

template <typename Key, typename Value>
using Map = std::unordered_map<Key, Value>;

template <typename T> using Arr2 = std::array<T, 2>;
template <typename T> using Arr3 = std::array<T, 3>;
template <typename T> using Arr4 = std::array<T, 4>;
template <typename T> using Arr9 = std::array<T, 9>;

template <typename T> class Id;
template <typename T> class CId;
template <typename T> using IdSet = std::unordered_set<Id<T>>;
template <typename T> using IdArr2 = Arr2<Id<T>>;
template <typename T> using IdArr3 = Arr3<Id<T>>;
template <typename T> using IdArr4 = Arr4<Id<T>>;

using NCoord = Int;
using FCoord = Float;
using NCoord2D = generic::geometry::Point2D<Int>;
using FCoord2D = generic::geometry::Point2D<Float>;

template <typename T> using Optional = boost::optional<T>;

} // namespace nano

#define NS_TRACE(args...) generic::log::Trace(args)
#define NS_DEBUG(args...) generic::log::Debug(args)
#define NS_INFO (args...) generic::log::Info (args)
#define NS_ERROR(args...) generic::log::Error(args)