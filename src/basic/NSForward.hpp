#pragma once
#include "NSConfig.hpp"
#include "NSAlias.hpp"
#include "NSVersion.hpp"
#include <boost/hana.hpp>
#include <type_traits>
namespace nano {

class Binding;

class Material;
class MaterialLib;
class MaterialProp;
class MaterialPropValue;
class MaterialPropTable;
class MaterialPropPolynomial;
enum class MaterialType;

using MaterialId = Id<Material>;
using MaterialLibId = Id<MaterialLib>;
using MaterialPropId = Id<MaterialProp>;
using MaterialPropValueId = Id<MaterialPropValue>;
using MaterialPropTableId = Id<MaterialPropTable>;
using MaterialPropPolynomialId = Id<MaterialPropPolynomial>;

class Shape;
class ShapeCircle;
class ShapeFromTemplate;
class ShapePath;
class ShapePolygon;
class ShapePolygonWithHoles;
class ShapeRect;

using ShapeId = Id<Shape>;
using ShapeCircleId = Id<ShapeCircle>;
using ShapeFromTemplateId = Id<ShapeFromTemplate>;
using ShapePathId = Id<ShapePath>;
using ShapePolygonId = Id<ShapePolygon>;
using ShapePolygonWithHolesId = Id<ShapePolygonWithHoles>;
using ShapeRectId = Id<ShapeRect>;

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

using LutNumbers = std::vector<Float>;
using LutIndices = std::vector<LutNumbers>;

} // namespace liberty

namespace package {

class Cell;
class CircuitCell;
class FootprintCell;
class FootprintPin;
class Layout;
class Package;
class Padstack;
class Pin;
class StackupLayer;

using CellId = Id<Cell>;
using CircuitCellId = Id<CircuitCell>;
using FootprintCellId = Id<FootprintCell>;
using FootprintPinId = Id<FootprintPin>;
using LayoutId = Id<Layout>;
using PackageId = Id<Package>;
using PadstackId = Id<Padstack>;
using PinId = Id<Pin>;
using StackupLayerId = Id<StackupLayer>;

} // namespace package

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
    Material,
    MaterialLib,
    MaterialProp,
    Shape,
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
    package::Cell,
    package::Layout,
    package::Package,
    package::Padstack,
    package::Pin,
    package::StackupLayer,
    ///
    parasitic::Net,
    parasitic::Parasitic
>;

