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

    void AddCell(Id<Cell> cell);

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
    (Float, defaultCellLeakagePwr),
    (Float, defaultFanoutLoad),
    (Float, defaultMaxTransition),
    (Float, defaultOutputPinCap),
    (InPlaceSwapMode, defaultInPlaceSwapMode),
    (Float, inputThresholdPctFall),
    (Float, inputThresholdPctRise),
    (Int, nomProcess),
    (Float, nomVoltage),
    (Float, nomTemperature),
    (Float, outputThresholdPctFall),
    (Float, outputThresholdPctRise),
    (Float, slewDerateFromLibrary),
    (Float, slewLowerThresholdPctFall),
    (Float, slewLowerThresholdPctRise),
    (Float, slewUpperThresholdPctFall),
    (Float, slewUpperThresholdPctRise),
    (IdVec<OperatingConditions, NameLut>, operatingConditions),
    (std::string, defaultOperatingConditions),
    (IdVec<LutTemplate, NameLut>, outputCurrentTemplate),
    (IdVec<LutTemplate, NameLut>, lutTemplates),
    (IdVec<LutTemplate, NameLut>, pwrLutTemplates),
    (IdVec<Voltage, NameLut>, inputVoltage),
    (IdVec<Voltage, NameLut>, outputVoltage),
    (IdVec<NormalizedDriverWaveform>, normalizedDriverWaveform),
    (IdVec<Cell>, cells))
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::Library)