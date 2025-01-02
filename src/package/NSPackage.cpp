#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Package)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Package::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Package);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Package)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Package::Package(std::string name)
 : NamedObj(std::move(name))
{
}

void Package::AddCell(CellId cell)
{
    m_.cells.Add(cell);
}

void Package::AddPadstack(PadstackId padstack)
{
    m_.padstacks.Add(padstack);
}

void Package::AddStackupLayer(StackupLayerId layer)
{
    m_.stackupLayers.Add(layer);
}

void Package::AddComponentLayer(ComponentLayerId layer)
{
    m_.componentLayers.Add(layer);
}

void Package::SortStackupLayers(bool ascending)
{
    m_.stackupLayers.Sort([&](const StackupLayerId & lhs, const StackupLayerId & rhs) {
        return ascending ? lhs->GetElevation() < rhs->GetElevation() : lhs->GetElevation() > rhs->GetElevation();
    });
}

LayerId Package::FindStackupLayer(const std::string & name) const
{
    return m_.stackupLayers.Lookup<lut::Name>(name);
}

} // namespace nano::package