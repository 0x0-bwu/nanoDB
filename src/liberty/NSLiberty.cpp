#include "NSLiberty.h"
#include <core/liberty>

#include "liberty/parser/NSLibertyParser.hpp"
namespace nano::liberty {

using namespace parser;
using namespace grammar;
namespace x3 = boost::spirit::x3;
namespace ascii = x3::ascii;
using ascii::space;

bool ParseNumbers(std::string_view s, LutNumbers & numbers)
{
    using x3::eol;
    using x3::float_;
    return x3::phrase_parse(s.begin(), s.end(), float_ % ',', space | '\\' | eol, numbers);
}

bool ParseDelayModel(std::string_view s, DelayModel & dm)
{
    struct Symbols : x3::symbols<DelayModel>
    {
        Symbols() { add("table_lookup", DelayModel::TABLE_LOOKUP); }
    } const symbols;
    return x3::phrase_parse(s.begin(), s.end(), symbols, space, dm);
}

bool ParseCapUnit(std::string_view s, CapUnit::Unit & u)
{
    struct Symbols : x3::symbols<CapUnit::Unit>
    {
        Symbols() 
        { 
            add("F", CapUnit::Unit::F)
            ("MF", CapUnit::Unit::MF)
            ("UF", CapUnit::Unit::UF)
            ("NF", CapUnit::Unit::NF)
            ("PF", CapUnit::Unit::PF)
            ("FF", CapUnit::Unit::PF); 
        }
    } const symbols;
    return x3::phrase_parse(s.begin(), s.end(), x3::no_case[symbols], space, u); 
}

bool ParseCurrUnit(std::string_view s, CurrUnit & cu)
{
    struct Symbols : x3::symbols<CurrUnit::Unit>
    {
        Symbols() 
        { 
            add("A", CurrUnit::Unit::A)
            ("MA", CurrUnit::Unit::MA)
            ("UA", CurrUnit::Unit::UA);
        }
    } const symbols;
    Float scale;
    CurrUnit::Unit unit;
    auto f1 = [&](auto & ctx){ scale = x3::_attr(ctx); };
    auto f2 = [&](auto & ctx){ unit = x3::_attr(ctx); };
    auto r = x3::phrase_parse(s.begin(), s.end(), (x3::float_[f1] > x3::no_case[symbols[f2]]), space);
    cu = CurrUnit(scale, unit);
    return r;
}

bool ParsePwrUnit(std::string_view s, PwrUnit & pu)
{
    struct Symbols : x3::symbols<PwrUnit::Unit>
    {
        Symbols() 
        { 
            add("W", PwrUnit::Unit::W)
            ("MW", PwrUnit::Unit::MW)
            ("UW", PwrUnit::Unit::UW)
            ("NW", PwrUnit::Unit::NW)
            ("PW", PwrUnit::Unit::PW);
        }
    } const symbols;
    Float scale;
    PwrUnit::Unit unit;
    auto f1 = [&](auto & ctx){ scale = x3::_attr(ctx); };
    auto f2 = [&](auto & ctx){ unit = x3::_attr(ctx); };
    auto r = x3::phrase_parse(s.begin(), s.end(), (x3::float_[f1] > x3::no_case[symbols[f2]]), space);
    pu = PwrUnit(scale, unit);
    return r;
}

bool ParseResUnit(std::string_view s, ResUnit & ru)
{
    struct Symbols : x3::symbols<ResUnit::Unit>
    {
        Symbols() 
        { 
            add("KOHM", ResUnit::Unit::KOHM)
            ("OHM", ResUnit::Unit::OHM);
        }
    } const symbols;
    Float scale;
    ResUnit::Unit unit;
    auto f1 = [&](auto & ctx){ scale = x3::_attr(ctx); };
    auto f2 = [&](auto & ctx){ unit = x3::_attr(ctx); };
    auto r = x3::phrase_parse(s.begin(), s.end(), (x3::float_[f1] > x3::no_case[symbols[f2]]), space);
    ru = ResUnit(scale, unit);
    return r;
}

bool ParseTimeUnit(std::string_view s, TimeUnit & tu)
{
    struct Symbols : x3::symbols<TimeUnit::Unit>
    {
        Symbols() 
        { 
            add("PS", TimeUnit::Unit::Picosecond)
            ("NS", TimeUnit::Unit::Nanosecond)
            ("US", TimeUnit::Unit::Microsecond)
            ("MS", TimeUnit::Unit::Millisecond)
            ("S", TimeUnit::Unit::Second);
        }
    } const symbols;
    Float scale;
    TimeUnit::Unit unit;
    auto f1 = [&](auto & ctx){ scale = x3::_attr(ctx); };
    auto f2 = [&](auto & ctx){ unit = x3::_attr(ctx); };
    auto r = x3::phrase_parse(s.begin(), s.end(), (x3::float_[f1] > x3::no_case[symbols[f2]]), space);
    tu = TimeUnit(scale, unit);
    return r;
}

bool ParseVoltUnit(std::string_view s, VoltUnit & vu)
{
    struct Symbols : x3::symbols<VoltUnit::Unit>
    {
        Symbols() 
        { 
            add("UV", VoltUnit::Unit::UV)
            ("MV", VoltUnit::Unit::MV)
            ("V", VoltUnit::Unit::V);
        }
    } const symbols;
    Float scale;
    VoltUnit::Unit unit;
    auto f1 = [&](auto & ctx){ scale = x3::_attr(ctx); };
    auto f2 = [&](auto & ctx){ unit = x3::_attr(ctx); };
    auto r = x3::phrase_parse(s.begin(), s.end(), (x3::float_[f1] > x3::no_case[symbols[f2]]), space);
    vu = VoltUnit(scale, unit);
    return r;
}

bool ParseInPlaceSwapMode(std::string_view s, InPlaceSwapMode & im)
{
    struct Symbols : x3::symbols<InPlaceSwapMode>
    {
        Symbols() { add("match_footprint", InPlaceSwapMode::MATCH_FOOTPRINT); }
    } const symbols;
    return x3::phrase_parse(s.begin(), s.end(), symbols, space, im);
}

bool ParseIOType(std::string_view s, IOType & ioType)
{
    struct Symbols : x3::symbols<IOType>
    {
        Symbols()
        { 
            add("input", IOType::INPUT)
            ("inout", IOType::INOUT)
            ("output", IOType::OUTPUT);
        }
    } const symbols;
    return x3::phrase_parse(s.begin(), s.end(), symbols, space, ioType);
}

bool ParsePGType(std::string_view s, PGType & pgType)
{
    struct Symbols : x3::symbols<PGType>
    {
        Symbols()
        { 
            add("primary_power", PGType::PRIMARY_POWER)
            ("primary_ground", PGType::PRIMARY_GROUND)
            ("backup_power ", PGType::BACKUP_POWER)
            ("backup_ground  ", PGType::BACKUP_GROUND)
            ("internal_power  ", PGType::INTERNAL_POWER)
            ("internal_ground  ", PGType::INTERNAL_GROUND)
            ("pwell ", PGType::PWELL)
            ("nwell ", PGType::NWELL)
            ("deepnwell ", PGType::DEEPNWELL)
            ("deeppwell ", PGType::DEEPPWELL);
        }
    } const symbols;
    return x3::phrase_parse(s.begin(), s.end(), symbols, space, pgType);
}

bool ParseTimingSense(std::string_view s, TimingSense & ts)
{
    struct Symbols : x3::symbols<TimingSense>
    {
        Symbols()
        {
            add("non_unate"  , TimingSense::NON_UNATE     )
            ("negative_unate", TimingSense::NEGATIVE_UNATE)
            ("positive_unate", TimingSense::POSITIVE_UNATE);
        }
    } const symbols;
    return x3::phrase_parse(s.begin(), s.end(), symbols, space, ts);
}

bool ParseTimingType(std::string_view s, TimingType & tt)
{
    struct Symbols : x3::symbols<TimingType>
    {
        Symbols()
        {
            add("combinational"        , TimingType::COMBINATIONAL           )
            ("combinational_rise"      , TimingType::COMBINATIONAL_RISE      )
            ("combinational_fall"      , TimingType::COMBINATIONAL_FALL      )
            ("three_state_disable"     , TimingType::THREE_STATE_DISABLE     )
            ("three_state_enable"      , TimingType::THREE_STATE_ENABLE      )
            ("three_state_disable_rise", TimingType::THREE_STATE_DISABLE_RISE)
            ("three_state_disable_fall", TimingType::THREE_STATE_DISABLE_FALL)
            ("three_state_enable_rise" , TimingType::THREE_STATE_ENABLE_RISE )
            ("three_state_enable_fall" , TimingType::THREE_STATE_ENABLE_FALL );

        }
    } const symbols;
    return x3::phrase_parse(s.begin(), s.end(), symbols, space, tt);
}

std::string_view toString(const SimpleAttribute::Value & value)
{
    return boost::get<std::string>(value).c_str();
}

Float toFloat(const SimpleAttribute::Value & value)
{
    if (auto * f = boost::get<float>(&value))
        return *f;
    else if (auto * i = boost::get<int>(&value))
        return Float(*i);
    return std::numeric_limits<Float>::quiet_NaN();
}

LutNumbers toNumbers(const std::vector<SimpleAttribute::Value> & values)
{
    LutNumbers numbers;
    for (const auto & value : values)
        NS_CHECK_TRUE(ParseNumbers(toString(value), numbers));
    return numbers;
}

class LibertyObjParser
{
public:
    virtual ~LibertyObjParser() = default;
    virtual void operator() (const DefineStatement  &) {}
    virtual void operator() (const SimpleAttribute  &) {}
    virtual void operator() (const ComplexAttribute &) {}
    virtual void operator() (const GroupStatement   &) {}
    virtual void Build() {}
    void Parse(const GroupStatement & group)
    {
        for (const auto & statement : group.statements)
            boost::apply_visitor(*this, statement);
    }
};

class OperatingConditionsParser : public LibertyObjParser
{
public:
    OperatingConditions & core;
    explicit OperatingConditionsParser(Id<OperatingConditions> operatingConditions)
     : core(*operatingConditions)
    {
    }
    
    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::process == sa.name)
            core->process = boost::get<int>(sa.value);
        else if (kw::temperature == sa.name)
            core->temperature = toFloat(sa.value);
        else if (kw::voltage == sa.name)
            core->voltage = toFloat(sa.value);
    }
};

