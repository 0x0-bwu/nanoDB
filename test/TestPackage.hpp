#pragma once
#include "TestCommon.hpp"
#include <nano/core/package>

using namespace boost::unit_test;

using LossPowers = nano::IdVec<nano::power::LossPower, nano::NameLut>;
namespace detail {

using namespace nano;
using namespace package;

LossPowers CreateLossPowers()
{
    using namespace nano;
    LossPowers lossPowers;
    auto diodeLut = nano::Create<LookupTable1D>(
        Vec<Float>{TempUnit(25).inKelvins(), TempUnit(125).inKelvins(), TempUnit(150).inKelvins()}, Vec<Float>{20.4, 21.7, 21.8});
    auto dieLut = nano::Create<LookupTable1D>(
        Vec<Float>{TempUnit(25).inKelvins(), TempUnit(125).inKelvins(), TempUnit(150).inKelvins()}, Vec<Float>{108.0, 124.0, 126.5});
    lossPowers.emplace_back(nano::Create<power::LossPower>("Diode", ScenarioId(0), diodeLut));
    lossPowers.emplace_back(nano::Create<power::LossPower>("TopDie", ScenarioId(1), dieLut));
    lossPowers.emplace_back(nano::Create<power::LossPower>("BotDie", ScenarioId(2), dieLut));
    return lossPowers;
}

void SetupMaterials(Id<Package> pkg)
{
    using namespace nano;
    pkg->SetMaterialLib(nano::Create<MaterialLib>("mat_lib"));
    BOOST_CHECK(pkg->GetMaterialLib());
    auto matLib = pkg->GetMaterialLib();
    BOOST_CHECK(matLib and matLib->GetName() == "mat_lib");
    
    auto matAl = nano::Create<Material>("Al");
    matAl->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropValue>(238));
    matAl->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropValue>(880));
    matAl->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(2700));
    matAl->SetProperty(Material::RESISTIVITY, nano::Create<MaterialPropValue>(2.82e-8));
    matLib->AddMaterial(matAl);

    auto matCu = nano::Create<Material>("Cu");
    matCu->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropPolynomial>(Vec<Vec<Float>>{{437.6, -0.165, 1.825e-4, -1.427e-7, 3.979e-11}}));
    matCu->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropPolynomial>(Vec<Vec<Float>>{{342.8, 0.134, 5.535e-5, -1.971e-7, 1.141e-10}}));
    matCu->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(8850));
    matLib->AddMaterial(matCu);

    auto matAir = nano::Create<Material>("Air");
    matAir->SetMaterialType(MaterialType::FLUID);
    matAir->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropValue>(0.026));
    matAir->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropValue>(1.003));
    matAir->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(1.225));
    matLib->AddMaterial(matAir);

    auto matSiC = nano::Create<Material>("SiC");
    matSiC->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropPolynomial>(Vec<Vec<Float>>{{1860, -11.7, 0.03442, -4.869e-5, 2.675e-8}}));
    matSiC->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropPolynomial>(Vec<Vec<Float>>{{-3338, 33.12, -0.1037, 0.0001522, -8.553e-8}}));
    matSiC->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(3210));
    matLib->AddMaterial(matSiC);

    auto matAlN = nano::Create<Material>("AlN");
    matAlN->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropPolynomial>(Vec<Vec<Float>>{{421.7867, -1.1262, 0.001}}));
    matAlN->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropPolynomial>(Vec<Vec<Float>>{{170.2, -2.018, 0.032, -8.957e-5, 1.032e-7, -4.352e-11}}));
    matAlN->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(3260));
    matLib->AddMaterial(matAlN);
    
    auto matSolder = nano::Create<Material>("Solder");
    matSolder->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropValue>(55));
    matSolder->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropValue>(218));
    matSolder->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(7800));
    matSolder->SetProperty(Material::RESISTIVITY, nano::Create<MaterialPropValue>(11.4e-8));
    matLib->AddMaterial(matSolder);

    auto matFR4 = nano::Create<Material>("FR4");
    matFR4->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropValue>(0.3));
    matFR4->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropValue>(700));
    matFR4->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(1700));
    matLib->AddMaterial(matFR4);
}

Id<Padstack> CreateBondingWireSolderJoints(Id<Package> pkg, std::string name, Float bwRadius)
{
    auto padstack = nano::Create<Padstack>(std::move(name), pkg);
    pkg->AddPadstack(padstack);

    padstack->SetTopSolderBumpMaterial(pkg->GetMaterialLib()->FindMaterial("Solder"));
    padstack->SetBotSolderBallMaterial(pkg->GetMaterialLib()->FindMaterial("Solder"));
    Float bumpR = 1.1 * bwRadius;
    auto topBump = nano::Create<ShapeRect>(pkg->GetCoordUnit(), FCoord2D{-bumpR, -bumpR}, FCoord2D{bumpR, bumpR});
    auto botBall = nano::Create<ShapeRect>(pkg->GetCoordUnit(), FCoord2D{-bumpR, -bumpR}, FCoord2D{bumpR, bumpR});
    padstack->SetTopSolderBumpParameters(topBump, 0.05);
    padstack->SetBotSolderBallParameters(botBall, 0.05);   
    return padstack; 
}

