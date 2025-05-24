#pragma once
#include <nano/core/package>

namespace nano::package::edit {

/// package
Id<Package> CreatePackage(std::string name);

/// material
Id<Material> CreateMaterial(Id<MaterialLib> matLib, std::string name);
Id<Material> CreateMaterial(Id<Package> pkg, std::string name);

/// material property
Id<MaterialPropValue> CreateMaterialPropValue(Float value);
Id<MaterialPropPolynomial> CreateMaterialPropPolynomial(Vec<Vec<Float>> coeffs);


} // namespace nano::package::edit
