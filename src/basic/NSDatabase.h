#pragma once
#include "NSForward.hpp"
#include "generic/utils/LinearMap.hpp"
#include "generic/utils/Version.hpp"
#include "generic/utils/Index.hpp"

#include <boost/hana.hpp>
#include <unordered_map>
#include <typeindex>
#include <string>

namespace nano {

namespace hana = boost::hana;

template<typename Tag>
using Index = generic::utils::Index<Tag, IdType>;

template<typename Key, typename Value>
using LinearMap = generic::utils::LinearMap<Key, Value>;

template <typename T> class Id;
template <typename T> class Entity;

template <typename T>
class Container
{
public:
    class Recycler
    {
    public:
        size_t Size() const { return m_ids.size(); }
        bool Empty() const { return m_ids.empty(); }
        void Add(Id<T> t) { m_ids.emplace_back(t); }
        void Clear() { m_ids.clear(); }
        Id<T> Take()
        {
            auto t = m_ids.back();
            m_ids.pop_back();
            return t;
        }

    public:
        template <typename Archive>
        void serialize(Archive & ar, const unsigned int)
        {
            ar & boost::serialization::make_nvp("ids", m_ids);
        }
    private:
        // members
        std::vector<Id<T>> m_ids;
    };

    ~Container() { Reset(); }

    template <typename Derived>
    Derived & operator[] (const Id<Derived> id)
    { 
        static_assert(std::is_same_v<traits::BaseOf<T>, T>, "should be derived class or self");
        return (Derived&)(*m_data[IdType(id)]);
    }   

    template <typename Derived>
    const Derived & operator[] (const Id<Derived> id) const
    { 
        static_assert(std::is_same_v<traits::BaseOf<T>, T>, "should be derived class or self");
        return (Derived&)(*m_data[IdType(id)]);
    }

    template <typename Derived, typename... Args>
    Id<Derived> Create(Args &&... args)
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        static_assert(std::is_constructible_v<Derived, Args...>, "should be constructible from args...");
        auto id = m_recycler.Empty() ? m_data.Append(nullptr) : m_recycler.Take();
        NS_ASSERT(m_data[id] == nullptr);
        m_data[id] = new Derived(std::forward<Args>(args)...);
        static_cast<Entity<T>&>(*id).SetId(id);
        return Id<Derived>(id);
    }

    bool Remove(Id<T> id)
    {
        if (nullptr == m_data[id]) return false;
        NS_ASSERT(size_t(id) < m_data.Size());
        m_recycler.Add(id);
        delete m_data[id];
        m_data[id] = nullptr;
        return true;
    }

    void Reset()
    {
        m_recycler.Clear();
        for (auto * p : m_data) {
            if (not p) continue;
            delete p;
        }
        m_data.Clear();
    }
public:
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("recycler", m_recycler);
        ar & boost::serialization::make_nvp(toString<T>().data(), m_data);
    }
private:
    // members
    Recycler m_recycler;
    LinearMap<Id<T>, T*> m_data;
};

class NamedObj
{
public:
    NamedObj(std::string name);
    virtual ~NamedObj() = default;

    std::string_view GetName() const;
public:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
protected:
    NamedObj() = default;
    // members
    std::string m_name;
};

enum class ArchiveFormat { TXT = 0, XML = 1, BIN = 2};

template <typename... Eles>
class Collection : public NamedObj
{
public:
    using Elements = hana::tuple<hana::type<Eles>...>;
    using CollectionMap = hana::map<hana::pair<hana::type<Eles>, Container<Eles>>...>;
    explicit Collection(std::string name)
     : NamedObj(std::move(name))
    {
    }

    ~Collection() { Reset(); }

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    bool Save(std::string_view filename, ArchiveFormat fmt) const;
    bool Load(std::string_view filename, ArchiveFormat fmt);
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

    template <typename T, typename... Args>
    Id<T> Create(Args &&... args)
    {
        return this->Get<traits::BaseOf<T>>().template Create<T>(std::forward<Args>(args)...);
    }
    
    template <typename T, typename... Args>
    bool Remove(Args &&... args)
    {
        return this->Get<traits::BaseOf<T>>().Remove(std::forward<Args>(args)...);
    }

    template <typename T>
    constexpr auto & Get() { return m_data[hana::type_c<traits::BaseOf<T>>]; }

    template <typename T>
    constexpr const auto & Get() const { return m_data[hana::type_c<traits::BaseOf<T>>]; }

    template <typename T>
    void Reset() { Get<traits::BaseOf<T>>().Reset(); }

    void Reset()
    {
        hana::for_each(m_data, [&](auto & c){
            Reset<typename std::decay_t<decltype(hana::first(c))>::type>();
        });
    }
public:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
private:
    Collection() = default;
    // members
    CollectionMap m_data;
    Version m_version{CURRENT_VERSION};
};

class Database
{
    static_assert(hana::size(traits::elementNameMap) - hana::size(traits::inheritanceMap) == hana::size(Content::Elements()), "don't forget to register elements name!");
public:
    ~Database() = default;
    Database(Database &&) = delete;
    Database(const Database &) = delete;
    Database & operator= (Database &&) = delete;
    Database & operator= (const Database &) = delete;

    static Content & Current();
    static bool Create(std::string name);
    static bool SetCurrent(std::string_view name);
    static Ptr<Content> Find(std::string_view name);
    static bool Close(std::string_view name);
    static void Shutdown();

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    static bool Save(std::string_view name, std::string_view filename, ArchiveFormat fmt);
    static bool SaveCurrent(std::string_view filename, ArchiveFormat fmt);
    static bool Load(std::string_view filename, ArchiveFormat fmt);
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

    static Database & Instance();
private:
    Database() = default;

