#include "NSDatabase.h"
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::NamedObj)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::Content)

#include "generic/tools/FileSystem.hpp"
#include "generic/common/Archive.hpp"

#include <nano/db>
namespace nano {

static std::string currentDir;

void CloseLog()
{
    generic::log::ShutDown();
}

void InitLog(std::string_view dirname)
{
    CloseLog();
    using namespace generic::log;
    auto logger = MultiSinksLogger("nano", {
        std::make_shared<FileSinkMT>(std::string(dirname) + "/nano.log"),
        std::make_shared<StreamSinkMT>(std::cout)});
    logger->SetLevel(generic::log::Level::Trace);
    generic::log::SetDefaultLogger(logger);
}

traits::NameIndexMap Binding::nameIndexMap = [](auto elements) {
    traits::NameIndexMap niMap;
    hana::for_each(elements, [&](auto e){
        using T = typename std::decay_t<decltype(hana::first(e))>::type;
        [[maybe_unused]] auto res = niMap.emplace(toString<T>(), typeid(T));
        NS_ASSERT_MSG(res.second, "duplicated type name!");
    });
    return niMap;
}(traits::elementNameMap);

traits::IndexNameMap Binding::indexNameMap = [](auto elements) {
    traits::IndexNameMap inMap;
    hana::for_each(elements, [&](auto e){
        using T = typename std::decay_t<decltype(hana::first(e))>::type;
        inMap.emplace(typeid(T), toString<T>());
    });
    return inMap;
}(traits::elementNameMap);

NamedObj::NamedObj(std::string name)
: m_name(name)
{
}

std::string_view NamedObj::GetName() const
{
    return m_name.c_str();
}

Database::Database()
{
    SetCurrentDirImpl(generic::fs::CurrentPath().string());
}

Content & Database::Current()
{
    return Instance().CurrentImpl();
}

std::string_view Database::CurrentDir()
{
    return currentDir.c_str();
}

void Database::SetCurrentDir(const std::string& dir)
{
    Instance().SetCurrentDirImpl(dir);
}

bool Database::Create(const std::string& name)
{
    return Instance().CreateImpl(name) ? SetCurrent(name) : false;
}

bool Database::SetCurrent(std::string_view name)
{
    if (Instance().SetCurrentImpl(name)) {
        InitLog(CurrentDir());
        return true;
    }
    return false;
}

Ptr<Content> Database::Find(std::string_view name)
{
    return Instance().FindImpl(name);
}

bool Database::Close(std::string_view name)
{
    if (Instance().CloseImpl(name)) {
        CloseLog();
        return true;
    }
    return false;
}

void Database::Shutdown()
{
    CloseLog();
    return Instance().ShutdownImpl();
}

Database & Database::Instance()
{
    static Database database;
    return database;
}

Content & Database::CurrentImpl()
{
    NS_ASSERT_MSG(nullptr != m_current, "need create or load database firstly!");
    return *m_current;
}

void Database::SetCurrentDirImpl(const std::string& dir)
{
    currentDir = dir;
    generic::fs::CreateDir(currentDir);
}

bool Database::CreateImpl(const std::string& name)
{
    if (name.empty()) return false;
    auto iter = m_contents.find(name);
    if (iter != m_contents.end()) {
        NS_ERROR("database %1% already exists!", name);
        return false;
    }
    auto c = std::make_unique<Content>(name);
    auto res = m_contents.emplace(c->GetName(), std::move(c));
    m_current = res.first->second.get();
    return res.second;
}

bool Database::SetCurrentImpl(std::string_view name)
{
    auto content = FindImpl(name);
    if (not content) return false;
    m_current = content;
    return true;
}

Ptr<Content> Database::FindImpl(std::string_view name)
{
    auto iter = m_contents.find(name);
    if (iter == m_contents.cend()) return nullptr;
    return iter->second.get();
}

CPtr<Content> Database::FindImpl(std::string_view name) const 
{
    return const_cast<Database&>(*this).FindImpl(name);
}

bool Database::CloseImpl(std::string_view name)
{
    bool res{false};
    if (m_current && name == m_current->GetName()) {
        res = m_contents.erase(name);
        if (res) m_current = nullptr;
    }
    return res;
}

void Database::ShutdownImpl()
{
    m_contents.clear();
    m_current = nullptr;
}

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void NamedObj::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & boost::serialization::make_nvp("name", m_name);
}
NS_SERIALIZATION_FUNCTIONS_IMP(NamedObj);

template <typename... Eles>
template <typename Archive>
void Collection<Eles...>::serialize(Archive & ar, const unsigned int version)
{
    if constexpr (Archive::is_loading::value) Reset();
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    hana::for_each(m_data, [&](auto & c){
        using T = typename std::decay_t<decltype(hana::first(c))>::type;
        boost::serialization::serialize(ar,  m_data[hana::type<T>()], version);
    });
}

NS_SERIALIZATION_FUNCTIONS_IMP(Content);

template <typename... Eles>
bool Collection<Eles...>::Save(std::string_view filename, ArchiveFormat fmt) const
{
    unsigned int version = CURRENT_VERSION.toInt();
    return nano::Save(*this, version, filename, fmt);
}

template <typename... Eles>
bool Collection<Eles...>::Load(std::string_view filename, ArchiveFormat fmt)
{
    unsigned int version{0};
    if (nano::Load(*this, version, filename, fmt)) {
        SetCurrentDir(generic::fs::DirName(filename).string());
        m_version = Version(version);
        return true;
    }
    return false;
}

template bool Content::Save(std::string_view filename, ArchiveFormat fmt) const;
template bool Content::Load(std::string_view filename, ArchiveFormat fmt);

bool Database::Save(std::string_view name, std::string_view filename, ArchiveFormat fmt)
{
    return Instance().SaveImpl(name, filename, fmt);
}

bool Database::SaveCurrent(std::string_view filename, ArchiveFormat fmt)
{
    return Instance().SaveImpl(Current().GetName(), filename, fmt);
}

bool Database::Load(std::string_view filename, ArchiveFormat fmt)
{
    if (Instance().LoadImpl(filename, fmt)) {
        InitLog(CurrentDir());
        return true;
    }
    return false;
}

bool Database::SaveImpl(std::string_view name, std::string_view filename, ArchiveFormat fmt) const
{
    auto content = FindImpl(name);
    if (content) return content->Save(filename, fmt);
    return false;
}

bool Database::LoadImpl(std::string_view filename, ArchiveFormat fmt)
{
    auto c = std::make_unique<Content>("");
    m_current = c.get();
    if (c->Load(filename, fmt)) {
        if (FindImpl(c->GetName().data())) {
            NS_ERROR("failed to load, database %1% alread exists!", c->GetName());
        }
        else {
            auto res = m_contents.emplace(c->GetName(), std::move(c));
            m_current = res.second ? res.first->second.get() : nullptr;
            return res.second;
        }
    }
    m_current = nullptr;
    return false;
}
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

} // namespace nano