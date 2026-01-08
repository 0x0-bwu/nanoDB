#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::LibrarySet)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT
template <typename Archive>
void LibrarySet::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(LibrarySet);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}

NS_SERIALIZATION_FUNCTIONS_IMP(LibrarySet)
#endif//NANO_BOOST_SERIALIZATION_SUPPORT

LibrarySet::LibrarySet(std::string name)
 : NamedObj(std::move(name))
{
    NS_CLASS_MEMBERS_INITIALIZE
}

LibrarySet::LibrarySet()
 : LibrarySet("")
{
}

void LibrarySet::AddLibrary(Id<Library> library)
{
    NS_ASSERT(library);
    m_.libraries.emplace_back(library);
}

} // namespace nano::liberty