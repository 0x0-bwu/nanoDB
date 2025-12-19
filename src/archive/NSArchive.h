#pragma once

#include <nano/core/basic>

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

namespace boost::serialization {

template <typename Archive, typename... Eles>
inline void serialize(Archive & ar, nano::Collection<Eles...> & collection, const unsigned int version)
{
    if constexpr (Archive::is_loading::value) collection.Reset();
    ar & boost::serialization::make_nvp("name", boost::serialization::base_object<nano::NamedObj>(collection));
    hana::for_each(collection.m_data, [&](auto & c){
        using T = typename std::decay_t<decltype(hana::first(c))>::type;
        boost::serialization::serialize(ar,  collection.m_data[hana::type<T>()], version);
    });
}

} // namespace boost::serialization

namespace nano::archive {

namespace detail {
template <typename... Args>
bool Save(const Collection<Args...> & collection, std::string_view filename, Format fmt);

template <typename... Args>
bool Load(std::string_view filename, Format fmt, Collection<Args...> & collection);
} // namespace detail

bool Save(std::string_view name, std::string_view filename, Format fmt);
bool SaveCurrent(std::string_view filename, Format fmt);
bool Load(std::string_view filename, Format fmt);

} // namespace nano::archive
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
