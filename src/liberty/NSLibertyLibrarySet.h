#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class Library;
class LibrarySet : public NamedObj, public Entity<LibrarySet>
{
public:
    explicit LibrarySet(std::string name);
    LibrarySet();

    void AddLibrary(Id<Library> library);

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (IdVec<Library, NameLut>, libraries)
    )
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::LibrarySet)