class LutTemplateParser : public LibertyObjParser
{
public:
    LutTemplate & core;
    explicit LutTemplateParser(Id<LutTemplate> lutTemplate)
     : core(*lutTemplate) {}

    void operator() (const SimpleAttribute & sa) override
    {
        NS_ASSERT(sa.name.starts_with("variable_"));
        core->variables.emplace_back(toString(sa.value));
    }

    void operator() (const ComplexAttribute & ca) override
    {
        NS_ASSERT(ca.name.starts_with("index_"));
        core->indices.emplace_back(toNumbers(ca.values));       
    }
};

class VoltageParser : public LibertyObjParser
{
public:
    Voltage & core;
    explicit VoltageParser(Id<Voltage> voltage)
     : core(*voltage) {}
    
    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::vil == sa.name or kw::vol == sa.name)
            core->low = toFloat(sa.value);
        else if (kw::vih == sa.name or kw::voh == sa.name)
            core->high = toFloat(sa.value);
        else if (kw::vimax == sa.name or kw::vomax == sa.name)
            core->max = toFloat(sa.value);
        else if (kw::vimin == sa.name or kw::vomin == sa.name)
            core->min = toFloat(sa.value);
    }
};

class LutParser : public LibertyObjParser
{
public:
    Id<Lut> lut;
    Lut & core;
    explicit LutParser(Id<Lut> lut)
     : lut(lut), core(*lut) {}

