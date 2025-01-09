#include "NSPackageFlatten.h"
#include <core/package>

#include <boost/algorithm/string.hpp>
namespace nano::package::utils {

bool FlattenUtility::Merge(Id<Layout> layout, CId<Layout> other, const Transform2D & trans, std::string_view prefix)
{
    auto sep = HierObj::GetHierSep();
    auto hierName = [&](std::string_view name) {
        return std::string(prefix) + std::string(sep) + std::string(name);
    };

    //Net
    std::unordered_map<CId<Net>, CId<Net>> netMap{{CId<Net>(), CId<Net>()}};
    auto netIter = other->GetNetCIter();
    while (auto net = netIter.Next()) {
        auto clone = net->Clone(hierName(net->GetName()));
        netMap.emplace(net, layout->AddNet(clone));
    }

    //ConnObj
    auto connObjIter = other->GetConnObjIter();
    while (auto connObj = connObjIter.Next()) {
        auto clone = Id<ConnObj>();
        if (auto bw = connObj->GetBondingWire(); bw) {
            auto bwClone = bw->Clone<BondingWire>(hierName(bw->GetName()));
            bwClone->Transform(trans);
            //todo
            clone = bwClone;
        }
        else if (auto rw = connObj->GetRoutingWire(); rw) {
            auto rwClone = rw->Clone<RoutingWire>();
            rwClone->Transform(trans);
            clone = rwClone;
        }
        else if (auto ps = connObj->GetPadstackInst(); ps) {
            auto psClone = ps->Clone<PadstackInst>();
            psClone->AddTransform(trans);
            clone = psClone;
        }
        NS_ASSERT(clone);
        clone->SetNet(netMap.at(connObj->GetNet()));
    }


    return true;
}
} // namespace nano::package::utils
