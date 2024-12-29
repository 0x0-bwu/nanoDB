#pragma once
#include "NSDatabase.h"

namespace nano {

namespace lut {
template <typename T>
class None
{
public:
    void Build(const std::vector<Id<T>> &) {}
    void Add(const Id<T>) {}
    void Reserve(size_t) {}
    void Clear() {}
};

template <typename T>
class Name
{
public:
    void Build(const std::vector<Id<T>> & data)
    {
        Clear();
        Reserve(data.size());
        for(auto id : data) Add(id);
    }

    void Add(const Id<T> id)
    {
        [[maybe_unused]] auto res = m_lut.emplace(id->GetName(), id).second;
        NS_ASSERT_MSG(res, "duplicated key in lut");
    }
    
    Id<T> Lookup(std::string_view name) const
    {
        auto iter = m_lut.find(name);
        return iter == m_lut.cend() ? Id<T>() : iter->second;;
    }

    void Reserve(size_t size) { m_lut.reserve(size); }

    void Clear() { m_lut.clear(); }
private:
    std::unordered_map<std::string_view, Id<T>> m_lut;
};

} // namespace lut
template <typename T, template <typename> class... Luts>
class IdVec
{
    using LutsMap = hana::map<hana::pair<hana::type<Luts<T>>, Luts<T>>...>;
public:
    // std-style APIs
    Id<T> & operator[] (size_t i) { return m_data[i]; }
    const Id<T> & operator[] (size_t i) const { return m_data[i]; }

    Id<T> & at(size_t i) { return m_data.at(i); }
    const Id<T> & at(size_t i) const { return m_data.at(i); }

    auto begin() { return m_data.begin(); }
    auto end() { return m_data.end(); }
    auto cbegin() const { return m_data.cbegin(); }
    auto cend() const { return m_data.cend(); }

    auto & front() { return m_data.front(); }
    const auto & front() const { return m_data.front(); }

    auto & back() { return m_data.back(); }
    const auto & back() const { return m_data.back(); }

    template <typename... Args>
    Id<T> & emplace_back(Args && ... args)
    {
        auto & id = m_data.emplace_back(std::forward<Args>(args)...);
        hana::for_each(m_luts, [&](auto & c){ hana::second(c).Add(id); });
        return id;
    }
    
    void clear()
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c).Clear(); });
        m_data.clear();
    }

    void reserve(size_t size)
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c).Reserve(size); });
        m_data.reserve(size);
    }

    size_t size() const { return m_data.size(); }

    template <template <typename> class Lut, typename Key>
    Id<T> Lookup(Key && key) const 
    {
        return m_luts[hana::type_c<Lut<T>>].Lookup(std::forward<Key>(key));
    } 

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        if constexpr (Archive::is_saving::value)
            ar & boost::serialization::make_nvp("data", m_data);
        else {
            ar & boost::serialization::make_nvp("data", m_data);
            hana::for_each(m_luts, [&](auto & c){ hana::second(c).Build(m_data); });
        }
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

private:
    std::vector<Id<T>> m_data;
    LutsMap m_luts;
};

} // namespace nano
