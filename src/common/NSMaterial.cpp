#include <core/common>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::Material)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::MaterialProp)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::MaterialPropValue)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::MaterialPropTable)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::MaterialPropPolynomial)

namespace nano {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void MaterialProp::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(MaterialProp);
}
NS_SERIALIZATION_FUNCTIONS_IMP(MaterialProp)

template <typename Archive>
void MaterialPropValue::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(MaterialPropValue);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProp);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(MaterialPropValue)

template <typename Archive>
void MaterialPropTable::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(MaterialPropTable);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProp);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(MaterialPropTable)

template <typename Archive>
void MaterialPropPolynomial::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(MaterialPropPolynomial);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(MaterialProp);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(MaterialPropPolynomial)

template <typename Archive>
void Material::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Material);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Material)

template <typename Archive>
void MaterialLib::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(MaterialLib);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(MaterialLib)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

MaterialPropValue::MaterialPropValue(const std::array<Float, 9> & values)
{
    SetTensorProperty(values);
}

MaterialPropValue::MaterialPropValue(const std::array<Float, 3> & values)
{
    SetAnisotropicProperty(values);
}

MaterialPropValue::MaterialPropValue(Float value)
{
    SetSimpleProperty(value);
}

void MaterialPropValue::SetSimpleProperty(Float value)
{
    m_.values.assign(1, value);
}

void MaterialPropValue::SetAnisotropicProperty(const std::array<Float, 3> & values)
{
    m_.values.assign(values.begin(), values.end());
}

void MaterialPropValue::SetTensorProperty(const std::array<Float, 9> & values)
{
    m_.values.assign(values.begin(), values.end());
}

bool MaterialPropValue::GetSimpleProperty(Float & value) const
{
    if (m_.values.size() != 1) return false;
    value = m_.values[0];
    return true;
}

bool MaterialPropValue::GetAnisotropicProperty(size_t row, Float & value) const
{
    NS_ASSERT_MSG(row < 3, "index out of range");
    if (m_.values.size() != 3) return false;
    value = m_.values[row];
    return true;
}

bool MaterialPropValue::GetTensorProperty(size_t row, size_t col, Float & value) const
{
    NS_ASSERT_MSG(row < 3 and col < 3, "index out of range");
    if (m_.values.size() != 9) return false;
    value = m_.values[row * 3 + col];
    return true;
}

void MaterialPropValue::GetDimensions(size_t & row, size_t & col) const
{
    if (auto size = m_.values.size(); 1 == size) {
        row = col = 1; return;
    }
    else if (3 == size) {
        row = 3; col = 1; return;
    }
    else if (9 == size) {
        row = 3; col = 3; return;
    }
    else {
        row = col = 0;
    }
}

MaterialPropPolynomial::MaterialPropPolynomial(std::vector<std::vector<Float>> coefficients)
{
    m_.coefficients = std::move(coefficients);
}

bool MaterialPropPolynomial::GetSimpleProperty(Float index, Float & value) const
{
    if (m_.coefficients.size() != 1) return false;
    value = Calculate(m_.coefficients[0], index);
    return true;
}

bool MaterialPropPolynomial::GetAnisotropicProperty(Float index, size_t row, Float & value) const
{
    NS_ASSERT_MSG(row < 3, "index out of range");
    if (m_.coefficients.size() != 3) return false;
    value = Calculate(m_.coefficients[row], index);
    return true;
}

bool MaterialPropPolynomial::GetTensorProperty(Float index, size_t row, size_t col, Float & value) const
{
    NS_ASSERT_MSG(row < 3 and col < 3, "index out of range");
    if (m_.coefficients.size() != 9) return false;
    value = Calculate(m_.coefficients[row * 3 + col], index);
    return true;
}

void MaterialPropPolynomial::GetDimensions(size_t & row, size_t & col) const
{
    if (auto size = m_.coefficients.size(); 1 == size) {
        row = col = 1; return;
    }
    else if (3 == size) {
        row = 3; col = 1; return;
    }
    else if (9 == size) {
        row = 3; col = 3; return;
    }
    else {
        row = col = 0;
    }
}

Float MaterialPropPolynomial::Calculate(const std::vector<Float> & coefficients, Float index)
{
    NS_ASSERT(not coefficients.empty());
    auto value = coefficients.front();
    for (size_t i = 1; i < coefficients.size(); ++i)
        value += std::pow(index, i) * coefficients[i];
    return value;
}

Material::Material(std::string name)
 : NamedObj(std::move(name))
{
    m_.type = MaterialType::RIGID;
}

Material::Material() : Material("")
{
}

bool Material::hasProperty(Prop prop) const
{
    return m_.props.find(prop) != m_.props.cend();
}

void Material::SetProperty(Prop prop, MaterialPropId value)
{
    m_.props[prop] = value;
}

MaterialPropId Material::GetProperty(Prop prop) const
{
    NS_ASSERT(hasProperty(prop));
    return m_.props.at(prop);
}

void Material::RemoveProperty(Prop prop)
{
    m_.props.erase(prop);
}

MaterialLib::MaterialLib(std::string name)
 : NamedObj(std::move(name))
{
}

void MaterialLib::AddMaterial(MaterialId mat)
{
    m_.materials.emplace_back(mat);
}

MaterialId MaterialLib::FindMaterial(std::string_view name) const
{
    return m_.materials.Lookup<lut::Name>(name);
}

} // namespace nano