#include <nano/core/liberty>
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::Pin)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::InputPin)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::OutputPin)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::PwrGndPin)
NS_SERIALIZATION_CLASS_EXPORT_IMP(nano::liberty::SignalPin)

namespace nano::liberty {

#ifdef NANO_BOOST_SERIALIZATION_SUPPORT

template <typename Archive>
void Pin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & NS_SERIALIZATION_ENTITY_OBJECT_NVP(Pin);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NamedObj);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(Pin)

template <typename Archive>
void PwrGndPin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Pin);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(PwrGndPin)

template <typename Archive>
void SignalPin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Pin);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(SignalPin)

template <typename Archive>
void InputPin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SignalPin);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(InputPin)

template <typename Archive>
void OutputPin::serialize(Archive & ar, const unsigned int version)
{
    NS_UNUSED(version);
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(SignalPin);
    NS_SERIALIZATION_CLASS_MEMBERS(ar);
}
NS_SERIALIZATION_FUNCTIONS_IMP(OutputPin)

#endif//NANO_BOOST_SERIALIZATION_SUPPORT

Pin::Pin(const std::string& name, Id<Cell> cell, IOType type)
 : NamedObj(name)
{
    NS_CLASS_MEMBERS_INITIALIZE
    m_.cell = cell;
    m_.ioType = type;
}

Pin::Pin()
 : Pin("", Id<Cell>(), IOType::UNKNOWN)
{
}

PwrGndPin::PwrGndPin(const std::string& name, Id<Cell> cell, IOType type)
 : Pin(name, cell, type)
{
    NS_CLASS_MEMBERS_INITIALIZE
}

PwrGndPin::PwrGndPin()
 : PwrGndPin("", Id<Cell>(), IOType::UNKNOWN)
{
}

SignalPin::SignalPin(const std::string& name, Id<Cell> cell, IOType type)
 : Pin(name, cell, type)
{
    NS_CLASS_MEMBERS_INITIALIZE
}

SignalPin::SignalPin()
 : SignalPin("", Id<Cell>(), IOType::UNKNOWN)
{
}

InputPin::InputPin(const std::string& name, Id<Cell> cell)
 : SignalPin(name, cell, IOType::INPUT)
{
    NS_CLASS_MEMBERS_INITIALIZE
}

InputPin::InputPin()
 : InputPin("", Id<Cell>())
{
}

OutputPin::OutputPin(const std::string& name, Id<Cell> cell)
 : SignalPin(name, cell, IOType::OUTPUT)
 {
    NS_CLASS_MEMBERS_INITIALIZE
 }

OutputPin::OutputPin()
 : OutputPin("", Id<Cell>())
{
}

} // namespace nano::liberty