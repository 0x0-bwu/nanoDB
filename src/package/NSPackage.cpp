#include <nano/core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Package)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Package::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(FootprintCell);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Package)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Package::Package(std::string name)
 : FootprintCell(std::move(name), CId<Package>())
{
    NS_CLASS_MEMBERS_INITIALIZE
}

Package::Package() : Package("")
{
}

Id<Cell> Package::AddCell(Id<Cell> cell)
{
    return m_.cells.Add(cell);
}

Id<Padstack> Package::AddPadstack(Id<Padstack> padstack)
{
    return m_.padstacks.Add(padstack);
}

Id<StackupLayer> Package::AddStackupLayer(Id<StackupLayer> layer)
{
    return m_.stackupLayers.Add(layer);
}

void Package::SortStackupLayers()
{
    m_.stackupLayers.Sort([&](const Id<StackupLayer> & lhs, const Id<StackupLayer> & rhs) {
        return lhs->GetElevation() > rhs->GetElevation();
    });
}

CId<StackupLayer> Package::GetTopStackupLayer() const
{
    return m_.stackupLayers.front();
}

CId<StackupLayer> Package::GetBotStackupLayer() const
{
    return m_.stackupLayers.back();
}

void Package::GetStackupLayerRange(CId<StackupLayer> top, CId<StackupLayer> bot, Vec<CId<StackupLayer>> & range) const
{
    range.clear();
    bool inRange = false;
    for (const auto & layer : m_.stackupLayers) {
        if (layer == top) inRange = true;
        if (inRange) range.emplace_back(layer);
        if (layer == bot) {
            range.emplace_back(layer);
            break;
        }
    }
}

CId<StackupLayer> Package::FindStackupLayer(std::string_view name) const
{
    return m_.stackupLayers.Lookup<lut::Name>(name);
}

Id<StackupLayer> Package::FindStackupLayer(std::string_view name)
{
    return m_.stackupLayers.Lookup<lut::Name>(name);
}

CId<Cell> Package::FindCell(std::string_view name) const
{
    return m_.cells.Lookup<lut::Name>(name);
}

Id<Cell> Package::FindCell(std::string_view name)
{
    return m_.cells.Lookup<lut::Name>(name);
}

Id<CellInst> Package::SetTop(Id<CellInst> top)
{
    m_.top = top;
    return GetTop();
}

auto Package::GetCellIter()
{
    return m_.cells.GetIter<Cell>();
}

auto Package::GetCellIter() const
{
    return m_.cells.GetCIter<Cell>();
}

auto Package::GetPadstackIter()
{
    return m_.padstacks.GetIter<Padstack>();
}

auto Package::GetPadstackIter() const
{
    return m_.padstacks.GetCIter<Padstack>();
}  

Float Package::GetHeight() const
{
    return m_.stackupLayers.front()->GetElevation() - 
           m_.stackupLayers.back()->GetElevation() +
           m_.stackupLayers.back()->GetThickness();
}

bool Package::isBlackBox() const
{
    return false;
}

} // namespace nano::package