#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::Library)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Library::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Library);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Library)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Library::Library(const std::string& name)
 : NamedObj(name)
{
    NS_CLASS_MEMBERS_INITIALIZE
}

Library::Library()
 : Library("")
{
}

void Library::SetFilename(std::string_view filename)
{
    m_.filename = filename;
}

std::string_view Library::GetFilename() const
{
    return m_.filename;
}

void Library::AddCell(Id<Cell> cell)
{
    m_.cells.emplace_back(cell);
}

size_t Library::NumOfCells() const
{
    return m_.cells.size();
}

} // namespace nano::liberty