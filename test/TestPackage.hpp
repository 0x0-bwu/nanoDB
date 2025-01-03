#pragma once
#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <core/package>

#include "generic/tools/FileSystem.hpp"

using namespace nano;
using namespace boost::unit_test;

namespace detail {

using namespace package;
void SetupMaterials(PackageId pkg)
{
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
    matCu->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropPolynomial>(std::vector<std::vector<Float>>{{437.6, -0.165, 1.825e-4, -1.427e-7, 3.979e-11}}));
    matCu->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropPolynomial>(std::vector<std::vector<Float>>{{342.8, 0.134, 5.535e-5, -1.971e-7, 1.141e-10}}));
    matCu->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(8850));
    matLib->AddMaterial(matCu);

    auto matAir = nano::Create<Material>("Air");
    matAir->SetMaterialType(MaterialType::FLUID);
    matAir->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropValue>(0.026));
    matAir->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropValue>(1.003));
    matAir->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(1.225));
    matLib->AddMaterial(matAir);

    auto matSiC = nano::Create<Material>("SiC");
    matSiC->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropPolynomial>(std::vector<std::vector<Float>>{{1860, -11.7, 0.03442, -4.869e-5, 2.675e-8}}));
    matSiC->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropPolynomial>(std::vector<std::vector<Float>>{{-3338, 33.12, -0.1037, 0.0001522, -8.553e-8}}));
    matSiC->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(3210));
    matLib->AddMaterial(matSiC);

    auto matAlN = nano::Create<Material>("AlN");
    matAlN->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropPolynomial>(std::vector<std::vector<Float>>{{421.7867, -1.1262, 0.001}}));
    matAlN->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropPolynomial>(std::vector<std::vector<Float>>{{170.2, -2.018, 0.032, -8.957e-5, 1.032e-7, -4.352e-11}}));
    matAlN->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(3260));
    matLib->AddMaterial(matAlN);
    
    auto matSolder = nano::Create<Material>("Solder");
    matSolder->SetProperty(Material::THERMAL_CONDUCTIVITY, nano::Create<MaterialPropValue>(55));
    matSolder->SetProperty(Material::SPECIFIC_HEAT, nano::Create<MaterialPropValue>(218));
    matSolder->SetProperty(Material::MASS_DENSITY, nano::Create<MaterialPropValue>(7800));
    matSolder->SetProperty(Material::RESISTIVITY, nano::Create<MaterialPropValue>(11.4e-8));
    matLib->AddMaterial(matSolder);
}

PadstackId CreateBondingWireSolderJoints(PackageId pkg, std::string name, Float bwRadius)
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

FootprintCellId CreateSicFootprintCell(PackageId pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto sicDie = nano::Create<FootprintCell>("SicDie", pkg);
    sicDie->SetComponentType(ComponentType::IC);
    sicDie->SetSolderBallBumpHeight(0.1);
    auto solder = pkg->GetMaterialLib()->FindMaterial("Solder");
    sicDie->SetSolderFillingMaterial(solder);
    auto boundary = nano::Create<ShapeRect>(coordUnit, FCoord2D(-2.545, -2.02), FCoord2D(2.545, 2.02));
    sicDie->SetBoundary(boundary);
    auto sic = pkg->GetMaterialLib()->FindMaterial("SiC");
    sicDie->SetMaterial(sic);
    sicDie->SetHeight(0.18);

    sicDie->AddPin(nano::Create<FootprintPin>("G", sicDie, coordUnit.toCoord(FCoord2D(-1.25,  1.0)), IOType::INPUT));
    sicDie->AddPin(nano::Create<FootprintPin>("B", sicDie, coordUnit.toCoord(FCoord2D(-1.25,  0.0)), IOType::INPUT));
    sicDie->AddPin(nano::Create<FootprintPin>("D", sicDie, coordUnit.toCoord(FCoord2D(-1.25, -1.0)), IOType::INPUT));
    sicDie->AddPin(nano::Create<FootprintPin>("A", sicDie, coordUnit.toCoord(FCoord2D( 1.25,  1.0)), IOType::INPUT));
    sicDie->AddPin(nano::Create<FootprintPin>("C", sicDie, coordUnit.toCoord(FCoord2D( 1.25,  0.0)), IOType::INPUT));
    sicDie->AddPin(nano::Create<FootprintPin>("E", sicDie, coordUnit.toCoord(FCoord2D( 1.25, -1.0)), IOType::INPUT));
    sicDie->AddPin(nano::Create<FootprintPin>("K", sicDie, coordUnit.toCoord(FCoord2D(-2.00, -0.5)), IOType::INPUT));
    
    pkg->AddCell(sicDie);
    return sicDie;
}

