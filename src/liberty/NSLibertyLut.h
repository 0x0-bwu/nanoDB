#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class LutParser;
class LutTemplate;
class Lut : public Entity<Lut>
{
public:
    friend class LutParser;
    explicit Lut(Id<LutTemplate> lutTemplate);
    Lut() = default;

    size_t Dimension() const;

private:
NS_SERIALIZATION_FUNCTIONS_DECLARATION
NS_DEFINE_CLASS_MEMBERS(
    (Id<LutTemplate>, lutTemplate),
    (LutIndices, indices),
    (LutNumbers, values))
};

class CcsLutParser;
class CcsLut : public Lut
{
public:
    friend class CcsLutParser;
    explicit CcsLut(Id<LutTemplate> lutTemplate);
    CcsLut();
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (Float, referenceTime))
};

class NormalizedDriverWaveformParser;
class NormalizedDriverWaveform : public Lut
{
public:
    friend class NormalizedDriverWaveformParser;
    NormalizedDriverWaveform(Id<LutTemplate> lutTemplate);
    NormalizedDriverWaveform() = default;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION
    NS_DEFINE_CLASS_MEMBERS(
    (std::string,driverWaveformName))
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::Lut)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::CcsLut)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::NormalizedDriverWaveform)
NS_INHERIT_FROM_BASE(nano::liberty::CcsLut, nano::liberty::Lut)
NS_INHERIT_FROM_BASE(nano::liberty::NormalizedDriverWaveform, nano::liberty::Lut)