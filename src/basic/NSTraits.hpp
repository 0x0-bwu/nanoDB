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

// cloneable
template <typename T>
concept Cloneable = requires (T & t) { []<typename U>(nano::Cloneable<U> &){}(t); };

} // namespace nano::traits