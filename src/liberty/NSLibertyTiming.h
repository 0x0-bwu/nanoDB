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
    (IdArr2<Lut>, delayTable),
    (IdArr2<Lut>, oslewTable),
    (IdArr2<Lut>, receiverCapacitance1),
    (IdArr2<Lut>, receiverCapacitance2),
    (IdArr2<OutputCurrent>, outputCurrent))
    // IdArr2 = [fall, rise]
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::Timing)