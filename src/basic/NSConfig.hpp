#pragma once

#define NANO_STRING(s) #s
#define NANO_CURRENT_CXX_VERSION 20
#undef GENERIC_CURRENT_CXX_VERSION
#define GENERIC_CURRENT_CXX_VERSION NANO_CURRENT_CXX_VERSION
#define NANO_BOOST_SERIALIZATION_SUPPORT

#define NS_DEFINE_CONSTEXPR_STRING_VIEW(s) inline static constexpr std::string_view s = #s
#define NS_INHERITANCE(CLASS, FROM, BASE)                                                      \
static_assert(std::is_base_of_v<BASE, FROM >, "should be derived class or self");              \
static_assert(std::is_base_of_v<FROM, CLASS>, "should be derived class or self");              \
inline FROM & operator-- (CLASS & c) { return static_cast<FROM &>(c); }                        \
inline FROM & operator-- (CLASS & c, int) { return static_cast<FROM &>(c); }                   \
inline const FROM & operator-- (const CLASS & c) { return static_cast<const FROM &>(c); }      \
inline const FROM & operator-- (const CLASS & c, int) { return static_cast<const FROM &>(c); } \
/**/
#define NS_INHERIT_FROM_BASE(CLASS, BASE) NS_INHERITANCE(CLASS, BASE, BASE)
#define NS_DEFINE_CLASS_MEMBERS(...)                                          \
private:                                                                      \
    struct NANO_CLASS_MEMBERS {                                               \
    BOOST_HANA_DEFINE_STRUCT(NANO_CLASS_MEMBERS, __VA_ARGS__);} m_;           \
    NANO_CLASS_MEMBERS * operator-> () noexcept { return &m_; }               \
public:                                                                       \
    const NANO_CLASS_MEMBERS * operator->() const noexcept { return &m_; }    \
    const NANO_CLASS_MEMBERS & operator* () const noexcept { return  m_; }    \
private:                                                                      \
/**/

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    #ifndef GENERIC_BOOST_SERIALIZATION_SUPPORT
        #define GENERIC_BOOST_SERIALIZATION_SUPPORT
    #endif
    #include "generic/common/Archive.hpp"
    #include "generic/geometry/Serialization.hpp"
    #include <boost/serialization/binary_object.hpp>
    #define NS_SERIALIZATION_ENTITY_OBJECT_NVP(name)                               \
        boost::serialization::make_nvp(                                            \
            NANO_STRING(Entity_##name),                                            \
            boost::serialization::base_object<Entity<name>>(*this)                 \
        )                                                                          \
    /**/

    #define NS_SERIALIZATION_ABSTRACT_CLASS(T) BOOST_SERIALIZATION_ASSUME_ABSTRACT(T)
    #define NS_SERIALIZATION_FUNCTIONS_DECLARATION                                  \
    friend class boost::serialization::access;                                      \
    template <typename Archive> void serialize(Archive &, const unsigned int);      \
    /**/
    #define NS_SERIALIZATION_ABSTRACT_CLASS_FUNCTIONS_DECLARATION                   \
    friend class boost::serialization::access;                                      \
    template<typename Archive> void serialize(Archive &, unsigned int) {}           \
    /**/

    #define NS_SERIALIZATION_CLASS_EXPORT(T) BOOST_CLASS_EXPORT(T)
    #define NS_SERIALIZATION_CLASS_EXPORT_KEY(T) BOOST_CLASS_EXPORT_KEY(T)
    #define NS_SERIALIZATION_CLASS_EXPORT_IMP(T) BOOST_CLASS_EXPORT_IMPLEMENT(T)
    #define NS_SERIALIZATION_FUNCTIONS_IMP(T)                                                                           \
    template void T::serialize<boost::archive::text_iarchive>(boost::archive::text_iarchive &, const unsigned int);     \
    template void T::serialize<boost::archive::xml_iarchive>(boost::archive::xml_iarchive &, const unsigned int);       \
    template void T::serialize<boost::archive::binary_iarchive>(boost::archive::binary_iarchive &, const unsigned int); \
    template void T::serialize<boost::archive::text_oarchive>(boost::archive::text_oarchive &, const unsigned int);     \
    template void T::serialize<boost::archive::xml_oarchive>(boost::archive::xml_oarchive &, const unsigned int);       \
    template void T::serialize<boost::archive::binary_oarchive>(boost::archive::binary_oarchive &, const unsigned int); \
    /**/

    #define NS_SERIALIZATION_CLASS_MEMBERS(ARCHIVE)                               \
    {                                                                             \
        size_t index{0};                                                          \
        hana::for_each(hana::members(m_), [&](const auto & m) {                   \
            auto & mm = const_cast<std::decay_t<decltype(m)>&>(m);                \
            auto tag = "member" + std::to_string(index++);                        \
            ARCHIVE & boost::serialization::make_nvp(tag.c_str(), mm);            \
        });                                                                       \
    }                                                                             \
    /**/
#else
    #define NS_SERIALIZATION_ENTITY_OBJECT_NVP(T)    
    #define NS_SERIALIZATION_CLASS_EXPORT(T)
    #define NS_SERIALIZATION_ABSTRACT_CLASS(T)
    #define NS_SERIALIZATION_CLASS_EXPORT_KEY(T)
    #define NS_SERIALIZATION_CLASS_EXPORT_IMP(T)
    #define NS_SERIALIZATION_FUNCTIONS_DECLARATION
    #define NS_SERIALIZATION_ABSTRACT_CLASS_FUNCTIONS_DECLARATION
    #define NS_SERIALIZATION_CLASS_MEMBER(ARCHIVE)
#endif //NANO_BOOST_SERIALIZATION_SUPPORT