Id<FootprintCell> CreateSicFootprintCell(Id<Package> pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto sicDie = nano::Create<FootprintCell>("SicDie", pkg);
    sicDie->SetComponentType(ComponentType::IC);
    auto boundary = nano::Create<ShapeRect>(coordUnit, FCoord2D(-2.545, -2.02), FCoord2D(2.545, 2.02));
    sicDie->SetBoundary(boundary);
    auto sic = pkg->GetMaterialLib()->FindMaterial("SiC");
    sicDie->SetMaterial(sic);
    sicDie->SetHeight(0.18);

    auto topFootprint = sicDie->AddFootprint(nano::Create<Footprint>("Top", sicDie, FootprintLocation::TOP));
    topFootprint->SetSolderBallBumpThickness(0.1);
    auto solder = pkg->GetMaterialLib()->FindMaterial("Solder");
    topFootprint->SetSolderFillingMaterial(solder);
    topFootprint->SetSolderMaterial(solder);
    topFootprint->SetBoundary(boundary);

    topFootprint->AddPin(nano::Create<FootprintPin>("G", topFootprint, coordUnit.toCoord(FCoord2D(-1.25,  1.0)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("B", topFootprint, coordUnit.toCoord(FCoord2D(-1.25,  0.0)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("D", topFootprint, coordUnit.toCoord(FCoord2D(-1.25, -1.0)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("A", topFootprint, coordUnit.toCoord(FCoord2D( 1.25,  1.0)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("C", topFootprint, coordUnit.toCoord(FCoord2D( 1.25,  0.0)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("E", topFootprint, coordUnit.toCoord(FCoord2D( 1.25, -1.0)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("K", topFootprint, coordUnit.toCoord(FCoord2D(-2.00, -0.5)), IOType::INPUT));

    auto botFootprint = sicDie->AddFootprint(nano::Create<Footprint>("Bot", sicDie, FootprintLocation::BOT));
    botFootprint->SetSolderBallBumpThickness(0.1);
    botFootprint->SetSolderFillingMaterial(solder);
    botFootprint->SetSolderMaterial(solder);
    botFootprint->AddPin(nano::Create<FootprintPin>("GND", botFootprint, coordUnit.toCoord(FCoord2D(0,  0)), IOType::INPUT));
    botFootprint->SetBoundary(boundary);

    pkg->AddCell(sicDie);
    return sicDie;
}

Id<FootprintCell> CreateDiodeFootprintCell(Id<Package> pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto diode = nano::Create<FootprintCell>("Diode", pkg);
    diode->SetComponentType(ComponentType::IC);
    auto boundary = nano::Create<ShapeRect>(coordUnit, FCoord2D(-2.25, -2.25), FCoord2D(2.25, 2.25));
    diode->SetBoundary(boundary);
    auto sic = pkg->GetMaterialLib()->FindMaterial("SiC");
    diode->SetMaterial(sic);
    diode->SetHeight(0.18);

    auto topFootprint = diode->AddFootprint(nano::Create<Footprint>("Top", diode, FootprintLocation::TOP));
    topFootprint->SetSolderBallBumpThickness(0.1);
    auto solder = pkg->GetMaterialLib()->FindMaterial("Solder");
    topFootprint->SetSolderFillingMaterial(solder);
    topFootprint->SetSolderMaterial(solder);
    topFootprint->SetBoundary(boundary);

    topFootprint->AddPin(nano::Create<FootprintPin>("A", topFootprint, coordUnit.toCoord(FCoord2D(-1.125,  1.50)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("B", topFootprint, coordUnit.toCoord(FCoord2D(-1.125,  0.75)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("C", topFootprint, coordUnit.toCoord(FCoord2D(-1.125,  0.00)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("D", topFootprint, coordUnit.toCoord(FCoord2D(-1.125, -0.75)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("E", topFootprint, coordUnit.toCoord(FCoord2D(-1.125, -1.50)), IOType::INPUT));

    topFootprint->AddPin(nano::Create<FootprintPin>("F", topFootprint, coordUnit.toCoord(FCoord2D( 1.125,  1.50)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("G", topFootprint, coordUnit.toCoord(FCoord2D( 1.125,  0.75)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("H", topFootprint, coordUnit.toCoord(FCoord2D( 1.125,  0.00)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("I", topFootprint, coordUnit.toCoord(FCoord2D( 1.125, -0.75)), IOType::INPUT));
    topFootprint->AddPin(nano::Create<FootprintPin>("J", topFootprint, coordUnit.toCoord(FCoord2D( 1.125, -1.50)), IOType::INPUT));

    auto botFootprint = diode->AddFootprint(nano::Create<Footprint>("Bot", diode, FootprintLocation::BOT));
    botFootprint->SetSolderBallBumpThickness(0.1);
    botFootprint->SetSolderFillingMaterial(solder);
    botFootprint->SetSolderMaterial(solder);
    botFootprint->SetBoundary(boundary);
    botFootprint->AddPin(nano::Create<FootprintPin>("GND", botFootprint, coordUnit.toCoord(FCoord2D(0,  0)), IOType::INPUT));

    pkg->AddCell(diode);
    return diode;
}

Id<FootprintCell> CreateGateResistanceFootprintCell(Id<Package> pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto res = nano::Create<FootprintCell>("Rg", pkg);
    res->SetComponentType(ComponentType::RESISTOR);
    auto boundary = nano::Create<ShapeRect>(coordUnit, FCoord2D(-1.05, -0.65), FCoord2D(1.05, 0.65));
    res->SetBoundary(boundary);
    auto sic = pkg->GetMaterialLib()->FindMaterial("SiC");
    res->SetMaterial(sic);
    res->SetHeight(0.5);

    auto solder = pkg->GetMaterialLib()->FindMaterial("Solder");
    auto botFootprint = res->AddFootprint(nano::Create<Footprint>("Bot", res, FootprintLocation::BOT));
    botFootprint->SetSolderBallBumpThickness(0.1);
    botFootprint->SetSolderFillingMaterial(solder);
    botFootprint->SetSolderMaterial(solder);
    botFootprint->SetBoundary(boundary);
    
    pkg->AddCell(res);
    return res;
}

Id<Layout> CreateBaseLayout(Id<Package> pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto cell = pkg->AddCell(nano::Create<CircuitCell>("Base", pkg));
    auto ccell = CId<CircuitCell>(cell);
    auto layout = Id<CircuitCell>(cell)->SetLayout(nano::Create<Layout>(CId<CircuitCell>(cell)));
    BOOST_CHECK(layout);

    auto boundary = nano::Create<ShapePolygonWithHoles>();
    auto outline = ShapePolygon(coordUnit, {{-52.2, -29.7}, {52.2, -29.7}, {52.5, 29.7}, {-52.2, 29.7}}, 5.3).GetOutline();
    boundary->SetOutline(outline);
    boundary->AddHole(ShapeCircle(coordUnit, {-46.5, -24}, 3.85).GetOutline());
    boundary->AddHole(ShapeCircle(coordUnit, { 46.5, -24}, 3.85).GetOutline());
    boundary->AddHole(ShapeCircle(coordUnit, { 46.5,  24}, 3.85).GetOutline());
    boundary->AddHole(ShapeCircle(coordUnit, {-46.5,  24}, 3.85).GetOutline());
    layout->SetBoundary(boundary);

    auto gateNet = layout->AddNet(nano::Create<Net>("Gate", layout));
    layout->AddNet(nano::Create<Net>("Drain", layout));
    layout->AddNet(nano::Create<Net>("Source", layout));
    layout->AddNet(nano::Create<Net>("Kelvin", layout));
    auto noNet = layout->AddNet(nano::Create<Net>("NoNet", layout));

    Vec<FCoord2D> dPLoc{
        {-3, 24}, {-3, 23.275}, {-3, 22.55}, {-4, 23.275}, {-4, 22.55}, {-3, 6.525}, {-3, 5.8}, {-3, 5.075}, {-4, 6.525}, {-4, 5.8},
    };
    Vec<FCoord2D> sPLoc{
        {3, 24}, {3, 23.275}, {3, 22.55}, {4, 21.825}, {3, 21.825}, {3, 6.525}, {3, 5.8}, {3, 5.075}, {3, 7.25}, {4, 7.25},
    };

    auto topCuLayer = pkg->FindStackupLayer("TopCuLayer");
    BOOST_CHECK(topCuLayer);
    for (size_t i = 0; i < dPLoc.size(); ++i) {
        auto bw1 = nano::Create<BondingWire>("DS1_" + std::to_string(i), noNet, 0.15);
        bw1->SetStartLayer(topCuLayer, coordUnit.toCoord(dPLoc[i]));
        bw1->SetEndLayer(topCuLayer, coordUnit.toCoord(sPLoc[i]));
        layout->AddConnObj(bw1);

        dPLoc[i][1] *= -1; sPLoc[i][1] *= -1;
        auto bw2 = nano::Create<BondingWire>("DS2_" + std::to_string(i), noNet, 0.15);
        bw2->SetStartLayer(topCuLayer, coordUnit.toCoord(dPLoc[i]));
        bw2->SetEndLayer(topCuLayer, coordUnit.toCoord(sPLoc[i]));
        layout->AddConnObj(bw2);
    }

    Vec<FCoord2D> gPLoc{{-3, 3}, {-3, 1.8}};
    for (size_t i = 0; i < gPLoc.size(); ++i) {
        const auto & p = gPLoc.at(i);
        auto bw1 = nano::Create<BondingWire>("G1_" + std::to_string(i), noNet, 0.0635);
        bw1->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(p[0], p[1])));
        bw1->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(-p[0], p[1])));
        layout->AddConnObj(bw1);

        auto bw2 = nano::Create<BondingWire>("G2_" + std::to_string(i), noNet, 0.0635);
        bw2->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(p[0], -p[1])));
        bw2->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(-p[0], -p[1])));
        layout->AddConnObj(bw2);
    }

    auto kelvinBw0 = nano::Create<BondingWire>("KelvinBw0", gateNet, 0.0635);
    kelvinBw0->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(30.15, -5.95)));
    kelvinBw0->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, -5.95)));
    layout->AddConnObj(kelvinBw0);

    auto kelvinBw = nano::Create<BondingWire>("KelvinBw", gateNet, 0.0635);
    kelvinBw->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(30.15, 5)));
    kelvinBw->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(30.15, -5)));
    layout->AddConnObj(kelvinBw);

    auto gateBw0 = nano::Create<BondingWire>("GateBw0", gateNet, 0.0635);
    gateBw0->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32, -12.375)));
    gateBw0->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, -12.375)));
    layout->AddConnObj(gateBw0);

    auto gateBw = nano::Create<BondingWire>("GateBw", gateNet, 0.0635);
    gateBw->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32, 5)));
    gateBw->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32, -5)));
    layout->AddConnObj(gateBw);

    auto gateBw1 = nano::Create<BondingWire>("GateBw1", gateNet, 0.0635);
    gateBw1->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32.5, 3.0)));
    gateBw1->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(41.3, 3.35)));
    layout->AddConnObj(gateBw1);

    auto gateBw2 = nano::Create<BondingWire>("GateBw2", gateNet, 0.0635);
    gateBw2->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32.5, 1.8)));
    gateBw2->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, 1.0375)));
    layout->AddConnObj(gateBw2);

    auto gateBw3 = nano::Create<BondingWire>("GateBw3", gateNet, 0.0635);
    gateBw3->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32.5, -1.8)));
    gateBw3->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, -0.3625)));
    layout->AddConnObj(gateBw3);

    auto gateBw4 = nano::Create<BondingWire>("GateBw4", gateNet, 0.0635);
    gateBw4->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32.5, -3)));
    gateBw4->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, -2.7)));
    layout->AddConnObj(gateBw4);

    return layout;
}

