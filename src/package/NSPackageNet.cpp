#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Net)

namespace nano::package {

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

Net::Net(std::string name, CId<Layout> layout)
 : NamedObj(std::move(name))
{
    m_.layout = layout;
}

Ptr<Net> Net::CloneFrom(const Net & src)
{
    NamedObj::CloneFrom(src);
    m_ = src.m_;
    return this;
}

} // namespace nano::package