    void operator() (const SimpleAttribute  & sa) override
    {
    }

    void operator() (const ComplexAttribute & ca) override
    {
        if (ca.name.starts_with("index_"))
            core->indices.emplace_back(toNumbers(ca.values));
        else if (ca.name.starts_with("values"))
            core->values = toNumbers(ca.values);
    }
private:

};

class CcsLutParser : public LutParser
{
public:
    CcsLut & core;
    explicit CcsLutParser(Id<CcsLut> ccsLut)
     : LutParser(Id<Lut>(ccsLut)), core(*ccsLut) {}
    
    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::reference_time == sa.name)
            core->referenceTime = toFloat(sa.value);
    }
};

class NormalizedDriverWaveformParser : public LutParser
{
public:
    NormalizedDriverWaveform & core;
    explicit NormalizedDriverWaveformParser(Id<NormalizedDriverWaveform> n)
     : LutParser(Id<Lut>(n)), core(*n) {}

    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::driver_waveform_name == sa.name)
            core->driverWaveformName = toString(sa.value);
    }
};

class OutputCurrentParser : public LibertyObjParser
{
public:
    Id<OutputCurrent> outputCurrent;
    OutputCurrent & core;
    explicit OutputCurrentParser(Id<OutputCurrent> outputCurrent)
     : outputCurrent(outputCurrent), core(*outputCurrent) {}
    
