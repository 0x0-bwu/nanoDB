#pragma once
#include "NSDatabase.h"

namespace nano {

template <typename IdContainer, typename T = typename IdContainer::object_type, bool Mutable = true, 
          typename std::enable_if_t<std::is_base_of_v<typename IdContainer::object_type, T>, bool> = true>
class IdIterator
{
public:
    using Iterator = typename IdContainer::const_iterator;
    using ReturnType = std::conditional_t<Mutable, Id<T>, CId<T>>; 
    explicit IdIterator(const IdContainer & container)
     : m_curr(container.cbegin()), m_end(container.cend())
    {
    }

    ReturnType Next() // return current and advance one
    {
        while (m_curr != m_end) {
            if (auto res = ReturnType(*m_curr); res) {
                m_curr++; return res;
            }
            m_curr++;
        }
        return ReturnType();
    }

    ReturnType Current() const
    {
        while (m_curr != m_end) {
            if (auto res = ReturnType(*m_curr); res)
                return res;
            m_curr++;
        }
        return ReturnType();
    }

    ReturnType operator*  () const noexcept { return Current(); }
    ReturnType operator-> () const noexcept { return Current(); }
private:
    mutable Iterator m_curr;
    const Iterator m_end;
};

namespace lut {

template <typename T, typename KeyFn>
class Lut
{
public:
    using Key = std::invoke_result_t<KeyFn, const Id<T>>;
    void Build(const std::vector<Id<T>> & data)
    {
        Clear();
        Reserve(data.size());
        for(auto id : data) Add(id);
    }
    void Add(const Id<T> id)
    {
        NS_ASSERT(id);
        [[maybe_unused]] auto res = m_lut.emplace(m_keyFn(id), id).second;
        NS_ASSERT_MSG(res, "duplicated key in lut");
    }
    
    void Remove(const Id<T> id)
    {
        m_lut.erase(m_keyFn(id));
    }

    Id<T> Lookup(const Key & key) const
    {
        auto iter = m_lut.find(key);
        return iter == m_lut.cend() ? Id<T>() : iter->second;;
    }
    void Reserve(size_t size) { m_lut.reserve(size); }
    void Clear() { m_lut.clear(); }
private:
    KeyFn m_keyFn;
    std::unordered_map<Key, Id<T>> m_lut;
};

template <typename T>
struct NameGetter
{
    std::string_view operator() (const Id<T> id) const
    {
        return id->GetName(); 
    }
};

template <typename T, typename... Lut>
struct Luts
{
public:
    using LutsMap = hana::map<hana::pair<hana::type<Lut>, Lut>...>;

    void Build(const std::vector<Id<T>> & data)
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c).Build(data); });
    }

    void Add(Id<T> id)
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c).Add(id); });
    }

    void Remove(Id<T> id)
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c).Remove(id); });
    }

    template <template <typename> class L, typename K>
    Id<T> Lookup(K && key) const 
    {
        return m_luts[hana::type_c<L<T>>].Lookup(std::forward<K>(key));
    } 

    void Reserve(size_t size)
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c).Reserve(size); });
    }

    void Clear()
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c).Clear(); });
    }
private:
    LutsMap m_luts;
};

template <typename T>
using Name = Lut<T, NameGetter<T>>;

} // namespace lut

template <typename T>
using NoneLut = lut::Luts<T>;

template <typename T>
using NameLut = lut::Luts<T, lut::Name<T>>;

template <typename T, template <typename> class Luts = NoneLut>
class IdVec
{
public:
    // std-style APIs
    using reference = Id<T>&;
    using value_type = Id<T>;
    using iterator = typename std::vector<Id<T>>::iterator;
    using const_iterator = typename std::vector<Id<T>>::const_iterator;
    using object_type = T;

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
        m_luts.Add(id);
        return id;
    }
    
    void clear()
    {
        m_luts.Clear();
        m_data.clear();
    }

    void reserve(size_t size)
    {
        m_luts.Reserve(size);
        m_data.reserve(size);
    }

    size_t size() const { return m_data.size(); }

    ///
    template <typename Derived>
    using Iter = IdIterator<IdVec<T, Luts>, Derived, /*Mutable=*/true>;

    template <typename Derived>
    using CIter = IdIterator<IdVec<T, Luts>, Derived, /*Mutable=*/false>;

    template <typename Derived = T>
    Iter<Derived> GetIter() const { return Iter<Derived>(*this); }

    template <typename Derived = T>
    CIter<Derived> GetCIter() const { return CIter<Derived>(*this); }

    template <template <typename> class Lut, typename Key>
    Id<T> Lookup(Key && key) const 
    {
        return m_luts.template Lookup<Lut>(std::forward<Key>(key));
    }

    template <typename Func>
    void Sort(Func && func)
    {
        std::sort(m_data.begin(), m_data.end(), std::forward<Func>(func));
    }

    Id<T> Add(Id<T> id) { return emplace_back(id); }

    void Remove(Id<T> id)
    {
        m_luts.Remove(id);
        m_data.erase(std::remove(m_data.begin(), m_data.end(), id), m_data.end());
    }

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        if constexpr (Archive::is_saving::value)
            ar & boost::serialization::make_nvp("data", m_data);
        else {
            ar & boost::serialization::make_nvp("data", m_data);
            m_luts.Build(m_data);
        }
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

private:
    Luts<T> m_luts;
    std::vector<Id<T>> m_data;
};

} // namespace nano
