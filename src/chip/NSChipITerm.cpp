#include <nano/core/chip>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::chip::ITerm)

namespace nano::chip {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void ITerm::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(ITerm);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(ITerm)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

ITerm::ITerm(const std::string& name, Id<Inst> inst, Id<Net> net, IOType ioType)
 : NamedObj(name)
{
    m_.inst = inst;
    m_.net = net;
    m_.ioType = ioType;
}

} // namespace nano::chip