#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class CellParser;
class Cell : public NamedObj, public Entity<Cell>
{
public:
    friend class CellParser;
    Cell(std::string name, Id<Library> library);
    Cell() = default;

    Id<Library> GetLibrary() const;

    template <typename Range>
    void AddSignalPins(const Range & range)
    {
        for (auto pin : range) AddSignalPin(pin);
    }

    void AddPwrGndPin(Id<PwrGndPin> pin);

    void AddSignalPin(Id<SignalPin> pin);
    
    size_t NumOfPwrGndPins() const;
    size_t NumOfSignalPins() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Library>, library),
    (NSFloat, area),
    (IdVec<PwrGndPin>, pgPins),
    (IdVec<SignalPin>, signalPins),
    (IdVec<LeakagePower>, leakagePower))
};

inline Id<Library> Cell::GetLibrary() const
{
    return m_.library;
}

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::Cell)