    void operator() (const GroupStatement & group) override
    {
        if (kw::vector == group.groupName) {
            auto ccsLut = nano::Create<CcsLut>();
            m_ccsLutParsers.emplace_back(ccsLut, group.name.c_str()).first.Parse(group);
            core.AddCcsLut(ccsLut);
        }
    }
private:
    std::vector<std::pair<CcsLutParser, std::string_view>> m_ccsLutParsers;
};

class LeakagePowerParser : public LibertyObjParser
{
public:
    LeakagePower & core;
    explicit LeakagePowerParser(Id<LeakagePower> leakagePower)
     : core(*leakagePower) {}

    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::value == sa.name)
            core->value = toFloat(sa.value);
        else if (kw::when == sa.name)
            core->when = toString(sa.value);
        else if (kw::related_pg_pin == sa.name)
            core->relatedPG = toString(sa.value);
    }
    
};

class InternalPowerParser : public LibertyObjParser
{
public:
    InternalPower & core;
    explicit InternalPowerParser(Id<InternalPower> internalPower)
     : core(*internalPower) {}
    
    void operator() (const SimpleAttribute & sa)
    {
        if (kw::related_pin == sa.name)
            core->relatedPin = toString(sa.value);
        else if (kw::related_pg_pin == sa.name)
            core->relatedPG = toString(sa.value);
    } 

    void operator() (const GroupStatement & group)
    {
        if (kw::fall_power == group.groupName or
            kw::rise_power == group.groupName) {
            auto lut = nano::Create<Lut>();
            m_lutParsers.emplace_back(lut, group.name.c_str()).first.Parse(group);
            core->power[kw::fall_power == group.groupName ? FALL : RISE] = lut;
        }
    }
private:
    std::vector<std::pair<LutParser, std::string_view>> m_lutParsers;
};

class TimingParser : public LibertyObjParser
{
public:
    Id<Timing> timing;
    Timing & core;
    explicit TimingParser(Id<Timing> timing)
     : timing(timing), core(*timing) {}

    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::timing_sense == sa.name)
            NS_CHECK_TRUE(ParseTimingSense(toString(sa.value), core->timingSense));
        else if (kw::timing_type == sa.name)
            NS_CHECK_TRUE(ParseTimingType(toString(sa.value), core->timingType));
    }
    void operator() (const GroupStatement & group) override
    {
        if (kw::cell_fall == group.groupName or
            kw::cell_rise == group.groupName) {
            auto & lut = core->delayTable[
            kw::cell_fall == group.groupName ? FALL : RISE];
            lut = nano::Create<Lut>();
            m_lutParsers.emplace_back(lut, group.name.c_str()).first.Parse(group);
        }
        else if (kw::fall_transition == group.groupName or
                 kw::rise_transition == group.groupName) {
            auto & lut = core->oslewTable[
            kw::fall_transition == group.groupName ? FALL : RISE];
            lut = nano::Create<Lut>();
            m_lutParsers.emplace_back(lut, group.name.c_str()).first.Parse(group);
        }
        else if (kw::receiver_capacitance1_fall == group.groupName or
                 kw::receiver_capacitance1_rise == group.groupName) {
            auto & lut = core->receiverCapacitance1[
            kw::receiver_capacitance1_fall == group.groupName ? FALL : RISE];
            lut = nano::Create<Lut>();
            m_lutParsers.emplace_back(lut, group.name.c_str()).first.Parse(group);         
        }
        else if (kw::receiver_capacitance2_fall == group.groupName or
                 kw::receiver_capacitance2_rise == group.groupName) {
            auto & lut = core->receiverCapacitance2[
            kw::receiver_capacitance2_fall == group.groupName ? FALL : RISE];
            lut = nano::Create<Lut>();
            m_lutParsers.emplace_back(lut, group.name.c_str()).first.Parse(group);         
        }
        else if (kw::output_current_fall == group.groupName or
                 kw::output_current_rise == group.groupName) {
            auto & outputCurrent = core->outputCurrent[
                kw::output_current_fall == group.groupName ? FALL : RISE];
            outputCurrent = nano::Create<OutputCurrent>();
            m_outputCurrentParsers.emplace_back(outputCurrent).Parse(group);
        }
    }
