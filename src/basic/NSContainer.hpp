#pragma once
#include "NSTraits.hpp"
#include "generic/utils/ZipView.hpp"
#include "generic/tools/Hash.hpp"
#include <shared_mutex>
namespace nano {

template <typename IdContainer, typename T = typename IdContainer::object_type, bool Mutable = true>
requires std::is_base_of_v<typename IdContainer::object_type, T>
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
    explicit Lut(Vec<Id<T>> & data)
     : m_data(data) {}

    void Add(const Id<T> & id)
    {
        NS_ASSERT(id);
        if (m_build) {
            std::unique_lock l(m_mutex);
            [[maybe_unused]] auto res = m_lut.emplace(m_keyFn(id), id).second;
            NS_ASSERT_MSG(res, "duplicated key in lut");
        }
    }

    void Remove(const Id<T> & id)
    {
        if (m_build) {
            std::unique_lock l(m_mutex);
            m_lut.erase(m_keyFn(id));
        }
    }

    Id<T> Lookup(const Key & key) const
    {
        Build();
        std::shared_lock l(m_mutex);
        auto iter = m_lut.find(key);
        return iter == m_lut.cend() ? Id<T>() : iter->second;;
    }

    void Clear()
    {
        std::unique_lock l(m_mutex);
        m_build = false;
        m_lut.clear();
    }
private:
    void Build() const
    {
        if (m_build) return;
        static std::mutex m;
        std::lock_guard l(m);
        if (m_build) return;
        m_lut.reserve(m_data.size());
        [[maybe_unused]] bool res{false};
        for (auto id : m_data) {
            res = m_lut.emplace(m_keyFn(id), id).second;
            NS_ASSERT_MSG(res, "duplicated key in lut");
        }
        m_build = true;
    }
private:
    KeyFn m_keyFn;
    Vec<Id<T>> & m_data;
    mutable HashMap<Key, Id<T>> m_lut;
    mutable std::shared_mutex m_mutex;
    mutable std::atomic<bool> m_build{false};
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
    using LutsMap = hana::map<hana::pair<hana::type<Lut>, UPtr<Lut>>...>;

    explicit Luts(Vec<Id<T>> & data)
    {
        hana::for_each(m_luts, [&](auto & c){ 
            using LutType = typename std::decay_t<decltype(hana::first(c))>::type;
            hana::second(c) = std::make_unique<LutType>(data); 
        });
    }

    void Add(const Id<T> & id)
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c)->Add(id); });
    }

    void Remove(const Id<T> & id)
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c)->Remove(id); });
    }

    template <template <typename> class L, typename K>
    Id<T> Lookup(K && key) const 
    {
        return m_luts[hana::type_c<L<T>>]->Lookup(std::forward<K>(key));
    } 

    void Clear()
    {
        hana::for_each(m_luts, [&](auto & c){ hana::second(c)->Clear(); });
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
    using iterator = typename Vec<Id<T>>::iterator;
    using const_iterator = typename Vec<Id<T>>::const_iterator;
    using object_type = T;
    IdVec() : m_luts(std::make_unique<Luts<T>>(m_data)) {}
    explicit IdVec(Vec<Id<T>> && data)
     : IdVec() { m_data = std::move(data); }

    Id<T> & operator[] (size_t i) { return m_data[i]; }
    const Id<T> & operator[] (size_t i) const { return m_data[i]; }

    Id<T> & at(size_t i) { return m_data.at(i); }
    const Id<T> & at(size_t i) const { return m_data.at(i); }

    iterator begin() { return m_data.begin(); }
    iterator end() { return m_data.end(); }
    const_iterator begin() const { return m_data.cbegin(); }
    const_iterator end() const { return m_data.cend(); }
    const_iterator cbegin() const { return m_data.cbegin(); }
    const_iterator cend() const { return m_data.cend(); }

    auto & front() { return m_data.front(); }
    const auto & front() const { return m_data.front(); }

    auto & back() { return m_data.back(); }
    const auto & back() const { return m_data.back(); }

    template <typename... Args>
    Id<T> & emplace_back(Args && ... args)
    {
        auto & id = m_data.emplace_back(std::forward<Args>(args)...);
        m_luts->Add(id);
        return id;
    }
    
    void clear()
    {
        m_luts->Clear();
        m_data.clear();
    }

    void reserve(size_t size)
    {
        m_data.reserve(size);
    }

    size_t size() const { return m_data.size(); }

    size_t count(const Id<T> & id) const
    {
        return std::count(m_data.begin(), m_data.end(), id);
    }

    size_t count(const CId<T> & id) const
    {
        return count(id.ConstCast());
    }

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
        return m_luts->template Lookup<Lut>(std::forward<Key>(key));
    }

    template <typename Func>
    void Sort(Func && func)
    {
        std::sort(m_data.begin(), m_data.end(), std::forward<Func>(func));
    }

    Id<T> Add(Id<T> id) { return emplace_back(id); }

    void Remove(Id<T> & id, bool destroy = false)
    {
        m_luts->Remove(id);
        m_data.erase(std::remove(m_data.begin(), m_data.end(), id), m_data.end());
        if (destroy) id.Destroy();
    }

    IdVec<T, Luts> Clone() const requires (traits::Cloneable<T>)
    {
        Vec<Id<T>> data; data.reserve(m_data.size());
        for (auto id : m_data) data.emplace_back(Id<T>(id->Clone()));
        return IdVec<T, Luts>(std::move(data));
    }

    void Destroy()
    {
        for (auto id : m_data) id.Destroy();
        Clear();
    }

    void Clear() { clear(); }

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("data", m_data);
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

private:
    UPtr<Luts<T>> m_luts;
    Vec<Id<T>> m_data;
};


template <typename... Containers>
auto Zip(Containers & ... containers)
{
    return generic::utils::Zip<Containers...>(containers...);
}

} // namespace nano

namespace std {

template <typename T, template <typename> class Luts>
struct hash<::nano::IdVec<T, Luts>>
{
    size_t operator() (const ::nano::IdVec<T, Luts> & v) const
    {
        return generic::hash::OrderedHash(v);
    }
};

} // namespace std
