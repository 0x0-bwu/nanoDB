#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class PinParser;
class Pin : public NamedObj, public Entity<Pin>
{
public:
    friend class PinParser;
    Pin(std::string name, Id<Cell> cell, IOType type = IOType::UNKNOWN);
    Pin() = default;

    Id<Cell> GetCell() const;

private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Id<Cell>, cell),
    (IOType, ioType))
};

inline Id<Cell> Pin::GetCell() const
{
    return m_.cell;
}

class PwrGndPinParser;
class PwrGndPin : public Pin
{
public:
    friend class PwrGndPinParser;
    PwrGndPin(std::string name, Id<Cell> cell, IOType type = IOType::UNKNOWN);
    PwrGndPin() = default;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (PGType, pgType),
    (std::string, voltageName))
};

class SignalPinParser;
class SignalPin : public Pin
{
public:
    friend class SignalPinParser;
protected:
    SignalPin(std::string name, Id<Cell> cell, IOType type = IOType::UNKNOWN);
    SignalPin() = default;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (std::string, relatedGndPin),
    (std::string, relatedPwrPin),
    (IdVec<InternalPower>, internalPower))
};

class InputPinParser;
class InputPin : public SignalPin
{
public:
    friend class InputPinParser;
    InputPin(std::string name, Id<Cell> cell);
    InputPin() = default;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (Arr2<std::string>, driverWaveform),
    (Float, maxTransition),
    (Float, pinCapacitane),
    (Arr2<Float>, capacitance),
    (Arr2<Arr2<Float>>, capRange),
    (std::string, inputVoltage))
};

class OutputPinParser;
class OutputPin : public SignalPin
{
public:
    friend class OutputPinParser;
    OutputPin(std::string name, Id<Cell> cell);
    OutputPin() = default;
private:
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_DEFINE_CLASS_MEMBERS(
    (std::string, function),
    (std::string, powerDownFunction),
    (std::string, outputVoltage),
    (Float, maxCapacitance),
    (IdVec<Timing>, timings))
};

} // namespace nano::liberty
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::Pin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::InputPin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::OutputPin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::PwrGndPin)
NS_SERIALIZATION_CLASS_EXPORT_KEY(nano::liberty::SignalPin)
NS_INHERIT_FROM_BASE(nano::liberty::PwrGndPin, nano::liberty::Pin)
NS_INHERIT_FROM_BASE(nano::liberty::SignalPin, nano::liberty::Pin)
NS_INHERITANCE(nano::liberty::InputPin, nano::liberty::SignalPin, nano::liberty::Pin)
NS_INHERITANCE(nano::liberty::OutputPin, nano::liberty::SignalPin, nano::liberty::Pin)