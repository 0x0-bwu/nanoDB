#include <nano/core/chip>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::chip::Chip)

namespace nano::chip {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Chip::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Chip);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Chip)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Chip::Chip(const std::string& name)
 : NamedObj(name)
{
}

void Chip::AddBlock(Id<Block> block)
{
    m_.blocks.emplace_back(block);
}

void Chip::SetTop(Id<Block> block)
{
    NS_ASSERT(std::find(m_.blocks.begin(), m_.blocks.end(), block) != m_.blocks.cend());
    m_.top = block;
}

Id<Block> Chip::GetTop() const
{
    return m_.top;
}

Id<Block> Chip::FindBlock(std::string_view name) const
{
    return m_.blocks.Lookup<lut::Name>(name);
}

} // namespace nano::chip