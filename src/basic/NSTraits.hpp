#pragma once
#include "NSForward.hpp"
namespace nano::traits {

// std extension
template<typename>
struct is_std_array : std::false_type {};

template<typename T, std::size_t N>
struct is_std_array<std::array<T,N>> : std::true_type {};

// 
template <typename T>
inline static constexpr bool IsArr = std::is_array_v<T> or is_std_array<T>::value;

// member init
template <typename Struct>
constexpr inline void Init(Struct & s)
{
    using namespace boost;
    auto init = [](auto && self, auto & t) {
        using T = std::decay_t<decltype(t)>;
        if constexpr (hana::Struct<T>::value) Init(t);
        else if constexpr (IsArr<T>) {
            for (size_t i = 0; i < std::size(t); ++i) self(self, t[i]);
        }
        else if constexpr (std::is_arithmetic_v<T>) t = 0;
        else if constexpr (std::is_pointer_v<T>) t = nullptr;
    };
    hana::for_each(hana::accessors<Struct>(), [&](auto pair) { init(init, hana::second(pair)(s)); });
}

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
concept Cloneable = requires (T & t) { []<typename U>(nano::Cloneable<U> &){}(t); };

template <typename T>
concept Nameable = requires (const T & t) { { t.GetName() } -> std::same_as<std::string_view>; }; 

template<typename T>
concept Hashable = requires(const T & t) { { std::hash<T>{}(t) } -> std::convertible_to<std::size_t>; };

template <typename T>
concept HanaStruct = hana::Struct<T>::value;

} // namespace nano::traits

#define NS_INHERITANCE(CLASS, FROM, BASE)                                                      \
static_assert(std::is_base_of_v<BASE, FROM >, "should be derived class or self");              \
static_assert(std::is_base_of_v<FROM, CLASS>, "should be derived class or self");              \
inline FROM & operator-- (CLASS & c) { return static_cast<FROM &>(c); }                        \
inline FROM & operator-- (CLASS & c, int) { return static_cast<FROM &>(c); }                   \
inline const FROM & operator-- (const CLASS & c) { return static_cast<const FROM &>(c); }      \
inline const FROM & operator-- (const CLASS & c, int) { return static_cast<const FROM &>(c); } \
inline nano::Id<FROM> operator-- (nano::Id<CLASS> id) { return nano::Id<FROM>(id); }           \
inline nano::Id<FROM> operator-- (nano::Id<CLASS> id, int) { return nano::Id<FROM>(id); }      \
/**/
#define NS_INHERIT_FROM_BASE(CLASS, BASE) NS_INHERITANCE(CLASS, BASE, BASE)
               
#define NS_CLASS_MEMBERS_DEFINE(...)                                                           \
protected:                                                                                     \
    struct NANO_MEMBERS {                                                                      \
    BOOST_HANA_DEFINE_STRUCT(NANO_MEMBERS, __VA_ARGS__);} m_;                                  \
    NANO_MEMBERS * operator-> () noexcept { return &m_; }                                      \
public:                                                                                        \
    const NANO_MEMBERS * operator->() const noexcept { return &m_; }                           \
    const NANO_MEMBERS & operator* () const noexcept { return  m_; }                           \
private:                                                                                       \
/**/

#define NS_INIT_HANA_STRUCT(OBJECT) nano::traits::Init(OBJECT);
#define NS_CLASS_MEMBERS_INITIALIZE NS_INIT_HANA_STRUCT(m_)

#define NS_CLONE_FUNCTIONS_DECLARATION(CLASS)                                                  \
protected:                                                                                     \
    CLASS * CloneFrom(const CLASS &);                                                          \
    CLASS * CloneImpl(Index id) const override                                                 \
    {                                                                                          \
        auto clone = new CLASS;                                                                \
        clone->SetId(id);                                                                      \
        clone->SetBinding(GetBinding());                                                       \
        return clone->CloneFrom(*this);                                                        \
    }                                                                                          \
private:                                                                                       \
/**/

//EOL