    Content & CurrentImpl();
    bool CreateImpl(std::string name);
    bool SetCurrentImpl(std::string_view name);
    Ptr<Content> FindImpl(std::string_view name);
    CPtr<Content> FindImpl(std::string_view name) const;
    bool CloseImpl(std::string_view name);
    void ShutdownImpl();

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    bool SaveImpl(std::string_view name, std::string_view filename, ArchiveFormat fmt) const;
    bool LoadImpl(std::string_view filename, ArchiveFormat fmt);
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

private:
    Ptr<Content> m_current{nullptr};
    std::unordered_map<std::string_view, UPtr<Content>> m_contents;
};

template <typename T>
class Entity
{
public:
    friend class Container<T>;
    virtual ~Entity() = default;

    bool isValid() const { return m_id != Id<T>::INVALID_ID; }

    template <typename Derived>
    bool Identical(const Id<Derived> id) const
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        return &Get<T>(Id<T>(id)) == static_cast<const T*>(this);
    }
    
    /// binds
    template <typename Other>
    void Bind(const Id<Other> other);

    template <typename Other>
    void Unbind();

    template <typename Other>
    Id<Other> GetBind() const;
    ///
private:
    void SetId(Id<T> id) { m_id = IdType(id); }
public:
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("id", m_id);
        ar & boost::serialization::make_nvp("binding", m_binding);
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
private:
    IdType m_id = Id<T>::INVALID_ID;
    IdType m_binding = Id<T>::INVALID_ID;
};

namespace traits {
using NameIndexMap = std::unordered_map<std::string_view, std::type_index>;
using IndexNameMap = std::unordered_map<std::type_index, std::string_view>;
} // namespace traits

class Binding : public Entity<Binding>
{
public:
    using Key = std::type_index; 
    using BindingMap = std::unordered_map<std::type_index, IdType>;
    static traits::NameIndexMap nameIndexMap;
    static traits::IndexNameMap indexNameMap; 
    template <typename T>
    void Set(const Id<T> other)
    {
        m_bindingMap.emplace(typeid(T), IdType(other));
    }

    template <typename T>
    void Unset()
    {
        m_bindingMap.erase(std::type_index(typeid(T)));
    }

    template <typename T>
    Id<T> Get() const
    {
        auto iter = m_bindingMap.find(std::type_index(typeid(T)));
        if (iter == m_bindingMap.cend()) Id<T>();
        return Id<T>(iter->second);
    }
public:
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Binding);
        ar & boost::serialization::make_nvp("binding_map", m_bindingMap);
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
private:
    BindingMap m_bindingMap;
};

template <typename T, typename... Args>
inline Id<T> Create(Args &&... args)
{
    return Database::Current().Create<T>(std::forward<Args>(args)...);
}

template <typename T>
inline bool Remove(const Id<T> id)
{
    return Database::Current().Remove<T>(id);
}

template <typename T>
inline T & Get(const Id<T> id)
{
    return Database::Current().Get<T>()[id];
}

template <typename T>
class Id : public Index<Id<T>>
{
public:
    using Index<Id<T>>::m_id;
    using Index<Id<T>>::INVALID_ID;
    using SizeType = typename Index<Id<T>>::SizeType;

    Id() : Index<Id<T>>() {}
    explicit Id(SizeType id) : Index<Id<T>>(id) {}

    template <typename Other, typename std::enable_if_t<std::is_base_of_v<T, Other>, bool> = true>
    Id(Id<Other> other) : Id(SizeType(other))
    {
    }

    bool isNull() const
    {
        return m_id == Id<T>::INVALID_ID  or nullptr == this->operator->(); 
    }

    operator bool() const override { return not isNull(); }

    bool operator!() const { return isNull(); }

    T & operator * () const noexcept
    {
        return nano::Get<T>(*this);
    }

    T * operator-> () const noexcept
    {
        return &nano::Get<T>(*this);
    }

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("id", boost::serialization::base_object<Index<Id<T>>>(*this));
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
};

/// binds
template <typename T>
template <typename Other>
void Entity<T>::Bind(const Id<Other> other)
{
    if (m_binding == Id<T>::INVALID_ID)
        m_binding = IdType(nano::Create<Binding>());
    Id<Binding>(m_binding)->Set<Other>(other);
}

template <typename T>
template <typename Other>
void Entity<T>::Unbind()
{
    if(m_binding != Id<T>::INVALID_ID)
        Id<Binding>(m_binding)->Unset<Other>();
}

template <typename T>
template <typename Other>
Id<Other> Entity<T>::GetBind() const
{
    if(m_binding != Index<T>::INVALID_ID)
        return Id<Binding>(m_binding)->Get<Other>();
    return Id<Other>();
}
///

} // namespace nano
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::NamedObj)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::Content)
#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
namespace boost::serialization {

template <typename Archive>
void serialize(Archive & ar, typename nano::Binding::BindingMap & m, const unsigned int version)
{
    using namespace nano;
    NS_UNUSED(version);
    size_t size{m.size()};
    ar & boost::serialization::make_nvp("size", size);
    if constexpr (Archive::is_saving::value) {
        for (const auto & [type, id] : m) {
            std::string name = Binding::indexNameMap.at(type).data();
            ar & boost::serialization::make_nvp("name", name);
            ar & boost::serialization::make_nvp("id", id);
        }
    }
    else {
        m.clear();
        IdType id;
        std::string name;
        for (size_t i = 0; i < size; ++i) {
            ar & boost::serialization::make_nvp("name", name);
            ar & boost::serialization::make_nvp("id", id);
            auto iter = Binding::nameIndexMap.find(name);
            NS_ASSERT(iter != Binding::nameIndexMap.cend());
            m.emplace(iter->second, id);
        }
    }
}
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
} // namespace boost::serialization