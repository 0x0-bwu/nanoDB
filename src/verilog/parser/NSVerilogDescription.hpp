#pragma once
#include <boost/phoenix/fusion.hpp>
#include <boost/variant.hpp>
#include <unordered_map>
#include <string>
#include <vector>
namespace nano::verilog::ast {

struct Aggregate
{
    int left{0};
    int right{0};
    std::string name;
};

struct Assignment
{
    std::string lhs;
    std::string rhs;
};

struct ImplicitInstance
{
    std::string type;
    std::string name;
    Vec<std::string> ports;
};

struct ExplicitInstance
{
    std::string type;
    std::string name;
    using Ports = HashMap<std::string, std::string>;
    Ports ports;//[formal, actual]
};

using Declaration = boost::variant<Aggregate, std::string>;
using Instance = boost::variant<ImplicitInstance, ExplicitInstance>;

using Binding = Pair<std::string, std::string>;
using Bindings = Vec<Binding>;

struct Netlist
{
    std::string name;
    Vec<std::string> ports;
    Vec<Declaration> inputs;
    Vec<Declaration> inouts;
    Vec<Declaration> outputs;
    Vec<Declaration> signals;
    Vec<Assignment> assignments;
    Vec<Instance> instances;
};

using Netlists = Vec<Netlist>;
using VerilogDescription = Netlists;

} // namespace nano::verilog::ast


BOOST_FUSION_ADAPT_STRUCT(
    nano::verilog::ast::Aggregate,
    (int, left)
    (int, right)
    (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::verilog::ast::Assignment,
    (std::string, lhs)
    (std::string, rhs)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::verilog::ast::ImplicitInstance,
    (std::string, type)
    (std::string, name)
    (nano::Vec<std::string>, ports)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::verilog::ast::ExplicitInstance,
    (std::string, type)
    (std::string, name)
    (nano::verilog::ast::ExplicitInstance::Ports, ports)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::verilog::ast::Binding,
    (std::string, first)
    (std::string, second)
)

BOOST_FUSION_ADAPT_STRUCT(
    nano::verilog::ast::Netlist,
    (std::string, name)
    (nano::Vec<std::string>, ports)
    (nano::Vec<nano::verilog::ast::Declaration>, inputs)
    (nano::Vec<nano::verilog::ast::Declaration>, inouts)
    (nano::Vec<nano::verilog::ast::Declaration>, outputs)
    (nano::Vec<nano::verilog::ast::Declaration>, signals)
    (nano::Vec<nano::verilog::ast::Assignment>, assignments)
    (nano::Vec<nano::verilog::ast::Instance>, instances)
)