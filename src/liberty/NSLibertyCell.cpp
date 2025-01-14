#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::Cell)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Cell::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Cell);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Cell)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Cell::Cell(std::string name, Id<Library> library)
 : NamedObj(std::move(name))
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.library = library;
}

Cell::Cell()
 : Cell("", Id<Library>())
{
}

void Cell::AddPwrGndPin(Id<PwrGndPin> pin)
{
    NS_ASSERT(Identical(pin->GetCell()));
    m_.pgPins.emplace_back(pin);
}

void Cell::AddSignalPin(Id<SignalPin> pin)
{
    NS_ASSERT(Identical(pin->GetCell()));
    m_.signalPins.emplace_back(pin);
}

size_t Cell::NumOfPwrGndPins() const
{
    return m_.pgPins.size();
}

size_t Cell::NumOfSignalPins() const
{
    return m_.signalPins.size();
}

} // namespace nano::liberty