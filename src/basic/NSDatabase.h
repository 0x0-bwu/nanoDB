#pragma once
#include "NSHash.hpp"
#include "generic/utils/LinearMap.hpp"
#include "generic/utils/Version.hpp"

#include <boost/noncopyable.hpp>
#include <unordered_map>
#include <typeindex>
#include <string>

namespace nano {

namespace hana = boost::hana;

template<typename Key, typename Value>
using LinearMap = generic::utils::LinearMap<Key, Value>;

template <typename T> class Entity;
template <typename T>
class Container
{
public:
    Container()  { static_assert(std::is_base_of_v<Entity<T>, T>, "T should be derived class of Entity<T>"); }
    ~Container() { Reset(); }

    template <typename Derived, bool M>
    Derived * operator[] (const Id<Derived, M> & id)
    { 
        static_assert(std::is_same_v<traits::BaseOf<Derived>, T>, "should be derived class or self");
        return static_cast<Derived *>(m_data[Index(id)]);
    }   

    template <typename Derived, bool M>
    const Derived * operator[] (const Id<Derived, M> & id) const
    { 
        return const_cast<Container<T>*>(this)->operator[]<Derived>(id);
    }
    
    Index Allocate() { return Index(m_data.Append(nullptr)); }

    template <typename Derived>
    Id<Derived> Register(Index id, Derived * p)
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        NS_ASSERT(m_data[id] == nullptr and p);
        static_cast<Entity<T>&>(*p).SetId(id);
        m_data[id] = p;
        return Id<Derived>(id);
    }

    template <typename Derived, typename... Args>
    Id<Derived> Create(Args &&... args)
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        static_assert(std::is_constructible_v<Derived, Args...>, "should be constructible from args...");
        return Register(Allocate(), new Derived(std::forward<Args>(args)...));
    }

    template <typename Derived, typename UnaryPred>
    Id<Derived> Find(UnaryPred && pred) const
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        for (size_t i = 0; i < m_data.size(); ++i) {
            if (auto * d = dynamic_cast<Derived*>(m_data[i]); d) {
                if (pred(*d)) return Id<Derived>(i);
            }
        }
        return Id<Derived>();
    }

    template <typename Derived, typename UnaryPred, typename Vec>
    void FindAll(UnaryPred && pred, Vec & vec) const
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        vec.clear();
        for (size_t i = 0; i < Size(); ++i) {
            if (auto * d = dynamic_cast<Derived*>(m_data[i]); d) {
                if (pred(*d)) vec.emplace_back(i);
            }
        }
    }

    bool Remove(const Id<T> & id)
    {
        if (Index(id) >= Size()) return false;
        if (nullptr == m_data[id]) return false;
        delete m_data[id];
        m_data[id] = nullptr;
        return true;
    }

    void Reset()
    {
        for (auto * p : m_data) {
            if (p) delete p;
        }
        m_data.Clear();
    }
    
    size_t Size() const
    {
        return m_data.size();
    }

    size_t Hash() const
    {
        return generic::hash::OrderedHash(m_data, [&](auto * p) { return p ? p->Entity<T>::Hash() : 0; });        
    }

public:
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp(toString<T>().data(), m_data);
    }
private:
    // members
    LinearMap<Id<T>, T*> m_data;
};

class NamedObj
{
public:
    explicit NamedObj(std::string name);
    virtual ~NamedObj() = default;

    std::string_view GetName() const;
public:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
protected:
    NamedObj() = default;
    size_t Hash() const { return std::hash<std::string>()(m_name); }
    Ptr<NamedObj> CloneFrom(const NamedObj & src) { m_name = src.m_name; return this; }
    // members
    std::string m_name;
private:
    friend class package::utils::FlattenUtility;
    template <typename T> friend class Cloneable;
    void Rename(std::string name) { m_name = std::move(name); }
};

template <typename... Eles>
class Collection : public NamedObj
{
public:
    using Elements = hana::tuple<hana::type<Eles>...>;
    using CollectionMap = hana::map<hana::pair<hana::type<Eles>, Container<Eles>>...>;
    explicit Collection(std::string name) : NamedObj(std::move(name)) {}
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

    template <typename T, typename UnaryPred>
    Id<T> Find(UnaryPred && pred) const
    {
        return this->Get<traits::BaseOf<T>>().template Find<T>(std::forward<UnaryPred>(pred));
    }

