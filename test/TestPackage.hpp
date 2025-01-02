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
    auto matLib = nano::Create<MaterialLib>("mat_lib");
    pkg->SetMaterialLib(matLib);
    BOOST_CHECK(pkg->GetMaterialLib());
    
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

PadstackId CreateBondwireSolderJoints(PackageId pkg, std::string name, Float bwRadius)
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
    auto baseCell = nano::Create<CircuitCell>("base", pkg);
    pkg->AddCell(baseCell);
    auto baseLayout = nano::Create<Layout>(baseCell);
    baseCell->SetLayout(baseLayout);

    auto boundary = nano::Create<ShapePolygonWithHoles>();
    auto outline = ShapePolygon(coordUnit, {{-52.2, -29.7}, {52.2, -29.7}, {52.5, 29.7}, {-52.2, 29.7}}, 5.3).GetOutline();
    boundary->SetOutline(outline);
    boundary->AddHole(ShapeCircle(coordUnit, {-46.5, -24}, 3.85).GetOutline());
    boundary->AddHole(ShapeCircle(coordUnit, { 46.5, -24}, 3.85).GetOutline());
    boundary->AddHole(ShapeCircle(coordUnit, { 46.5,  24}, 3.85).GetOutline());
    boundary->AddHole(ShapeCircle(coordUnit, {-46.5,  24}, 3.85).GetOutline());
    baseLayout->SetBoundary(boundary);

    baseLayout->AddNet(nano::Create<Net>("Gate", baseLayout));
    baseLayout->AddNet(nano::Create<Net>("Drain", baseLayout));
    baseLayout->AddNet(nano::Create<Net>("Source", baseLayout));
    baseLayout->AddNet(nano::Create<Net>("Kelvin", baseLayout));
    auto noNet = baseLayout->AddNet(nano::Create<Net>("NoNet", baseLayout));

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
        baseLayout->AddConnObj(bw1);

        dPLoc[i][1] *= -1; sPLoc[i][1] *= -1;
        auto bw2 = nano::Create<BondingWire>("DS2_" + std::to_string(i), noNet, 0.15);
        bw2->SetStartLayer(topCuLayer, coordUnit.toCoord(dPLoc[i]), false);
        bw2->SetEndLayer(topCuLayer, coordUnit.toCoord(sPLoc[i]), false);
        baseLayout->AddConnObj(bw2);
    }

    return baseLayout;

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

    auto thinBwSolderDef = detail::CreateBondwireSolderJoints(pkg, "Thin Solder Joints", 0.0635);
    auto thickBwSolderDef = detail::CreateBondwireSolderJoints(pkg, "Thick Solder Joints", 0.15);

    //cells
    auto sicDieDef = detail::CreateSicFootprintCell(pkg);
    auto diodeDef = detail::CreateDiodeFootprintCell(pkg);
    auto gateResDef = detail::CreateGateResistanceFootprintCell(pkg);

    auto baseLayout = detail::CreateBaseLayout(pkg);


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
