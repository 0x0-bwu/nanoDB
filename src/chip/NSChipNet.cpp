#include <nano/core/chip>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::chip::Net)

namespace nano::chip {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Net::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Net);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}

NS_SERIALIZATION_FUNCTIONS_IMP(Net)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Net::Net(std::string name, Id<Block> block)
 : NamedObj(std::move(name))
{
    m_.block = block;
}

} // namespace nano::chip