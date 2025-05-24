#include "NSPackageEditInterface.h"

namespace nano::package::edit {


Id<Package> CreatePackage(std::string name)
{
    auto pkg = nano::Create<Package>(std::move(name));
    auto matLib = nano::Create<MaterialLib>("mat_lib");

    pkg->SetMaterialLib(matLib);
    
    return pkg;
}

Id<Material> CreateMaterial(Id<MaterialLib> matLib, std::string name)
{
    NS_ASSERT(matLib);
    auto mat = nano::Create<Material>(std::move(name));
    matLib->AddMaterial(mat);
    return mat;
}

Id<Material> CreateMaterial(Id<Package> pkg, std::string name)
{
    if (auto matLib = pkg->GetMaterialLib(); matLib) {
        return CreateMaterial(matLib, std::move(name));
    }
    return Id<Material>();
}

Id<MaterialPropValue> CreateMaterialPropValue(Float value)
{
    return nano::Create<MaterialPropValue>(value);
}

Id<MaterialPropPolynomial> CreateMaterialPropPolynomial(Vec<Vec<Float>> coeffs)
{
    return nano::Create<MaterialPropPolynomial>(std::move(coeffs));
}

} // namespace nano::package::edit