FootprintCellId CreateDiodeFootprintCell(PackageId pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto diode = nano::Create<FootprintCell>("Diode", pkg);
    diode->SetComponentType(ComponentType::IC);
    diode->SetSolderBallBumpHeight(0.1);
    auto solder = pkg->GetMaterialLib()->FindMaterial("Solder");
    diode->SetSolderFillingMaterial(solder);
    auto boundary = nano::Create<ShapeRect>(coordUnit, FCoord2D(-2.25, -2.25), FCoord2D(2.25, 2.25));
    diode->SetBoundary(boundary);
    auto sic = pkg->GetMaterialLib()->FindMaterial("SiC");
    diode->SetMaterial(sic);
    diode->SetHeight(0.18);

    diode->AddPin(nano::Create<FootprintPin>("A", diode, coordUnit.toCoord(FCoord2D(-1.125,  1.50)), IOType::INPUT));
    diode->AddPin(nano::Create<FootprintPin>("B", diode, coordUnit.toCoord(FCoord2D(-1.125,  0.75)), IOType::INPUT));
    diode->AddPin(nano::Create<FootprintPin>("C", diode, coordUnit.toCoord(FCoord2D(-1.125,  0.00)), IOType::INPUT));
    diode->AddPin(nano::Create<FootprintPin>("D", diode, coordUnit.toCoord(FCoord2D(-1.125, -0.75)), IOType::INPUT));
    diode->AddPin(nano::Create<FootprintPin>("E", diode, coordUnit.toCoord(FCoord2D(-1.125, -1.50)), IOType::INPUT));

    diode->AddPin(nano::Create<FootprintPin>("F", diode, coordUnit.toCoord(FCoord2D( 1.125,  1.50)), IOType::INPUT));
    diode->AddPin(nano::Create<FootprintPin>("G", diode, coordUnit.toCoord(FCoord2D( 1.125,  0.75)), IOType::INPUT));
    diode->AddPin(nano::Create<FootprintPin>("H", diode, coordUnit.toCoord(FCoord2D( 1.125,  0.00)), IOType::INPUT));
    diode->AddPin(nano::Create<FootprintPin>("I", diode, coordUnit.toCoord(FCoord2D( 1.125, -0.75)), IOType::INPUT));
    diode->AddPin(nano::Create<FootprintPin>("J", diode, coordUnit.toCoord(FCoord2D( 1.125, -1.50)), IOType::INPUT));

    pkg->AddCell(diode);
    return diode;
}

FootprintCellId CreateGateResistanceFootprintCell(PackageId pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto res = nano::Create<FootprintCell>("Rg", pkg);
    res->SetComponentType(ComponentType::RESISTOR);
    auto boundary = nano::Create<ShapeRect>(coordUnit, FCoord2D(-1.05, -0.65), FCoord2D(1.05, 0.65));
    res->SetBoundary(boundary);
    auto sic = pkg->GetMaterialLib()->FindMaterial("SiC");
    res->SetMaterial(sic);
    res->SetHeight(0.5);
    
    pkg->AddCell(res);
    return res;
}

