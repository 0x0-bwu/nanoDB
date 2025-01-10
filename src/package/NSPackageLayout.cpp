#include <core/package>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::package::Layout)

namespace nano::package {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Layout::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Layout);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Layout)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Layout::Layout(CId<CircuitCell> cell)
{
    m_.cell = cell;
}

void Layout::SetBoundary(Id<Shape> boundary)
{
    m_.boundary = boundary;
}

Id<Net> Layout::AddNet(Id<Net> net)
{
    return m_.nets.Add(net);
}

Id<ConnObj> Layout::AddConnObj(Id<ConnObj> connObj)
{
    return m_.connObjs.Add(connObj);
}

Id<Component> Layout::AddComponent(Id<Component> component)
{
    return m_.components.Add(component);
}

Ptr<Layout> Layout::CloneFrom(const Layout & src)
{
    m_.cell = src.m_.cell;
    m_.boundary = src.m_.boundary->Clone();
    
    m_.nets = src.m_.nets.Clone();
    HashMap<CId<Net>, CId<Net>> netMap{{CId<Net>(), CId<Net>()}};
    for (auto [src, tar] : Zip(src.m_.nets, m_.nets)) {
        netMap.emplace(src, tar);
    }
    
    m_.components = src.m_.components.Clone();
    HashMap<CId<Component>, CId<Component>> compMap{{CId<Component>(), CId<Component>()}};
    HashMap<CId<ComponentPin>, CId<ComponentPin>> compPinMap{{CId<ComponentPin>(), CId<ComponentPin>()}};
    HashMap<CId<ComponentLayer>, CId<ComponentLayer>> compLayerMap{{CId<ComponentLayer>(), CId<ComponentLayer>()}};
    for(auto [src, tar] : Zip(src.m_.components, m_.components)) {
        tar->SetLayout(CId<Layout>(GetCId()));
        compMap.emplace(src, tar);
        auto srcLayerIter = src->GetComponentLayerIter();
        auto tarLayerIter = tar->GetComponentLayerIter();
        while (auto srcLayer = srcLayerIter.Next()) {
            auto tarLayer = tarLayerIter.Next();
            NS_ASSERT(tarLayer);
            auto srcPinIter = srcLayer->GetComponentPinIter();
            auto tarPinIter = tarLayer->GetComponentPinIter();
            while (auto srcPin = srcPinIter.Next()) {
                auto tarPin = tarPinIter.Next();
                NS_ASSERT(tarPin);
                compPinMap.emplace(srcPin, tarPin);
            }
        }
    }

    m_.connObjs = src.m_.connObjs.Clone();
    for (auto & connObj : m_.connObjs) {
        connObj->SetNet(netMap.at(connObj->GetNet()));
        if (auto bw = connObj->GetBondingWire(); bw) {
            if (bw->GetStartPin())
                bw->SetStartPin(compPinMap.at(bw->GetStartPin()));
            else if (auto compLayer = CId<ComponentLayer>(bw->GetStartLayer()); compLayer)
                bw->SetStartLayer(compLayerMap.at(compLayer));
            if (bw->GetEndPin())
                bw->SetEndPin(compPinMap.at(bw->GetEndPin()));
            else if (auto compLayer = CId<ComponentLayer>(bw->GetEndLayer()); compLayer)
                bw->SetEndLayer(compLayerMap.at(compLayer));
        }
    }
    return this;
}

} // namespace nano::package