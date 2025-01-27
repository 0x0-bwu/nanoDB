#include "NSPackageExtension.h"
#include "kicad/NSKiCadExtension.h"
#include <nano/core/package>
namespace nano::package::extension {

Id<Package> CreateFromKiCad(std::string_view filename)
{
    return kicad::KiCadExtension().Load(filename);
}

} // namespace nano::package::extension