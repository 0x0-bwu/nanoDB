#pragma once
#include "NSLiberty.h"

namespace nano::liberty {

class PinParser;
class Pin : public NamedObj, public Entity<Pin>
{
public:
    friend class PinParser;
    Pin(const std::string& name, Id<Cell> cell, IOType type = IOType::UNKNOWN);

    Id<Cell> GetCell() const;

private:
    Pin();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
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
    PwrGndPin(const std::string& name, Id<Cell> cell, IOType type = IOType::UNKNOWN);
private:
    PwrGndPin();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (PGType, pgType),
    (std::string, voltageName))
};

class SignalPinParser;
class SignalPin : public Pin
{
public:
    friend class SignalPinParser;
protected:
    SignalPin(const std::string& name, Id<Cell> cell, IOType type = IOType::UNKNOWN);
private:
    SignalPin();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
    (std::string, relatedGndPin),
    (std::string, relatedPwrPin),
    (IdVec<InternalPower>, internalPower))
};

class InputPinParser;
class InputPin : public SignalPin
{
public:
    friend class InputPinParser;
    InputPin(const std::string& name, Id<Cell> cell);
private:
    InputPin();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
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
    OutputPin(const std::string& name, Id<Cell> cell);
private:
    OutputPin();
    NS_SERIALIZATION_FUNCTIONS_DECLARATION;
    NS_CLASS_MEMBERS_DEFINE(
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