#pragma once
#include "basic/NSDatabase.h"
#include "basic/NSContainer.hpp"
#include "common/NSCommon.hpp"
namespace nano::parasitic {

ParasiticId ReadSpef(std::string_view filename);

class Parasitic : public Entity<Parasitic>
{
public:
    friend ParasiticId ReadSpef(std::string_view filename);
    explicit Parasitic(std::string filename);
    Parasitic() = default;

    NetId FindNet(std::string_view name) const;

    void ClearNets(bool remove);
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION 
    NS_DEFINE_CLASS_MEMBERS(
    (std::string, filename),
    (IdVec<Net, lut::Name>, nets))   
};
    
} // namespace nano::parasitic
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::parasitic::Parasitic)