#pragma once
#include <nano/core/basic>

namespace nano {

class LookupTable : public Entity<LookupTable>
{
public:
    virtual ~LookupTable() = default;
    virtual bool isValid() const = 0;
    virtual Float Lookup(Float x, bool extrapolation) const = 0;
    virtual Float Lookup(Float x, Float y, bool extrapolation) const = 0;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
};

class LookupTable1D : public LookupTable
{
public:
    LookupTable1D(Vec<Float> x, Vec<Float> y);
    LookupTable1D() = default;

    bool isValid() const override { return m_.lut.isValid(); }
    Float Lookup(Float x, bool extrapolation) const override;
    Float Lookup(Float x, Float y, bool extrapolation) const override;
    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (Lut1D, lut))
};

class LookupTable2D : public LookupTable
{
public:
    LookupTable2D(Vec<Float> x, Vec<Float> y, Vec<Float> z);
    LookupTable2D() = default;

    bool isValid() const override { return m_.lut.isValid(); }
    Float Lookup(Float x, bool extrapolation) const override;
    Float Lookup(Float x, Float y, bool extrapolation) const override;
    size_t Hash() const override { return nano::Hash(m_); }
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_CLASS_MEMBERS_DEFINE(
    (Lut2D, lut))
};

} // namespace nano
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::LookupTable)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::LookupTable1D)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::LookupTable2D)
NS_INHERIT_FROM_BASE(nano::LookupTable1D, nano::LookupTable);
NS_INHERIT_FROM_BASE(nano::LookupTable2D, nano::LookupTable);