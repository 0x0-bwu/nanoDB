#pragma once
#include "NSConfig.hpp"

#include "generic/geometry/Geometries.hpp"
#include "generic/math/LookupTable.hpp"
#include "generic/utils/Index.hpp"
#include "generic/tools/Log.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/core/ignore_unused.hpp>
#include <unordered_set>
#include <memory>
#include <ranges>
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

template <typename T>
using UPtr = std::unique_ptr<T>;

template <typename T>
using Ref = T &;

template <typename T>
using CRef = const T &;

using Int = int64_t;
using Index = size_t;
using ScenarioId = generic::utils::Index<class Scenario, nano::Index>;

using Float32 = float;
using Float64 = double;
using Float = Float32;

inline static constexpr auto INVALID_INDEX = std::numeric_limits<Index>::max();
inline static constexpr auto INVALID_FLOAT = std::numeric_limits<Float>::max();//std::nan(quiet_Nan) has issue with AppleClang with -ffast-math

inline bool isValid(Index i) { return i != INVALID_INDEX; }
inline bool isValid(Float f) { return f != INVALID_FLOAT; }

template <typename T1, typename T2>
using Pair = std::pair<T1, T2>;

template <typename T>
using Set = std::set<T>;

template <typename Key, typename Value>
using Map = std::map<Key, Value>;

template <typename T>
using HashSet = std::unordered_set<T>;

template <typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;

template <typename T> using Vec  = std::vector<T>;
template <typename T, size_t N> using Arr = std::array<T, N>;
template <typename T> using Arr2 = Arr<T, 2>;
template <typename T> using Arr3 = Arr<T, 3>;
template <typename T> using Arr4 = Arr<T, 4>;
template <typename T> using Arr5 = Arr<T, 5>;
template <typename T> using Arr6 = Arr<T, 6>;
template <typename T> using Arr9 = Arr<T, 9>;

template <typename T> class Id;
template <typename T> class CId;
template <typename T> using IdSet = std::unordered_set<Id<T>>;
template <typename T> using IdArr2 = Arr2<Id<T>>;
template <typename T> using IdArr3 = Arr3<Id<T>>;
template <typename T> using IdArr4 = Arr4<Id<T>>;
template <typename T> using CIdArr2 = Arr2<CId<T>>;
template <typename T> using CIdArr3 = Arr3<CId<T>>;
template <typename T> using CIdArr4 = Arr4<CId<T>>;

using NCoord = Int;
using FCoord = Float;
template <typename Scalar>
using Coord2D = generic::geometry::Point2D<Scalar>;
using NCoord2D = Coord2D<Int>;
using FCoord2D = Coord2D<Float>;
using NCoord3D = generic::geometry::Point3D<Int>;
using FCoord3D = generic::geometry::Point3D<Float>;
using NBox2D   = generic::geometry::Box2D<NCoord>;
using FBox2D   = generic::geometry::Box2D<FCoord>;
using NBox3D   = generic::geometry::Box3D<NCoord>;
using FBox3D   = generic::geometry::Box3D<FCoord>;
using NPolygon = generic::geometry::Polygon2D<NCoord>;
using FPolygon = generic::geometry::Polygon2D<FCoord>;
using NPolyline = generic::geometry::Polyline2D<NCoord>;
using FPolyline = generic::geometry::Polyline2D<FCoord>;
using NPolygonWithHoles = generic::geometry::PolygonWithHoles2D<NCoord>;
using FPolygonWithHoles = generic::geometry::PolygonWithHoles2D<FCoord>;

using Lut1D = generic::math::LookupTable<Float, 1>;
using Lut2D = generic::math::LookupTable<Float, 2>;

template <typename T> using Optional = boost::optional<T>;

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
using ArchiveFormat = generic::archive::ArchiveFormat;

template <typename T>
inline bool Save(const T & t, unsigned int version, std::string_view filename, ArchiveFormat fmt)
{
    return generic::archive::Save(t, version, filename, fmt);
}

template <typename T>
inline bool Load(T & t, unsigned int & version, std::string_view filename, ArchiveFormat fmt)
{
    return generic::archive::Load(t, version, filename, fmt);
}
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

} // namespace nano

#define NS_TRACE(args...) generic::log::Trace(args)
#define NS_DEBUG(args...) generic::log::Debug(args)
#define NS_INFO (args...) generic::log::Info (args)
#define NS_ERROR(args...) generic::log::Error(args)