#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class LibraryParser;
class Library : public NamedObj, public Entity<Library>
{
public:
    friend class LibraryParser;
    explicit Library(std::string name);
    Library() = default;

    void SetFilename(std::string_view filename);
    std::string_view GetFilename() const;

    void AddCell(const Id<Cell> cell);

    size_t NumOfCells() const;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (std::string, filename),
    (std::string, comment),
    (std::string, date),
    (std::string, revision),
    (DelayModel, delayModel),
    (CapUnit, capLoadUnit),
    (CurrUnit, currentUnit),
    (PwrUnit, leakagePwrUnit),
    (ResUnit, pullResUnit),
    (TimeUnit, timeUnit),
    (VoltUnit, voltUnit),
    (VoltageMap, voltageMap),
    (NSFloat, defaultCellLeakagePwr),
    (NSFloat, defaultFanoutLoad),
    (NSFloat, defaultMaxTransition),
    (NSFloat, defaultOutputPinCap),
    (InPlaceSwapMode, defaultInPlaceSwapMode),
    (NSFloat, inputThresholdPctFall),
    (NSFloat, inputThresholdPctRise),
    (NSInt, nomProcess),
    (NSFloat, nomVoltage),
    (NSFloat, nomTemperature),
    (NSFloat, outputThresholdPctFall),
    (NSFloat, outputThresholdPctRise),
    (NSFloat, slewDerateFromLibrary),
    (NSFloat, slewLowerThresholdPctFall),
    (NSFloat, slewLowerThresholdPctRise),
    (NSFloat, slewUpperThresholdPctFall),
    (NSFloat, slewUpperThresholdPctRise),
    (IdVec<OperatingConditions, lut::Name>, operatingConditions),
    (std::string, defaultOperatingConditions),
    (IdVec<LutTemplate, lut::Name>, outputCurrentTemplate),
    (IdVec<LutTemplate, lut::Name>, lutTemplates),
    (IdVec<LutTemplate, lut::Name>, pwrLutTemplates),
    (IdVec<Voltage, lut::Name>, inputVoltage),
    (IdVec<Voltage, lut::Name>, outputVoltage),
    (IdVec<NormalizedDriverWaveform>, normalizedDriverWaveform),
    (IdVec<Cell>, cells))
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::Library)