private:
    std::vector<OutputCurrentParser> m_outputCurrentParsers;
    std::vector<std::pair<LutParser, std::string_view>> m_lutParsers;
};

class PinParser : public LibertyObjParser
{
public:
    Pin & core;
    explicit PinParser(Id<Pin> pin)
     : core(*pin) {}
    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::direction == sa.name)
            NS_CHECK_TRUE(ParseIOType(toString(sa.value), core->ioType));
    }
};

class PwrGndPinParser : public PinParser
{
public:
    PwrGndPin & core;
    explicit PwrGndPinParser(Id<PwrGndPin> pgPin)
     : PinParser(Id<Pin>(pgPin)), core(*pgPin) {}
    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::pg_type == sa.name)
            NS_CHECK_TRUE(ParsePGType(toString(sa.value), core->pgType));
        else if (kw::voltage_name == sa.name)
            core->voltageName = toString(sa.value);
        else 
            PinParser::operator()(sa);
    }
};

class SignalPinParser : public PinParser
{
public:
    SignalPin & core;
    explicit SignalPinParser(Id<SignalPin> signalPin)
     : PinParser(Id<Pin>(signalPin)), core(*signalPin) {}

    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::related_ground_pin == sa.name)
            core->relatedGndPin = toString(sa.value);
        else if (kw::related_power_pin == sa.name)
            core->relatedPwrPin = toString(sa.value);
        else
            PinParser::operator()(sa);
    }
    void operator() (const GroupStatement & group) override
    {
        if (kw::internal_power == group.groupName) {
            auto internalPower = nano::Create<InternalPower>();
            m_internalPowerParsers.emplace_back(internalPower).Parse(group);
            core->internalPower.emplace_back(internalPower);
        }
    }
private:
    std::vector<InternalPowerParser> m_internalPowerParsers;
};

class InputPinParser : public SignalPinParser
{
public:
    InputPin & core;
    explicit InputPinParser(Id<InputPin> inputPin)
     : SignalPinParser(Id<SignalPin>(inputPin))
     , core(*inputPin) {}
    
    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::driver_waveform_fall == sa.name)
            core->driverWaveform[FALL] = toString(sa.value);
        else if (kw::driver_waveform_rise == sa.name)
            core->driverWaveform[RISE] = toString(sa.value);
        else if (kw::max_transition == sa.name)
            core->maxTransition = toFloat(sa.value);
        else if (kw::capacitive == sa.name)
            core->pinCapacitane = toFloat(sa.value);
        else if (kw::fall_capacitance == sa.name)
            core->capacitance[FALL] = toFloat(sa.value);
        else if (kw::rise_capacitance == sa.name)
            core->capacitance[RISE] = toFloat(sa.value);
        else if (kw::input_voltage == sa.name)
            core->inputVoltage = toString(sa.value);   
        else
            SignalPinParser::operator()(sa);
    }
};

class OutputPinParser : public SignalPinParser
{
public:
    Id<OutputPin> outputPin;
    OutputPin & core;
    explicit OutputPinParser(Id<OutputPin> outputPin)
     : SignalPinParser(Id<SignalPin>(outputPin))
     , outputPin(outputPin), core(*outputPin) {}
    
    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::function == sa.name)
            core->function = toString(sa.value);
        else if (kw::power_down_function == sa.name)
            core->powerDownFunction = toString(sa.value);
        else if (kw::max_capacitance == sa.name)
            core->maxCapacitance = toFloat(sa.value);
        else if (kw::output_voltage == sa.name)
            core->outputVoltage = toString(sa.value);
        else
            SignalPinParser::operator()(sa);
    }

    void operator() (const GroupStatement & group) override
    {
        if (kw::timing == group.groupName) {
            auto timing = nano::Create<Timing>(Id<SignalPin>(outputPin));
            m_timingParsers.emplace_back(timing).Parse(group);
            core->timings.emplace_back(timing);
        }
        else
            SignalPinParser::operator()(group);
    }
private:
    std::vector<TimingParser> m_timingParsers;
};

class CellParser : public LibertyObjParser
{
public:
    Id<Cell> cell;
    Cell & core;
    explicit CellParser(Id<Cell> cell)
     : cell(cell), core(*cell) {}
    void operator() (const SimpleAttribute & sa) override
    {
        if (kw::area == sa.name)
            core->area = toFloat(sa.value);   
    }

