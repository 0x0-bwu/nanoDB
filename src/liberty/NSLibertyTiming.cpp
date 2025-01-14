#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::Timing)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Timing::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Timing);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Timing)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Timing::Timing(Id<SignalPin> pin)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.pin = pin;
}

Timing::Timing() : Timing(Id<SignalPin>())
{
}

Id<SignalPin> Timing::GetPin() const
{
    return m_.pin;
}
} // namespace nano::liberty