    template <typename T, typename UnaryPred, typename Vec>
    void FindAll(UnaryPred && pred, Vec & vec) const
    {
        return this->Get<traits::BaseOf<T>>().template FindAll<T>(std::forward<UnaryPred>(pred), vec);
    }

    template <typename T>
    constexpr auto & Get() { return m_data[hana::type_c<traits::BaseOf<T>>]; }

    template <typename T>
    constexpr const auto & Get() const { return m_data[hana::type_c<traits::BaseOf<T>>]; }

    template <typename T>
    size_t Size() const { return Get<T>().Size(); }

    template <typename T>
    void Reset() { Get<traits::BaseOf<T>>().Reset(); }

    void Reset()
    {
        hana::for_each(m_data, [&](auto & c){
            Reset<typename std::decay_t<decltype(hana::first(c))>::type>();
        });
    }

    std::string Checksum() const
    {
        size_t sum = 0;
        hana::for_each(m_data, [&](auto & c){
            sum += Get<typename std::decay_t<decltype(hana::first(c))>::type>().Hash();
        });
        std::stringstream ss; ss << std::hex << sum;
        return ss.str();
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
    static std::string_view CurrentDir();
    static void SetCurrentDir(const std::string& dir);
    static bool Create(const std::string& name);
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
    Database();
    Content & CurrentImpl();
    static void SetCurrentDirImpl(const std::string& dir);
    bool CreateImpl(const std::string& name);
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
    HashMap<std::string_view, UPtr<Content>> m_contents;
};

template <typename T>
class Cloneable
{
public:
    virtual ~Cloneable() = default;

    template <typename Derived = T>
    requires std::is_base_of_v<T, Derived>
    Id<Derived> Clone() const
    {
        auto id = Database::Current().Get<T>().Allocate();
        Database::Current().Get<T>().Register(id, CloneImpl(id));
        return Id<Derived>(id);
    }

    template <typename Derived = T>
    requires std::is_base_of_v<T, Derived>
    Id<Derived> Clone(std::string rename) const requires traits::Nameable<Derived>
    {
        auto id = Clone<Derived>();
        id->Rename(std::move(rename));
        return id;
    }

    SPtr<T> SharedClone() const { return SPtr<T>(CloneImpl(INVALID_INDEX)); }
    UPtr<T> UniqueClone() const { return UPtr<T>(CloneImpl(INVALID_INDEX)); }
    
protected:
    virtual Ptr<T> CloneImpl(Index id) const = 0;
};

template <typename T>
class Entity : private boost::noncopyable
{
public:
    friend class Container<T>;
    virtual ~Entity() = default;

    bool isValid() const { return m_id != INVALID_INDEX; }

    template <typename Derived, bool M>
    bool Identical(const generic::utils::Index<Id<Derived, M>, nano::Index> & id) const
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        using SizeType = typename Id<Derived, M>::SizeType;
        return &(*Id<T, M>(SizeType(id))) == static_cast<const T*>(this);
    }
    
    /// binds
    template <typename Other>
    void Bind(const generic::utils::Index<Id<Other>, nano::Index> & other);

    template <typename Other>
    void Unbind();

    template <typename Other>
    CId<Other> GetBind() const;

    template <typename Derived>
    Id<Derived> Cast()
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        return dynamic_cast<Derived *>(this) ? Id<Derived>(m_id) : Id<Derived>();
    }
    
    template <typename Derived>
    CId<Derived> Cast() const
    {
        static_assert(std::is_base_of_v<T, Derived>, "should be derived class or self");
        return dynamic_cast<const Derived *>(this) ? CId<Derived>(m_id) : CId<Derived>();
    }

    virtual size_t Hash() const { return 0; }
protected:
    void SetId(Index id)  { m_id = id; }
    Index GetId() const { return m_id; }
    void SetBinding(Index id)  { m_binding = id; }
    Index GetBinding() const { return m_binding; }
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
    Index m_id{INVALID_INDEX};
    Index m_binding{INVALID_INDEX};
};

namespace traits {
using NameIndexMap = HashMap<std::string_view, std::type_index>;
using IndexNameMap = HashMap<std::type_index, std::string_view>;
} // namespace traits

class Binding : public Entity<Binding>
{
public:
    using Key = std::type_index; 
    using BindingMap = HashMap<std::type_index, Index>;
    static traits::NameIndexMap nameIndexMap;
    static traits::IndexNameMap indexNameMap;