LayoutId CreateBaseLayout(PackageId pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto cell = pkg->AddCell(nano::Create<CircuitCell>("base", pkg));
    auto layout = CircuitCellId(cell)->SetLayout(nano::Create<Layout>(cell->GetCId()));
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

    std::vector<FCoord2D> dPLoc{
        {-3, 24}, {-3, 23.275}, {-3, 22.55}, {-4, 23.275}, {-4, 22.55}, {-3, 6.525}, {-3, 5.8}, {-3, 5.075}, {-4, 6.525}, {-4, 5.8},
    };
    std::vector<FCoord2D> sPLoc{
        {3, 24}, {3, 23.275}, {3, 22.55}, {4, 21.825}, {3, 21.825}, {3, 6.525}, {3, 5.8}, {3, 5.075}, {3, 7.25}, {4, 7.25},
    };

    auto topCuLayer = pkg->FindStackupLayer("TopCuLayer");
    BOOST_CHECK(topCuLayer);
    for (size_t i = 0; i < dPLoc.size(); ++i) {
        auto bw1 = nano::Create<BondingWire>("DS1_" + std::to_string(i), noNet, 0.15);
        bw1->SetStartLayer(topCuLayer, coordUnit.toCoord(dPLoc[i]), false);
        bw1->SetEndLayer(topCuLayer, coordUnit.toCoord(sPLoc[i]), false);
        layout->AddConnObj(bw1);

        dPLoc[i][1] *= -1; sPLoc[i][1] *= -1;
        auto bw2 = nano::Create<BondingWire>("DS2_" + std::to_string(i), noNet, 0.15);
        bw2->SetStartLayer(topCuLayer, coordUnit.toCoord(dPLoc[i]), false);
        bw2->SetEndLayer(topCuLayer, coordUnit.toCoord(sPLoc[i]), false);
        layout->AddConnObj(bw2);
    }

    std::vector<FCoord2D> gPLoc{{-3, 3}, {-3, 1.8}};
    for (size_t i = 0; i < gPLoc.size(); ++i) {
        const auto & p = gPLoc.at(i);
        auto bw1 = nano::Create<BondingWire>("G1_" + std::to_string(i), noNet, 0.0635);
        bw1->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(p[0], p[1])), false);
        bw1->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(-p[0], p[1])), false);
        layout->AddConnObj(bw1);

        auto bw2 = nano::Create<BondingWire>("G2_" + std::to_string(i), noNet, 0.0635);
        bw2->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(p[0], -p[1])), false);
        bw2->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(-p[0], -p[1])), false);
        layout->AddConnObj(bw2);
    }

    auto kelvinBw0 = nano::Create<BondingWire>("KelvinBw0", gateNet, 0.0635);
    kelvinBw0->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(30.15, -5.95)), false);
    kelvinBw0->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, -5.95)), false);
    layout->AddConnObj(kelvinBw0);

    auto kelvinBw = nano::Create<BondingWire>("KelvinBw", gateNet, 0.0635);
    kelvinBw->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(30.15, 5)), false);
    kelvinBw->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(30.15, -5)), false);
    layout->AddConnObj(kelvinBw);

    auto gateBw0 = nano::Create<BondingWire>("GateBw0", gateNet, 0.0635);
    gateBw0->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32, -12.375)), false);
    gateBw0->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, -12.375)), false);
    layout->AddConnObj(gateBw0);

    auto gateBw = nano::Create<BondingWire>("GateBw", gateNet, 0.0635);
    gateBw->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32, 5)), false);
    gateBw->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32, -5)), false);
    layout->AddConnObj(gateBw);

    auto gateBw1 = nano::Create<BondingWire>("GateBw1", gateNet, 0.0635);
    gateBw1->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32.5, 3.0)), false);
    gateBw1->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(41.3, 3.35)), false);
    layout->AddConnObj(gateBw1);

    auto gateBw2 = nano::Create<BondingWire>("GateBw2", gateNet, 0.0635);
    gateBw2->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32.5, 1.8)), false);
    gateBw2->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, 1.0375)), false);
    layout->AddConnObj(gateBw2);

    auto gateBw3 = nano::Create<BondingWire>("GateBw3", gateNet, 0.0635);
    gateBw3->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32.5, -1.8)), false);
    gateBw3->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, -0.3625)), false);
    layout->AddConnObj(gateBw3);

    auto gateBw4 = nano::Create<BondingWire>("GateBw4", gateNet, 0.0635);
    gateBw4->SetStartLayer(topCuLayer, coordUnit.toCoord(FCoord2D(32.5, -3)), false);
    gateBw4->SetEndLayer(topCuLayer, coordUnit.toCoord(FCoord2D(40.05, -2.7)), false);
    layout->AddConnObj(gateBw4);

    return layout;
}