Id<Layout> CreateDriverLayout(Id<Package> pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto cell = nano::Create<CircuitCell>("Driver", pkg);
    auto layout = cell->SetLayout(nano::Create<Layout>(CId<CircuitCell>(cell)));
    BOOST_CHECK(layout);

    auto boundary = nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-5.5, -14.725}, {5.5, -14.725}, {5.5, 14.725}, {-5.5, 14.725}});
    layout->SetBoundary(boundary);

    auto noNet = layout->AddNet(nano::Create<Net>("NoNet", layout));
    auto layer1 = pkg->FindStackupLayer("TopCuLayer");
    auto layer2 = pkg->FindStackupLayer("CeramicLayer");
    auto layer3 = pkg->FindStackupLayer("BotCuLayer");
    auto layer4 = pkg->FindStackupLayer("SolderLayer");
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{1.7,   9.625}, {4.7,   9.625}, {4.7, 13.925}, {1.7, 13.925}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{1.7,   4.325}, {4.7,   4.325}, {4.7,  8.625}, {1.7,  8.625}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{1.7, -13.925}, {4.7, -13.925}, {4.7,  1.075}, {3.2,  1.075}, {3.2, -1.775}, 
                       {4.2, -1.775}, {4.2, -4.925}, {3.2, -4.925}, {3.2, -7.025}, {4.2, -7.025}, {4.2, -11.425}, {1.7, -11.425}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{1.7, -10.325}, {3.2, -10.325}, {3.2, -8.225}, {2.2, -8.225}, {2.2, -3.875}, 
                       {3.2, -3.875}, {3.2, -2.825}, {2.2, -2.825}, {2.2, 2.175}, {4.7, 2.175}, {4.7, 3.225}, {1.7, 3.225}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer2, 
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{0.9, -14.725}, {5.5, -14.725}, {5.5, 14.725}, {0.9, 14.725}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer3,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{1.4, -14.225}, {5.0, -14.225}, {5.0, 14.225}, {1.4, 14.225}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer4,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{0.9, -14.725}, {5.5, -14.725}, {5.5, 14.725}, {0.9, 14.725}}, 0.25)));
    
    return layout;
}

