#pragma once
#define BOOST_TEST_INCLUDED
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <core/package>

#include "generic/tools/FileSystem.hpp"

using namespace nano;
using namespace boost::unit_test;

namespace detail {

void SetupMaterials(package::PackageId pkg)
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

package::PadstackId CreateBondwireSolderJoints(package::PackageId pkg, std::string name, Float bwRadius)
{
    auto padstack = nano::Create<package::Padstack>(std::move(name), pkg);
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

    auto thinBwSolderDef = detail::CreateBondwireSolderJoints(pkg, "Thin Solder Joints", 0.0635);
    auto thickBwSolderDef = detail::CreateBondwireSolderJoints(pkg, "Thick Solder Joints", 0.15);



    //cells
    auto baseCell = nano::Create<CircuitCell>("base");
    pkg->AddCell(baseCell);
    auto baseLayout = nano::Create<Layout>(baseCell);
    baseCell->SetLayout(baseLayout);




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
