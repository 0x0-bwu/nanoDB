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

    virtual bool GetSimpleProperty(Float, Float &) const = 0;
    virtual bool GetAnisotropicProperty(Float, size_t, Float &) const = 0;
    virtual bool GetTensorProperty(Float, size_t, size_t, Float &) const = 0;

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

    bool GetSimpleProperty(Float index, Float & value) const override;
    bool GetAnisotropicProperty(Float index, size_t row, Float & value) const override;
    bool GetTensorProperty(Float index, size_t row, size_t col, Float & value) const override;

    //1x1-simple, 3x1-anisotropic, 3x3-tensor
    void GetDimensions(size_t & row, size_t & col) const override;

    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Vec<Float>, values))
};

class MaterialPropTable : public MaterialProp
{
public:
    bool isPropTable() const override { return true; }

    bool GetSimpleProperty(Float index, Float & value) const override;
    bool GetAnisotropicProperty(Float index, size_t row, Float & value) const override;
    bool GetTensorProperty(Float index, size_t row, size_t col, Float & value) const override;
    size_t Hash() const override { return nano::Hash(m_); }

private:
    template <typename ValueGetter>
    Float Lookup(Float index, ValueGetter && getter) const
    {
        if (m_.values.empty()) return INVALID_FLOAT;
        if (m_.values.size() == 1) return getter(m_.values.begin()->second);
        auto h = m_.values.lower_bound(index);
        if (h == m_.values.begin()) return getter(h->second);
        auto l = h; --l;
        if (auto lValue = getter(l->second), hValue = getter(l->second); 
            nano::isValid(lValue) && nano::isValid(hValue)) {
            auto ratio = (index - l->first) / (h->first - l->first);
            return lValue + ratio * (hValue - lValue);
        }
        return INVALID_FLOAT;
    }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Map<Float, Id<MaterialProp>>, values))
};

class MaterialPropPolynomial : public MaterialProp
{
public:
    explicit MaterialPropPolynomial(Vec<Vec<Float>> coefficients);
    MaterialPropPolynomial() = default;

    bool isPropPolynomial() const { return true; }

    bool GetSimpleProperty(Float index, Float & value) const override;
    bool GetAnisotropicProperty(Float index, size_t row, Float & value) const override;
    bool GetTensorProperty(Float index, size_t row, size_t col, Float & value) const override;

    //1x1-simple, 3x1-anisotropic, 3x3-tensor
    void GetDimensions(size_t & row, size_t & col) const override;

    size_t Hash() const override { return nano::Hash(m_); } 
private:
    static Float Calculate(const Vec<Float> & coefficients, Float index);
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (Vec<Vec<Float>>, coefficients))
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

    size_t Hash() const override { return nano::Hash(m_); } 
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
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

    auto GetMaterialIter() { return m_.materials.GetIter<Material>(); }
    auto GetMaterialIter() const { return m_.materials.GetCIter<Material>(); }

    size_t Hash() const override { return nano::Hash(m_); } 
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
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