Id<Layout> CreateBotBridgeLayout(Id<Package> pkg, const Vec<FCoord2D> & locations, const LossPowers & lossPowers)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto cell = nano::Create<CircuitCell>("BotBridge", pkg);
    auto layout = cell->SetLayout(nano::Create<Layout>(CId<CircuitCell>(cell)));
    BOOST_CHECK(layout);

    auto boundary = nano::Create<ShapeRect>(coordUnit, FCoord2D(-16.75, -12.5), FCoord2D(16.75, 12.5));
    layout->SetBoundary(boundary);

    auto noNet = layout->AddNet(nano::Create<Net>("NoNet", layout));
    auto layer1 = pkg->FindStackupLayer("TopCuLayer");
    auto layer2 = pkg->FindStackupLayer("CeramicLayer");
    auto layer3 = pkg->FindStackupLayer("BotCuLayer");
    auto layer4 = pkg->FindStackupLayer("SolderLayer");
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-15.45, -11.2}, {13.35, -11.2}, {13.95, -11.6}, {15.45, -11.6},
                        {15.45, -10.8}, {-15.05, -10.8}, {-15.05, -1.3}, {-14.7, -0.7}, {-14.7, 11.45}, {-15.45, 11.45}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-14.2, -10.4}, {15.45, -10.4}, {15.45, -9.6}, {13.95, -9.6},
                        {13.35, -10}, {-13.8, -10}, {-13.8, -2.55}, {-11.1, -2.55}, {-11.1, 11.45}, {-11.85, 11.45}, {-11.85, -2}, {-14.2, -2}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-12.6, -8.8}, {12.35, -8.8}, {12.95, -8.4}, {15.45, -8.4},
                        {15.45, -5.97}, {7.95, -5.97}, {7.95, 9.03}, {15.45, 9.03}, {15.45, 11.45}, {-9.75, 11.45}, {-9.75, -3.75}, {-12.6, -3.75}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-13.65, -0.7}, {-12.9, -0.7}, {-12.9, 2.6}, {-13.65, 2.6}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-13.65, 3.725}, {-12.9, 3.725}, {-12.9, 7.025}, {-13.65, 7.025}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-13.65, 8.15}, {-12.9, 8.15}, {-12.9, 11.45}, {-13.65, 11.45}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{9.5, -4.77}, {15.8, -4.77}, {15.8, 7.83}, {9.5, 7.83}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer2,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-16.75, -12.5}, {16.75, -12.5}, {16.75, 12.5}, {-16.75, 12.5}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer3,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-16.25, -12}, {16.25, -12}, {16.25, 12}, {-16.25, 12}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer4,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-16.75, -12.5}, {16.75, -12.5}, {16.75, 12.5}, {-16.75, 12.5}}, 0.25)));

    auto botDieLossPower = lossPowers.Lookup<lut::Name>("BotDie");
    NS_ASSERT(botDieLossPower);
    IdArr3<Component> dieComps;
    auto sicDie = CId<FootprintCell>(pkg->FindCell("SicDie"));
    BOOST_CHECK(sicDie);
    auto sicBotFp = sicDie->FindFootprint("Bot");
    BOOST_CHECK(sicBotFp);
    auto sicTopFp = sicDie->FindFootprint("Top");
    BOOST_CHECK(sicTopFp);
    Vec<std::string> diePinNames{"G", "B", "D", "A", "C", "E", "K"};
    for (size_t i = 0; i < dieComps.size(); ++i) {
        dieComps[i] = layout->AddComponent(nano::Create<Component>("Die" + std::to_string(i + 1), sicDie, layout));
        dieComps[i]->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, locations.at(i)));
        auto botLayer = dieComps[i]->AddComponentLayer(nano::Create<ComponentLayer>("Bot", dieComps[i], sicBotFp));
        botLayer->AddPin(nano::Create<ComponentPin>("GND", botLayer, sicBotFp->FindPin("GND")));
        botLayer->SetConnectedLayer(layer1);

        auto topLayer = dieComps[i]->AddComponentLayer(nano::Create<ComponentLayer>("Top", dieComps[i], sicTopFp));
        for (const auto & pin : diePinNames)
            topLayer->AddPin(nano::Create<ComponentPin>(pin, topLayer, sicTopFp->FindPin(pin.c_str())));

        dieComps[i]->Bind<power::LossPower>(botDieLossPower);
    }
    auto diodeLossPower = lossPowers.Lookup<lut::Name>("Diode");
    NS_ASSERT(diodeLossPower);
    IdArr3<Component> diodeComps;
    auto diode = CId<FootprintCell>(pkg->FindCell("Diode"));
    BOOST_CHECK(diode);
    auto diodeBotFp = diode->FindFootprint("Bot");
    BOOST_CHECK(diodeBotFp);
    auto diodeTopFp = diode->FindFootprint("Top");
    BOOST_CHECK(diodeTopFp);
    Vec<std::string> diodePinNames{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    for (size_t i = 0; i < diodeComps.size(); ++i) {
        diodeComps[i] = layout->AddComponent(nano::Create<Component>("Diode" + std::to_string(i + 1), diode, layout));
        diodeComps[i]->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, locations.at(i + dieComps.size())));
        auto botLayer = diodeComps[i]->AddComponentLayer(nano::Create<ComponentLayer>("Bot", diodeComps[i], diodeBotFp));
        botLayer->AddPin(nano::Create<ComponentPin>("GND", botLayer, diodeBotFp->FindPin("GND")));
        botLayer->SetConnectedLayer(layer1);

        auto topLayer = diodeComps[i]->AddComponentLayer(nano::Create<ComponentLayer>("Top", diodeComps[i], diodeTopFp));
        for (const auto & pin : diodePinNames)
            topLayer->AddPin(nano::Create<ComponentPin>(pin, topLayer, diodeTopFp->FindPin(pin.c_str())));
        diodeComps[i]->Bind<power::LossPower>(diodeLossPower);
    }
    
    auto rg = CId<FootprintCell>(pkg->FindCell("Rg"));
    BOOST_CHECK(rg);
    Vec<FCoord2D> resLocs{{-14.17, 10.5}, {-14.17, 6.075}, {-14.17, 1.65}};
    for (size_t i = 0; i < resLocs.size(); ++i) {
        auto res = layout->AddComponent(nano::Create<Component>("R" + std::to_string(i + 1), rg, layout));
        res->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, resLocs.at(i)));
        auto compLayer = res->AddComponentLayer(nano::Create<ComponentLayer>("Bot", res, rg->FindFootprint("Bot")));
        compLayer->SetConnectedLayer(layer1);
    }

    Vec<FCoord2D> gateBwLocs{{-13.275f, 8.6f}, {-13.275f, 4.05f}, {-13.275f, -0.27f}};
    for (size_t i = 0; i < gateBwLocs.size(); ++i) {
        auto bw = nano::Create<BondingWire>("GateBw" + std::to_string(i), noNet, 0.0635);
        bw->SetStartPin(dieComps[i]->FindComponentPin("Top", "G"));
        bw->SetEndLayer(layer1, coordUnit.toCoord(gateBwLocs.at(i)));
        layout->AddConnObj(bw);
    }

    Vec<std::string> iPins{"A", "B", "C", "D", "E"};
    for (size_t i = 0; i < dieComps.size(); ++i) {
        for (size_t j = 0; j < iPins.size(); ++j) {
            auto bw = nano::Create<BondingWire>(iPins.at(j), noNet, 0.15);
            bw->SetStartPin(dieComps[i]->FindComponentPin("Top", iPins.at(j)));
            bw->SetEndPin(diodeComps[i]->FindComponentPin("Top", iPins.at(j)));
            layout->AddConnObj(bw);
        }
    }

    Vec<std::string> oPins{"F", "G", "H", "I", "J"};
    Vec<Vec<FCoord2D>> diodePLocs {
        {{12.650,  7.105}, {12.650,  6.380}, {11.075,  7.105}, {11.075,  6.380}, {11.075,  5.655}},
        {{14.225,  7.105}, {14.225,  6.380}, {12.650, -2.595}, {11.075, -2.595}, {11.075, -3.320}},
        {{12.650, -3.320}, {14.225, -3.320}, {11.075, -4.045}, {12.650, -4.045}, {14.225, -4.045}}
    };

    for (size_t i = 0; i < diodePLocs.size(); ++i) {
        for (size_t j = 0; j < oPins.size(); ++j) {
            auto bw = nano::Create<BondingWire>(oPins.at(j), noNet, 0.15);
            bw->SetStartPin(diodeComps[i]->FindComponentPin("Top", oPins.at(j)));
            bw->SetEndLayer(layer1, coordUnit.toCoord(diodePLocs.at(i).at(j)));
            layout->AddConnObj(bw);
        }
    }

    Vec<FCoord2D> kelvinBwPLocs{{-11.475, 8.15}, {-11.475, 3.6}, {-11.475, -0.72}};
    for (size_t i = 0; i < kelvinBwPLocs.size(); ++i) {
        auto bw = nano::Create<BondingWire>("KelvinBw" + std::to_string(i), noNet, 0.0635);
        bw->SetStartPin(dieComps[i]->FindComponentPin("Top", "K"));
        bw->SetEndLayer(layer1, coordUnit.toCoord(kelvinBwPLocs.at(i)));
        layout->AddConnObj(bw);
    }    
    return layout;
}


