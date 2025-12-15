#include "NSDatabase.h"
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::NamedObj)

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
: m_name(std::move(name))
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

void Database::SetCurrentDir(std::string dir)
{
    Instance().SetCurrentDirImpl(std::move(dir));
}

bool Database::Create(std::string name)
{
    return Instance().CreateImpl(name) ? SetCurrent(name.c_str()) : false;
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

void Database::SetCurrentDirImpl(std::string dir)
{
    currentDir = std::move(dir);
    generic::fs::CreateDir(currentDir);
}

bool Database::CreateImpl(std::string name)
{
    if (name.empty()) return false;
    auto iter = m_contents.find(name.c_str());
    if (iter != m_contents.end()) {
        NS_ERROR("database %1% already exists!", name.c_str());
        return false;
    }
    auto c = std::make_unique<Content>(std::move(name));
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

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

} // namespace nano