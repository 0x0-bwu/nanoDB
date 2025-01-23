#pragma once

#define NANO_CURRENT_CXX_VERSION 20
#undef  GENERIC_CURRENT_CXX_VERSION
#define GENERIC_CURRENT_CXX_VERSION NANO_CURRENT_CXX_VERSION

#define NS_STRING(s) #s
#define NS_EOL GENERIC_DEFAULT_EOL
#define NS_SEPS GENERIC_FOLDER_SEPS

#define NS_DEFINE_CONSTEXPR_STRING_VIEW(s) inline static constexpr std::string_view s = #s

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    #ifndef GENERIC_BOOST_SERIALIZATION_SUPPORT
        #define GENERIC_BOOST_SERIALIZATION_SUPPORT
    #endif
    #include "generic/common/Archive.hpp"
    #include "generic/geometry/Serialization.hpp"
    #include <boost/serialization/binary_object.hpp>
    #define NS_SERIALIZATION_ENTITY_OBJECT_NVP(name)                                           \
        boost::serialization::make_nvp(                                                        \
            NS_STRING(Entity_##name),                                                          \
            boost::serialization::base_object<Entity<name>>(*this)                             \
        )                                                                                      \
    /**/

    #define NS_SERIALIZATION_ABSTRACT_CLASS(T) BOOST_SERIALIZATION_ASSUME_ABSTRACT(T)
    #define NS_SERIALIZATION_FUNCTIONS_DECLARATION                                             \
    friend class boost::serialization::access;                                                 \
    template <typename Archive> void serialize(Archive &, const unsigned int);                 \
    /**/
    #define NS_SERIALIZATION_ABSTRACT_CLASS_FUNCTIONS_DECLARATION                              \
    friend class boost::serialization::access;                                                 \
    template<typename Archive> void serialize(Archive &, unsigned int) {}                      \
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
    #define NS_SERIALIZATION_HANA_STRUCT(ARCHIVE, OBJECT)                                                               \
    {                                                                                                                   \
        auto & object = const_cast<std::decay_t<decltype(OBJECT)> &>(OBJECT);                                           \
        hana::for_each(hana::keys(object), [&](auto key) {                                                              \
            ARCHIVE & boost::serialization::make_nvp(key.c_str(), hana::at_key(object, key));                           \
        });                                                                                                             \
    }                                                                                                                   \
    /**/
    #define NS_SERIALIZATION_CLASS_MEMBERS(ARCHIVE) NS_SERIALIZATION_HANA_STRUCT(ARCHIVE, m_)

    namespace nano { using ArchiveFormat = generic::archive::ArchiveFormat; }
#else
    #define NS_SERIALIZATION_ENTITY_OBJECT_NVP(T)    
    #define NS_SERIALIZATION_CLASS_EXPORT(T)
    #define NS_SERIALIZATION_ABSTRACT_CLASS(T)
    #define NS_SERIALIZATION_CLASS_EXPORT_KEY(T)
    #define NS_SERIALIZATION_CLASS_EXPORT_IMP(T)
    #define NS_SERIALIZATION_FUNCTIONS_DECLARATION
    #define NS_SERIALIZATION_ABSTRACT_CLASS_FUNCTIONS_DECLARATION
    #define NS_SERIALIZATION_HANA_STRUCT(ARCHIVE, OBJECT)
    #define NS_SERIALIZATION_CLASS_MEMBER(ARCHIVE)
#endif //NANO_BOOST_SERIALIZATION_SUPPORT
