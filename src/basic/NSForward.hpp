#pragma once
#include "NSCommon.hpp"
#include <boost/hana.hpp>
#include <type_traits>
namespace nano {

class Binding;

namespace chip {

class Block;
class BTerm;
class Chip;
class Inst;
class ITerm;
class Net;

using BlockId = Id<Block>;
using BTermId = Id<BTerm>;
using ChipId = Id<Chip>;
using InstId = Id<Inst>;
using ITermId = Id<ITerm>;
using NetId = Id<Net>;

} // namespace chip

namespace liberty {

class CcsLut;
class Cell;
class InputPin;
class InternalPower;
class LeakagePower;
class Library;
class Lut;
class LutTemplate;
class NormalizedDriverWaveform;
class OperatingConditions;
class OutputCurrent;
class OutputPin;
class Pin;
class PwrGndPin;
class SignalPin;
class Timing;
class Voltage;

using LutNumbers = std::vector<NSFloat>;
using LutIndices = std::vector<LutNumbers>;

} // namespace liberty

namespace parasitic {

class Net;
class Parasitic;

using NetId = Id<Net>;
using ParasiticId = Id<Parasitic>;

} // namespace parasitic


template <typename... Args>
class Collection;
using Content = Collection<
    ///
    Binding,
    ///
    chip::Block,
    chip::BTerm,
    chip::Chip,
    chip::Inst,
    chip::ITerm,
    chip::Net,
    ///
    liberty::Cell,
    liberty::InternalPower,
    liberty::LeakagePower,
    liberty::Library,
    liberty::Lut,
    liberty::LutTemplate,
    liberty::OperatingConditions,
    liberty::OutputCurrent,
    liberty::Pin,
    liberty::Timing,
    liberty::Voltage,
    ///
    parasitic::Net,
    parasitic::Parasitic
>;

namespace hana = boost::hana;
namespace traits {

using namespace std::literals::string_view_literals;
inline constexpr static auto elementNameMap = hana::make_map(
    hana::make_pair(hana::type_c<Binding                          >, "Binding"sv                        ),
    hana::make_pair(hana::type_c<chip::Block                      >, "ChipBlock"sv                      ),
    hana::make_pair(hana::type_c<chip::BTerm                      >, "ChipBTerm"sv                      ),
    hana::make_pair(hana::type_c<chip::Chip                       >, "Chip"sv                           ),
    hana::make_pair(hana::type_c<chip::Inst                       >, "ChipInst"sv                       ),
    hana::make_pair(hana::type_c<chip::ITerm                      >, "ChipITerm"sv                      ),
    hana::make_pair(hana::type_c<chip::Net                        >, "ChipNet"sv                        ),
    hana::make_pair(hana::type_c<liberty::CcsLut                  >, "LibertyCcsLut"sv                  ),
    hana::make_pair(hana::type_c<liberty::Cell                    >, "LibertyCell"sv                    ),
    hana::make_pair(hana::type_c<liberty::InputPin                >, "LibertyInputPin"sv                ),
    hana::make_pair(hana::type_c<liberty::InternalPower           >, "LibertyInternalPower"sv           ),
    hana::make_pair(hana::type_c<liberty::LeakagePower            >, "LibertyLeakagePower"sv            ),
    hana::make_pair(hana::type_c<liberty::Library                 >, "LibertyLibrary"sv                 ),
    hana::make_pair(hana::type_c<liberty::Lut                     >, "LibertyLut"sv                     ),
    hana::make_pair(hana::type_c<liberty::LutTemplate             >, "LibertyLutTemplate"sv             ),
    hana::make_pair(hana::type_c<liberty::NormalizedDriverWaveform>, "LibertyNormalizedDriverWaveform"sv),
    hana::make_pair(hana::type_c<liberty::OperatingConditions     >, "LibertyOperatingConditions"sv     ),
    hana::make_pair(hana::type_c<liberty::OutputCurrent           >, "LibertyOutputCurrent"sv           ),
    hana::make_pair(hana::type_c<liberty::OutputPin               >, "LibertyOutputPin"sv               ),
    hana::make_pair(hana::type_c<liberty::Pin                     >, "LibertyPin"sv                     ),
    hana::make_pair(hana::type_c<liberty::PwrGndPin               >, "LibertyPwrGndPin"sv               ),
    hana::make_pair(hana::type_c<liberty::SignalPin               >, "LibertySignalPin"sv               ),
    hana::make_pair(hana::type_c<liberty::Timing                  >, "LibertyTiming"sv                  ),
    hana::make_pair(hana::type_c<liberty::Voltage                 >, "LibertyVoltage"sv                 ),
    hana::make_pair(hana::type_c<parasitic::Net                   >, "ParasiticNet"sv                   ),
    hana::make_pair(hana::type_c<parasitic::Parasitic             >, "Parasitic"sv                      )
);

inline constexpr static auto inheritanceMap = hana::make_map(
    hana::make_pair(hana::type_c<Binding                          >, hana::type_c<Binding                     >),
    hana::make_pair(hana::type_c<chip::Block                      >, hana::type_c<chip::Block                 >),
    hana::make_pair(hana::type_c<chip::BTerm                      >, hana::type_c<chip::BTerm                 >),
    hana::make_pair(hana::type_c<chip::Chip                       >, hana::type_c<chip::Chip                  >),
    hana::make_pair(hana::type_c<chip::Inst                       >, hana::type_c<chip::Inst                  >),
    hana::make_pair(hana::type_c<chip::ITerm                      >, hana::type_c<chip::ITerm                 >),
    hana::make_pair(hana::type_c<chip::Net                        >, hana::type_c<chip::Net                   >),
    hana::make_pair(hana::type_c<liberty::CcsLut                  >, hana::type_c<liberty::Lut                >),
    hana::make_pair(hana::type_c<liberty::Cell                    >, hana::type_c<liberty::Cell               >),
    hana::make_pair(hana::type_c<liberty::InputPin                >, hana::type_c<liberty::Pin                >),
    hana::make_pair(hana::type_c<liberty::InternalPower           >, hana::type_c<liberty::InternalPower      >),
    hana::make_pair(hana::type_c<liberty::LeakagePower            >, hana::type_c<liberty::LeakagePower       >),
    hana::make_pair(hana::type_c<liberty::Library                 >, hana::type_c<liberty::Library            >),
    hana::make_pair(hana::type_c<liberty::Lut                     >, hana::type_c<liberty::Lut                >),
    hana::make_pair(hana::type_c<liberty::LutTemplate             >, hana::type_c<liberty::LutTemplate        >),
    hana::make_pair(hana::type_c<liberty::NormalizedDriverWaveform>, hana::type_c<liberty::Lut                >),
    hana::make_pair(hana::type_c<liberty::OperatingConditions     >, hana::type_c<liberty::OperatingConditions>),
    hana::make_pair(hana::type_c<liberty::OutputCurrent           >, hana::type_c<liberty::OutputCurrent      >),
    hana::make_pair(hana::type_c<liberty::OutputPin               >, hana::type_c<liberty::Pin                >),
    hana::make_pair(hana::type_c<liberty::Pin                     >, hana::type_c<liberty::Pin                >),
    hana::make_pair(hana::type_c<liberty::PwrGndPin               >, hana::type_c<liberty::Pin                >),
    hana::make_pair(hana::type_c<liberty::SignalPin               >, hana::type_c<liberty::Pin                >),
    hana::make_pair(hana::type_c<liberty::Timing                  >, hana::type_c<liberty::Timing             >),
    hana::make_pair(hana::type_c<liberty::Voltage                 >, hana::type_c<liberty::Voltage            >),
    hana::make_pair(hana::type_c<parasitic::Net                   >, hana::type_c<parasitic::Net              >),
    hana::make_pair(hana::type_c<parasitic::Parasitic             >, hana::type_c<parasitic::Parasitic        >)
);

static_assert(hana::size(elementNameMap) == hana::size(inheritanceMap), 
    "key of element name map and inheritance map should be consistant");

template <typename T>
using BaseOf = typename std::decay_t<decltype(inheritanceMap[hana::type_c<T>])>::type;

} // namespace traits

template <typename T>
inline static constexpr const std::string_view & toString() { return traits::elementNameMap[hana::type_c<T>]; }

} // namespace nano