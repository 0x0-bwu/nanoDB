#pragma once
#include <nano/core/basic>

namespace nano {

class MaterialProp : public Entity<MaterialProp>
{
public:
    virtual ~MaterialProp() = default;
    virtual bool isPropValue() const { return false; }
    virtual bool isPropTable() const { return false; }
    virtual bool isPropPolyNomial() const { return false; }

    //1x1-simple, 3x1-anisotropic, 3x3-tensor
    virtual void GetDimensions(size_t & row, size_t & col) const { row = col = 0; }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
};

class MaterialPropValue : public MaterialProp
{
public:
    explicit MaterialPropValue(const Arr9<Float> & values);
    explicit MaterialPropValue(const Arr3<Float> & values);
    explicit MaterialPropValue(Float value);
    MaterialPropValue() = default;

    bool isPropValue() const override { return true; }

    void SetSimpleProperty(Float value);
    void SetAnisotropicProperty(const Arr3<Float> & values);
    void SetTensorProperty(const Arr9<Float> & values);

    bool GetSimpleProperty(Float & value) const;
    bool GetAnisotropicProperty(size_t row, Float & value) const;
    bool GetTensorProperty(size_t row, size_t col, Float & value) const;

    //1x1-simple, 3x1-anisotropic, 3x3-tensor
    void GetDimensions(size_t & row, size_t & col) const override;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (std::vector<Float>, values))
};

class MaterialPropTable : public MaterialProp
{
public:
    // todo
    bool isPropTable() const override { return true; }

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (std::map<Float, Id<MaterialProp>>, values))
};

class MaterialPropPolynomial : public MaterialProp
{
public:
    explicit MaterialPropPolynomial(std::vector<std::vector<Float>> coefficients);
    MaterialPropPolynomial() = default;

    bool isPropPolynomial() const { return true; }

    bool GetSimpleProperty(Float index, Float & value) const;
    bool GetAnisotropicProperty(Float index, size_t row, Float & value) const;
    bool GetTensorProperty(Float index, size_t row, size_t col, Float & value) const;

    //1x1-simple, 3x1-anisotropic, 3x3-tensor
    void GetDimensions(size_t & row, size_t & col) const override;

private:
    static Float Calculate(const std::vector<Float> & coefficients, Float index);
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (std::vector<std::vector<Float>>, coefficients))
};

enum class MaterialType { RIGID, FLUID };

class Material : public NamedObj, public Entity<Material>
{
public:
    enum Prop                              // unit in SI
    {
        PERMITTIVITY = 3,                  //F/m
        PERMEABILITY = 4,                  //H/m
        CONDUCTIVITY = 5,                  //S/m
        DIELECTRIC_LOSS_TANGENT = 6,
        MAGNETIC_LOSS_TANGENT = 7,
        RESISTIVITY = 8,                   //ohm·m
        THERMAL_CONDUCTIVITY = 14,         //W/(m·K)
        MASS_DENSITY = 34,                 //kg/m^3
        SPECIFIC_HEAT = 38,                //J/(kg·K)
        YOUNGSMODULUS = 39,                //Pa
        POISSONSRATIO = 40,
        THERMAL_EXPANSION_COEFFICIENT = 42 //1/K
    };
    Material(std::string name);
    Material();

    bool hasProperty(Prop prop) const;
    void SetProperty(Prop prop, Id<MaterialProp> value);
    Id<MaterialProp> GetProperty(Prop prop) const;
    void RemoveProperty(Prop prop);

    void SetMaterialType(MaterialType type) { m_.type = type; }
    MaterialType GetMaterialType() const { return m_.type; }

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (MaterialType, type),
    (HashMap<Prop, Id<MaterialProp>>, props))
};

class MaterialLib : public NamedObj, public Entity<MaterialLib>
{
public:
    MaterialLib(std::string name);
    MaterialLib() = default;

    void AddMaterial(Id<Material> mat);
    Id<Material> FindMaterial(std::string_view name) const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (IdVec<Material, NameLut>, materials))
};

} // namespace nano
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::Material)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::MaterialLib)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::MaterialProp)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::MaterialPropValue)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::MaterialPropTable)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::MaterialPropPolynomial)
NS_INHERIT_FROM_BASE(nano::MaterialPropValue, nano::MaterialProp)
NS_INHERIT_FROM_BASE(nano::MaterialPropTable, nano::MaterialProp)
NS_INHERIT_FROM_BASE(nano::MaterialPropPolynomial, nano::MaterialProp)
