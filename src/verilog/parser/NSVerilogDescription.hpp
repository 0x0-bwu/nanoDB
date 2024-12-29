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
    std::vector<std::string> ports;
};

struct ExplicitInstance
{
    std::string type;
    std::string name;
    using Ports = std::unordered_map<std::string, std::string>;
    Ports ports;//[formal, actual]
};

using Declaration = boost::variant<Aggregate, std::string>;
using Instance = boost::variant<ImplicitInstance, ExplicitInstance>;

using Binding = std::pair<std::string, std::string>;
using Bindings = std::vector<Binding>;

struct Netlist
{
    std::string name;
    std::vector<std::string> ports;
    std::vector<Declaration> inputs;
    std::vector<Declaration> inouts;
    std::vector<Declaration> outputs;
    std::vector<Declaration> signals;
    std::vector<Assignment> assignments;
    std::vector<Instance> instances;
};

using Netlists = std::vector<Netlist>;
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
    (std::vector<std::string>, ports)
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
    (std::vector<std::string>, ports)
    (std::vector<nano::verilog::ast::Declaration>, inputs)
    (std::vector<nano::verilog::ast::Declaration>, inouts)
    (std::vector<nano::verilog::ast::Declaration>, outputs)
    (std::vector<nano::verilog::ast::Declaration>, signals)
    (std::vector<nano::verilog::ast::Assignment>, assignments)
    (std::vector<nano::verilog::ast::Instance>, instances)
)