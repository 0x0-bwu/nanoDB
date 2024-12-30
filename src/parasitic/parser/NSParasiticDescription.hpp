
#pragma once
#include <boost/spirit/home/qi/operator/optional.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/variant.hpp>
#include <unordered_map>
#include <optional>
#include <sstream>
#include <ostream>
#include <memory>
#include <tuple>
namespace nano::parasitic::ast {

namespace io { inline static constexpr char sp(32), eol('\n'); }

using IOType = nano::IOType;
enum class ConnectType : int8_t
{
    INTERNAL,
    EXTERNAL
};

using SpefName = boost::variant<int, std::string>;

struct SpefNode
{
	SpefName name;	
	Optional<SpefName> subName;
    bool isInternal() const { return subName.has_value() and boost::get<int>(&subName.value()); }
};

struct SpefCoord
{
	float x;
	float y;
};

struct SpefValue
{
	int index;
	SpefNode n1;
	Optional<SpefNode> n2;
	float value;
};

using SpefValues = std::vector<SpefValue>;

//*PORTS section
struct SpefPort 
{
    SpefName name;
    IOType ioType;
	Optional<SpefCoord> coord;
};

using SpefPorts = std::vector<SpefPort>;

//*CONN section
struct SpefConn
{
    ConnectType type;
    SpefNode node;
    IOType ioType;
    Optional<SpefCoord> coord;
    Optional<float> load;
    Optional<SpefName> driver;
};

using SpefConns = std::vector<SpefConn>;

//*D_NET section
struct SpefNet
{
	SpefName name;
	float lumpCap;
	SpefConns connections;
	SpefValues resistances;
	SpefValues capacitances;  
};

using SpefNets = std::vector<SpefNet>;

struct SpefHeader
{
	std::string standard;
	std::string designName;
	std::string date;
	std::string vendor;
	std::string program;
	std::string version;
	std::string designFlow;
	std::string divider;
	std::string delimiter;
	std::string busDelimiter;
	std::string timeUnit;
	std::string capacitanceUnit;
	std::string resistanceUnit;
	std::string inductanceUnit;
};

using SpefNamePair = std::pair<int, std::string>;
using SpefNameMap = std::unordered_map<int, std::string>;
struct SpefDescription
{
	SpefHeader header;
	SpefNameMap nameMap;
	std::vector<SpefPort> ports;
	std::vector<SpefNet> nets;
};

} // namespace nano::parasitic::ast


BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefValue,
    (int, index)
    (nano::parasitic::ast::SpefNode, n1)
    (nano::Optional<nano::parasitic::ast::SpefNode>, n2)
    (float, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefCoord,
    (float, x)
    (float, y)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefNode,
    (nano::parasitic::ast::SpefName, name)
    (nano::Optional<nano::parasitic::ast::SpefName>, subName)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefPort,
    (nano::parasitic::ast::SpefName, name)
    (nano::IOType, ioType)
    (nano::Optional<nano::parasitic::ast::SpefCoord>, coord)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefNamePair,
    (int, first)
    (std::string, second)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefConn,
    (nano::parasitic::ast::ConnectType, type)
    (nano::parasitic::ast::SpefNode, node)
    (nano::parasitic::ast::IOType, ioType)
    (nano::Optional<nano::parasitic::ast::SpefCoord>, coord)
    (nano::Optional<float>, load)
    (nano::Optional<nano::parasitic::ast::SpefName>, driver)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefNet,
    (nano::parasitic::ast::SpefName, name)
    (float, lumpCap)
    (nano::parasitic::ast::SpefConns, connections)
    (nano::parasitic::ast::SpefValues, resistances)
    (nano::parasitic::ast::SpefValues, capacitances)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefHeader,
    (std::string, standard)
    (std::string, designName)
    (std::string, date)
    (std::string, vendor)
    (std::string, program)
    (std::string, version)
    (std::string, designFlow)
    (std::string, divider)
    (std::string, delimiter)
    (std::string, busDelimiter)
    (std::string, timeUnit)
    (std::string, capacitanceUnit)
    (std::string, resistanceUnit)
    (std::string, inductanceUnit)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::parasitic::ast::SpefDescription,
    (nano::parasitic::ast::SpefHeader, header)
    (nano::parasitic::ast::SpefNameMap, nameMap)
    (nano::parasitic::ast::SpefPorts, ports)
    (nano::parasitic::ast::SpefNets, nets)
)

inline std::ostream & operator<< (std::ostream & os, const nano::parasitic::ast::SpefHeader & spefHeader)
{
    using namespace nano::parasitic::ast;
    os << "HEADER:" << io::eol;
    boost::fusion::for_each(spefHeader, [](auto & t){std::cout << t << io::eol;});
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::parasitic::ast::SpefNameMap & spefNameMap)
{
    using namespace nano::parasitic::ast;
    os << "NAME MAP:" << io::eol;
    for (const auto & [index, name] : spefNameMap)
        os << index << io::sp << name << io::eol;    
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::parasitic::ast::SpefPorts & spefPorts)
{
    using namespace nano::parasitic::ast;
    os << "PORTS: " << spefPorts.size() << io::eol;
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::parasitic::ast::SpefName & spefName)
{
    using namespace nano::parasitic::ast;
    if (auto * index = boost::get<int>(&spefName))
        os << '*' << *index;
    else if (auto * name = boost::get<std::string>(&spefName))
        os << *name;
    os << io::eol;
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::parasitic::ast::SpefNet & spefNet)
{
    using namespace nano::parasitic::ast;
    os << "NET: " << spefNet.name << io::eol;
    os << "LUMP CAP: " << spefNet.lumpCap << io::eol;
    os << "CONNECTIONS: " << spefNet.connections.size() << io::eol;
    os << "RESISTANCES: " << spefNet.resistances.size() << io::eol;
    os << "CAPACITANCES: " << spefNet.capacitances.size() << io::eol;
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::parasitic::ast::SpefNets & spefNets)
{
    using namespace nano::parasitic::ast;
    os << "NETS: " << spefNets.size() << io::eol;
    return os;
}

inline std::ostream & operator<< (std::ostream & os, const nano::parasitic::ast::SpefDescription & spefDescription)
{
    using namespace nano::parasitic::ast;
    os << spefDescription.header << io::eol;
    os << spefDescription.nameMap << io::eol;
    os << spefDescription.ports << io::eol;
    os << spefDescription.nets << io::eol;
    return os;
}