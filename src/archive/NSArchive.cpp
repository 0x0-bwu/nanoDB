#include "NSArchive.h"
#include <nano/db>

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

namespace nano::archive {


namespace detail {

template <typename... Args>
bool Save(const Collection<Args...> & collection, std::string_view filename, Format fmt)
{
    unsigned int version = CURRENT_VERSION.toInt();
    return generic::archive::Save(collection, version, filename, fmt);
}

template <typename... Args>
bool Load(std::string_view filename, Format fmt, Collection<Args...> & collection)
{
    unsigned int version{0};
    if (generic::archive::Load(collection, version, filename, fmt)) {
        SetCurrentDir(generic::fs::DirName(filename).string());
        collection.m_version = Version(version);
        return true;
    }
    return false;
}

template bool Save(const Content & content, std::string_view filename, Format fmt);
template bool Load(std::string_view filename, Format fmt, Content & content);

} // namespace detail

bool Save(std::string_view name, std::string_view filename, Format fmt)
{
    if (auto content = Database::Find(name); content) {
        return detail::Save(*content, filename, fmt);
    }
    return false;
}

bool SaveCurrent(std::string_view filename, Format fmt)
{
    return detail::Save(Database::Current(), filename, fmt);
}

bool Load(std::string_view filename, Format fmt)
{
    auto & db = Database::Instance();
    auto c = std::make_unique<Content>("");
    db.m_current = c.get();
    if (detail::Load(filename, fmt, *db.m_current)) {
        if (Database::Find(c->GetName().data())) {
            NS_ERROR("failed to load, database %1% alread exists!", c->GetName());
        }
        else {
            auto res = db.m_contents.emplace(c->GetName(), std::move(c));
            db.m_current = res.second ? res.first->second.get() : nullptr;
            InitLog(CurrentDir());
            return res.second;
        }
    }
    db.m_current = nullptr;
    return false;
}

} // namespace nano::archive

#endif //NANO_BOOST_SERIALIZATION_SUPPORT