    template <typename T>
    void Set(const generic::utils::Index<Id<T>, nano::Index> & other)
    {
        m_bindingMap.emplace(typeid(T), Index(other));
    }

    template <typename T>
    void Unset()
    {
        m_bindingMap.erase(std::type_index(typeid(T)));
    }

    template <typename T>
    CId<T> Get() const
    {
        auto iter = m_bindingMap.find(std::type_index(typeid(T)));
        if (iter == m_bindingMap.cend()) CId<T>();
        return CId<T>(iter->second);
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
inline bool Remove(Id<T> & id)
{
    return Database::Current().Remove<T>(id);
}

template <typename T, typename UnaryPred>
inline Id<T> Find(UnaryPred && pred)
{
    return Database::Current().Find<T>(std::forward<UnaryPred>(pred));
}

template <typename T, typename UnaryPred, typename Vec>
inline void FindAll(UnaryPred && pred, Vec & vec)
{
    return Database::Current().FindAll<T>(std::forward<UnaryPred>(pred), vec);
}

inline std::string_view CurrentDir()
{
    return Database::CurrentDir();
}

inline void SetCurrentDir(const std::string& dir)
{
    Database::SetCurrentDir(dir);
}

template <typename T, bool Mutable>
class Id : public generic::utils::Index<Id<T, Mutable>, nano::Index>
{
public:
    using Type = std::conditional_t<Mutable, T, const T>;
    using generic::utils::Index<Id<T, Mutable>, nano::Index>::m_id;
    using SizeType = typename generic::utils::Index<Id<T, Mutable>, nano::Index>::SizeType;

    Id() : generic::utils::Index<Id<T, Mutable>, nano::Index>() {}

    template <bool M>
    Id(const generic::utils::Index<Id<T, M>, nano::Index> & id) = delete;

    explicit Id(SizeType id) : generic::utils::Index<Id<T, Mutable>, nano::Index>(id) {}

    template <typename Derived, bool M>
    requires std::is_base_of_v<T, Derived>
    Id(const Id<Derived, M> & derived) : Id(SizeType(derived)) {}// implicit convert from derived to base

    template <typename Base, bool M>
    requires (not std::is_same_v<Base, T> and std::is_base_of_v<Base, T>)
    explicit Id(const Id<Base, M> & base) // explicit convert from base to derived, will do dynamic cast check
    {
        m_id = dynamic_cast<Type *>(base.operator->()) ? SizeType(base) : INVALID_INDEX;
    }

    CId<T> GetCId() const { return CId<T>(m_id); }

    bool isNull() const { return m_id == INVALID_INDEX or nullptr == this->operator->(); }

    operator bool() const override { return not isNull(); }

    bool operator!() const { return isNull(); }
    
    Id<T, true> ConstCast() const requires (not Mutable) { return Id<T, true>(m_id); }

    Type & operator * () const
    {
        return *(this->operator->());
    }

    Type * operator-> () const 
    {
        if (INVALID_INDEX == m_id) return nullptr;
        auto & container = Database::Current().Get<T>();
        NS_ASSERT(m_id < container.Size());
        return container[Id<T, Mutable>(m_id)];
    }

    void Destroy() { nano::Remove<T>(*this); }

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
    template <typename Archive>
    void serialize(Archive & ar, const unsigned int)
    {
        ar & boost::serialization::make_nvp("id", boost::serialization::base_object<generic::utils::Index<Id<T, Mutable>, nano::Index>>(*this));
    }
#endif//NANO_BOOST_SERIALIZATION_SUPPORT
};

/// binds
template <typename T>
template <typename Other>
void Entity<T>::Bind(const generic::utils::Index<Id<Other>, nano::Index> & other)
{
    if (m_binding == INVALID_INDEX)
        m_binding = Index(nano::Create<Binding>());
    Id<Binding>(m_binding)->Set<Other>(other);
}

template <typename T>
template <typename Other>
void Entity<T>::Unbind()
{
    if(m_binding != INVALID_INDEX)
        Id<Binding>(m_binding)->Unset<Other>();
}

template <typename T>
template <typename Other>
CId<Other> Entity<T>::GetBind() const
{
    if(m_binding != INVALID_INDEX)
        return CId<Binding>(m_binding)->Get<Other>();
    return CId<Other>();
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
        Index id;
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
} // namespace boost::serialization
#endif//NANO_BOOST_SERIALIZATION_SUPPORT