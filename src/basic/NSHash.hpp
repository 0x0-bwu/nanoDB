#pragma once
#include "generic/tools/Hash.hpp"
#include "NSForward.hpp"
#include "NSTraits.hpp"

namespace nano {

template <typename T> std::size_t Hash(const T & t) { return std::hash<T>()(t); }

} // namespace nano

namespace std {
template <typename T>
struct hash<::nano::Id<T>>
{
    std::size_t operator() (const ::nano::Id<T> & id) const noexcept
    {
        return std::hash<::nano::IdType>()(::nano::IdType(id));
    }
};

template <typename T>
struct hash<::nano::CId<T>>
{
    std::size_t operator() (const ::nano::CId<T> & id) const noexcept
    {
        return std::hash<::nano::IdType>()(::nano::IdType(id));
    }
};

template <typename T>
struct hash<::nano::Vec<T>>
{
    std::size_t operator() (const ::nano::Vec<T> & t) const
    {
        return generic::hash::OrderedHash(t);
    }
};

template <typename Key, typename Value>
struct hash<::nano::Map<Key, Value>>
{
    std::size_t operator() (const ::nano::Map<Key, Value> & t) const
    {
        std::size_t seed = 0;
        for (const auto & [key, val] : t)
            generic::hash::HashCombine(seed, ::nano::Hash(key), ::nano::Hash(val));
        return seed;
    }
};

template <typename Key, typename Value>
struct hash<::nano::HashMap<Key, Value>>
{
    std::size_t operator() (const ::nano::HashMap<Key, Value> & t) const
    {
        std::size_t seed = 0;
        for (const auto & [key, val] : t)
            seed ^= generic::hash::HashCombine(0, ::nano::Hash(key), ::nano::Hash(val));
        return seed;
    }
};

template <typename Scalar, std::size_t DIM>
struct hash<generic::math::LookupTable<Scalar, DIM>>
{
    std::size_t operator() (const generic::math::LookupTable<Scalar, DIM> & lut) const
    {
        if (not lut.isValid()) return 0;
        size_t seed{0};
        for (size_t i = 0; i < DIM; ++i)
            for (size_t j = 0; j < lut[i].size(); ++j)
                generic::hash::HashCombine(seed, lut[i][j]);  
        for (auto & v : *lut) generic::hash::HashCombine(seed, v);
        return seed;
    }
};

template <::nano::traits::HanaStruct T>
struct hash<T>
{
    std::size_t operator() (const T & t) const
    {
        size_t seed = 0;
        boost::hana::for_each(boost::hana::keys(t), [&](auto key) {
            // if constexpr (::nano::traits::Hashable<std::decay_t<decltype(boost::hana::at_key(t, key))>>)
                seed = generic::hash::HashCombine(seed, ::nano::Hash(boost::hana::at_key(t, key)));
        }); 
        return seed;
    }
};

} // namespace std