#pragma once
#include "NSForward.hpp"

namespace nano::traits {

// inheritance
template <typename T, typename = hana::when<true>>
struct Base { using type = std::decay_t<T>; };

template <typename T>
struct Base<T, hana::when<hana::contains(inheritanceMap, hana::type_c<T>)>>
{
    using type = typename std::decay_t<decltype(inheritanceMap[hana::type_c<T>])>::type;
};

template <typename T>
using BaseOf = typename Base<T>::type;

template <typename T>
constexpr bool Cloneable = std::is_base_of_v<nano::Cloneable<BaseOf<T>>, T>;

} // namespace nano::traits