Id<Layout> CreateTopBridgeLayout(Id<Package> pkg, const Vec<FCoord2D> & locations, const LossPowers & lossPowers)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto cell = nano::Create<CircuitCell>("TopBridge", pkg);
    auto layout = cell->SetLayout(nano::Create<Layout>(CId<CircuitCell>(cell)));
    BOOST_CHECK(layout);

    auto boundary = nano::Create<ShapeRect>(coordUnit, FCoord2D(-16.75, -12.5), FCoord2D(16.75, 12.5));
    layout->SetBoundary(boundary);

    auto noNet = layout->AddNet(nano::Create<Net>("NoNet", layout));
    auto layer1 = pkg->FindStackupLayer("TopCuLayer");
    auto layer2 = pkg->FindStackupLayer("CeramicLayer");
    auto layer3 = pkg->FindStackupLayer("BotCuLayer");
    auto layer4 = pkg->FindStackupLayer("SolderLayer");
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-15.45, -11.6}, {-13.95, -11.6}, {-13.35, -11.2}, {13.35, -11.2},
                        {13.95, -11.6}, {15.45, -11.6}, {15.45, -10.8}, {-15.45, -10.8}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-15.45, -10.4}, {15.45, -10.4}, {15.45, -9.6}, {13.95, -9.6},
                        {13.35, -10}, {-13.35, -10}, {-13.95, -9.6}, {-15.45, -9.6}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-15.45, -8.4}, {-12.95, -8.4}, {-12.35, -8.8}, {-9.15, -8.8},
                        {-9.15, -3.1}, {-15.45, -3.1}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-15.45, -1.9}, {-7.95, -1.9}, {-7.95, -8.8}, {9.75, -8.8},
                        {9.75, 11.45}, {-7.95, 11.45}, {-7.95, 4.45}, {-15.45, 4.45}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-15.45, 5.65}, {-9.15, 5.65}, {-9.15, 11.45}, {-15.45, 11.45}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{11.1, -8.5}, {12.9, -8.5}, {12.9, -6.55}, {11.85, -6.55}, {11.85, 11.45}, {11.1, 11.45}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{12.9, -5.5}, {13.65, -5.5}, {13.65, -2.2}, {12.9, -2.2}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{12.9, 0.95}, {13.65, 0.95}, {13.65, 4.25}, {12.9, 4.25}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{12.9,  7.4}, {13.65,  7.4}, {13.65, 10.7}, {12.9, 10.7}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{13.65, -8.5}, {15.45, -8.5}, {15.45, 11.45}, {14.7, 11.45},
                        {14.7, -0.125}, {13.65, -0.125}, {13.65, -1.125}, {14.7, -1.125}, {14.7, -6.55}, {13.65, -6.55}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer2,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-16.75, -12.5}, {16.75, -12.5}, {16.75, 12.5}, {-16.75, 12.5}})));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer3,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-16.25, -12}, {16.25, -12}, {16.25, 12}, {-16.25, 12}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer4,
                       nano::Create<ShapePolygon>(coordUnit, Vec<FCoord2D>{{-16.75, -12.5}, {16.75, -12.5}, {16.75, 12.5}, {-16.75, 12.5}})));
    
    auto topDieLossPower = lossPowers.Lookup<lut::Name>("TopDie");
    NS_ASSERT(topDieLossPower);
    IdArr3<Component> dieComps;
    auto sicDie = CId<FootprintCell>(pkg->FindCell("SicDie"));
    BOOST_CHECK(sicDie);
    auto sicBotFp = sicDie->FindFootprint("Bot");
    BOOST_CHECK(sicBotFp);
    auto sicTopFp = sicDie->FindFootprint("Top");
    BOOST_CHECK(sicTopFp);
    Vec<std::string> diePinNames{"G", "B", "D", "A", "C", "E", "K"};
    for (size_t i = 0; i < dieComps.size(); ++i) {
        dieComps[i] = layout->AddComponent(nano::Create<Component>("Die" + std::to_string(i + 1), sicDie, layout));
        dieComps[i]->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, locations.at(i), Mirror2D::Y));
        auto botLayer = dieComps[i]->AddComponentLayer(nano::Create<ComponentLayer>("Bot", dieComps[i], sicBotFp));
        botLayer->AddPin(nano::Create<ComponentPin>("GND", botLayer, sicBotFp->FindPin("GND")));
        botLayer->SetConnectedLayer(layer1);

        auto topLayer = dieComps[i]->AddComponentLayer(nano::Create<ComponentLayer>("Top", dieComps[i], sicTopFp));
        for (const auto & pin : diePinNames)
            topLayer->AddPin(nano::Create<ComponentPin>(pin, topLayer, sicTopFp->FindPin(pin.c_str())));

        dieComps[i]->Bind<power::LossPower>(topDieLossPower);
    }
    auto diodeLossPower = lossPowers.Lookup<lut::Name>("Diode");
    NS_ASSERT(diodeLossPower);
    IdArr3<Component> diodeComps;
    auto diode = CId<FootprintCell>(pkg->FindCell("Diode"));
    BOOST_CHECK(diode);
    auto diodeBotFp = diode->FindFootprint("Bot");
    BOOST_CHECK(diodeBotFp);
    auto diodeTopFp = diode->FindFootprint("Top");
    BOOST_CHECK(diodeTopFp);
    Vec<std::string> diodePinNames{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    for (size_t i = 0; i < diodeComps.size(); ++i) {
        diodeComps[i] = layout->AddComponent(nano::Create<Component>("Diode" + std::to_string(i + 1), diode, layout));
        diodeComps[i]->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, locations.at(i + dieComps.size()), Mirror2D::Y));
        auto botLayer = diodeComps[i]->AddComponentLayer(nano::Create<ComponentLayer>("Bot", diodeComps[i], diodeBotFp));
        botLayer->AddPin(nano::Create<ComponentPin>("GND", botLayer, diodeBotFp->FindPin("GND")));
        botLayer->SetConnectedLayer(layer1);

        auto topLayer = diodeComps[i]->AddComponentLayer(nano::Create<ComponentLayer>("Top", diodeComps[i], diodeTopFp));
        for (const auto & pin : diodePinNames)
            topLayer->AddPin(nano::Create<ComponentPin>(pin, topLayer, diodeTopFp->FindPin(pin.c_str())));

        diodeComps[i]->Bind<power::LossPower>(diodeLossPower);
    }

    auto rg = CId<FootprintCell>(pkg->FindCell("Rg"));
    BOOST_CHECK(rg);
    Vec<FCoord2D> resLocs{{14.17, 8.35}, {14.17, 1.9}, {14.17, -4.55}};
    for (size_t i = 0; i < resLocs.size(); ++i) {
        auto res = layout->AddComponent(nano::Create<Component>("R" + std::to_string(i + 1), rg, layout));
        res->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, resLocs.at(i)));
        auto compLayer = res->AddComponentLayer(nano::Create<ComponentLayer>("Bot", res, rg->FindFootprint("Bot")));
        compLayer->SetConnectedLayer(layer1);
    }

    Vec<FCoord2D> gateBwLocs{{13.275f, 10.25f}, {13.275f, 3.8f}, {13.275f, -2.65f}};
    for (size_t i = 0; i < gateBwLocs.size(); ++i) {
        auto bw = nano::Create<BondingWire>("GateBw" + std::to_string(i), noNet, 0.0635);
        bw->SetStartPin(dieComps[i]->FindComponentPin("Top", "G"));
        bw->SetEndLayer(layer1, coordUnit.toCoord(gateBwLocs.at(i)));
        layout->AddConnObj(bw);
    }

    Vec<std::string> iPins{"A", "B", "C", "D", "E"};
    for (size_t i = 0; i < dieComps.size(); ++i) {
        for (size_t j = 0; j < iPins.size(); ++j) {
            auto bw = nano::Create<BondingWire>(iPins.at(j), noNet, 0.15);
            bw->SetStartPin(dieComps[i]->FindComponentPin("Top", iPins.at(j)));
            bw->SetEndPin(diodeComps[i]->FindComponentPin("Top", iPins.at(j)));
            layout->AddConnObj(bw);
        }
    }

    Vec<std::string> oPins{"F", "G", "H", "I", "J"};
    Vec<Vec<FCoord2D>> diodePLocs {
        {{-10.15, 10.7250}, {-10.15, 10.000}, {-10.15,  9.2700}, {-10.15,  8.5500}, {-10.15,  7.8250}},
        {{-10.15,  7.1000}, {-10.15,  6.375}, {-11.15,  6.3750}, {-10.15, -3.8125}, {-10.15, -4.5250}},
        {{-10.15, -5.2375}, {-10.15, -5.950}, {-10.15, -6.6625}, {-10.15, -7.3750}, {-10.15, -8.0875}}
    };

    for (size_t i = 0; i < diodePLocs.size(); ++i) {
        for (size_t j = 0; j < oPins.size(); ++j) {
            auto bw = nano::Create<BondingWire>(oPins.at(j), noNet, 0.15);
            bw->SetStartPin(diodeComps[i]->FindComponentPin("Top", oPins.at(j)));
            bw->SetEndLayer(layer1, coordUnit.toCoord(diodePLocs.at(i).at(j)));
            layout->AddConnObj(bw);
        }
    }

    Vec<FCoord2D> kelvinBwPLocs{{11.475, 8.08}, {11.475, 1.33}, {11.475, -5.42}};
    for (size_t i = 0; i < kelvinBwPLocs.size(); ++i) {
        auto bw = nano::Create<BondingWire>("KelvinBw" + std::to_string(i), noNet, 0.0635);
        bw->SetStartPin(dieComps[i]->FindComponentPin("Top", "K"));
        bw->SetEndLayer(layer1, coordUnit.toCoord(kelvinBwPLocs.at(i)));
        layout->AddConnObj(bw);
    }

    return layout;
}

} // namespace detail

