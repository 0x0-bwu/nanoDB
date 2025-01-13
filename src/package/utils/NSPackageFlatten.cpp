#include "NSPackageFlatten.h"
#include <nano/core/package>

#include <boost/algorithm/string.hpp>
namespace nano::package::utils {

bool FlattenUtility::Merge(Id<Layout> layout, Id<Layout> other, std::string_view prefix)
{
    auto sep = HierObj::GetHierSep();
    auto hierName = [&](std::string_view name) {
        return std::string(prefix) + std::string(sep) + std::string(name);
    };

    //Net
    auto netIter = other->GetNetIter();
    while (auto net = netIter.Next()) {
        net->Rename(hierName(net->GetName()));
        layout->AddNet(net);
    }

    //Component
    auto compIter = other->GetComponentIter();
    while (auto comp = compIter.Next()) {
        comp->Rename(hierName(comp->GetName()));
        layout->AddComponent(comp);
    }

    //ConnObj
    auto connObjIter = other->GetConnObjIter();
    while (auto connObj = connObjIter.Next()) {
        if (auto bw = connObj->GetBondingWire(); bw) {
            bw->Rename(hierName(bw->GetName()));
        }
        layout->AddConnObj(connObj);
    }
    return true;
}
} // namespace nano::package::utils