    void operator() (const GroupStatement & group) override
    {
        if (kw::pg_pin == group.groupName) {
            auto pgPin = nano::Create<PwrGndPin>(group.name, cell);
            PwrGndPinParser(pgPin).Parse(group);
            core->pgPins.emplace_back(pgPin);
        }
        else if (kw::leakage_power == group.groupName) {
            auto leakagePower = nano::Create<LeakagePower>();
            LeakagePowerParser(leakagePower).Parse(group);
            core->leakagePower.emplace_back(leakagePower);
        }
        else if (kw::pin == group.groupName) {
            IOType ioType{IOType::UNKNOWN};
            for (const auto & s : group.statements) {
                if (auto * sa = boost::get<SimpleAttribute>(&s); sa) {
                    if (kw::direction == sa->name) {
                        NS_CHECK_TRUE(ParseIOType(toString(sa->value), ioType));
                        break;
                    }
                }
            }
            NS_ASSERT(IOType::UNKNOWN != ioType);
            if (IOType::INPUT == ioType) {
                auto inputPin = nano::Create<InputPin>(group.name, cell);
                InputPinParser(inputPin).Parse(group);
                core->signalPins.emplace_back(Id<SignalPin>(inputPin));
            }
            else if (IOType::OUTPUT == ioType) {
                auto outputPin = nano::Create<OutputPin>(group.name, cell);
                OutputPinParser(outputPin).Parse(group);
                core->signalPins.emplace_back(Id<SignalPin>(outputPin));
            }
            else {
                NS_ASSERT(false);
                //todo, inout
            }
        }
    }
};
class LibraryParser : public LibertyObjParser
{
public:
    Id<Library> library;
    Library & core;
    explicit LibraryParser(Id<Library> library)
     : library(library), core(*library) {}
    