namespace hana = boost::hana;
namespace traits {

using namespace std::literals::string_view_literals;
inline constexpr static auto elementNameMap = hana::make_map(
    hana::make_pair(hana::type_c<Binding                          >, "Binding"sv                        ),
    hana::make_pair(hana::type_c<Material                         >, "Material"sv                       ),
    hana::make_pair(hana::type_c<MaterialLib                      >, "MaterialLib"sv                    ),
    hana::make_pair(hana::type_c<MaterialProp                     >, "MaterialProp"sv                   ),
    hana::make_pair(hana::type_c<MaterialPropPolynomial           >, "MaterialPropPolynomial"sv         ),
    hana::make_pair(hana::type_c<MaterialPropTable                >, "MaterialPropTable"sv              ),
    hana::make_pair(hana::type_c<MaterialPropValue                >, "MaterialPropValue"sv              ),
    hana::make_pair(hana::type_c<Shape                            >, "Shape"sv                          ),
    hana::make_pair(hana::type_c<ShapeCircle                      >, "ShapeCircle"sv                    ),
    hana::make_pair(hana::type_c<ShapeFromTemplate                >, "ShapeFromTemplate"sv              ),
    hana::make_pair(hana::type_c<ShapePath                        >, "ShapePath"sv                      ),
    hana::make_pair(hana::type_c<ShapePolygon                     >, "ShapePolygon"sv                   ),
    hana::make_pair(hana::type_c<ShapePolygonWithHoles            >, "ShapePolygonWithHoles"sv          ),
    hana::make_pair(hana::type_c<ShapeRect                        >, "ShapeRect"sv                      ),
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
    hana::make_pair(hana::type_c<package::Cell                    >, "Cell"sv                           ),
    hana::make_pair(hana::type_c<package::CircuitCell             >, "CircuitCell"sv                    ),
    hana::make_pair(hana::type_c<package::FootprintCell           >, "FootprintCell"sv                  ),
    hana::make_pair(hana::type_c<package::FootprintPin            >, "FootpinrtPin"sv                   ),
    hana::make_pair(hana::type_c<package::Layout                  >, "Layout"sv                         ),
    hana::make_pair(hana::type_c<package::Package                 >, "Package"sv                        ),
    hana::make_pair(hana::type_c<package::Padstack                >, "Padstack"sv                       ),
    hana::make_pair(hana::type_c<package::Pin                     >, "Pin"sv                            ),
    hana::make_pair(hana::type_c<package::StackupLayer            >, "StackupLayer"sv                   ),
    hana::make_pair(hana::type_c<parasitic::Net                   >, "ParasiticNet"sv                   ),
    hana::make_pair(hana::type_c<parasitic::Parasitic             >, "Parasitic"sv                      )
);

inline constexpr static auto inheritanceMap = hana::make_map(
    hana::make_pair(hana::type_c<MaterialPropPolynomial           >, hana::type_c<MaterialProp         >),
    hana::make_pair(hana::type_c<MaterialPropTable                >, hana::type_c<MaterialProp         >),
    hana::make_pair(hana::type_c<MaterialPropValue                >, hana::type_c<MaterialProp         >),
    hana::make_pair(hana::type_c<ShapeCircle                      >, hana::type_c<Shape                >),
    hana::make_pair(hana::type_c<ShapeFromTemplate                >, hana::type_c<Shape                >),
    hana::make_pair(hana::type_c<ShapePath                        >, hana::type_c<Shape                >),
    hana::make_pair(hana::type_c<ShapePolygon                     >, hana::type_c<Shape                >),
    hana::make_pair(hana::type_c<ShapePolygonWithHoles            >, hana::type_c<Shape                >),
    hana::make_pair(hana::type_c<ShapeRect                        >, hana::type_c<Shape                >),
    hana::make_pair(hana::type_c<liberty::CcsLut                  >, hana::type_c<liberty::Lut         >),
    hana::make_pair(hana::type_c<liberty::InputPin                >, hana::type_c<liberty::Pin         >),
    hana::make_pair(hana::type_c<liberty::NormalizedDriverWaveform>, hana::type_c<liberty::Lut         >),
    hana::make_pair(hana::type_c<liberty::OutputPin               >, hana::type_c<liberty::Pin         >),
    hana::make_pair(hana::type_c<liberty::PwrGndPin               >, hana::type_c<liberty::Pin         >),
    hana::make_pair(hana::type_c<liberty::SignalPin               >, hana::type_c<liberty::Pin         >),
    hana::make_pair(hana::type_c<package::CircuitCell             >, hana::type_c<package::Cell        >),
    hana::make_pair(hana::type_c<package::FootprintCell           >, hana::type_c<package::Cell        >),
    hana::make_pair(hana::type_c<package::FootprintPin            >, hana::type_c<package::Pin         >)
);

template <typename T, typename = hana::when<true>>
struct Base { using type = std::decay_t<T>; };

template <typename T>
struct Base<T, hana::when<hana::contains(inheritanceMap, hana::type_c<T>)>>
{
    using type = typename std::decay_t<decltype(inheritanceMap[hana::type_c<T>])>::type;
};

template <typename T>
using BaseOf = typename Base<T>::type;

} // namespace traits

template <typename T>
inline static constexpr const std::string_view & toString() { return traits::elementNameMap[hana::type_c<T>]; }

} // namespace nano