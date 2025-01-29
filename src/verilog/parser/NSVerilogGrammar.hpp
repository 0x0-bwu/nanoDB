#pragma once
#include "NSVerilogDescription.hpp"
#include "generic/tools/Parser.hpp"

namespace nano::verilog::grammar {

template <typename Iterator>
using ErrorHandler = generic::parser::ErrorHandler<Iterator>;

namespace qi = boost::spirit::qi;
namespace phx = boost::phoenix;
namespace spirit = boost::spirit;
namespace ascii = boost::spirit::ascii;

using qi::eoi;
using qi::eol;
using qi::eps;
using qi::lit;
using qi::int_;
using qi::char_;
using qi::_1;
using qi::_2;
using qi::_3;
using qi::_4;
using qi::_val;
using qi::lexeme;
using phx::at_c;
using phx::push_back;

template <typename Iterator>
struct VerilogSkipper : qi::grammar<Iterator>
{
    qi::rule<Iterator> skip;
    VerilogSkipper()
     : VerilogSkipper::base_type(skip)
    {
        skip = ascii::space |
            lexeme[lit('`' ) >> *(char_ - (eol | eoi)) >> (eol | eoi)] | 
            lexeme[lit("//") >> *(char_ - (eol | eoi)) >> (eol | eoi)] |
            lexeme[lit("/*") >> *(char_ - qi::string("*/")) >> lit("*/")];
        skip.name("skip");
    }
};

template <typename Iterator, typename Skipper>
struct VerilogGrammar : qi::grammar<Iterator, ast::VerilogDescription(), Skipper>
{
    ErrorHandler<Iterator> & errHandler;
    qi::rule<Iterator, std::string()> name;
    qi::rule<Iterator, std::string()> supply;
    qi::rule<Iterator, std::string()> comment;
    qi::rule<Iterator, std::string()> constant;
    qi::rule<Iterator, Vec<std::string>(), Skipper> ports;
    qi::rule<Iterator, ast::Binding(), Skipper> binding;
    qi::rule<Iterator, ast::Bindings(), Skipper> bindings;
    qi::rule<Iterator, ast::Aggregate(), Skipper> aggregate;
    qi::rule<Iterator, ast::Declaration(), Skipper> declaration;
    qi::rule<Iterator, ast::Assignment(), Skipper> assignment;
    qi::rule<Iterator, ast::ImplicitInstance(), Skipper> implicitInstance;
    qi::rule<Iterator, ast::ExplicitInstance(), Skipper> explicitInstance;
    qi::rule<Iterator, ast::Instance(), Skipper> instance;
    qi::rule<Iterator, ast::Netlist(), Skipper> netlist;
    qi::rule<Iterator, ast::Netlists(), Skipper> netlists;

    VerilogGrammar(ErrorHandler<Iterator> & errHandler)
     : VerilogGrammar::base_type(netlists), errHandler(errHandler)
    {
        netlists = *netlist[push_back(_val, _1)];
        
        netlist = "module" > name[at_c<0>(_val) = _1] >
            (
                lit(';') |
                (
                    lit('(') >> -(ports[at_c<1>(_val) = _1] | bindings) >> lit(')') >> lit(';') >>
                    *(
                        (
                            ("input"  >> declaration[push_back(at_c<2>(_val), _1)] % ',') |
                            ("inout"  >> declaration[push_back(at_c<3>(_val), _1)] % ',') |
                            ("output" >> declaration[push_back(at_c<4>(_val), _1)] % ',') |
                            ("wire"   >> declaration[push_back(at_c<5>(_val), _1)] % ',') |
                            (assignment[push_back(at_c<6>(_val), _1)])                    |
                            (supply >> declaration % ',')                                 |
                            (!lit("endmodule")) >> (instance[push_back(at_c<7>(_val), _1)])
                        )
                        >> lit(';')
                    )
                )
            ) > "endmodule"
        ;
        
        name = !char_('`') >> char_("a-zA-Z_") >> *char_("a-zA-Z_0-9[]/$");
        comment = lit('`') >> +char_("a-zA-Z_");
        supply = "supply" >> char_("0-1");
        constant = "1'b" >> char_("0-1");
        ports = ((-(lit("input") | lit("inout") | lit("output")) >> name) | eps) % ',';
        aggregate = lit('[') >> int_ >> lit(':') >> int_ >> lit(']') >> name;
        declaration = (aggregate | name)[_val = _1];
        assignment = "assign" >> name >> lit('=') >> (name | constant);
        instance = implicitInstance | explicitInstance;
        implicitInstance = name >> name >> lit('(') >> ports >> lit(')');
        explicitInstance = name >> name >> lit('(') >> binding % ',' >> lit(')');
        binding = lit('.') >> name >> lit('(') >> (name | constant) >> lit(')');
        bindings = binding % ',';

        name.name("name");
        constant.name("constant");
        ports.name("ports");
        binding.name("binding");
        aggregate.name("aggregate");
        declaration.name("declaration");
        assignment.name("assignment");
        implicitInstance.name("implicit instance");
        explicitInstance.name("explicit instance");
        instance.name("instance");
        netlist.name("netlist");
        netlists.name("netlists");

        // Error handling: on error in expr, call ErrorHandler.
        qi::on_error<qi::fail>(netlists, phx::function<ErrorHandler<Iterator> >(errHandler)("expecting ", _4, _3));
    }
};

} // nano::verilog::grammar