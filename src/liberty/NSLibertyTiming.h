#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class TimingParser;
class Timing : public Entity<Timing>
{
public:
    friend class TimingParser;
    explicit Timing(Id<SignalPin> pin);
    Timing() = default;

    Id<SignalPin> GetPin() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Id<SignalPin>, pin),
    (Id<SignalPin>, relatedPin),
    (TimingSense, timingSense),
    (TimingType, timingType),
    (Arr2<LutId>, delayTable),
    (Arr2<LutId>, oslewTable),
    (Arr2<LutId>, receiverCapacitance1),
    (Arr2<LutId>, receiverCapacitance2),
    (Arr2<OutputCurrentId>, outputCurrent))
    // Arr2 = [fall, rise]
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::Timing)