    ~LibraryParser() { Build(); }
    void operator() (const SimpleAttribute & sa) override
    {   
        if (kw::comment == sa.name)
            core->comment = toString(sa.value);
        else if (kw::date == sa.name)
            core->date = toString(sa.value);
        else if (kw::revision == sa.name)
            core->revision = toString(sa.value);
        else if (kw::delay_model == sa.name)
            NS_CHECK_TRUE(ParseDelayModel(toString(sa.value), core->delayModel));
        else if (kw::current_unit == sa.name)
            NS_CHECK_TRUE(ParseCurrUnit(toString(sa.value), core->currentUnit));
        else if (kw::leakage_power_unit == sa.name)
            NS_CHECK_TRUE(ParsePwrUnit(toString(sa.value), core->leakagePwrUnit));
        else if (kw::pulling_resistance_unit == sa.name)
            NS_CHECK_TRUE(ParseResUnit(toString(sa.value), core->pullResUnit));
        else if (kw::time_unit == sa.name)
            NS_CHECK_TRUE(ParseTimeUnit(toString(sa.value), core->timeUnit));
        else if (kw::voltage_unit == sa.name)
            NS_CHECK_TRUE(ParseVoltUnit(toString(sa.value), core->voltUnit));
        else if (kw::default_cell_leakage_power == sa.name)
            core->defaultCellLeakagePwr = toFloat(sa.value);
        else if (kw::default_fanout_load == sa.name)
            core->defaultFanoutLoad = toFloat(sa.value);
        else if (kw::default_max_transition == sa.name)
            core->defaultMaxTransition = toFloat(sa.value);
        else if (kw::default_output_pin_cap == sa.name)
            core->defaultOutputPinCap = toFloat(sa.value);
        else if (kw::in_place_swap_mode == sa.name)
            NS_CHECK_TRUE(ParseInPlaceSwapMode(toString(sa.value), core->defaultInPlaceSwapMode));
        else if (kw::input_threshold_pct_fall == sa.name)
            core->inputThresholdPctFall = toFloat(sa.value);
        else if (kw::input_threshold_pct_rise == sa.name)
            core->inputThresholdPctRise = toFloat(sa.value);
        else if (kw::nom_process == sa.name)
            core->nomProcess = boost::get<int>(sa.value);
        else if (kw::nom_temperature == sa.name)
            core->nomTemperature = toFloat(sa.value);
        else if (kw::nom_voltage == sa.name)
            core->nomVoltage = toFloat(sa.value);
        else if (kw::output_threshold_pct_fall == sa.name)
            core->outputThresholdPctFall = toFloat(sa.value);
        else if (kw::output_threshold_pct_rise == sa.name)
            core->outputThresholdPctRise = toFloat(sa.value);
        else if (kw::slew_derate_from_library == sa.name)
            core->slewDerateFromLibrary = toFloat(sa.value);
        else if (kw::slew_lower_threshold_pct_fall == sa.name)
            core->slewLowerThresholdPctFall = toFloat(sa.value);
        else if (kw::slew_lower_threshold_pct_rise == sa.name)
            core->slewLowerThresholdPctRise = toFloat(sa.value);
        else if (kw::slew_upper_threshold_pct_fall == sa.name)
            core->slewUpperThresholdPctFall = toFloat(sa.value);
        else if (kw::slew_upper_threshold_pct_rise == sa.name)
            core->slewUpperThresholdPctRise = toFloat(sa.value);
        else if (kw::default_operating_conditions == sa.name)
            core->defaultOperatingConditions = toString(sa.value);
    }
    void operator() (const ComplexAttribute & ca) override
    {
        if (kw::capacitive_load_unit == ca.name) {
            NS_ASSERT(2 == ca.values.size());
            CapUnit::Unit unit = CapUnit::Unit::FF;
            NS_CHECK_TRUE(ParseCapUnit(toString(ca.values.back()), unit));
            auto scale = toFloat(ca.values.front());
            core->capLoadUnit = CapUnit(scale, unit);
        }
        else if (kw::voltage_map == ca.name) {
            NS_ASSERT(2 == ca.values.size());
            core->voltageMap.emplace(toString(ca.values.front()), toFloat(ca.values.back()));
        }
    }
    void operator() (const GroupStatement & group) override
    {
        if (kw::operating_conditions == group.groupName) {
            auto operatingConditions = nano::Create<OperatingConditions>(group.name);
            OperatingConditionsParser(operatingConditions).Parse(group);
            core->operatingConditions.emplace_back(operatingConditions);
        }
        else if (kw::output_current_template == group.groupName) {
            auto lutTemplate = nano::Create<LutTemplate>(group.name);
            LutTemplateParser(lutTemplate).Parse(group);
            core->outputCurrentTemplate.emplace_back(lutTemplate);
        }
        else if (kw::lu_table_template == group.groupName) {
            auto lutTemplate = nano::Create<LutTemplate>(group.name);
            LutTemplateParser(lutTemplate).Parse(group);
            core->lutTemplates.emplace_back(lutTemplate);
        }
        else if (kw::power_lut_template == group.groupName) {
            auto lutTemplate = nano::Create<LutTemplate>(group.name);
            LutTemplateParser(lutTemplate).Parse(group);
            core->pwrLutTemplates.emplace_back(lutTemplate);
        }
        else if (kw::input_voltage == group.groupName) {
            auto voltage = nano::Create<Voltage>(group.groupName);
            VoltageParser(voltage).Parse(group);
            core->inputVoltage.emplace_back(voltage);
        }
        else if (kw::output_voltage == group.groupName) {
            auto voltage = nano::Create<Voltage>(group.groupName);
            VoltageParser(voltage).Parse(group);
            core->outputVoltage.emplace_back(voltage);
        }
        else if (kw::normalized_driver_waveform == group.groupName) {
            auto ndw = nano::Create<NormalizedDriverWaveform>();
            NormalizedDriverWaveformParser(ndw).Parse(group);
            core->normalizedDriverWaveform.emplace_back(ndw);
        }
        else if (kw::cell == group.groupName) {
            auto cell = nano::Create<Cell>(group.name, library);
            m_cellParsers.emplace_back(cell).Parse(group);
            core->cells.emplace_back(cell);
        }
    }

    void Build() override
    {
        for (auto & parser : m_cellParsers) parser.Build();  
    }
private:
    std::vector<CellParser> m_cellParsers;
};

Id<Library> LoadLibrary(std::string_view filename)
{
    LibertyDescription ast;
    LibertyParser libertyParser(ast);
    auto res = libertyParser(filename.data());
    if (not res) {
        NS_ERROR(libertyParser.error);
        return Id<Library>();
    }
    NS_ASSERT("library" == ast.groupName);
    auto library = nano::Create<Library>(std::string(ast.name));
    LibraryParser(library).Parse(ast);
    library->SetFilename(filename);
    return library;
}

} // namespace nano::liberty