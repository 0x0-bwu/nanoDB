#pragma once
#include <nano/common>

namespace nano::parasitic {

Id<Parasitic> ReadSpef(std::string_view filename);

class Parasitic : public Entity<Parasitic>
{
public:
    friend Id<Parasitic> ReadSpef(std::string_view filename);
    explicit Parasitic(std::string filename);
    Parasitic() = default;

    Id<Net> FindNet(std::string_view name) const;

    void ClearNets(bool remove);
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION 
    NS_CLASS_MEMBERS_DEFINE(
    (std::string, filename),
    (IdVec<Net, NameLut>, nets))   
};
    
} // namespace nano::parasitic
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::parasitic::Parasitic)