LayoutId CreateDriverLayout(PackageId pkg)
{
    const auto & coordUnit = pkg->GetCoordUnit();
    auto cell = nano::Create<CircuitCell>("driver", pkg);
    auto layout = cell->SetLayout(nano::Create<Layout>(CId<CircuitCell>(cell)));
    BOOST_CHECK(layout);

    auto boundary = nano::Create<ShapePolygon>(coordUnit, std::vector<FCoord2D>{{-5.5, -14.725}, {5.5, -14.725}, {5.5, 14.725}, {-5.5, 14.725}});
    layout->SetBoundary(boundary);

    auto noNet = layout->AddNet(nano::Create<Net>("NoNet", layout));
    auto layer1 = pkg->FindStackupLayer("TopCuLayer");
    auto layer2 = pkg->FindStackupLayer("CeramicLayer");
    auto layer3 = pkg->FindStackupLayer("BotCuLayer");
    auto layer4 = pkg->FindStackupLayer("SolderLayer");
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1,
                       nano::Create<ShapePolygon>(coordUnit, std::vector<FCoord2D>{{1.7,   9.625}, {4.7,   9.625}, {4.7, 13.925}, {1.7, 13.925}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, std::vector<FCoord2D>{{1.7,   4.325}, {4.7,   4.325}, {4.7,  8.625}, {1.7,  8.625}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, std::vector<FCoord2D>{{1.7, -13.925}, {4.7, -13.925}, {4.7,  1.075}, {3.2,  1.075}, {3.2, -1.775}, 
                       {4.2, -1.775}, {4.2, -4.925}, {3.2, -4.925}, {3.2, -7.025}, {4.2, -7.025}, {4.2, -11.425}, {1.7, -11.425}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer1, 
                       nano::Create<ShapePolygon>(coordUnit, std::vector<FCoord2D>{{1.7, -10.325}, {3.2, -10.325}, {3.2, -8.225}, {2.2, -8.225}, {2.2, -3.875}, 
                       {3.2, -3.875}, {3.2, -2.825}, {2.2, -2.825}, {2.2, 2.175}, {4.7, 2.175}, {4.7, 3.225}, {1.7, 3.225}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer2, 
                       nano::Create<ShapePolygon>(coordUnit, std::vector<FCoord2D>{{0.9, -14.725}, {5.5, -14.725}, {5.5, 14.725}, {0.9, 14.725}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer3,
                       nano::Create<ShapePolygon>(coordUnit, std::vector<FCoord2D>{{1.4, -14.225}, {5.0, -14.225}, {5.0, 14.225}, {1.4, 14.225}}, 0.25)));
    layout->AddConnObj(nano::Create<RoutingWire>(noNet, layer4,
                       nano::Create<ShapePolygon>(coordUnit, std::vector<FCoord2D>{{0.9, -14.725}, {5.5, -14.725}, {5.5, 14.725}, {0.9, 14.725}}, 0.25)));
    
    return layout;
}

} // namespace detail

void t_create_package()
{
    using namespace nano::package;
    Database::Create("CAS300M12BM2");
    auto pkg = nano::Create<Package>("CAS300M12BM2");
    BOOST_CHECK(pkg);

    detail::SetupMaterials(pkg);

    CoordUnit coordUnit(CoordUnit::Unit::Millimeter);
    pkg->SetCoordUnit(coordUnit);

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
    auto driverLayout = detail::CreateDriverLayout(pkg);

    auto base = nano::Create<CellInst>("Base", baseLayout->GetCell());
    auto driver = base->AddCellInst(nano::Create<CellInst>("Driver", driverLayout->GetCell(), base));

    auto filename = generic::fs::DirName(__FILE__).string() + "/data/archive/CAS300M12BM2.xml";
    Database::SaveCurrent(filename, ArchiveFormat::XML);
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