void t_create_package()
{
    using namespace nano;
    using namespace nano::package;
    Vec<Float> parameters = {
        -5.23, 8.93, -5.23, 3.86, -5.23, -1.21, 3.71, 8.08, 3.71, 1.33, 3.71, -5.42,
        5.23, 8.08, 5.23, 1.33, 5.23, -5.42, -3.7, 8.08, -3.7, 1.33, -3.7, -5.42,
    };
    Database::Create("CAS300M12BM2");
    auto pkg = nano::Create<Package>("CAS300M12BM2");
    BOOST_CHECK(pkg);

    detail::SetupMaterials(pkg);

    CoordUnit coordUnit(CoordUnit::Unit::MILLIMETER);
    pkg->SetCoordUnit(coordUnit);

    auto lossPowers = detail::CreateLossPowers();

    //layers
    auto matAlN = pkg->GetMaterialLib()->FindMaterial("AlN");
    auto matAir = pkg->GetMaterialLib()->FindMaterial("Air");
    auto matCu = pkg->GetMaterialLib()->FindMaterial("Cu");
    auto matSold = pkg->GetMaterialLib()->FindMaterial("Solder");
    pkg->AddStackupLayer(nano::Create<StackupLayer>("TopCuLayer", LayerType::CONDUCTING, 0, 0.3, matCu, matAir));
    pkg->AddStackupLayer(nano::Create<StackupLayer>("CeramicLayer", LayerType::DIELECTRIC,  -0.3, 0.38, matAlN, matAir));
    pkg->AddStackupLayer(nano::Create<StackupLayer>("BotCuLayer", LayerType::CONDUCTING, -0.68, 0.3, matCu, matAir));
    pkg->AddStackupLayer(nano::Create<StackupLayer>("SolderLayer", LayerType::CONDUCTING, -0.98, 0.1, matSold, matAir));
    pkg->AddStackupLayer(nano::Create<StackupLayer>("BaseLayer", LayerType::CONDUCTING, -1.08, 3, matCu, matCu));
    pkg->SortStackupLayers();

    auto thinBwSolderDef = detail::CreateBondingWireSolderJoints(pkg, "Thin Solder Joints", 0.0635);
    auto thickBwSolderDef = detail::CreateBondingWireSolderJoints(pkg, "Thick Solder Joints", 0.15);

    //cells
    auto sicDieDef = detail::CreateSicFootprintCell(pkg);
    auto diodeDef = detail::CreateDiodeFootprintCell(pkg);
    auto gateResDef = detail::CreateGateResistanceFootprintCell(pkg);

    auto baseLayout = detail::CreateBaseLayout(pkg);
    auto base = pkg->SetTop(nano::Create<CellInst>("Base", baseLayout->GetCell()));

    auto driverLayout = detail::CreateDriverLayout(pkg); 
    auto driver = nano::Create<CellInst>("Driver", driverLayout->GetCell(), base);
    driver->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, {44, 0}, Mirror2D::XY));
    base->AddCellInst(driver);
    Vec<FCoord2D> botLocations;
    for (size_t i = 0; i < 6; ++i)
        botLocations.emplace_back(parameters.at(i * 2), parameters.at(i * 2 + 1));
    auto botBridgeLayout = detail::CreateBotBridgeLayout(pkg, botLocations, lossPowers);

    auto botBridgeInst1 = nano::Create<CellInst>("BotBridge1", botBridgeLayout->GetCell(), base);
    botBridgeInst1->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, {-17.75, 13}));
    base->AddCellInst(botBridgeInst1);
    auto botBridgeInst2 = nano::Create<CellInst>("BotBridge2", botBridgeLayout->GetCell(), base);
    botBridgeInst2->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, {-17.75, -13}, Mirror2D::X));
    base->AddCellInst(botBridgeInst2);

    Vec<FCoord2D> topLocations;
    for (size_t i = 0; i < 6; i++)
        topLocations.emplace_back(parameters.at(i * 2 + 12), parameters.at(i * 2 + 13));
    auto topBridgeLayout = detail::CreateTopBridgeLayout(pkg, topLocations, lossPowers);

    auto topBridgeInst1 = nano::Create<CellInst>("TopBridge1", topBridgeLayout->GetCell(), base);
    topBridgeInst1->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, {17.75, 13}));
    base->AddCellInst(topBridgeInst1);
    auto topBridgeInst2 = nano::Create<CellInst>("TopBridge2", topBridgeLayout->GetCell(), base);
    topBridgeInst2->SetTransform(nano::CreateTransform2D(coordUnit, 1, 0, {17.75, -13}, Mirror2D::X));
    base->AddCellInst(topBridgeInst2);

    base->Flatten();
    auto bwIter = base->GetFlattenedLayout()->GetBondingWireIter();
    while (auto bw = bwIter.Next()) {
        if (generic::math::EQ<Float>(bw->GetRadius(),0.0635))
            bw->SetSolderJoints(thinBwSolderDef);
        else bw->SetSolderJoints(thickBwSolderDef);
        bw->SetBondingWireType(BondingWireType::JEDEC4);
        bw->SetMaterial(matCu);
        bw->SetHeight(0.5);
    }
    
    auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/CAS300M12BM2.nano/database.bin";
    nano::test::variables[BOOST_HANA_STRING("package_checksum")] = Database::Current().Checksum();
    Database::SaveCurrent(filename, ArchiveFormat::BIN);
    Database::Shutdown();
}

test_suite * create_nano_package_test_suite()
{
    test_suite * package_suite = BOOST_TEST_SUITE("s_package_test");
    //
    package_suite->add(BOOST_TEST_CASE(&t_create_package));
    //
    return package_suite;
}
